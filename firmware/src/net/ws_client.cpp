#include "net/ws_client.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <WiFi.h>
#include <string.h>

#include "config.h"
#include "secrets.h"

namespace {

WebSocketsClient g_ws;

StateCb g_on_state = nullptr;
FaceCb g_on_face = nullptr;
CaptionCb g_on_caption = nullptr;
BacklightCb g_on_backlight = nullptr;
AudioCb g_on_audio = nullptr;

bool g_connected = false;
uint16_t g_seq = 0;
uint32_t g_backoff_ms = WS_RECONNECT_MIN_MS;

uint8_t g_txbuf[AUDIO_HDR_BYTES + AUDIO_FRAME_BYTES];

void sendJson(const JsonDocument& doc) {
  if (!g_connected) return;
  char out[512];
  const size_t n = serializeJson(doc, out, sizeof(out));
  g_ws.sendTXT(out, n);
}

void sendHello() {
  JsonDocument doc;
  doc["t"] = "hello";
  doc["fw"] = FW_VERSION;
  doc["id"] = FURBY_ID;
  doc["role"] = FURBY_ROLE;
  doc["token"] = BRAIN_TOKEN;
  doc["mac"] = WiFi.macAddress();
  doc["rssi"] = WiFi.RSSI();
  sendJson(doc);
}

void handleText(const uint8_t* payload, size_t len) {
  JsonDocument doc;
  if (deserializeJson(doc, payload, len)) {
    Serial.println("ws: bad json from brain, ignoring");
    return;
  }

  const char* t = doc["t"] | "";

  if (!strcmp(t, "state")) {
    if (g_on_state) g_on_state(furbyStateFrom(doc["value"] | "idle"));
  } else if (!strcmp(t, "face")) {
    if (g_on_face) {
      g_on_face(doc["expression"] | "neutral", doc["blink"] | false);
    }
  } else if (!strcmp(t, "caption")) {
    if (g_on_caption) {
      g_on_caption(doc["line1"] | (const char*)nullptr,
                   doc["line2"] | (const char*)nullptr);
    }
  } else if (!strcmp(t, "backlight")) {
    if (g_on_backlight) g_on_backlight(doc["level"] | 100);
  } else if (!strcmp(t, "ping")) {
    JsonDocument pong;
    pong["t"] = "pong";
    pong["n"] = doc["n"] | 0;
    sendJson(pong);
  } else if (!strcmp(t, "speak_begin") || !strcmp(t, "speak_end") ||
             !strcmp(t, "tool_ran")) {
    // Accompanied by state frames that already drive behaviour; log for now.
    Serial.printf("ws: %s\n", t);
  } else {
    Serial.printf("ws: unknown frame type '%s'\n", t);
  }
}

void handleBinary(const uint8_t* payload, size_t len) {
  if (len < AUDIO_HDR_BYTES) return;
  if (payload[0] != AUDIO_MAGIC) return;
  if (payload[1] != AUDIO_TYPE_SPK) return;

  const size_t samples = (len - AUDIO_HDR_BYTES) / 2;
  if (g_on_audio) {
    g_on_audio(reinterpret_cast<const int16_t*>(payload + AUDIO_HDR_BYTES),
               samples);
  }
}

void onEvent(WStype_t type, uint8_t* payload, size_t len) {
  switch (type) {
    case WStype_CONNECTED:
      g_connected = true;
      g_backoff_ms = WS_RECONNECT_MIN_MS;
      Serial.println("ws: connected");
      sendHello();
      break;

    case WStype_DISCONNECTED:
      g_connected = false;
      Serial.printf("ws: disconnected, retry in %u ms\n",
                    static_cast<unsigned>(g_backoff_ms));
      // Exponential backoff with jitter, capped. Hammering a dead brain just
      // burns battery.
      g_backoff_ms = min<uint32_t>(g_backoff_ms * 2, WS_RECONNECT_MAX_MS);
      g_ws.setReconnectInterval(g_backoff_ms + (esp_random() % 500));
      break;

    case WStype_TEXT:
      handleText(payload, len);
      break;

    case WStype_BIN:
      handleBinary(payload, len);
      break;

    default:
      break;
  }
}

}  // namespace

void ws_begin(StateCb on_state, FaceCb on_face, CaptionCb on_caption,
              BacklightCb on_backlight, AudioCb on_audio) {
  g_on_state = on_state;
  g_on_face = on_face;
  g_on_caption = on_caption;
  g_on_backlight = on_backlight;
  g_on_audio = on_audio;

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);  // sleep adds latency we cannot afford mid-conversation
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.printf("wifi: connecting to %s", WIFI_SSID);
  const uint32_t start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(250);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() != WL_CONNECTED) {
    // Keep booting anyway - the reconnect logic keeps trying, and a Furby that
    // is deaf beats one that is bricked.
    Serial.println("wifi: FAILED, continuing offline");
  } else {
    Serial.printf("wifi: %s  rssi %d dBm\n", WiFi.localIP().toString().c_str(),
                  WiFi.RSSI());
  }

  g_ws.begin(BRAIN_HOST, BRAIN_PORT, BRAIN_PATH);
  g_ws.onEvent(onEvent);
  g_ws.setReconnectInterval(WS_RECONNECT_MIN_MS);
  g_ws.enableHeartbeat(WS_PING_INTERVAL_MS, WS_PING_INTERVAL_MS * 2, 3);
}

void ws_loop() { g_ws.loop(); }

bool ws_connected() { return g_connected; }

int ws_rssi() { return WiFi.RSSI(); }

void ws_send_audio(const int16_t* pcm, size_t samples) {
  if (!g_connected) return;  // drop, do not buffer - stale audio is worse
  const size_t bytes = samples * 2;
  if (bytes > AUDIO_FRAME_BYTES) return;

  g_txbuf[0] = AUDIO_MAGIC;
  g_txbuf[1] = AUDIO_TYPE_MIC;
  g_txbuf[2] = g_seq & 0xFF;
  g_txbuf[3] = (g_seq >> 8) & 0xFF;
  ++g_seq;

  memcpy(g_txbuf + AUDIO_HDR_BYTES, pcm, bytes);
  g_ws.sendBIN(g_txbuf, AUDIO_HDR_BYTES + bytes);
}

void ws_send_touch(int x, int y) {
  JsonDocument doc;
  doc["t"] = "event";
  doc["kind"] = "touch";
  doc["x"] = x;
  doc["y"] = y;
  sendJson(doc);
}

void ws_send_status() {
  JsonDocument doc;
  doc["t"] = "status";
  doc["rssi"] = WiFi.RSSI();
  doc["heap"] = ESP.getFreeHeap();
  sendJson(doc);
}

void ws_send_error(const char* code, const char* msg) {
  JsonDocument doc;
  doc["t"] = "error";
  doc["code"] = code;
  doc["msg"] = msg;
  sendJson(doc);
}

void ws_send_eos() {
  JsonDocument doc;
  doc["t"] = "eos";
  sendJson(doc);
}
