# 03 - Cutting the hole and mounting the panel

This used to be a wiring guide. There is no wiring. It is now a cutting guide.

**Total connections in this build: one USB-C cable.**

---

## Before you cut

1. Fur off, batteries out (`01-teardown.md`).
2. Panel in hand. **Do not cut from the datasheet dimensions** — hold the real
   board against the real belly.
3. Belly measurements written into `08-build-log.md`.
4. Decide where the USB-C cable exits. Usually straight down through the battery
   hatch, which is already a hole.

## Making the template

1. Put the panel face-down on paper, draw round the **visible screen area**,
   not the whole PCB. The PCB is bigger than the glass.
2. Add 1 mm all round for clearance. The hole should reveal the glass and hide
   the PCB edge.
3. Cut the paper template out. Hold it on the belly. Look at it from a few feet
   away — this is the Furby's new face, and a rectangle that is 3 mm off-centre
   reads as *wrong* even when you can't say why.
4. Mark the centre line against the Furby's own symmetry (the seam, the feet),
   not against the belly's widest point.

## Cutting

Masking tape over the whole belly area first. Mark the rectangle on the tape.

1. **Drill the four corners.** A 3-4 mm bit, or a hole punch, or the tip of a
   hot screwdriver. This is the step that stops the plastic cracking, and
   cracking is the failure mode that ends the build.
2. **Cut between the corner holes** with a craft knife. Many light passes, not
   one deep one. ABS scores and snaps; it does not slice.
3. **File the edges** flat and slightly chamfered.
4. Test-fit the panel. Take material off the *hole*, never off the panel.

Cut **small**. You can always open a hole up. You cannot close one.

## Mounting

The panel sits behind the hole, glass forward, held by tape on the PCB's back
edges against the inside of the shell.

```
        outside                         inside
   ______________________________________________
        fur                    |
   ---------------------       |
        plastic shell          |
   ====[  hole  ]==============|
        ^                      |
        | 1-2 mm gap           |
       [ GLASS ]               |
       [ PANEL PCB ]  <-- VHB tape on these edges, to the shell
                              |
                        USB-C out the bottom
```

Notes:

- **The belly is curved and the panel is flat.** Over a 105 mm span you will get
  a gap at the left and right edges. Options, in order of laziness: (a) let the
  fur overlap and hide it, (b) a craft-foam wedge behind the panel, (c) a
  printed bezel.
- **Do not tape over the mic hole.** Find it on the board before you commit —
  covering the onboard microphone with VHB is a very annoying mistake to
  diagnose later.
- **Leave the reset button reachable**, or at least remember where it is.
- Support the panel's weight on the shell, not on the tape alone, if you can —
  a lip of foam underneath takes the load.

## The fur

Cut the fur opening **smaller than the plastic hole**, so the fur overlaps the
screen edge by 2-3 mm. A fur fringe around the glass looks deliberate. A gap
between fur and glass looks like damage.

Seal the cut fur edge with a thin line of fabric glue or clear nail varnish, or
it will fray on camera.

## Power routing

USB-C cable from the panel, out through the battery hatch, to the power bank.
The power bank can live inside the body cavity (where the AAs were, plus the
space the shell gives you) or outside as a deliberate "life support" gag.

Right-angle USB-C plug is worth the extra couple of pounds — a straight plug
needs 20 mm of clearance behind the board that you probably don't have.

## Panel pin reference

You do not need this for the build. It matters only if you later add something
to a Crowtail port.

| Interface | Note |
|---|---|
| USB-C | Power + serial flashing |
| BAT (PH2.0-2P) | 1S LiPo, 3.7-4.2 V |
| I2C (HY2.0-4P) | Crowtail — sensors go here, no soldering |
| UART (HY2.0-4P / XH2.54-4P) | Crowtail |
| microSD slot | Local assets, sounds, images |
| Onboard I2S mic | Already wired on the board |
| Onboard speaker + amp | Already wired on the board |

**The exact GPIO numbers differ between CrowPanel models.** Take them from the
Elecrow wiki page for *your* board and put them in
`firmware/include/config.h` — the values in there now are placeholders and are
marked as such.

## Assembly order

1. Flash the firmware **before mounting**, while the board is on your desk and
   the USB port is easy to reach.
2. Confirm wifi connects and the brain responds. Talk to it on the desk.
3. Only then cut the hole.
4. Mount, route the cable, fur back on.

Bringing it up on the desk first means that if something doesn't work after
mounting, you know it's the mounting.
