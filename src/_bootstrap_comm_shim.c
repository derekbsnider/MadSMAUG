/*
 * Bootstrap-only shim — four remaining stubs.
 *
 *   slot_lookup: returns -1 unconditionally. The upstream
 *     implementation does `if (fBootDb) { bug(...); abort(); }`
 *     when a positive slot isn't found in skill_table; during boot
 *     the table is empty, so any caller that passes a positive
 *     value (e.g. an `int value[6]` field freshly read from
 *     skills.dat) trips the abort. Returning -1 here makes the
 *     callers fall through to their `not found` paths instead.
 *
 *     Earlier sessions chased a red-herring "madc zero-extends
 *     int args into RDI" hypothesis — repro tests with both
 *     local-int and struct.int-member sources confirm madc passes
 *     int args correctly (movsxd via IRBuilder::load on a 4-byte
 *     signed Mem). The abort path, not arg passing, was the real
 *     trigger.
 *
 *   act: empty. Tried un-stubbing 2026-04-30 — boot reaches
 *     `Reading in area files... (help.are)` and crashes during
 *     the imc-help.are duplicate-entry handling with
 *     `malloc(): corrupted top size` (heap corruption inside a
 *     libc dlopen path). act() itself isn't called during boot,
 *     so something its compile pulls in disturbs the funcnode
 *     layout — likely candidates are the transitively-referenced
 *     {r,o,m}prog_act_trigger helpers or one of act_string's
 *     static-local buffers. Needs proper bisection; revert kept
 *     in for now.
 *
 *   to_channel / boot_log: empty. Variadic-format-arg pipeline
 *     was reported to corrupt the heap; revisit once act() is
 *     stable.
 */
int slot_lookup(int slot) { return -1; }
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }
void boot_log(const char *str, ...) { return; }
