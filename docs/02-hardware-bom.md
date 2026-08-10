# 02 — Hardware BOM

This BOM follows the project rule: **buy the fewest complete modules that get a portable v0.1 working.**

## Buy first

| # | Part | Job | Buy now? |
|---|---|---|---|
| 1 | **Elecrow CrowPanel ESP32 2.13" e-paper HMI — DIE01021S** | Main Furby controller + stomach display | Yes |
| 2 | **M5Stack Unit CamS3-5MP — U174-B** | 5MP camera + built-in microphone + microSD + second ESP32-S3 | Yes |
| 3 | **5,000–10,000mAh USB power bank** | Portable power without designing a battery circuit | Yes |
| 4 | **Short USB cables** | Programming + internal power | Yes |
| 5 | **3M VHB / removable mounting tape** | Fast mounting | Yes |
| 6 | **Thin ABS/acrylic/foam sheet** | Backing plate behind the existing stomach opening | Yes |
| 7 | **16–32GB microSD card** | Optional camera capture/offline storage | Useful |

## Main controller: Elecrow 2.13-inch e-paper CrowPanel

Verified from Elecrow's current documentation:

- ESP32-S3, up to 240 MHz
- 8 MB flash
- 8 MB PSRAM
- Wi-Fi
- 122 × 250 black/white e-paper
- partial refresh
- no backlight required
- UART0
- GPIO expansion
- SH1.0 2-pin 3.7 V battery connector
- onboard charging circuit
- menu/back/rotary/reset/boot controls

This board becomes the **body/status controller**. It is not an all-in-one audio/video board.

## Camera + microphone: M5Stack Unit CamS3-5MP

Verified from M5Stack's current documentation:

- ESP32-S3-WROOM-1-N16R8
- 16 MB flash
- 8 MB PSRAM
- 5 MP PY260 camera
- up to 2592 × 1944 stills
- 88° diagonal field of view
- built-in PDM microphone
- microSD slot
- Wi-Fi image transmission
- 40 × 24 × 11 mm
- supplied Grove2USB-C programming adapter and cable

This board becomes the **eyes + ears module**.

## Do NOT buy yet

| Part | Why not yet |
|---|---|
| External Wi-Fi antenna hardware | Test real Wi-Fi range first. RF surgery is not a lazy v0.1 task. |
| Separate ESP32 dev board | The CrowPanel and CamS3 already contain ESP32-S3s. |
| Raspberry Pi | Adds cost, power and software without helping the first milestone. |
| LTE/4G modem | Use phone hotspot. |
| GPS module | Use phone GPS/context later. |
| Dual MEMS microphone array | Validate the CamS3 microphone first. |
| Custom PCB | Premature until the final peripheral set is known. |
| LiPo pack + custom BMS | Use a USB power bank first. |
| Motor driver | Movement is deliberately postponed. |
| Replacement Furby eye displays | Cosmetic stretch goal. |

## Speaker / voice output

The 2.13-inch CrowPanel does **not** replace a speaker system. For the first prototype, TTS can play through the PC or phone while the rest of the physical loop is validated.

After v0.1, choose one of these:

1. **Lazy option:** tiny self-contained Bluetooth speaker hidden inside Furby, driven by the phone/PC audio route.
2. **Integrated option:** small I2S amplifier + speaker controlled by the CrowPanel. Better final product, but adds wiring/soldering.

Do not block the entire project on Furby having an internal speaker on day one.

## Mounting materials

The stomach opening is already approximately four inches across, so the display should be mounted to a backing plate rather than relying on the fur/plastic edge itself.

Recommended v0.1 stack:

```text
fur / shell edge
      ↓
thin backing plate
      ↓
VHB / removable tape
      ↓
CrowPanel
```

Do not 3D-print a bezel until the real board has been held against the Furby and the final screen position is confirmed.

## Power rule

Use a normal USB power bank first. It gives you charging, protection and portability with effectively zero battery engineering.

Once the completed electronics are measured for real current draw and internal fit, a dedicated 3.7 V cell can be considered.
