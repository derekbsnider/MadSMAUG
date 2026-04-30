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

/* (B) skill / slot lookup against the empty skill_table */
int skill_lookup(const char *name) { return -1; }
int slot_lookup(int slot) { return -1; }

/* (D) number_range — divides by (to-from), pin to from */
int number_range(int from, int to)
{
    if (to <= from) return from;
    return from;
}

/* (E) to_channel — asmjit codegen corrupts caller frames */
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }

/* (F) boot_log — variadic-fmt path wedges */
void boot_log(const char *str, ...) { return; }

/* (C) tables.c — load_commands now works (the silent-exit was
 * adjacent-string-literal concat in COMMAND_FILE; fixed in madc
 * lexer). load_skill_table / load_classes / load_races etc. likely
 * follow the same pattern; un-stubbing one at a time. */

/* (J) house.c loaders */
void load_homedata(void) {}
void load_accessories(void) {}
void load_homebuy(void) {}
int  get_secflag(char *flag) { return 0; }

/* (I) make_wizlist / make_retiredlist */
void make_wizlist(void) {}
void make_retiredlist(void) {}

/* (G) update.c tick handlers — un-stubbing these crashes during
 * the per-pulse violence_update / mobile_update / char_update
 * walk: NULL-deref at small struct offsets (update.c:2329 +0x18,
 * fight.c:3327). The simulation gets far enough to log
 * "X hit a DEATH TRAP in room N" before hitting the deref. */
void mobile_update(void) {}
void char_update(void) {}
void obj_update(void) {}
void aggr_update(void) {}
void tele_update(void) {}
void auth_update(void) {}
void update_handler(void) {}
void auction_update(void) {}
void char_check(void) {}
void reboot_check(int64_t reset) {}
void advance_level(CHAR_DATA *ch) {}
void gain_exp(CHAR_DATA *ch, int gain) {}
void gain_condition(CHAR_DATA *ch, int iCond, int value) {}
void remove_portal(OBJ_DATA *portal) {}
void delete_variable(VARIABLE_DATA *vd) {}
