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
 *   act / to_channel / boot_log: empty. Variadic-format-arg
 *     pipeline corrupts the heap when un-stubbed.
 */
int slot_lookup(int slot) { return -1; }
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }
void boot_log(const char *str, ...) { return; }
