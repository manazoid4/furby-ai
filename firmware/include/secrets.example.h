// secrets.example.h
//
//   cp include/secrets.example.h include/secrets.h
//
// secrets.h is gitignored. Do not commit it, and do not leave it on screen
// while filming (docs/07-safety-and-security.md, privacy checklist).

#pragma once

#define WIFI_SSID       "your-ssid"
#define WIFI_PASS       "your-password"

// LAN address of the machine running `furby-brain`.
// Keep this on your LAN. Do not port-forward it.
#define BRAIN_HOST      "192.168.1.10"
#define BRAIN_PORT      8765
#define BRAIN_PATH      "/furby"

// Shared secret sent in the hello frame. Not real security on its own - it
// keeps a stray client off, nothing more. LAN-only is the actual control.
#define BRAIN_TOKEN     "change-me"
