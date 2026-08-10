# furby-ai

A portable AI control toy built inside a **Furby Boom A6847 (Purple Waves)**.

The goal is deliberately lazy: use complete modules, tape/adhesive before custom brackets, USB power before custom battery electronics, and let a PC/phone/cloud service do the expensive AI work.

> **Current physical status:** the Furby's stomach already has a neat ~4-inch rectangular opening. The build is now centred on a **2.13-inch Elecrow CrowPanel ESP32-S3 e-paper module** mounted in that opening, with a separate **M5Stack Unit CamS3-5MP** for camera + microphone.

## The idea

```text
                         FURBY

        ┌─────────────────────────────────┐
        │ CrowPanel ESP32-S3              │
        │ 2.13" e-paper                   │
        │ status / buttons / control      │
        └──────────────┬──────────────────┘
                       │ Wi-Fi
        ┌──────────────┴──────────────────┐
        │ CamS3-5MP                       │
        │ camera + PDM microphone + SD    │
        └──────────────┬──────────────────┘
                       │ Wi-Fi
                       ▼
              phone hotspot / home Wi-Fi
                       │
          ┌────────────┴────────────┐
          ▼                         ▼
      PC / local AI            cloud agents
      Ollama / tools           hosted LLMs
      local agents             vision / TTS
```

The ESP32 boards are **peripherals, not the main AI brain**. They capture context, show status, expose physical controls and talk to a bridge. Reasoning stays on stronger hardware.

## Why two ESP32-S3 modules?

- **CrowPanel:** owns the stomach UI, buttons, future Furby sensors and command status.
- **CamS3:** owns camera capture, microphone/audio recording and optional microSD storage.
- Camera work cannot bog down the display/controller.
- Either module can reboot or be replaced independently.
- In v0.1 they do not even need a wired data connection: both can simply join the same Wi-Fi network.

## Hardware target — v0.1

| Part | Job |
|---|---|
| Furby Boom A6847 Purple Waves | Body / shell / future physical controls |
| Elecrow CrowPanel ESP32 2.13" e-paper | Main body controller + sunlight-readable stomach UI |
| M5Stack Unit CamS3-5MP (U174-B) | 5MP camera + microphone + microSD + second ESP32-S3 |
| 5,000-10,000mAh USB power bank | Portable power without a custom charging circuit |
| Short USB cables | Internal power/programming |
| VHB / removable mounting tape | Fast mounting |
| Thin ABS/acrylic/foam sheet | Simple backing plate for the existing stomach opening |

### Intentionally not required for v0.1

- custom PCB
- Raspberry Pi inside the Furby
- LTE/GPS modules — use the phone
- external Wi-Fi antenna modification
- original Furby motor reverse-engineering
- replacement eye displays
- dual microphone array

Those can become later upgrades only after the basic loop works.

## Operating modes

### Home

`Furby -> home Wi-Fi -> PC -> local agents / Ollama`

### Outside

`Furby -> phone hotspot -> cloud AI`

### Remote-home

`Furby -> phone hotspot -> secure bridge -> home PC -> local agents`

Remote-home is a later milestone. The first portable version only needs the phone hotspot and a cloud endpoint.

## First milestone

**v0.0.1 = PC -> Furby e-paper.**

Nothing else matters until this works:

```text
PC sends status
      ↓ Wi-Fi
CrowPanel receives it
      ↓
FURB-1
ONLINE
```

Then add, in this order:

1. CamS3 still image -> PC/vision model
2. CamS3 microphone/audio capture
3. phone-hotspot portable mode
4. agent routing + safe PC tools
5. onboard speaker/audio output
6. tongue switch as physical approve/cancel
7. original Furby movement

## Repository

```text
docs/        build decisions, BOM, architecture, safety and build log
firmware/    CrowPanel ESP32-S3 firmware
brain/       PC-side Python bridge / agent router
```

The existing Python protocol scaffold remains useful, but the firmware is being migrated away from the abandoned 4.3-inch colour-panel architecture.

## Safety model

The Furby should never expose an unrestricted shell endpoint. PC actions go through an allow-listed tool layer; destructive/high-impact actions can later require a **physical confirmation** such as pressing the Furby's tongue.

See [`docs/07-safety-and-security.md`](docs/07-safety-and-security.md).

## Docs

1. [Overview / lazy-build rules](docs/00-overview.md)
2. [Teardown notes](docs/01-teardown.md)
3. [Hardware BOM](docs/02-hardware-bom.md)
4. [Mounting and wiring](docs/03-wiring.md)
5. [Power](docs/04-power.md)
6. [Architecture](docs/05-architecture.md)
7. [Protocol](docs/06-protocol.md)
8. [Safety and security](docs/07-safety-and-security.md)
9. [Build log](docs/08-build-log.md)
10. [YouTube plan](docs/09-youtube-plan.md)
11. [Portable/outdoor mode](docs/10-portable-mode.md)

## Legal

Furby® is a trademark of Hasbro, Inc. This project is unaffiliated with Hasbro and documents modification of a privately owned toy. No Hasbro firmware, audio or artwork is redistributed. MIT licensed; see [LICENSE](LICENSE).
