# 04 — Power

## v0.1: use a USB power bank

Do not start this project by designing a battery system.

For the portable prototype, use a normal **5,000–10,000mAh USB power bank** and short cables.

Why:

- charging is already solved
- cell protection is already solved
- portable immediately
- easy to replace
- lets us measure real runtime before designing anything custom

## Basic layout

```text
USB power bank
   ├── CrowPanel
   └── CamS3 / camera power path
```

If the chosen power bank only has one usable output, use a small powered split/hub only after verifying it behaves correctly. Two independent outputs is simpler.

## CrowPanel battery option — later

Elecrow documents a **3.7 V SH1.0 2-pin BAT connector with an onboard charging circuit** on the 2.13-inch CrowPanel.

That makes a dedicated internal Li-ion/LiPo pack possible later, but it is not automatically the best first choice. We still need to know:

- finished-system current draw
- camera power arrangement
- desired runtime
- safe mounting location
- charging access
- whether original Furby movement is added

## Camera power

The CamS3 should initially be powered exactly as its normal development setup expects. Avoid modifying power rails until camera + microphone + Wi-Fi are confirmed stable on the desk.

## Outdoor priority

The actual portable chain is:

```text
power bank → Furby electronics
phone battery → hotspot / mobile data
```

Keep those independent in v0.1. Furby should not charge the phone and the phone should not be required to power Furby.

## Power-saving strategy

E-paper is ideal for the portable build because it needs no backlight and can retain the displayed image without continuous screen power. The firmware should therefore avoid pointless refreshes.

Suggested UI policy:

- refresh immediately on major state changes
- use partial refresh where supported
- do not animate continuously
- update battery/network indicators periodically, not every second
- let the display remain on the last useful status while idle

## Before switching to an internal cell

Record:

1. CrowPanel current at idle
2. CrowPanel current during refresh/Wi-Fi activity
3. CamS3 current idle
4. CamS3 current during capture/transmission
5. total runtime from the chosen power bank

Only then choose a dedicated cell.
