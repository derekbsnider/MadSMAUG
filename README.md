# MadSMAUG

A port of the [SMAUG 1.8](https://en.wikipedia.org/wiki/SMAUG) MUD
codebase to the [Mad-C (madc)](https://github.com/derekbsnider/madc)
programming language.

The goal is to run the full ~158k-line SMAUG 1.8 server *without* a
separate C toolchain — madc JIT-compiles the `.mad` sources to
x86-64 machine code in-process.

## Status

Phase F (source port) — in progress.

- ✅ `src/hashstr.mad` — complete, compiles and runs end-to-end via
  `SMAUG.mad`, produces correct link-count hash stats.
- ⏳ Next target: to be decided (config / mud.h equivalents, then
  tables.c, db.c, comm.c …).

See `docs/port-progress.md` for the gap analysis and
file-by-file progress.

## Building

Requires a madc binary on `PATH` (or pointed at via `$MADC`).

```bash
bin/madc SMAUG.mad
```

`SMAUG.mad` is the top-level bootstrap file; it `#include`s every
ported source in dependency order with `int main()` last. See the
madc README for the app-named bootstrap convention.

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
- `upstream/smaug1.8.tgz` — original SMAUG 1.8 source tarball,
  preserved for reference and extraction of files as they are ported.
