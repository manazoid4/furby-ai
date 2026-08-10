"""Wire protocol - must match firmware/include/types.h and docs/06-protocol.md.

Three places share this vocabulary (here, types.h, the docs). When you change a
frame, change all three in the same commit, or you will spend an evening
wondering why the Furby ignores you.
"""

from __future__ import annotations

import struct
from dataclasses import dataclass

AUDIO_MAGIC = 0xF0
AUDIO_TYPE_MIC = 0x01
AUDIO_TYPE_SPK = 0x02
AUDIO_HDR_BYTES = 4

_HDR = struct.Struct("<BBH")


@dataclass(frozen=True)
class AudioFrame:
    kind: int
    seq: int
    pcm: bytes  # 16-bit signed LE mono @ 16 kHz


def encode_audio(pcm: bytes, seq: int, kind: int = AUDIO_TYPE_SPK) -> bytes:
    """Frame PCM for the wire."""
    return _HDR.pack(AUDIO_MAGIC, kind, seq) + pcm


def decode_audio(data: bytes) -> AudioFrame | None:
    """Parse an audio frame, or return None for anything malformed.

    Returning None rather than raising is deliberate: a bad frame from a flaky
    link should drop one 20 ms slice, not tear down the session.
    """
    if len(data) < AUDIO_HDR_BYTES:
        return None
    magic, kind, seq = _HDR.unpack_from(data, 0)
    if magic != AUDIO_MAGIC:
        return None
    if kind not in (AUDIO_TYPE_MIC, AUDIO_TYPE_SPK):
        return None
    return AudioFrame(kind=kind, seq=seq, pcm=data[AUDIO_HDR_BYTES:])
