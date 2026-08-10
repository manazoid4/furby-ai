# 02 - Bill of materials

Rough UK/GBP, 2026. The entire required build is **four lines**.

## Required

| # | Part | Why this one | Approx |
|---|---|---|---|
| 1 | **Elecrow CrowPanel Advance 4.3"** — ESP32-S3, 800×480 IPS capacitive touch | The whole build in one board: microcontroller, screen, **onboard I2S microphone**, **onboard speaker + amplifier**, USB-C, LiPo JST connector, Crowtail I2C/UART ports, 16 MB flash / 8 MB PSRAM, onboard RF antenna. This single choice is what removes the soldering. | £45-55 |
| 2 | **USB-C power bank**, 5 V, 2 A+, 5000-10000 mAh | Power, with no charging circuit to build. Any phone power bank works. Slim ones fit the Furby's cavity better. | £10-15 |
| 3 | **USB-C cable, short right-angle** | Panel to power bank inside the shell. Right-angle so it doesn't bulge. | £5 |
| 4 | **3M VHB double-sided tape** (or Command strips) | Mounts the panel. Genuinely strong enough; this is what phone screens are held on with. | £6 |

**Required total: roughly £70.**

## Optional — the camera ("eyes")

You asked about a mini wireless Bluetooth camera. Straight answer: **Bluetooth
cameras are not really a thing.** BLE bandwidth cannot carry usable video, so
anything sold as a "mini wireless camera" is wifi. Which is fine — wifi is what
we want anyway.

| Part | Why | Approx |
|---|---|---|
| **Seeed XIAO ESP32S3 Sense** | Best option by a distance. Camera **and** microphone on a thumbnail-sized board, USB-C powered, wifi, no soldering, and it ships with the camera ribbon attached. Streams JPEG frames to the PC where a vision model looks at them. Mount it in the beak or an eye socket. | £22 |
| ESP32-CAM (AI-Thinker) | Half the price, but needs a separate USB programmer and is fiddly to flash. Not lazy. | £8 + £5 |
| Any wifi "spy cam" with RTSP | Zero build effort, but you're putting an unknown Chinese cloud camera in your living room. Read `docs/07`. | £15 |

Take the XIAO. It talks to the same brain over the same wifi, so the repo
supports it as a second WebSocket client rather than as a hardware change.

## Optional — nice to have

| Part | For | Approx |
|---|---|---|
| 1S LiPo 2000-3000 mAh with JST-PH plug | Replaces the power bank; plugs straight into the panel's BAT connector. Tidier, but it's a lithium cell in a fur toy — read the safety doc. | £12 |
| PETG/PLA filament | Printed bezel to hide the cut edge and take up the belly curvature. | £- |
| Craft foam sheet, 2-3 mm | The no-printer version of the bezel. Cut a rectangle, cut the middle out. | £3 |
| Command strips | Removable mounting if you want to undo it later. | £5 |

## Tools

| Tool | Note |
|---|---|
| Craft knife / scalpel | The main tool. Sharp blade, many light passes. |
| Small drill or hole punch | Drill the four corners first, then cut between them. Stops the plastic cracking. |
| Metal ruler, fine marker | Marking the rectangle. |
| Needle file or sandpaper | Cleaning up the cut edge. |
| Masking tape | Tape over the belly before marking — protects the plastic and takes pencil. |

**No soldering iron. No multimeter needed** (except to confirm the batteries are
out). No bench supply, no logic analyser.

## What runs the brain

No purchase needed if you have a decent PC.

- **Local, GPU:** an NVIDIA card with 6 GB+ VRAM runs faster-whisper `small.en`
  plus a 7-8B model comfortably.
- **Local, weak GPU (e.g. GTX 1660 Ti, 6 GB):** stick to a 4B-class model and
  modest context, or it spills to CPU and latency collapses.
- **Cloud:** Claude or OpenAI for the LLM, ElevenLabs for TTS. Pennies per
  conversation, no GPU, but the Furby dies when your internet does.

## Ordering discipline

Order the panel first and **hold it against the belly before cutting anything**.
If 4.3" doesn't fit, the 3.5" board (480×320) is the same family, same firmware,
same connectors — only the hole gets smaller.
