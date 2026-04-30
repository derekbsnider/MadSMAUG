#!/usr/bin/env bash
# MadSMAUG.sh — boot SMAUG 1.8 under madc.
#
# Usage:
#     ./MadSMAUG.sh [port]
#
# Requires:
#     madc on $PATH (or set $MADC=/path/to/madc).
#     Get madc from https://github.com/derekbsnider/madc.
#
# Optional environment variables:
#     MADSMAUG_DATA  Where to store the writable runtime tree.
#                    Defaults to ./runtime.
#     MADC_CPU_LIMIT CPU-time rlimit for the JIT process (seconds).
#                    Default 300; 0 disables.
#     MADC_MEM_LIMIT Address-space rlimit (MB). Default 4096; 0 disables.

set -euo pipefail

REPO="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# ---- 1. Find madc ----------------------------------------------------------

if [[ -n "${MADC:-}" ]]; then
    if [[ ! -x "$MADC" ]]; then
        echo "MadSMAUG: \$MADC=$MADC is not executable" >&2
        exit 1
    fi
elif command -v madc >/dev/null 2>&1; then
    MADC="$(command -v madc)"
else
    cat >&2 <<'EOF'
MadSMAUG: madc not found.

Either:
  - put the madc binary on your $PATH, or
  - set MADC=/path/to/madc before invoking this script.

Get madc from https://github.com/derekbsnider/madc — `make -C src`
produces ./bin/madc.
EOF
    exit 1
fi

# ---- 2. Extract upstream tarball if needed --------------------------------

if [[ ! -d "$REPO/upstream/smaug1.8" ]]; then
    if [[ ! -f "$REPO/upstream/smaug1.8.tgz" ]]; then
        echo "MadSMAUG: upstream/smaug1.8.tgz missing — cannot bootstrap" >&2
        exit 1
    fi
    echo "MadSMAUG: extracting upstream/smaug1.8.tgz ..."
    tar -C "$REPO/upstream" -xzf "$REPO/upstream/smaug1.8.tgz"
fi

# Apply any patches/*.patch to the freshly-extracted upstream tree.  These
# are temporary workarounds for codegen edge cases that are still being
# fixed in madc itself; the sentinel ensures we apply them exactly once
# per extraction.
SENTINEL="$REPO/upstream/smaug1.8/.madc-patches-applied"
if [[ ! -f "$SENTINEL" && -d "$REPO/patches" ]]; then
    shopt -s nullglob
    patches=("$REPO"/patches/*.patch)
    shopt -u nullglob
    if (( ${#patches[@]} > 0 )); then
        echo "MadSMAUG: applying ${#patches[@]} patch(es) to upstream/smaug1.8 ..."
        for p in "${patches[@]}"; do
            patch -d "$REPO" -p1 < "$p"
        done
    fi
    : > "$SENTINEL"
fi

# ---- 3. Set up the runtime data tree ---------------------------------------
#
# SMAUG opens its data files via paths like ../system/sysdata.dat,
# ../player/a/Aragorn, ../area/area.lst etc.  Those paths are relative
# to the cwd, which must be the area/ directory.  We build a parallel
# tree under $MADSMAUG_DATA so the upstream extraction stays pristine
# (and gitignored).
#
# `area` and `system` need to be writable copies — SMAUG writes to
# bug.txt, sysdata.dat, etc.  The rest can be symlinks pointed at the
# read-mostly upstream copies.

DATA="${MADSMAUG_DATA:-$REPO/runtime}"

if [[ ! -d "$DATA/area" || ! -d "$DATA/system" ]]; then
    echo "MadSMAUG: setting up runtime data tree at $DATA ..."
    mkdir -p "$DATA"
    rm -rf "$DATA/area" "$DATA/system"
    cp -rL "$REPO/upstream/smaug1.8/area"   "$DATA/area"
    cp -rL "$REPO/upstream/smaug1.8/system" "$DATA/system"
    for d in boards building clans classes corpses councils deity gods houses log new player races vault backup deleted doc; do
        if [[ -d "$REPO/upstream/smaug1.8/$d" ]]; then
            ln -sfn "$REPO/upstream/smaug1.8/$d" "$DATA/$d"
        fi
    done
fi

# ---- 4. Hand off to madc ---------------------------------------------------
#
# cwd must be $DATA/area so SMAUG's `../system/...` paths resolve.
# madc resolves #include directives relative to the source file, so
# the absolute path to SMAUG.mad keeps mud.h / upstream_src/*.c
# discoverable regardless of what cwd we're in.

cd "$DATA/area"
exec "$MADC" "$REPO/src/SMAUG.mad" "$@"
