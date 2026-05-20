# MadSMAUG

Running the [SMAUG 1.8](https://en.wikipedia.org/wiki/SMAUG) MUD
codebase on the [Mad-C (madc)](https://github.com/derekbsnider/madc)
compiler — compiling the original C source directly, not a port.

SMAUG is a MUD (Multi-User Dungeon) engine originally written in C by
[Derek Snider](https://github.com/derekbsnider) in the 1990s, building
on the Merc/Diku MUD lineage. It became one of the most widely used
MUD codebases in the online gaming community. The full codebase is
roughly 158,000 lines of C across hundreds of source files.

MadSMAUG compiles and runs the original SMAUG 1.8 C source using madc
— no separate C toolchain required. madc JIT-compiles the sources
directly to x86-64 machine code in-process, or can produce a
standalone native Linux executable.

## Status

**SMAUG runs end-to-end on madc** — both JIT and as a native executable.

The server reaches the telnet greeting, supports full character
creation, room navigation, and combat. The standalone `smaug.exe` native
executable survives repeated combat rounds and can complete the Newgate
room 109 serpent fight cleanly.

| Phase       |   % | Notes |
|-------------|----:|-------|
| **Parse**   | ~86% | 136k of 158k lines ingested. 49 upstream translation units. |
| **Compile** | ~86% | Every ingested TU compiles cleanly. |
| **Link**    | ~95% | All 1878 user-defined functions bind labels. |
| **Runtime** | ~99% | JIT and native EXE both survive login, movement, and combat. |

See [smaug-progress.md](https://github.com/derekbsnider/madc/blob/master/docs/smaug-progress.md) in the madc repo for detailed file-by-file status.

### Remaining work

- IMC (MUD-network federation) sources — deferred to post-core
- Broader post-combat gameplay: additional encounters, spells,
  mobprogs, and longer session stability

## Building

Requires a [madc](https://github.com/derekbsnider/madc) binary on
`PATH` (or pointed at via `$MADC`).

### JIT mode (run directly)

```bash
bin/madc SMAUG.mad
```

### Native executable

```bash
madc -o smaug SMAUG.mad
./smaug
```

`SMAUG.mad` is the top-level bootstrap file — it `#include`s the
original upstream C sources in dependency order with `int main()` last.

### Runtime data setup

SMAUG needs a writable data directory tree. Quick setup using the
upstream data:

```bash
mkdir -p /tmp/smaug_run && cd /tmp/smaug_run
for d in gods player boards classes clans races; do
  ln -sfn /path/to/MadSMAUG/upstream/smaug1.8/$d $d
done
cp -rL /path/to/MadSMAUG/upstream/smaug1.8/area area
cp -rL /path/to/MadSMAUG/upstream/smaug1.8/system system
```

Then run from that directory.

## License

This repository inherits obligations from the DikuMUD / Merc / SMAUG
license stack. See `LICENSE` for the umbrella notice and
`LICENSE.diku`, `LICENSE.merc`, `LICENSE.smaug` for the three
underlying licenses — all apply to every source file in `src/` and
to the tarball in `upstream/`.

The madc tool itself is separately licensed (MPL 2.0) at
https://github.com/derekbsnider/madc.

## Related

- [madc](https://github.com/derekbsnider/madc) — the language / JIT
  compiler this project is built on.
  See the [madc wiki](https://github.com/derekbsnider/madc/wiki) for
  language documentation and guides.
- `upstream/smaug1.8.tgz` — original SMAUG 1.8 source tarball;
  madc uses these source files directly with a small SMAUG.mad shim.
