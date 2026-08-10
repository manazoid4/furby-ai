# 00 - Overview

## The one-line pitch

A Furby with a 4-inch touchscreen face in its belly that listens, thinks and
talks back — built in an afternoon, with a craft knife and some tape.

## Design principles

1. **Lazy on purpose.** Every decision goes to the option with the fewest
   steps. One board instead of six. Adhesive instead of screws. USB-C instead
   of a battery circuit. If a feature needs a soldering iron, it does not go in
   v1.
2. **One board does everything.** The CrowPanel Advance 4.3" is an ESP32-S3
   *plus* an 800×480 touchscreen *plus* a microphone *plus* a speaker and amp.
   That single choice deletes the microphone wiring, the amplifier, the e-ink
   module, the level shifting and the whole breadboard stage.
3. **Local-first.** Default path is offline: faster-whisper, Ollama, Piper.
   Cloud is a config flag, not a dependency.
4. **The toy is dumb.** The panel captures audio, plays audio and draws a face.
   All reasoning is on the PC, so you can swap models without reflashing.
5. **Every dangerous thing is opt-in.** Command execution is allow-listed and
   off by default. See `07-safety-and-security.md`.

## What we deliberately are NOT doing (in v1)

| Dropped | Why | Could return as |
|---|---|---|
| Driving the original motor | Needs a motor driver, wiring, cam decoding, and a power rail that doesn't brown out. Days of work for a twitch. | v2, "making it move again" |
| Reusing the stock speaker | The panel has its own speaker. Reusing the Furby's means cutting and joining wires. | never, honestly |
| Separate mics + amp | The panel has an onboard I2S mic and a speaker amp. | never |
| Replacing the LCD eyes | Shell surgery on the face. The belly screen *is* the face now. | v3 stretch |
| Battery management circuit | A USB-C power bank is a battery management circuit someone else already built and certified. | optional LiPo later |

Dropping motion is the big one and it is the right call. A Furby that talks,
listens and has an expressive animated face reads as *alive* on camera. A Furby
that also twitches slightly is not meaningfully more alive, and costs ten times
the effort.

## The build, end to end

1. Take the fur off. Take the batteries out.
2. Cut a rectangular hole in the belly, roughly 4 inches across.
3. Stick the panel in the hole with VHB tape.
4. Run a USB-C cable out the back to a power bank.
5. Flash the firmware with your wifi details.
6. Run `furby-brain` on your PC.
7. Talk to the Furby.

There is no step involving solder, and no step where getting it wrong destroys
a component. The only irreversible step is step 2 — measure twice.

## Milestones

- **M0 — Brain works headless.** Python pipeline + simulator. Talk to your
  laptop, get Furby-voiced replies. *No hardware.*
- **M1 — Panel on the desk.** Mic → wifi → brain → speaker. Board still in its
  box, not in the toy.
- **M2 — The face.** Animated eyes and moods on the 800×480 screen.
- **M3 — The hole.** Cut, mount, power. The point of no return.
- **M4 — Tools.** Allow-listed command execution, timers, home automation.
- **M5 — Eyes (optional).** XIAO ESP32S3 Sense, "what am I holding?"
- **M6 — Polish.** Persona tuning, latency, boot-to-talk under 3 s.

## Latency budget (target, local path)

| Stage | Target |
|---|---|
| Endpointing (VAD silence) | 400 ms |
| STT (faster-whisper small.en, GPU) | 300 ms |
| LLM first token | 400 ms |
| TTS first chunk (Piper, streamed) | 200 ms |
| Network + buffering | 200 ms |
| **End of speech to first sound out** | **~1.5 s** |

Streaming is what makes this tolerable. Do not wait for the full LLM response
before starting TTS — sentence-chunk it. See `brain/furbybrain/pipeline.py`.

## Known hard problems (be honest in the videos)

| Problem | Mitigation |
|---|---|
| Speaker feedback into the mic | Half-duplex by default: stop capturing while speaking. The panel's mic and speaker are centimetres apart, so this matters more here than in a normal build. |
| The belly is curved, the panel is flat | A 4-inch flat panel on a curved belly leaves gaps at the edges. Plan a printed or foam bezel, or accept the gap and hide it under the fur. |
| Cutting the hole is irreversible | Paper template first. Drill the corners, then cut between them. |
| Fur has to sit over or around the screen | Cut the fur opening *smaller* than the plastic hole and let it overlap the bezel — a fur fringe around the screen looks intentional; a gap looks broken. |
| Screen brightness at 800×480 eats power | A power bank hides this entirely. Dim the backlight when idle. |
