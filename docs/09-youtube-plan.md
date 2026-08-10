# 09 - YouTube series plan

## The premise

"I gave a 2013 Furby a brain." The comedy is the gap between a cursed children's
toy and a competent AI assistant. Lean into the cursedness - do not try to make
this look like a polished consumer product. The audience is here for a haunted
Furby, not for a smart speaker.

## Why this has a shot

- Furby is a nostalgia trigger for roughly everyone aged 25-45.
- "Cursed toy + AI" is an existing, proven format with headroom left in it.
- **Anyone can copy this.** No soldering, four parts, one cut. That is the
  single biggest lever on shares and saves — "I could actually do this" beats
  "that's impressive" every time. Lead with it.
- The glowing 800x480 face in the belly is a strong visual hook, and it is
  legible on a thumbnail at phone size.

## The angle that makes it spread

The build being *lazy* is not an apology, it is the hook. Most maker content
looks unreachable: bench supplies, oscilloscopes, reflow. This is a craft knife,
some tape, and one board off Amazon.

Title and thumbnail should both promise that. "No soldering" in the first ten
seconds. Show the entire parts pile in one shot - four items - and let the
audience count them.

## Format

**Long-form series** (10-18 min per episode, one per milestone) is the spine.
**Shorts** are cut from the same footage, not filmed separately - one long-form
shoot should yield 4-6 shorts.

## Episode plan

| # | Title angle | Beats | Hook (first 5 s) |
|---|---|---|---|
| 1 | "I gave a Furby an AI brain - with no soldering" | premise, the four-item parts pile, fur off, the single-motor reveal as a teardown aside | the finished Furby answering a question, hard cut to a craft knife going into its stomach |
| 2 | "Cutting a screen into its stomach" | template, drilling the corners, the cut, first power-on in the shell | the moment the face lights up inside the Furby |
| 3 | "Teaching it to hear and speak" | mic, STT, TTS, voice design, the first sentence | the first thing it ever says, unedited |
| 4 | "Giving it a face" | expression rendering, blinking, why a crude face beats a real one | eyes tracking, blinking, reacting - no words |
| 5 | "It can use my computer now" | tools, the allow-list, and *why* the allow-list | Furby sets a timer, then tries something it isn't allowed to and gets refused |
| 6 | "Now it can see" | XIAO camera, "what am I holding?", the on-demand capture rule | Furby correctly identifying something absurd |
| 7 | "Living with it for a week" | payoff, bloopers, what broke | the funniest failure of the week, cold open |

## The one rule for the hook

**Show the finished thing in the first five seconds of every episode.** Never
open with "hey guys, so in this video". The Furby talking is the hook; nothing
you say is.

## Shorts (cut from the long-form footage)

- The first word it ever says
- The four parts, counted out on the table - "that's the whole build"
- The knife going in. Cutting into a Furby is inherently uncomfortable to watch
- Asking it to delete a file, and watching the allow-list refuse
- The face blinking, macro shot
- The moment the fur goes back on
- Whisper test: how quietly can you talk before it stops hearing you
- "What's inside a Furby?" - pure teardown, no AI, wide appeal

## Thumbnails

- Furby face, dead centre, filling the frame. Eyes matter more than anything.
- Belly screen visible and legible, with real text on it.
- 2-4 words of text maximum. "IT'S ALIVE", "FURBY vs AI", "I MADE IT THINK".
- High contrast. The purple/white "Purple Waves" fur reads well against a dark
  background - use that, it's a free brand.
- Consistent treatment across all 8 episodes so the series reads as a series.

## Title patterns that fit this

- "I gave a Furby an AI brain"
- "I rebuilt a 2013 Furby so it can actually think"
- "Putting an LLM inside a Furby was a mistake"
- "This Furby can use my computer"

Avoid clickbait you don't pay off. The build is genuinely interesting; a title
that oversells it costs more in retention than it gains in clicks.

## Filming discipline (the part people skip)

- **Film everything, from day one.** Storage is cheap; a re-shoot of a first-time
  reaction is impossible.
- **Two cameras minimum:** a wide shot of the bench and a macro on the work. The
  macro is where the interest is.
- **Record clean audio separately.** Phone camera audio will not carry an
  18-minute video.
- **Capture failures deliberately.** The brownout that reboots it mid-sentence is
  better content than the success. Do not tidy the failures out of the edit.
- **Save the "before" shots.** The intact toy, the original PCB, the fur coming
  off. You get one chance at each.

## Description template

```
Turning a 2013 Furby Boom into a local-first AI companion.
Everything is open source: https://github.com/<user>/furby-ai

Parts list: docs/02-hardware-bom.md
Wiring:     docs/03-wiring.md
Why it can't run arbitrary commands: docs/07-safety-and-security.md

00:00 ...
```

Link the repo in the pinned comment as well as the description. The repo is the
credibility - the maker audience will check it, and a real repo converts them.

## Cadence

Do not promise a schedule you can't keep. Bank episodes 1-3 fully edited before
publishing episode 1, then release fortnightly. A stalled series after episode 2
is the default outcome for projects like this; banking is the only reliable
defence.
