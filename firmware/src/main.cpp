// main.cpp - task setup for the CrowPanel Advance 4.3".
//
// Task layout (docs/05-architecture.md):
//   core 1: audio in, audio out   - underruns are audible
//   core 0: net, ui               - dropped frames here are not
//
// Nothing from the Furby is wired to this board. No motor task, no switch
// polling, no tilt or tickle sensors. The toy is a shell and this panel is the
// whole machine; the only physical input is the panel's own touchscreen.

#include <Arduino.h>

#include "config.h"
#include "audio/mic_i2s.h"
#include "audio/spk_i2s.h"
#include "net/ws_client.h"

namespace {

// Conversation state, mirrored from the brain's `state` frames.
volatile FurbyState g_state = FurbyState::Idle;

// Current face. Rendering is M2 - for now this is tracked and logged so the
// protocol end-to-end can be tested before any graphics code exists.
char g_expression[16] = "neutral";
char g_caption1[48] = "";
char g_caption2[48] = "";

int g_backlight = BACKLIGHT_ACTIVE;
uint32_t g_last_activity_ms = 0;

void setBacklight(int level) {
  if (level < BACKLIGHT_MIN_KEEPALIVE) level = BACKLIGHT_MIN_KEEPALIVE;
  if (level > 100) level = 100;
  g_backlight = level;
  // TODO(M2): drive PIN_BACKLIGHT once the real pin is confirmed from the
  // Elecrow wiki for this exact board. Writing to a placeholder pin is worse
  // than not writing at all.
}

// ----------------------------------------------------------------- tasks ---

// Captures mic audio and uploads it while the brain has us listening.
//
// TODO(M6): the wake word belongs here. Until ESP-SR/WakeNet is in (needs
// Arduino-ESP32 3.x, see platformio.ini), capture is gated on the brain's
// state, so audio streams whenever we are LISTENING rather than only after a
// local wake word. That is a weaker privacy position than docs/07 describes,
// and it is called out there too. Fix before this lives in a shared room.
void taskAudioIn(void*) {
  static int16_t frame[AUDIO_FRAME_SAMPLES];
  for (;;) {
    if (!mic_read_frame(frame, AUDIO_FRAME_SAMPLES)) {
      vTaskDelay(pdMS_TO_TICKS(5));
      continue;
    }
    // The mic and speaker are centimetres apart on this board. Without the
    // half-duplex gate the Furby hears itself and talks to itself forever.
    const bool muted = (g_state == FurbyState::Speaking && HALF_DUPLEX_DEFAULT);
    if (g_state == FurbyState::Listening && !muted) {
      ws_send_audio(frame, AUDIO_FRAME_SAMPLES);
    }
  }
}

void taskAudioOut(void*) {
  for (;;) {
    if (!spk_pump()) vTaskDelay(pdMS_TO_TICKS(2));
  }
}

void taskNet(void*) {
  for (;;) {
    ws_loop();
    vTaskDelay(pdMS_TO_TICKS(2));
  }
}

void taskUi(void*) {
  uint32_t last_status = 0;

  for (;;) {
    const uint32_t now = millis();

    // Dim when nothing has happened for a while. Saves power, and reads as the
    // Furby dozing off rather than as a fault.
    if (g_state == FurbyState::Idle &&
        now - g_last_activity_ms > IDLE_DIM_MS &&
        g_backlight != BACKLIGHT_IDLE) {
      setBacklight(BACKLIGHT_IDLE);
    }

    if (now - last_status > STATUS_INTERVAL_MS) {
      last_status = now;
      ws_send_status();
    }

    // TODO(M2): draw the face here. Two eyes and a mouth from primitives,
    // interpolated between expression keyframes, plus a blink timer. A crude
    // cartoon face that blinks reads as alive; an almost-real one reads as
    // broken - see docs/05-architecture.md.

    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

}  // namespace

// ------------------------------------------------------------- callbacks ---

void onBrainState(FurbyState s) {
  g_state = s;
  g_last_activity_ms = millis();
  if (g_backlight != BACKLIGHT_ACTIVE) setBacklight(BACKLIGHT_ACTIVE);
  Serial.printf("state: %s\n", furbyStateName(s));
}

void onBrainFace(const char* expression, bool blink) {
  strncpy(g_expression, expression, sizeof(g_expression) - 1);
  g_expression[sizeof(g_expression) - 1] = '\0';
  g_last_activity_ms = millis();
  Serial.printf("face: %s%s\n", g_expression, blink ? " (blink)" : "");
}

void onBrainCaption(const char* line1, const char* line2) {
  if (line1) {
    strncpy(g_caption1, line1, sizeof(g_caption1) - 1);
    g_caption1[sizeof(g_caption1) - 1] = '\0';
  }
  if (line2) {
    strncpy(g_caption2, line2, sizeof(g_caption2) - 1);
    g_caption2[sizeof(g_caption2) - 1] = '\0';
  }
  Serial.printf("caption: %s | %s\n", g_caption1, g_caption2);
}

void onBrainBacklight(int level) { setBacklight(level); }

void onBrainAudio(const int16_t* pcm, size_t samples) {
  spk_push(pcm, samples);
}

// ------------------------------------------------------------------ boot ---

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.printf("\nfurby-ai fw %s booting (%s)\n", FW_VERSION, FURBY_ROLE);

  pinMode(PIN_BACKLIGHT, OUTPUT);
  setBacklight(BACKLIGHT_ACTIVE);
  g_last_activity_ms = millis();

  mic_begin();
  spk_begin();

  ws_begin(onBrainState, onBrainFace, onBrainCaption, onBrainBacklight,
           onBrainAudio);

  // Audio on core 1, everything else on core 0.
  xTaskCreatePinnedToCore(taskAudioIn, "audio_in", 4096, nullptr, 6, nullptr, 1);
  xTaskCreatePinnedToCore(taskAudioOut, "audio_out", 4096, nullptr, 6, nullptr, 1);
  xTaskCreatePinnedToCore(taskNet, "net", 8192, nullptr, 4, nullptr, 0);
  xTaskCreatePinnedToCore(taskUi, "ui", 4096, nullptr, 2, nullptr, 0);

  Serial.println("tasks up");
}

void loop() {
  // Everything runs in tasks. Keep the Arduino loop empty and let it idle.
  vTaskDelay(pdMS_TO_TICKS(1000));
}
