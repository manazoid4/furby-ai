# 04 - Power

The old version of this file was a thousand words about brownouts, bulk
capacitors and star grounding. All of that existed to stop a motor from
crashing the microcontroller. There is no motor now, so the entire problem is
gone.

**Power plan: plug a USB-C cable into a power bank.**

That's it. The rest of this page is optional detail.

## Option A — USB-C power bank (recommended)

| | |
|---|---|
| Cost | £10-15 |
| Build effort | zero |
| Runtime | roughly 6-15 h on a 10000 mAh bank, depending on screen brightness |
| Recharge | unplug the bank, or plug a charger into it in place |
| Safety | the bank's own protection and certification does the work |

A power bank is a battery, a charger, a protection circuit and a regulator that
someone else already built and certified. Using one is not a compromise; it is
the correct engineering answer for a one-off toy.

**Watch out for auto-shutoff.** Many power banks switch off when current drops
below ~50-100 mA, which an idle ESP32 with a dimmed screen can absolutely do.
If your Furby dies after a few quiet minutes, that's why. Fixes: pick a bank
with an always-on / low-current mode (often sold for trickle-charging), or keep
the screen backlight high enough to stay above the threshold.

## Option B — 1S LiPo into the panel's BAT connector

The CrowPanel Advance has a JST PH2.0 battery connector and onboard charging,
so a LiPo is genuinely plug-in too — no soldering, as long as you buy a cell
with the matching PH2.0 plug and correct polarity.

**Check the polarity before plugging it in.** JST PH2.0 is not standardised
across vendors; a reversed cell can destroy the board instantly. Compare the
cell's wire colours against the board's silkscreen, and if they disagree, do
not "just try it".

| | |
|---|---|
| Cost | £12 |
| Build effort | plug it in (after checking polarity) |
| Tidiness | much better — everything inside, charge over the panel's USB-C |
| Risk | it is a lithium pouch cell inside a fur-covered toy |

## Safety (this part is not optional)

- Never charge a LiPo sealed inside a fur toy unattended, and never overnight.
  Say this out loud in the video — people will copy this build.
- Buy cells **with a protection PCB**. Check for the small board at the wire end.
- No sharp edges, screws or cut plastic near the cell. Puncture is what starts
  fires.
- A puffed cell is a dead cell. Take it to a battery recycling point.
- A power bank sidesteps all of the above, which is the main argument for it.

## Power consumption, roughly

| State | Estimate |
|---|---|
| Screen on, full brightness, wifi active, streaming audio | 300-500 mA |
| Screen dimmed, idle, connected | 120-200 mA |
| Screen off, idle | 60-100 mA |

Measure yours and put the numbers in `08-build-log.md`. Dimming the backlight
when nobody has spoken for a few minutes is the single biggest saving, and it
doubles as a "the Furby went to sleep" behaviour, which is charming.

## Switching it off

The panel has a reset button but no power switch. Options:

- Unplug the USB-C. Laziest, and fine.
- Use a power bank with a physical button.
- Put an inline USB switch (£4, plug-in) on the cable.
