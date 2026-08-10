// spk_i2s.h - I2S playback into a MAX98357A driving the stock Furby speaker.
//
// The brain streams PCM down in bursts; this module owns the jitter buffer
// that turns bursty wifi into a continuous sample clock.

#pragma once

#include <stddef.h>
#include <stdint.h>

// Installs I2S port 1 in TX master mode, 16 kHz, 16-bit mono, and allocates
// the jitter buffer (PSRAM if available).
void spk_begin();

// Queues PCM from the network. Returns the number of samples accepted; a short
// return means the buffer is full and the brain is ahead of us.
size_t spk_push(const int16_t* pcm, size_t samples);

// Moves buffered samples into the I2S DMA. Call tightly from the audio_out
// task. Returns false when there was nothing to do, so the caller can yield.
bool spk_pump();

// Samples currently queued. Divide by AUDIO_SAMPLE_RATE for seconds of audio
// still to play.
size_t spk_queued();

// Drop everything queued - used on barge-in, when the user interrupts.
void spk_flush();

// True while there is audio left to play. Drives the half-duplex mic gate.
bool spk_active();
