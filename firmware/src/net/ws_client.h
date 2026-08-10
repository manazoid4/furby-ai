// ws_client.h - wifi and the WebSocket link to the brain.
//
// Frame formats are specified in docs/06-protocol.md and must match
// brain/furbybrain/protocol.py exactly.

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "types.h"

// Connects wifi, then the WebSocket. Non-blocking; ws_loop() drives it.
void ws_begin(StateCb on_state, FaceCb on_face, CaptionCb on_caption,
              BacklightCb on_backlight, AudioCb on_audio);

// Pumps the socket, the heartbeat and the reconnect backoff.
void ws_loop();

bool ws_connected();
int ws_rssi();

// Binary mic frame: 4-byte header + 16-bit LE PCM. Dropped silently when the
// socket is down - buffering mic audio through an outage is worse than losing
// it, because it arrives later as a stale burst.
void ws_send_audio(const int16_t* pcm, size_t samples);

void ws_send_touch(int x, int y);
void ws_send_status();
void ws_send_error(const char* code, const char* msg);

// Local VAD thinks the user stopped talking. The brain's VAD is authoritative;
// this is a hint that lets it cut latency.
void ws_send_eos();
