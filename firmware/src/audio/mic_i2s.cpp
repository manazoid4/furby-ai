#include "audio/mic_i2s.h"

#include <Arduino.h>
#include <driver/i2s.h>

#include "config.h"

namespace {

constexpr i2s_port_t kPort = I2S_NUM_0;

// One frame of stereo 32-bit slots. The ICS-43434 gives 24-bit data
// left-justified in each 32-bit slot.
int32_t g_raw[AUDIO_FRAME_SAMPLES * 2];

uint16_t g_peak = 0;
bool g_stereo_debug = false;

}  // namespace

void mic_begin() {
  const i2s_config_t cfg = {
      .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = AUDIO_SAMPLE_RATE,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  // both mics
      .communication_format = I2S_COMM_FORMAT_STAND_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 256,
      .use_apll = false,
      .tx_desc_auto_clear = false,
      .fixed_mclk = 0,
  };

  const i2s_pin_config_t pins = {
      .bck_io_num = PIN_MIC_BCLK,
      .ws_io_num = PIN_MIC_WS,
      .data_out_num = I2S_PIN_NO_CHANGE,
      .data_in_num = PIN_MIC_DIN,
  };

  ESP_ERROR_CHECK(i2s_driver_install(kPort, &cfg, 0, nullptr));
  ESP_ERROR_CHECK(i2s_set_pin(kPort, &pins));
  ESP_ERROR_CHECK(i2s_zero_dma_buffer(kPort));

  Serial.printf("mic: i2s0 up @ %d Hz, shift %d\n", AUDIO_SAMPLE_RATE,
                MIC_SHIFT);
}

bool mic_read_frame(int16_t* out, size_t samples) {
  if (samples > AUDIO_FRAME_SAMPLES) return false;

  const size_t want = samples * 2 * sizeof(int32_t);  // stereo
  size_t got = 0;

  const esp_err_t err = i2s_read(kPort, g_raw, want, &got, pdMS_TO_TICKS(100));
  if (err != ESP_OK || got != want) return false;

  uint16_t peak = 0;

  for (size_t i = 0; i < samples; ++i) {
    const int32_t l = g_raw[i * 2] >> MIC_SHIFT;
    const int32_t r = g_raw[i * 2 + 1] >> MIC_SHIFT;

    // Average the two capsules. Crude, but it is a real SNR win over one mic
    // and it costs nothing. Proper beamforming needs the stereo stream and a
    // delay estimate on the brain - that is what stereo_debug is for.
    int32_t v = g_stereo_debug ? l : ((l + r) / 2);

    if (v > 32767) v = 32767;
    if (v < -32768) v = -32768;

    out[i] = static_cast<int16_t>(v);

    const uint16_t a = static_cast<uint16_t>(abs(static_cast<int>(v)));
    if (a > peak) peak = a;
  }

  g_peak = peak;
  return true;
}

uint16_t mic_last_peak() { return g_peak; }

void mic_set_stereo_debug(bool on) { g_stereo_debug = on; }
