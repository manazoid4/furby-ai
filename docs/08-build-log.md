# 08 - Build log

Append-only. Newest at the top. One entry per session, however small.

This file is the raw material for the videos. Write it *while* you build, not
after — "the bit where it went wrong" is the content, and you will not remember
it accurately in a week.

Template:

```
## YYYY-MM-DD - short title
**Goal:**
**Did:**
**Broke:**
**Learned:**
**Next:**
**Footage:** (clip names / timestamps worth using)
```

---

## 2026-08-10 - Scope cut: no soldering
**Goal:** Simplify the build to something achievable in an afternoon.
**Did:** Replaced the entire discrete design (separate ESP32-S3 + 2.13" e-ink +
2× ICS-43434 mics + MAX98357A amp + DRV8833 motor driver + LiPo charging
circuit) with a single **CrowPanel Advance 4.3"** — ESP32-S3, 800×480 IPS touch,
onboard mic, onboard speaker and amp, USB-C, LiPo JST. Dropped motor control
entirely from v1. Rewrote docs 00-09 and stripped the firmware to match.
**Broke:** Nothing physical. Deleted the motion and e-ink firmware modules.
**Learned:**
- The originally linked Amazon part (B0FX4PZZMQ) is the CrowPanel **2.13" e-ink**,
  122×250, no mic, no speaker — too small for a 4-inch hole and it would have
  needed exactly the wiring we were trying to avoid. The Advance 4.3" is the
  same brand, one board, and needs none.
- "Mini wireless Bluetooth camera" is not a real product class. BLE can't carry
  usable video; everything sold that way is wifi. The XIAO ESP32S3 Sense
  (camera + mic, USB-C, no soldering) is the right answer and joins as a second
  WebSocket client rather than a hardware change.
- Losing the motor lost less than expected. An animated 800×480 face is more
  expressive than a single-motor cam ever was.
**Next:** Order the panel. Measure the belly. Do not cut until the panel is in
hand and held against it.
**Footage:** none.

---

## 2026-08-10 - Repo scaffolded
**Goal:** Structure the project before buying parts.
**Did:** Wrote docs 00-09 and the firmware skeleton for the discrete design.
**Learned:** There is no common 2.00" e-paper panel; 2.13" is the standard part.
Moot now — see above.
**Footage:** none.

---

## Measurement table

Fill this in **before cutting**. Dates as `YYYY-MM-DD`.

| Date | Measurement | Value | Notes |
|---|---|---|---|
| | Belly flat area (W × H mm) | | does 4.3" fit, or drop to 3.5"? |
| | Belly curvature drop over 100 mm | | decides wedge/bezel |
| | Clearance behind belly wall (mm) | | panel + connector depth |
| | Panel visible glass area (W × H mm) | | measured off the real board |
| | Panel outer PCB size (W × H mm) | | the hole must be smaller than this |
| | Idle current, screen dimmed | | power bank auto-shutoff risk |
| | Active current, screen bright | | runtime estimate |

## Open questions

- [ ] Does 4.3" fit the belly, or is 3.5" the realistic size?
- [ ] Is the original PCB directly behind the belly wall?
- [ ] Which GPIOs does *this* CrowPanel model use for the mic and speaker?
      (`firmware/include/config.h` has placeholders — get the real ones from the
      Elecrow wiki for the exact board)
- [ ] Does the power bank stay awake with the screen dimmed?
- [ ] Can the fur be cut and hemmed around the screen without fraying?
