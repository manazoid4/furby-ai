// mic_i2s.h - stereo I2S MEMS microphone capture (2x ICS-43434).
//
// Wiring: docs/03-wiring.md. Both mics share BCLK/WS/SD; the L/R pin on each
// breakout selects which half of the frame it drives.

#pragma once

#include <stddef.h>
#include <stdint.h>

// Installs I2S port 0 in RX master mode, 16 kHz, 32-bit slots, stereo.
void mic_begin();

// Blocks until one frame is available. Downmixes L+R to mono 16-bit PCM.
// Returns false on a read error or short read - the caller should back off,
// not spin.
bool mic_read_frame(int16_t* out, size_t samples);

// Last frame's peak absolute amplitude (0..32767). Useful for a level meter on
// the e-ink and for eyeballing mic gain during bring-up.
uint16_t mic_last_peak();

// Stream the left channel raw instead of the mono downmix. For beamforming /
// direction-of-arrival work later.
void mic_set_stereo_debug(bool on);
