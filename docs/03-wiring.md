# 03 — Mounting and wiring

## v0.1 rule

**Do not wire the two ESP32-S3 boards together unless there is a reason.**

For the first build:

```text
CrowPanel ── Wi-Fi ──┐
                     ├── PC / phone / cloud bridge
CamS3 ───── Wi-Fi ───┘
```

That is simpler to debug than making the CrowPanel proxy camera/audio traffic.

## Stomach mount

The Furby's stomach already has a neat rectangular opening roughly four inches across.

Use a simple sandwich:

```text
outside

fur / shell edge
      ↓
┌─────────────────────┐
│ thin backing plate  │
│   ┌─────────────┐   │
│   │  CROWPANEL  │   │
│   │    E-INK    │   │
│   └─────────────┘   │
└─────────────────────┘
      ↑ VHB / removable tape

inside
```

### Recommended process

1. Keep the CrowPanel complete in its acrylic case for the first fit test.
2. Cut a thin ABS/acrylic/foam backing plate slightly larger than the existing opening.
3. Put the plate behind the opening.
4. Centre the visible e-paper area before attaching anything permanently.
5. Use removable tape first.
6. Only switch to VHB once USB access, buttons and viewing angle are confirmed.
7. Leave enough slack to remove the panel for reflashing.

No bezel/CAD is required for v0.1.

## CrowPanel pins we know from Elecrow

For the exact 2.13-inch board, Elecrow's hardware repo documents the e-paper connection as:

```text
EPD SCK   GPIO12
EPD MOSI  GPIO11
EPD RES   GPIO10
EPD DC    GPIO13
EPD CS    GPIO14
EPD BUSY  GPIO9
```

User controls exposed by Elecrow's wiki:

```text
MENU          GPIO2
ROTARY DOWN   GPIO4
ROTARY UP     GPIO6
ROTARY CONF   GPIO5
EXIT/BACK     GPIO1
GPIO_D        GPIO40 / GPIO41
```

Treat these as board definitions in firmware, not Furby wiring.

## CamS3

The M5Stack Unit CamS3-5MP can be treated as a standalone Wi-Fi peripheral.

For v0.1:

1. Program/configure it on the desk using the supplied Grove2USB-C adapter.
2. Join the same Wi-Fi/hotspot as the CrowPanel.
3. Confirm a still image can be viewed/fetched on the network.
4. Confirm audio recording works.
5. Only then mount it in Furby.

Suggested physical placement: keep the board inside the head and expose only the camera lens through a small existing gap or a minimally invasive opening.

## Optional UART later

If Wi-Fi coordination becomes annoying, the CamS3 exposes a 4-pin HY2.0/Grove port:

```text
GND
5V
G20
G19
```

M5Stack documents G19/G20 as the serial/USB data pins for this unit. A future version could connect CrowPanel UART to the camera for command/control, but this is **not required for v0.1**.

## Furby controls later

The first Furby-native input worth wiring is the **tongue switch** because it has a genuinely useful role: physical approval/cancel.

Do this only after the e-paper + camera + network loop works.

Target interaction:

```text
DELETE 34 FILES?

PRESS TONGUE
TO APPROVE
```

The bridge remains responsible for enforcing the permission policy; a switch press is merely one approval factor, not unrestricted shell access.
