# 05 - System architecture

## Shape

```
  +------------------ FURBY (CrowPanel Advance 4.3") -------------------+
  |                                                                      |
  |  onboard I2S mic --> ring buffer --> PCM frames ---------------+      |
  |                                                                |      |
  |  onboard speaker <-- I2S TX <-- jitter buffer <-- audio frames |      |
  |  800x480 screen  <-- face renderer <----------- face frames  <-+      |
  |  touchscreen     --> event frames -----------------------------+      |
  |                                                                |      |
  +---------------------------- WebSocket -------------------------|-----+
                                                                   |
  +------------- XIAO ESP32S3 Sense (optional "eyes") ------------- |     |
  |  camera --> JPEG frames --------------------------------------> |     |
  +---------------------------------------------------------------|-----+
                                                                   |
  +--------------------- BRAIN (your PC) -------------------------- v ---+
  |                                                                      |
  |   ws server  -->  VAD (silero) --> STT (faster-whisper)               |
  |                                          |                           |
  |                                     transcript                       |
  |                                          v                           |
  |                     persona + memory + history (+ last camera frame)  |
  |                                          v                           |
  |                        LLM (Ollama local | Claude | OpenAI)           |
  |                          |                        |                  |
  |                     tool calls              text (streamed)          |
  |                          v                        v                  |
  |                  tool registry            sentence chunker           |
  |                  (ALLOW-LISTED)                   v                  |
  |                          |                 TTS (Piper | 11Labs)      |
  |                          +-----------------------+                   |
  |                                          v                           |
  |                     face + audio frames back to the panel            |
  +----------------------------------------------------------------------+
```

Two devices, one brain. The camera is a second WebSocket client, not a hardware
change to the panel — which is why it stays optional and why adding it needs no
tools.

## Why the brain is on the PC

- Model swapping without a reflash.
- Debugging a Python pipeline beats debugging a microcontroller.
- Tool execution belongs where the tools are — your filesystem, your apps.
- The panel's ESP32-S3 is busy driving an 800×480 display; it has no spare
  cycles for inference and never will.

The cost: the Furby is a brick without the network. Accepted. The firmware
keeps a "can't reach brain" face and a few canned lines.

## Firmware task layout (FreeRTOS)

| Task | Core | Priority | Job |
|---|---|---|---|
| `audio_in` | 1 | high | I2S RX from the onboard mic, ring buffer |
| `audio_out` | 1 | high | I2S TX to the onboard speaker from the jitter buffer |
| `net` | 0 | medium | wifi, WebSocket, framing, reconnect with backoff |
| `ui` | 0 | low | face rendering, touch events, backlight |

Audio on core 1, away from the wifi stack on core 0. Underruns are audible;
dropped protocol frames are not.

Note there is no `motion` task. There is no motor. See `00-overview.md` for why.

## Brain module map

| File | Status | Job |
|---|---|---|
| `protocol.py` | **implemented + tested** | frame encode/decode, shared with the firmware |
| `server.py` | planned | WebSocket server, one `Session` per device |
| `pipeline.py` | planned | orchestrates VAD → STT → LLM → tools → TTS, streaming |
| `stt.py` | planned | faster-whisper wrapper, pluggable |
| `llm.py` | planned | Ollama / Anthropic / OpenAI behind one interface |
| `tts.py` | planned | Piper / ElevenLabs behind one interface, chunked |
| `tools.py` | planned | the allow-list. Read `07-safety-and-security.md` first. |
| `persona.py` | planned | system prompt, mood state machine, expression vocabulary |
| `vision.py` | planned | camera frames → vision model, on demand only |
| `simulator.py` | planned | fake Furby using your laptop mic and speakers |

Built strictly test-first, one behaviour at a time. `protocol.py` is done;
everything else is a stub until its tests exist.

## Conversation state machine

```
IDLE --speech detected--> LISTENING --silence--> THINKING --first audio--> SPEAKING
  ^                          |                       |                        |
  |                          +-- timeout ------------+                        |
  +-----------------------------------------------------------------------------+
                        (or: touch event / barge-in)
```

`SPEAKING` gates mic capture (half-duplex). This matters more here than in most
builds: the panel's microphone and speaker are centimetres apart, so without
the gate the Furby hears itself, replies to itself, and does that forever.

`THINKING` shows a thinking face so latency reads as personality rather than as
a hang. Genuinely important trick.

## The face

The old build had one motor and a fixed gesture vocabulary. The new build has
800×480 pixels, so "expression" is a rendering problem instead of a mechanical
one — strictly better, and free.

Expression vocabulary (the LLM picks one of these and nothing else):

`neutral`, `happy`, `curious`, `thinking`, `surprised`, `sleepy`, `confused`,
`error`

Defined in `brain/furbybrain/persona.py`, rendered in
`firmware/src/display/face.cpp`. Keep the list small and closed — it is what
makes the model reliable, and it means a typo fails loudly instead of producing
a blank screen.

Rendering approach: two eyes and a mouth drawn with primitives, animated by
interpolating between expression keyframes. Do not attempt a photo-real face —
a crude cartoon face that blinks reads as alive; an almost-real one reads as
broken.
