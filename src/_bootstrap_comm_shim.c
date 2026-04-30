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
 *   act: empty. The previous "act un-stub corrupts the heap"
 *     diagnosis is misleading. Bisection (2026-04-30) showed
 *     that ANY runtime that reaches the second area_update tick
 *     now crashes deterministically at the same JIT offset
 *     +0x170771, which the source-map points at mud_prog.c:2437
 *     (`while ((*point = *i) != '\0') ++point, ++i;`). mud_prog
 *     does unbounded `point++ = ...` writes into a stack-local
 *     `char buf[MAX_INPUT_LENGTH]` — buffer overflow. With pre-
 *     enum-const-fold-fix JIT layout, the overflow stomped
 *     adjacent stack memory silently; the v0.12.0+ JIT region
 *     grew enough (more compact xor-zero encodings replaced by
 *     mov-imm now that ioperate() returns real enum values) to
 *     shift the stack so `point` walks into an unmapped page.
 *     Real fix is bounds-checking inside mud_prog or a per-mob-
 *     program guard buffer — out of scope for the as-is port.
 *     Workaround paths to consider: skip mob-program execution
 *     entirely (stub mprog_act_trigger / rprog_* / oprog_*) or
 *     bump MAX_INPUT_LENGTH in mud.h (risky — changes struct
 *     layouts).
 *
 *   to_channel / boot_log: empty. Variadic-format-arg pipeline
 *     was reported to corrupt the heap; revisit once act() is
 *     stable.
 */
int slot_lookup(int slot) { return -1; }
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }
void boot_log(const char *str, ...) { return; }
