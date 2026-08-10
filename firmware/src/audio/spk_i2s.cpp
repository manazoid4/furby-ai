#include "audio/spk_i2s.h"

#include <Arduino.h>
#include <driver/i2s.h>

#include "config.h"

namespace {

constexpr i2s_port_t kPort = I2S_NUM_1;

// 2 seconds of jitter buffer. Generous, because a wifi retransmit that stalls
// audio mid-word is far more noticeable than 100 ms of extra latency.
constexpr size_t kBufSamples = AUDIO_SAMPLE_RATE * 2;

int16_t* g_buf = nullptr;
volatile size_t g_head = 0;  // write
volatile size_t g_tail = 0;  // read

portMUX_TYPE g_mux = portMUX_INITIALIZER_UNLOCKED;

size_t queued() {
  const size_t h = g_head, t = g_tail;
  return (h >= t) ? (h - t) : (kBufSamples - t + h);
}

}  // namespace

void spk_begin() {
  g_buf = static_cast<int16_t*>(
      heap_caps_malloc(kBufSamples * sizeof(int16_t), MALLOC_CAP_SPIRAM));
  if (!g_buf) {
    // No PSRAM, or it is full. Fall back to internal RAM and shout about it -
    // a common cause of "audio works on the bench, dies in the toy".
    Serial.println("spk: PSRAM alloc failed, falling back to internal RAM");
    g_buf = static_cast<int16_t*>(malloc(kBufSamples * sizeof(int16_t)));
  }
  configASSERT(g_buf != nullptr);

  const i2s_config_t cfg = {
      .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = AUDIO_SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // MAX98357A SD pin floating
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 256,
      .use_apll = false,
      .tx_desc_auto_clear = true,  // play silence on underrun, not garbage
      .fixed_mclk = 0,
  };

  const i2s_pin_config_t pins = {
      .bck_io_num = PIN_SPK_BCLK,
      .ws_io_num = PIN_SPK_WS,
      .data_out_num = PIN_SPK_DOUT,
      .data_in_num = I2S_PIN_NO_CHANGE,
  };

  ESP_ERROR_CHECK(i2s_driver_install(kPort, &cfg, 0, nullptr));
  ESP_ERROR_CHECK(i2s_set_pin(kPort, &pins));
  ESP_ERROR_CHECK(i2s_zero_dma_buffer(kPort));

  Serial.printf("spk: i2s1 up, jitter buffer %u samples\n",
                static_cast<unsigned>(kBufSamples));
}

size_t spk_push(const int16_t* pcm, size_t samples) {
  size_t written = 0;

  portENTER_CRITICAL(&g_mux);
  for (size_t i = 0; i < samples; ++i) {
    const size_t next = (g_head + 1) % kBufSamples;
    if (next == g_tail) break;  // full - drop the rest, do not block
    g_buf[g_head] = pcm[i];
    g_head = next;
    ++written;
  }
  portEXIT_CRITICAL(&g_mux);

  return written;
}

bool spk_pump() {
  if (queued() == 0) return false;

  // Copy out in chunks so the critical section stays short.
  static int16_t chunk[256];
  size_t n = 0;

  portENTER_CRITICAL(&g_mux);
  while (n < 256 && g_tail != g_head) {
    chunk[n++] = g_buf[g_tail];
    g_tail = (g_tail + 1) % kBufSamples;
  }
  portEXIT_CRITICAL(&g_mux);

  if (n == 0) return false;

  size_t wrote = 0;
  i2s_write(kPort, chunk, n * sizeof(int16_t), &wrote, portMAX_DELAY);
  return true;
}

size_t spk_queued() { return queued(); }

void spk_flush() {
  portENTER_CRITICAL(&g_mux);
  g_tail = g_head;
  portEXIT_CRITICAL(&g_mux);
  i2s_zero_dma_buffer(kPort);
}

bool spk_active() { return queued() > 0; }
