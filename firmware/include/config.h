// config.h - single source of truth for pins and tunables.
//
// Board: Elecrow CrowPanel Advance 4.3" (ESP32-S3, 800x480 IPS touch).
// The board already carries the microphone, the speaker, the amplifier and the
// display, so there is nothing to wire and almost nothing to configure here.
//
// NOTHING FROM THE FURBY IS CONNECTED.
// No motor, no tickle switches, no tilt switch, no IR, no original speaker.
// The Furby is a shell. The only input device is the panel's own touchscreen.
//
// !! THE PIN NUMBERS BELOW ARE PLACEHOLDERS !!
// CrowPanel models differ. Take the real values from the Elecrow wiki page for
// your exact board and replace them here. The 3.5" Advance, for reference, has
// its I2S microphone on IO9 / IO10 / IO3 - the 4.3" may differ.

#pragma once

#include <stdint.h>

// ---------------------------------------------------------------- identity --
#define FW_VERSION      "0.2.0"
#define FURBY_ID        "furby-01"
#define FURBY_ROLE      "panel"

// ------------------------------------------------------------------- audio --
#define AUDIO_SAMPLE_RATE   16000
#define AUDIO_FRAME_SAMPLES 320                       // 20 ms @ 16 kHz
#define AUDIO_FRAME_BYTES   (AUDIO_FRAME_SAMPLES * 2) // 16-bit mono

// Onboard I2S microphone. PLACEHOLDER - verify against the Elecrow wiki.
#define PIN_MIC_BCLK    9
#define PIN_MIC_WS      10
#define PIN_MIC_DIN     3

// The onboard mic is a 24-bit I2S part in a 32-bit slot.
// sample16 = (int16_t)(raw32 >> MIC_SHIFT). Lower it if loud speech clips.
#define MIC_SHIFT       14

// Onboard speaker + amplifier. PLACEHOLDER - verify against the Elecrow wiki.
#define PIN_SPK_BCLK    15
#define PIN_SPK_WS      16
#define PIN_SPK_DOUT    17

// ----------------------------------------------------------------- display --
#define SCREEN_W        800
#define SCREEN_H        480
#define PIN_BACKLIGHT   38      // PLACEHOLDER - LCD backlight control

// Dim the screen after this long with no conversation. Saves power and reads
// as the Furby falling asleep, which is charming rather than broken-looking.
#define IDLE_DIM_MS         120000
#define BACKLIGHT_ACTIVE    100
#define BACKLIGHT_IDLE      25

// Some USB power banks cut out below ~50-100 mA. If the Furby dies after a few
// quiet minutes, that is why - raise this floor (docs/04-power.md).
#define BACKLIGHT_MIN_KEEPALIVE 20

// --------------------------------------------------------------- behaviour --
#define STATUS_INTERVAL_MS      30000
#define WS_PING_INTERVAL_MS     5000
#define WS_RECONNECT_MIN_MS     1000
#define WS_RECONNECT_MAX_MS     30000

// Half-duplex: stop capturing while the speaker plays. On this board the mic
// and speaker are centimetres apart, so without this the Furby hears itself,
// replies to itself, and does so forever.
#define HALF_DUPLEX_DEFAULT     true
