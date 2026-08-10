// config.h - verified board pins and small v0.1 tunables.
//
// Target: Elecrow CrowPanel ESP32 2.13" E-Paper HMI (DIE01021S).
//
// The original repo targeted a different 4.3" colour CrowPanel and contained
// placeholder audio/backlight pins. Those have been removed. Values below are
// based on Elecrow's documentation / official hardware repo for the 2.13"
// e-paper board.

#pragma once

#include <stdint.h>

// ---------------------------------------------------------------- identity --
#define FW_VERSION      "0.3.0-dev"
#define FURBY_ID        "furby-body-01"
#define FURBY_ROLE      "body"

// ------------------------------------------------------------- e-paper I/O --
#define EPD_PIN_SCK      12
#define EPD_PIN_MOSI     11
#define EPD_PIN_RES      10
#define EPD_PIN_DC       13
#define EPD_PIN_CS       14
#define EPD_PIN_BUSY      9

#define EPD_WIDTH        122
#define EPD_HEIGHT       250

// ---------------------------------------------------------- onboard controls --
#define PIN_MENU          2
#define PIN_ROTARY_DOWN   4
#define PIN_ROTARY_UP     6
#define PIN_ROTARY_CONF   5
#define PIN_BACK          1

// Elecrow exposes two general-purpose GPIOs on GPIO_D.
#define PIN_GPIO_D0      40
#define PIN_GPIO_D1      41

// --------------------------------------------------------------- wire/audio --
// The existing brain protocol has binary PCM frame support. Keep the framing
// constants so the shared protocol code still compiles, but this body board
// does not provide the old scaffold's onboard microphone/speaker path.
#define AUDIO_SAMPLE_RATE   16000
#define AUDIO_FRAME_SAMPLES 320
#define AUDIO_FRAME_BYTES   (AUDIO_FRAME_SAMPLES * 2)

// --------------------------------------------------------------- behaviour --
#define STATUS_INTERVAL_MS      30000
#define WS_PING_INTERVAL_MS      5000
#define WS_RECONNECT_MIN_MS      1000
#define WS_RECONNECT_MAX_MS     30000

// E-paper should update on meaningful state changes, not as an animation loop.
#define EPD_MIN_REFRESH_GAP_MS    500
