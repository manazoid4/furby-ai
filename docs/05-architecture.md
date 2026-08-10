# 05 — Architecture

## System split

The project is intentionally split into **peripherals** and **brains**.

```text
                        FURBY

   ┌──────────────────────┐       ┌──────────────────────┐
   │ CrowPanel ESP32-S3   │       │ CamS3 ESP32-S3      │
   │ 2.13" e-paper        │       │ 5MP cam + PDM mic   │
   │ buttons / future IO  │       │ microSD             │
   └──────────┬───────────┘       └──────────┬───────────┘
              │ Wi-Fi                        │ Wi-Fi
              └──────────────┬────────────────┘
                             ▼
                    network / phone hotspot
                             │
                             ▼
                       FURBY BRIDGE
                             │
          ┌──────────────────┼──────────────────┐
          ▼                  ▼                  ▼
      local AI          cloud AI            PC tools
      Ollama            hosted LLM          allow-list
      STT/TTS            vision             automation
```

## CrowPanel responsibility

The CrowPanel is the **body controller**, not the AI engine.

It should be responsible for:

- joining Wi-Fi
- maintaining a connection to the bridge
- rendering e-paper status
- reading its own menu/rotary/back controls
- later reading Furby tongue/tail/touch inputs
- later issuing movement commands
- reporting health/battery/network state

It should not run:

- a serious LLM
- heavyweight speech-to-text
- arbitrary shell commands
- camera encoding for the second board

## CamS3 responsibility

The CamS3 is deliberately independent.

It should be responsible for:

- camera capture
- JPEG/image transport
- PDM microphone recording/capture
- optional microSD logging
- health/status endpoint

The PC/bridge can address it directly. This means we do not need to route video through the CrowPanel.

## Why two ESP32-S3s is simpler, not more complex

It looks like more hardware, but each module is a complete product with a specialised peripheral set.

Benefits:

- camera workload cannot block e-paper/body control
- independent firmware updates
- fewer wires between modules
- easier fault isolation
- camera can be upgraded without replacing the stomach display
- CrowPanel can later survive/reboot independently of camera services

The cost is two network clients and two firmware/config surfaces. For this project, that is a good trade.

## Network modes

### Home

```text
CrowPanel ─┐
           ├─ home Wi-Fi ─ bridge on PC ─ local/cloud agents
CamS3 ─────┘
```

### Portable

```text
CrowPanel ─┐
           ├─ phone hotspot ─ internet ─ cloud service
CamS3 ─────┘
```

### Remote-home — later

```text
Furby → phone hotspot → authenticated relay/VPN path → home bridge → local agents
```

Do not make remote-home networking a prerequisite for the first outdoor demo.

## Device discovery

v0.1 can be very simple:

- static/configured bridge host
- device IDs: `furby-body-01`, `furby-cam-01`
- retry with backoff
- serial logs for IP addresses

Later add mDNS/service discovery if it genuinely removes friction.

## Message model

The bridge should think in events, not pixels.

Examples from bridge → CrowPanel:

```json
{"type":"state","value":"online"}
{"type":"state","value":"listening"}
{"type":"state","value":"thinking"}
{"type":"state","value":"done"}
{"type":"caption","line1":"RAM 31/40 GB","line2":"Chrome is highest"}
{"type":"confirm","id":"abc123","title":"Delete 34 files?"}
```

Examples from CrowPanel → bridge:

```json
{"type":"button","name":"confirm"}
{"type":"button","name":"back"}
{"type":"status","rssi":-61,"uptime_s":820}
```

CamS3 can remain a separate HTTP/stream/image endpoint rather than being forced into the same protocol immediately.

## Agent routing

The bridge owns routing decisions:

```text
request
  ↓
intent / policy
  ├── local model
  ├── cloud model
  ├── vision model
  ├── local agent
  └── safe PC tool
```

This lets the same Furby work at home and outdoors without reflashing firmware for every model/provider change.

## Safety boundary

The ESP32 should never be handed a generic remote-shell primitive.

For computer actions:

```text
Furby request
    ↓
bridge
    ↓
allow-listed action
    ↓
policy / confirmation
    ↓
PC action
```

A future tongue press can approve sensitive actions, but the bridge must still validate the requested action and arguments.

## Offline behaviour

When the network disappears, Furby should degrade gracefully:

- leave a clear `OFFLINE` state on e-paper
- retain local button/menu operation
- optionally allow camera capture to SD
- queue only explicitly safe non-destructive requests
- reconnect automatically

Do not attempt to turn the ESP32 into an offline LLM just to avoid showing `OFFLINE`.
