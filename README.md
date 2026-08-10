# furby-ai

Turning a 2013 Furby Boom (A6847, "Purple Waves") into an AI companion —
**without soldering anything.**

One board goes in the belly. It is the screen, the ears, the voice and the
computer, all at once. You cut a rectangular hole, stick the panel in, plug in
USB-C. That's the build.

All the thinking — speech-to-text, LLM, tool calls, text-to-speech — happens on
a PC on your LAN, or a cloud API if you'd rather pay than run a GPU.

> **Status:** scaffold. Nothing has been cut yet. Hardware claims in
> `docs/01-teardown.md` are tagged **VERIFIED** or **ASSUMED** — trust only the
> verified ones.

---

## The whole hardware build

| # | Part | Job |
|---|---|---|
| 1 | **CrowPanel Advance 4.3"** (ESP32-S3, 800×480 IPS touch) | Screen, microphone, speaker and microcontroller **in one board**. USB-C powered. |
| 2 | **USB-C power bank or 1S LiPo** | Power. Plug-in, no wiring. |
| 3 | **XIAO ESP32S3 Sense** (optional) | Eyes. Camera + mic, USB-C, streams over wifi. |
| 4 | **VHB tape, USB-C cable** | Assembly. |

No soldering iron. No motor driver. No breadboard. No separate e-ink module,
no separate microphones, no amplifier — the panel already has all of it.

## What it does (target)

| Capability | How |
|---|---|
| Hears you across a room | Panel's onboard I2S mic, streamed to the PC |
| Understands you | faster-whisper on the PC |
| Thinks | Local LLM via Ollama, or Claude / OpenAI over API |
| Talks back | Piper (local) or ElevenLabs, out of the panel's onboard speaker |
| Shows a face | 800×480 colour touchscreen in the belly — animated eyes, moods, subtitles |
| Sees (optional) | XIAO ESP32S3 Sense streaming frames to a vision model |
| Runs commands | Allow-listed tool registry on the PC. **Never raw shell.** See `docs/07` |
| Strong wifi | ESP32-S3 with an optimised onboard RF antenna |

## Repo layout

```
docs/        the build guide, one file per stage
firmware/    CrowPanel ESP32-S3 firmware (PlatformIO)
brain/       Python server: WebSocket + VAD + STT + LLM + TTS + tools
hardware/    BOM, panel pinout, cutting template
```

## Quick start (brain only, no hardware needed)

```bash
cd brain
pip install -e ".[dev]"
cp .env.example .env
pytest                      # the protocol and tool-registry tests
furby-brain                 # ws://0.0.0.0:8765
```

## Quick start (firmware)

```bash
cd firmware
cp include/secrets.example.h include/secrets.h   # wifi + brain host
pio run -t upload && pio device monitor
```

## Docs

1. [Overview and goals](docs/00-overview.md)
2. [Teardown: what's inside, and what we can ignore](docs/01-teardown.md)
3. [Bill of materials](docs/02-hardware-bom.md)
4. [Cutting the hole and mounting the panel](docs/03-wiring.md)
5. [Power](docs/04-power.md)
6. [System architecture](docs/05-architecture.md)
7. [Wire protocol](docs/06-protocol.md)
8. [Safety and security](docs/07-safety-and-security.md)
9. [Build log](docs/08-build-log.md)
10. [YouTube series plan](docs/09-youtube-plan.md)

## Legal

Furby(R) is a trademark of Hasbro, Inc. This project is an unaffiliated,
non-commercial modification of a toy the author owns. No Hasbro firmware, audio
or artwork is redistributed here. MIT licensed, see [LICENSE](LICENSE).
