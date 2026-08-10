# 10 — Portable / outdoor mode

The Furby should work away from home without adding a SIM modem, travel router or GPS board to v0.1.

## v0.1 portable stack

```text
Furby
 ├── CrowPanel ESP32-S3
 ├── CamS3 ESP32-S3
 └── USB power bank
          │
          ▼
      phone hotspot
          │
          ▼
       internet
          │
     cloud endpoint
```

## What the phone provides

- mobile internet
- Wi-Fi hotspot
- optional GPS/location context
- optional early-stage TTS/audio output
- a convenient setup screen if we later add a companion web app

Do not duplicate those functions inside Furby unless testing proves the phone dependency is a real problem.

## Outdoor first test

1. Power Furby from the USB power bank.
2. Enable phone hotspot.
3. Join CrowPanel to hotspot.
4. Join CamS3 to hotspot.
5. Confirm both IPs are reachable.
6. Send `OUTDOOR / ONLINE` to the e-paper.
7. Capture one image from CamS3.
8. Send that image to a cloud vision endpoint or remote bridge.
9. Display a short result on e-paper.
10. Walk away from home Wi-Fi and repeat.

## Home vs outdoor config

Keep the device firmware provider-agnostic.

Suggested network profile logic:

```text
try HOME_WIFI
  ↓ unavailable
try PHONE_HOTSPOT
  ↓ unavailable
show OFFLINE
```

Do not hard-code a cloud-model provider into the ESP32 firmware. It should only know where its bridge/API endpoint is.

## Offline behaviour

Useful offline states:

```text
OFFLINE
CAMERA READY

3 CAPTURES
SAVED
```

The CamS3 microSD can later buffer images/audio when there is no connection. The CrowPanel should keep showing the last state because e-paper retains content without a backlight.

## Remote access to the home PC

Treat this as a later milestone.

The safe model is:

```text
Furby → phone hotspot → authenticated public relay/VPN path → home bridge
```

Do not simply port-forward an unrestricted PC agent to the internet.

## Power-bank placement

Priorities:

1. cannot move around freely inside Furby
2. cable cannot pull against the CrowPanel
3. charging port remains reachable
4. no pressure against the camera board
5. no sharp battery deformation

Use removable foam/tape for the first fit. Permanent enclosure work comes after runtime and thermal testing.

## Outdoor UI

E-paper should show only useful state, not animations.

Example:

```text
FURB-1
ONLINE

PHONE 4G
AGENT READY
```

During a request:

```text
LISTENING
```

then:

```text
THINKING
CLOUD
```

then:

```text
DONE
```

This is deliberately simple and readable at a glance.
