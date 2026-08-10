"""Protocol round-trip tests.

The firmware and the brain each implement this wire format independently
(firmware/include/types.h vs furbybrain/protocol.py). These tests pin the
Python half, so a divergence fails here rather than showing up as "the Furby
ignores me sometimes".
"""

import pytest

from furbybrain import protocol as p


def test_audio_round_trip_preserves_pcm_and_seq():
    pcm = bytes(range(256)) * 2
    frame = p.decode_audio(p.encode_audio(pcm, seq=42, kind=p.AUDIO_TYPE_SPK))

    assert frame is not None
    assert frame.pcm == pcm
    assert frame.seq == 42
    assert frame.kind == p.AUDIO_TYPE_SPK


@pytest.mark.parametrize(
    "bad",
    [
        b"",                      # empty
        b"\xf0",                  # truncated header
        b"\x00\x01\x00\x00data",  # wrong magic
        b"\xf0\x09\x00\x00data",  # unknown type
    ],
)
def test_malformed_audio_frames_return_none(bad):
    # A flaky link should cost one 20 ms slice, not tear down the session.
    assert p.decode_audio(bad) is None
