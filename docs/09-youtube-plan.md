# 09 — YouTube plan

## Video premise

**“I replaced a Furby’s brain with AI and took it outside.”**

The project should be filmed as a transformation story, not an electronics lecture.

## Core hook

Open with the finished payoff before explaining anything:

```text
You: “Furby, check my PC.”

Furby stomach:
THINKING...

then:
RAM 31 / 40 GB
CHROME IS HIGHEST

You: “Can you fix it?”

Furby:
PRESS TONGUE
TO APPROVE
```

Then cut to:

> “This started as a 2013 Furby Boom. I cut a screen into its stomach and gave it access to local and cloud AI agents.”

## Story structure

### 1. Cold open — 0:00–0:20

Show 3 fast capabilities:

- sees something through camera
- sends/receives an agent task
- works outside over phone hotspot

Do not explain the wiring yet.

### 2. The object — 0:20–1:00

Show the original Purple Waves Furby and the existing stomach opening.

Explain one rule:

> “I wanted to do this with as little electronics work as possible.”

### 3. The lazy architecture — 1:00–2:00

Explain visually:

```text
E-paper ESP32 = body
Camera ESP32 = eyes/ears
PC/cloud = brain
Phone = outdoor internet
```

The dual-ESP32 point is a good narrative beat because it sounds overcomplicated until the viewer sees that it actually removes wiring.

### 4. First life — 2:00–3:30

The first meaningful success is not speech. It is:

```text
PC → Wi-Fi → Furby stomach
```

Capture the first time the display changes remotely.

### 5. Give it sight/hearing — 3:30–5:00

Add the CamS3. Show a raw frame first, then vision-model output.

Good demo:

> “What am I holding?”

Avoid fake staged AI answers. If it gets something wrong, use that in the edit.

### 6. Give it agency — 5:00–7:00

Connect the bridge to useful tools/agents.

Good tasks:

- PC status
- launch an app
- summarise a local task queue
- hand work to a local agent
- ask a cloud model when away from home

The physical-confirmation tongue switch becomes the strongest later payoff because it turns the toy into a real control surface.

### 7. Take it outside — 7:00–8:30

Phone hotspot + power bank.

Show:

- e-paper readable outdoors
- camera seeing the environment
- agent query over mobile data
- Furby still useful away from home

### 8. End with the next upgrade

Do not finish the video with “and that’s it.”

Finish on one unresolved capability:

> “It can see, hear and control my agents now. Next I want to wire its original tongue and movement back into the new brain.”

That naturally creates part two.

## Title ideas

Best default:

**I Replaced a Furby’s Brain With AI**

Alternatives:

- **I Gave a Furby Control of My Computer**
- **I Turned a 2013 Furby Into an AI Agent**
- **This Furby Can Control My AI Agents**
- **I Put Two ESP32s Inside a Furby**

Avoid putting obscure board names in the title.

## Thumbnail concepts

### A — strongest

- Furby filling most of frame
- obvious stomach e-paper
- small arrow/text: `AI BRAIN`
- laptop visibly reacting in background

### B — outdoor

- Furby held outside
- e-paper showing `ONLINE`
- phone hotspot symbol
- short text: `IT'S ALIVE`

### C — control angle

- Furby + laptop
- terminal/agent activity on laptop
- stomach reads `RUNNING...`
- text: `IT CONTROLS MY PC`

Keep thumbnail text to 2–4 words.

## Shorts/TikTok clips to capture while building

1. cutting/revealing the stomach opening
2. first e-paper boot
3. first remote status update
4. first camera frame from Furby
5. “what am I holding?” vision demo
6. Furby outside on mobile hotspot
7. physical tongue approval when implemented
8. original movement coming back to life later

## Build-log filming checklist

For every milestone capture:

- before
- one close-up during
- serial/software proof
- first successful run
- first failure
- final clean demonstration

The repo build log and video footage should tell the same story.
