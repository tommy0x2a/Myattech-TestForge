# TestForge

**Automated unit test generator for embedded C, C++, and assembly.**

TestForge helps firmware engineers generate high-quality, boundary-aware unit tests with realistic stubs and mocks — especially after HAL or register map changes. It is designed to work both directly and as a collaborative partner with AI coding assistants.

## The Dual-Door Architecture

TestForge uses a **Dual-Door** design:

- **Human Door**: A clear, persuasive experience for engineers (this repository and future website).
- **Agent Door**: A machine-readable MCP interface that allows AI coding assistants (Claude, Cursor, Grok, etc.) to discover, evaluate, and repeatedly call TestForge tools.

This means you can use TestForge directly, **or** let your AI coding assistant use it on your behalf for iterative stress testing and coverage analysis.

## Key Features

- Automated boundary value test generation for C, C++, Python, and embedded assembly
- Intelligent stub and mock template generation for hardware-dependent code
- Strong awareness of HAL and register map changes
- Output support for Unity, CppUTest, and Google Test
- Air-gap compatible with local license validation
- Designed to work natively with AI coding agents via MCP

## Getting Started

### Try TestForge

Download the 30-day fully featured trial:

- [Download for Windows](https://myattech.com) *(coming soon)*
- [Download for Linux](https://myattech.com) *(coming soon)*

### Use with AI Coding Assistants

TestForge exposes its capabilities through the Model Context Protocol (MCP).  
See the [Agent Door documentation](#for-ai-coding-agents) below.

## For AI Coding Agents (MCP)

TestForge can be used directly by AI coding assistants.

- [Capability Manifest](manifest.json) *(coming in v0.1)*
- Example tool calls and integration guide will be added here after the first MCP release.

## Licensing

TestForge source code is available under the **Business Source License 1.1**.

### Summary

- You may **view, modify, and use** the source code for **non-commercial** and **non-production** purposes.
- **Commercial use** of the software (including using it to provide a service or product to others) requires a paid commercial license from Myattech.
- After the **Change Date** (currently set to **January 1, 2029**), this code will automatically convert to the **Apache License 2.0**.

### Why This License?

We believe in transparency. Firmware engineers should be able to inspect the code they rely on. At the same time, TestForge is a commercial product, and its core test generation logic represents significant investment. The Business Source License allows us to offer source visibility while protecting the sustainability of the project.

### Commercial Licensing

If you would like to use TestForge in a commercial context, or if your organization requires different licensing terms, please contact us:

**Email:** tommy42@cox.net

We offer individual licenses ($149 one-time) and team licenses ($499 for up to 5 seats).

### Full License Text

- [LICENSE](LICENSE) — Business Source License 1.1
- [LICENSE-APACHE](LICENSE-APACHE) — Apache License 2.0 (future license)

## Building from Source

Instructions for building TestForge from source will be added after the initial public release.

## Contributing

We welcome bug reports, feature suggestions, and feedback.

Because of the Business Source License, we currently have limited ability to accept large code contributions. However, we are very interested in:

- Bug reports with reproducible examples
- Suggestions for improving test quality on real embedded codebases
- Feedback on the MCP / AI agent integration

Please open an issue to start a discussion.

## Roadmap

- v1.0: Core parser + test generation for C (Unity, CppUTest, Google Test)
- v1.1: C++ and Python support + improved HAL awareness
- v1.2: Embedded assembly support + MCP integration
- Future: RegisterGen companion tool, CI integration, binary analysis

## Contact & Links

- **Website**: [myattech.com](https://myattech.com) *(coming soon)*
- **Email**: tommy42@cox.net
- **X / Twitter**: [@tommy0x2a](https://x.com/tommy0x2a)

---

**Built by firmware engineers, for firmware engineers.**
