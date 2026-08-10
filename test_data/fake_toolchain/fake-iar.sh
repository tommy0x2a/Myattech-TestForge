#!/bin/bash
# TestForge™ — fake IAR wrapper for M16 toolchain validation.
# Proves the pluggable toolchain mechanism works: this script is
# named on the PATH, invoked by run_custom_build, prints a marker
# to stderr, then delegates to real gcc so the build actually succeeds.
echo "[fake-iar] Invoked with: $@" >&2
echo "[fake-iar] FAKE_TOOLCHAIN_MARKER" >&2
exec gcc "$@"
