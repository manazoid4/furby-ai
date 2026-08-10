// types.h - vocabulary shared across firmware modules.
//
// Keep this in sync with docs/06-protocol.md and with
// brain/furbybrain/protocol.py. Three places, one vocabulary.

#pragma once

#include <string.h>
#include <stddef.h>
#include <stdint.h>

// Mirrors the brain's `{"t":"state","value":...}` frame.
enum class FurbyState : uint8_t {
  Idle = 0,
  Listening,
  Thinking,
  Speaking,
  Error,
};

inline const char* furbyStateName(FurbyState s) {
  switch (s) {
    case FurbyState::Idle: return "idle";
    case FurbyState::Listening: return "listening";
    case FurbyState::Thinking: return "thinking";
    case FurbyState::Speaking: return "speaking";
    case FurbyState::Error: return "error";
  }
  return "idle";
}

inline FurbyState furbyStateFrom(const char* s) {
  if (!s) return FurbyState::Idle;
  if (!strcmp(s, "listening")) return FurbyState::Listening;
  if (!strcmp(s, "thinking")) return FurbyState::Thinking;
  if (!strcmp(s, "speaking")) return FurbyState::Speaking;
  if (!strcmp(s, "error")) return FurbyState::Error;
  return FurbyState::Idle;
}

// The closed expression vocabulary. The LLM picks one of these and nothing
// else; see docs/05-architecture.md and brain/furbybrain/persona.py.
// There is no motion vocabulary - there is no motor.
#define FACE_EXPRESSIONS \
  "neutral,happy,curious,thinking,surprised,sleepy,confused,error"

// Binary frame header - docs/06-protocol.md.
//   byte 0   magic 0xF0
//   byte 1   0x01 mic up, 0x02 speaker down, 0x03 camera JPEG up
//   byte 2-3 seq, uint16 LE
#define AUDIO_MAGIC     0xF0
#define AUDIO_TYPE_MIC  0x01
#define AUDIO_TYPE_SPK  0x02
#define AUDIO_TYPE_CAM  0x03
#define AUDIO_HDR_BYTES 4

// Callback signatures the net layer uses to reach the rest of the firmware.
typedef void (*StateCb)(FurbyState);
typedef void (*FaceCb)(const char* expression, bool blink);
typedef void (*CaptionCb)(const char* line1, const char* line2);
typedef void (*BacklightCb)(int level);
typedef void (*AudioCb)(const int16_t* pcm, size_t samples);
