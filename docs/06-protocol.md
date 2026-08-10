# 06 - Wire protocol

WebSocket. The panel is a client, the brain is the server. The optional camera
is a second client of the same server.

- **Text frames** = JSON control messages.
- **Binary frames** = audio and camera JPEGs, with a 4-byte header.

Keep it boring. Two implementations (C++ and Python) must agree, so anything
clever costs double.

## Binary frame

```
byte 0     : magic  0xF0
byte 1     : type   0x01 = mic PCM (up)
                    0x02 = speaker PCM (down)
                    0x03 = camera JPEG (up)
byte 2-3   : seq    uint16 little-endian, wraps
byte 4..n  : payload
```

Audio payload: 16-bit signed LE PCM, 16 kHz mono. 20 ms frames = 320 samples =
640 bytes. Small enough for low latency, large enough that framing overhead
stays under 1%.

Camera payload: a whole JPEG, sent only when the brain asks. Video is not
streamed continuously — it would saturate the link and there is no reason for
a toy to watch you constantly (`07-safety-and-security.md`).

Implemented and tested in `brain/furbybrain/protocol.py`; malformed frames
return `None` rather than raising, so a flaky link costs one 20 ms slice
instead of the session.

## Control messages (JSON text frames)

Every message: `{"t": "<type>", ...}`.

### Device -> Brain

```json
{"t":"hello","fw":"0.1.0","id":"furby-01","role":"panel","token":"change-me","rssi":-52}
{"t":"hello","fw":"0.1.0","id":"furby-cam","role":"camera","token":"change-me","rssi":-58}
{"t":"eos"}
{"t":"event","kind":"touch","x":412,"y":233}
{"t":"status","rssi":-54,"heap":142000,"vbat":4.02,"state":"idle"}
{"t":"error","code":"i2s_underrun","msg":"speaker buffer starved"}
```

- `hello` — on connect. `role` tells the brain whether this is the panel or the
  camera, so one server handles both.
- `eos` — local VAD thinks the user stopped. The brain's VAD is authoritative.
- `event` — a screen touch. Touching the Furby's face wakes it.

### Brain -> Device

```json
{"t":"speak_begin","ms_estimate":2400,"half_duplex":true}
{"t":"speak_end"}
{"t":"face","expression":"curious","blink":true}
{"t":"caption","line1":"what's a badger?","line2":""}
{"t":"state","value":"thinking"}
{"t":"backlight","level":40}
{"t":"capture"}
{"t":"tool_ran","name":"timer.set","ok":true}
{"t":"ping","n":42}
```

- `speak_begin` — speech incoming; the panel mutes its mic if `half_duplex`.
- `face` — one of the fixed expressions: `neutral`, `happy`, `curious`,
  `thinking`, `surprised`, `sleepy`, `confused`, `error`. Unknown names are
  rejected by the encoder, not silently ignored by the firmware.
- `caption` — optional subtitle text under the face.
- `backlight` — 0-100. Used to dim on idle, which saves power and doubles as
  "the Furby fell asleep".
- `capture` — asks the camera client for a single frame. Sent only when the
  conversation actually needs to see something.

The panel replies to `ping` with `{"t":"pong","n":42}`. Three missed pongs =
reconnect.

## Reconnect

Exponential backoff: 1 s, 2 s, 4 s, 8 s, capped at 30 s, with jitter. On
reconnect the device sends `hello` again; the brain treats it as a fresh
session but keeps conversation memory keyed on `id`.

## Versioning

`hello.fw` carries the firmware version. The brain refuses connections whose
major version it doesn't know, and says so out loud through the speaker. A
Furby that silently misbehaves after a firmware update is a bad afternoon.

## Why WebSocket

MQTT needs a broker and carries realtime audio badly. gRPC has no comfortable
ESP32 story. ESP-NOW is lower latency but needs a dongle on the PC and cannot
carry the camera. WebSocket also means the simulator and a browser debug page
work for free.
