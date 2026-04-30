/*
 * Bootstrap-only shim. After session-12's audit, the shim is far
 * smaller than it used to be and the comments now describe real
 * remaining issues, not stale "goto/VLA/coerce" gaps that have
 * since closed.
 *
 * The data files (commands.dat, skills.dat, classes/*.class,
 * races/*.race, gods/*) are NOT missing — they all ship in the
 * upstream SMAUG distribution and are accessible via
 * /tmp/smaug_run/. The original "data files missing" comments
 * here were wrong.
 *
 * What's actually still load-bearing as a stub:
 *
 *   (A) act() — global format-arg pipeline not fully wired.
 *   (B) skill_lookup / slot_lookup — empty skill_table because
 *       (C) load_commands silently exits during fread_command's
 *       skill_function lookup. Real path forward: dig into
 *       skill_function / add_command to find the inner failure.
 *       Same shape probably blocks load_skill_table /
 *       load_classes / load_races / load_socials etc.
 *   (D) number_range — divides by (to-from); pin to from.
 *   (E) to_channel — asmjit codegen corrupts caller frames.
 *   (F) boot_log — variadic format-string path wedges.
 *   (G) update_handler ladder — un-stubbing crashes at
 *       update.c:2329 (NULL+0x18) during mobile_update tick.
 *       The simulation gets far enough to run "X hit a DEATH TRAP
 *       in room N" log lines before the crash, so the per-mob
 *       walk is wired up but something inside char/object
 *       cleanup deref's a NULL pointer at field offset 24.
 *
 *   (H) tables.c data-file loaders — see (C) above. Stubbed only
 *       because the inner failure cascades.
 *   (I) make_wizlist / make_retiredlist — readdir cycles over
 *       the gods/ dir, but the upstream version's directory walk
 *       still wedges on something we haven't isolated.
 *   (J) house.c loaders — separate runtime issue not yet diagnosed.
 */

/* (A) act — global fmt state not fully wired */
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}

/* slot_lookup: still stubbed. With load_skill_table running for
 * real, calling the upstream slot_lookup during boot_db's object
 * loading still abort()s — some objects reference spell slots
 * not present in skills.dat. Returning -1 lets boot proceed and
 * those objects just get no spell affect bound. skill_lookup
 * (string-by-name) works fine now and is no longer stubbed. */
int slot_lookup(int slot) { return -1; }

/* number_range: keep stubbed. Real upstream has a guard against
 * (to-from)<1 but still SIGFPEs on `n % (to-from+1)` — an asmjit
 * codegen issue with the post-guard modulo, not the C source.
 * Pin to `from` until that's diagnosed. */
int number_range(int from, int to)
{
    if (to <= from) return from;
    return from;
}

/* (E) to_channel — asmjit codegen corrupts caller frames */
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }

/* (F) boot_log — variadic-fmt path wedges */
void boot_log(const char *str, ...) { return; }

/* (C) tables.c — load_commands now works after the
 * adjacent-string-literal concat fix (madc commit c41df84).
 * Trying to peel the rest at once. */

/* (J), (I) — try unstubbing in light of the lexer concat fix */

/* (G) update.c tick handlers — try unstubbing now */
