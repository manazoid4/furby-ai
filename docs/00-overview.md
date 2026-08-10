# 00 — Overview

## One-line pitch

Turn a Furby Boom A6847 into a **portable physical AI terminal** that can see, hear, show status, talk to local/cloud agents and eventually approve PC actions with Furby's own physical controls.

## Non-negotiable design rule: lazy first

This project should be finishable, not impressive-on-paper.

1. **Buy modules that already solve hard problems.**
2. **Wi-Fi before wiring.** If two boards can communicate through the PC/phone, do that before soldering them together.
3. **Tape before CAD.** VHB/removable tape + a flat backing plate is v0.1 mounting.
4. **USB power before battery engineering.** A power bank is the first portable power system.
5. **PC/phone/cloud does the thinking.** ESP32-S3 boards handle peripherals and control, not the main LLM.
6. **One new capability at a time.** Every milestone must leave a working Furby.
7. **No irreversible work unless required.** The stomach opening already exists; avoid further shell surgery until the electronics work on the desk.

## Current physical state

- Furby: Boom A6847, Purple Waves.
- A neat ~4-inch rectangular stomach opening has already been cut.
- The planned stomach module is the **Elecrow CrowPanel ESP32 2.13-inch e-paper HMI**.
- Camera/audio capture is delegated to a separate **M5Stack Unit CamS3-5MP**.

## Why the architecture changed

The original repo scaffold assumed a 4.3-inch colour CrowPanel with onboard mic/speaker and treated that single panel as the whole machine. That is no longer the build.

The 2.13-inch CrowPanel is better suited to the actual project because it gives us:

- ESP32-S3 controller
- e-paper that remains visible outdoors without a backlight
- low-power partial refresh
- 8 MB flash + 8 MB PSRAM
- Wi-Fi
- UART/GPIO expansion
- onboard controls
- 3.7 V battery connector/charging support

It does **not** solve camera/microphone/speaker by itself, so v0.1 separates those jobs instead of pretending it is an all-in-one board.

## Module responsibilities

### CrowPanel = body controller

Owns:

- stomach e-paper
- network status
- agent/activity state
- simple local menus
- future tongue/tail/touch inputs
- future motor commands
- future battery/status reporting

### CamS3 = eyes + ears

Owns:

- still images / camera stream
- microphone/audio recording
- microSD capture
- its own Wi-Fi connection

### PC / cloud = brain

Owns:

- speech-to-text
- LLM/agent routing
- local models
- vision model calls
- tool execution
- memory/context
- text-to-speech generation

### Phone = outdoor gateway

Owns:

- hotspot
- mobile data
- optional GPS/context
- optional TTS output during early prototypes

## v0.1 — what success means

Do not wait for a fully animated Furby.

A successful v0.1 does this:

1. CrowPanel boots and joins Wi-Fi.
2. PC sends a status message.
3. E-paper changes between `OFFLINE`, `ONLINE`, `LISTENING`, `THINKING`, `DONE`, `ERROR`.
4. CamS3 captures a still image that reaches the PC.
5. CamS3 captures microphone audio.
6. Both modules work through a phone hotspot outdoors.
7. The PC/cloud agent can return a useful response.

## v0.1 intentionally does NOT require

| Feature | Why postponed |
|---|---|
| Original Furby movement | Reverse-engineering motor/cam/encoder adds risk before the AI loop exists. |
| Replacement eyes | Cosmetic scope explosion. |
| External Wi-Fi antenna | Only add if real-world testing proves the onboard antenna inadequate. |
| Cellular modem | Phone already provides cellular data. |
| GPS module | Phone already has GPS. |
| Custom PCB | Unnecessary until the wiring stabilises. |
| Custom Li-ion pack | Power bank is faster and safer for prototyping. |
| Dual mic array | CamS3 built-in mic is enough to validate the concept. |

## Milestones

- **M0 — Repo truth:** docs match the actual hardware and physical Furby.
- **M1 — Desk display:** CrowPanel shows PC-sent status over Wi-Fi.
- **M2 — Camera:** CamS3 still image reaches the PC/vision model.
- **M3 — Hearing:** CamS3 microphone reaches STT.
- **M4 — Portable:** same loop works over phone hotspot + power bank.
- **M5 — Agent:** local/cloud routing and safe PC tools work.
- **M6 — Voice:** onboard or phone-assisted TTS output.
- **M7 — Physical approval:** tongue switch confirms sensitive actions.
- **M8 — Movement:** reuse Furby mechanics only after everything above works.

## First command to optimise for

A useful demo is more important than a generic chatbot:

> “Furby, check my PC and tell me what needs attention.”

The response should be short, appear on the stomach display, and optionally be spoken. Later, Furby's tongue can act as the physical confirmation control for actions.
