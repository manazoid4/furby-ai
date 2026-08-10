# 01 - Teardown: what's inside, and what we can ignore

Good news: in this build you barely have to care what's inside. The panel is
self-contained, so the Furby is a **costume and a stand**, not a machine you
have to reverse-engineer.

You need to know exactly three things:

1. How the fur comes off.
2. How much flat area the belly has.
3. What's behind the belly that you'd be cutting into.

Everything else is optional curiosity (and good B-roll).

Tags: **VERIFIED** = confirmed on our unit with a photo in `docs/img/`.
**ASSUMED** = community knowledge, treat as a hypothesis. Nothing is verified
yet.

---

## Getting the fur off

1. Batteries out first. Confirm dead with a multimeter across the terminals.
2. The fur ("the suit") is a sewn sleeve over the plastic body, held at the
   base by a stitched hem / drawstring and at the face by a plastic collar
   ring. **Photograph and label the ear and eyelash orientation before
   removal** — they only look right one way.
3. Ears pull off their splined posts.
4. You do **not** need to open the shell for the basic build. You only need the
   fur off, so you can see and cut the plastic belly.

## The one decision: alive or inert?

| | **Inert** (recommended) | **Still alive** |
|---|---|---|
| Original batteries | left out | left in |
| The Furby | a shell. Silent. Only the panel talks. | still babbles, moves and reacts on its own |
| Mic quality | clean | the Furby's own motor and speech contaminate every recording |
| Comedy value | lower | very high, briefly |

Take **inert**. The original toy fighting your AI for the microphone is funny
for one video and infuriating forever after. Keep the original board in place
and unpowered — it costs nothing, it keeps the shell rigid, and it means the
mod is reversible-ish.

## Measurements to take (before you order or cut)

These are the only numbers that matter.

- [ ] **Belly flat area, width × height in mm** — decides whether a 4.3" panel
      fits. The CrowPanel Advance 4.3" needs roughly 105 × 70 mm of visible
      area plus bezel; confirm against the actual board when it arrives.
- [ ] **Belly curvature** — how far the surface falls away over 100 mm. This
      decides whether the panel sits flush or needs a foam/printed wedge.
- [ ] **Clearance behind the belly wall, in mm** — the panel plus its connectors
      needs depth. If the original PCB or gearbox is right behind the belly, it
      has to move or come out.
- [ ] **Where the USB-C port will exit** — plan the cable route before cutting.

Fill these into `docs/08-build-log.md`. **Do not cut until they are filled in.**

## What's in there (for the video, not for the build)

- **One motor** driving everything — eyes, eyelids, ears, mouth and tilt all
  come off a single shaft via cams. ASSUMED. This is genuinely the most
  interesting fact about a Furby and it makes a great teardown beat.
- **LCD eye module** in the face. ASSUMED. We are not touching it.
- **Tickle switches** in the head, back, tummy and tail, a tilt switch, a light
  sensor, and an IR transceiver in the beak. ASSUMED. Unused in v1.
- **Small mylar speaker**, roughly 8 ohm. ASSUMED. Unused — the panel has its
  own.

If you want to reuse any of that later, the wiring notes live in the v2 branch
of this project's history, not here. v1 does not touch it.

## Open questions

- [ ] Is there enough flat belly for 4.3", or does it need the 3.5" board?
- [ ] Is the original PCB directly behind the belly wall?
- [ ] Can the fur be cut and hemmed around the screen without it fraying, or
      does it need a bezel ring to clamp the edge?
