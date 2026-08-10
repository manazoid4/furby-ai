# 08 — Build log

This file should stay brutally factual. Record what physically happened, what was tested, and what is still only planned.

## 2026-08-10 — project direction corrected

### Furby

- Model: **Furby Boom A6847**
- Colour: **Purple Waves**
- Stomach: a neat rectangular opening approximately **4 inches across has already been cut**.

### Architecture decision

The original scaffold assumed a 4.3-inch colour CrowPanel Advance with onboard microphone/speaker and treated it as the complete electronics package.

That plan has been abandoned.

The current v0.1 plan is:

```text
Elecrow 2.13" e-paper CrowPanel ESP32-S3
    = stomach display + body controller

M5Stack Unit CamS3-5MP
    = camera + microphone + microSD

USB power bank
    = portable power

phone hotspot / home Wi-Fi
    = connectivity

PC / cloud
    = AI + agents + tools + STT/TTS
```

### Why

- matches the hardware actually being considered/purchased
- far less fabrication
- e-paper works well outdoors
- two specialised ESP32 modules avoid camera/display contention
- no custom PCB required
- no need to reverse-engineer Furby mechanics before the AI loop works

## Current verified hardware facts

### Elecrow CrowPanel 2.13-inch

Verified from Elecrow documentation/hardware repo:

- ESP32-S3
- 8 MB flash
- 8 MB PSRAM
- 122 × 250 monochrome e-paper
- Wi-Fi
- partial refresh
- UART/GPIO
- 3.7 V BAT connector with charging circuit
- EPD pins: SCK 12, MOSI 11, RES 10, DC 13, CS 14, BUSY 9

### M5Stack Unit CamS3-5MP

Verified from M5Stack documentation:

- ESP32-S3-WROOM-1-N16R8
- 16 MB flash
- 8 MB PSRAM
- 5 MP camera
- built-in PDM mic
- microSD
- Wi-Fi
- 40 × 24 × 11 mm
- supplied Grove2USB-C programmer

## Next physical actions

Do these in order and record results here:

- [ ] receive CrowPanel
- [ ] power it on untouched on the desk
- [ ] run Elecrow factory/example firmware
- [ ] display `FURB-1 / ONLINE`
- [ ] connect CrowPanel to home Wi-Fi
- [ ] make PC change the displayed state
- [ ] temporary-fit CrowPanel behind stomach opening
- [ ] receive/configure CamS3
- [ ] view CamS3 image over Wi-Fi
- [ ] test CamS3 microphone
- [ ] mount camera temporarily
- [ ] run both from phone hotspot
- [ ] run both from portable power

## Do not claim yet

Until physically tested, do not say:

- final battery runtime
- microphone pickup distance
- Wi-Fi range inside the Furby shell
- final camera placement
- final CrowPanel mounting method
- original Furby motor compatibility
- onboard speaker solution

## Video rule

Film failures and ugly prototypes. Do not reconstruct them later.

Every time hardware changes:

1. 5-second wide shot
2. close-up of what changed
3. screen recording/serial output if relevant
4. immediate result

That creates the YouTube edit automatically while the project is being built.
