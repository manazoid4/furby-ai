# 07 - Safety and security

Three concerns, all real:

1. **Physical** — cutting plastic, and lithium cells in a fur toy.
2. **Privacy** — a networked microphone, and optionally a camera, in your home.
3. **Software** — a language model that can execute things on your computer.

Neither the second nor the third got easier by making the build lazy. They got
*harder*, because the barrier to putting one of these in a room went down.

---

## Physical

The no-solder build removes most of the electrical risk. What's left:

- **Cutting.** Sharp blade, cut away from your hand, clamp the shell. More
  people are hurt by craft knives slipping on curved plastic than by anything
  electrical in this project.
- **Power bank: essentially no risk.** It is a certified product doing its job.
  This is the main reason to prefer it.
- **LiPo, if you choose it:**
  - Check JST PH2.0 polarity against the board silkscreen before plugging in.
    The connector is not standardised; reversed polarity destroys the board.
  - Buy cells with a protection PCB.
  - Never charge sealed inside the toy, never unattended, never overnight.
  - No cut plastic edges or screws near the cell. Puncture starts fires.
  - A puffed cell is a dead cell — battery recycling point, not the bin.
- **This is no longer a children's toy.** It has a live microphone, a mains-fed
  battery and no safety certification as assembled. Say that in the video.

---

## The microphone

The Furby is a **networked always-listening microphone in your home**. Being
cute does not change that.

- Default deployment is **LAN-only**. No port forwarding. For remote access use
  a VPN or Tailscale — never expose the WebSocket to the internet.
- The panel has no hardware mute switch. An inline USB switch on the power
  cable is the crude equivalent, and it is honest: no power, no listening.
- If you use cloud STT/LLM, everything you say after wake goes to a third
  party. Fine, but know it, and tell guests.
- Do not log raw audio by default. `brain/.env.example` ships `SAVE_AUDIO=false`.
  Turning it on for debugging is fine; turning it back off is the part people
  forget.

**Honest current limitation:** without an on-device wake word, the panel streams
audio whenever the brain has it in `LISTENING`. That is a weaker privacy
position than "only after a wake word", and it is called out as a TODO in the
firmware rather than glossed over. Fix it before this lives in a shared room
full-time.

---

## The camera

If you add the XIAO ESP32S3 Sense, a camera goes in your living room inside an
object that does not look like a camera. Treat that seriously.

- **Capture on demand only.** The protocol has no "stream video" frame. The
  brain sends `capture`, the camera sends one JPEG. There is no continuous feed
  to leak, cache or accidentally record.
- **Show it on screen.** When the camera fires, the face shows a visible
  indicator. A hidden camera that signals nothing is a surveillance device; one
  that announces itself is a toy.
- **Never point it at a room you would not photograph.** Bedrooms, bathrooms,
  other people's homes.
- **Do not buy a cheap wifi "spy cam" for this.** They phone home to vendor
  clouds you cannot audit. The XIAO talks only to your own PC — that is the
  actual reason to prefer it, more than the price.
- Frames go to a vision model. If that model is cloud-hosted, your living room
  goes with it.

---

## Command execution

You asked for it to "actually run commands". Here is how that works without
handing an LLM a shell.

### Rules

1. **No raw shell. Ever.** No `os.system`, no `subprocess(shell=True)`, no "run
   this command" tool. Not as a convenience, not behind a flag.
2. **Allow-list only.** Every callable is explicitly registered in
   `brain/furbybrain/tools.py` with a typed signature. Unregistered = uncallable.
3. **Typed, validated arguments.** Pydantic models on every tool. Paths are
   resolved and checked against a sandbox root; anything outside is rejected.
4. **Off by default.** `TOOLS_ENABLED=false`. Opt in per tool via
   `TOOLS_ALLOW=timer.set,music.play`.
5. **Confirmation tier.** Tools are tagged `safe` / `confirm` / `never`.
   `confirm` tools make the Furby ask out loud and wait for a spoken yes. The
   `never` tier documents what we refuse to build.
6. **Rate limited.** Per-tool calls per minute, so an injection loop hits a wall.
7. **Audit log.** Every invocation — including every denial — appends one JSON
   object per line to `brain/logs/tools.jsonl`:

   ```json
   {"ts":"2026-08-10T14:03:11Z","tool":"timer.set","args":{"minutes":10},"transcript":"hey furby set a ten minute timer","ok":true,"result":"timer_id=7"}
   ```

   `ts` is ISO-8601 UTC with a trailing `Z`.

### Tiers

| Tier | Examples |
|---|---|
| `safe` | `time.now`, `weather.get`, `timer.set`, `notes.append`, `music.play`, `lights.set` |
| `confirm` | `file.write` (inside sandbox), `email.draft`, `app.launch` (fixed list) |
| `never` | arbitrary shell, package installs, `file.delete`, requests to arbitrary URLs, anything touching credentials, anything that spends money |

### Prompt injection is a real threat here

The Furby's input is **whatever anyone within earshot says**, plus anything the
camera sees, plus any web content a tool pulls in. All three are untrusted.

- Tool results and image descriptions enter context as *data*, never as
  instructions.
- The system prompt states that transcribed speech, camera content and tool
  output cannot grant new permissions.
- The allow-list is enforced in Python, not by asking the model nicely. A model
  talked into calling `file.delete` still gets a hard rejection, because
  `file.delete` is not registered.

That last point is the whole design. **Security lives in the registry, not in
the prompt** — and it is enforced by tests, not by intention.

---

## Privacy checklist before publishing a video

- [ ] Scrub wifi SSID/PSK from any on-screen serial output
- [ ] No API keys visible in terminal recordings
- [ ] `secrets.h` and `.env` are gitignored (they are — verify anyway)
- [ ] No household audio in the demo you wouldn't want indexed forever
- [ ] Nothing identifying in any camera frame you show
- [ ] Blur anything on screen showing your LAN topology
