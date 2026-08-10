"""furby-brain - speech, reasoning and tool execution for furby-ai.

The Furby itself is a thin client: microphones, a speaker, one motor and an
e-ink display on a WebSocket. Everything that thinks lives here.

Module map (docs/05-architecture.md):
    protocol.py   frame encode/decode, shared with the firmware
    server.py     WebSocket server, one Session per Furby
    pipeline.py   VAD -> STT -> LLM -> tools -> TTS, streamed
    stt.py        speech to text
    llm.py        Ollama / Anthropic / OpenAI behind one interface
    tts.py        Piper / ElevenLabs behind one interface
    tools.py      the allow-list. Read docs/07 before touching it.
    persona.py    system prompt, mood, gesture vocabulary
    simulator.py  fake Furby using your laptop mic and speakers
"""

__version__ = "0.1.0"

SAMPLE_RATE = 16_000
FRAME_SAMPLES = 320  # 20 ms
FRAME_BYTES = FRAME_SAMPLES * 2
