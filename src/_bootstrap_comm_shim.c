/*
 * Bootstrap-only shim for functions the MadSMAUG umbrella references
 * but haven't yet been ingested or supported. As the umbrella grows
 * and language gaps close, move entries off this shim and into the
 * real upstream sources.
 */

/* comm.c — un-stubbed in session 12 night so the upstream definitions
 * win the funcnode-dedupe race. Keep act() stubbed for now (the real
 * implementation depends on more global state we haven't fully wired). */
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}

/* mud_prog.c — uses `goto` labels not yet supported.
 * Signatures match the forward declarations in mud.h. */
void rprog_leave_trigger(CHAR_DATA *ch) {}
void rprog_enter_trigger(CHAR_DATA *ch) {}
void rprog_sleep_trigger(CHAR_DATA *ch) {}
void rprog_rest_trigger(CHAR_DATA *ch) {}
void rprog_rfight_trigger(CHAR_DATA *ch) {}
void rprog_death_trigger(CHAR_DATA *killer, CHAR_DATA *ch) {}
void rprog_speech_trigger(char *txt, CHAR_DATA *ch) {}
void rprog_random_trigger(CHAR_DATA *ch) {}
void rprog_time_trigger(CHAR_DATA *ch) {}
void rprog_hour_trigger(CHAR_DATA *ch) {}

void mprog_act_trigger(char *buf, CHAR_DATA *mob, CHAR_DATA *ch, void *obj, void *vo) {}
void mprog_bribe_trigger(CHAR_DATA *mob, CHAR_DATA *ch, int amount) {}
void mprog_entry_trigger(CHAR_DATA *mob) {}
void mprog_give_trigger(CHAR_DATA *mob, CHAR_DATA *ch, void *obj) {}
void mprog_greet_trigger(CHAR_DATA *mob) {}
void mprog_fight_trigger(CHAR_DATA *mob, CHAR_DATA *ch) {}
void mprog_hitprcnt_trigger(CHAR_DATA *mob, CHAR_DATA *ch) {}
void mprog_death_trigger(CHAR_DATA *killer, CHAR_DATA *mob) {}
void mprog_random_trigger(CHAR_DATA *mob) {}
void mprog_speech_trigger(char *txt, CHAR_DATA *mob) {}
void mprog_hour_trigger(CHAR_DATA *mob) {}
void mprog_time_trigger(CHAR_DATA *mob) {}

void oprog_speech_trigger(char *txt, CHAR_DATA *ch) {}
void oprog_random_trigger(void *obj) {}
void oprog_wear_trigger(CHAR_DATA *ch, void *obj) {}
bool oprog_use_trigger(CHAR_DATA *ch, void *obj, CHAR_DATA *vict, void *targ, void *vo) { return FALSE; }
void oprog_remove_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_sac_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_damage_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_repair_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_drop_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_zap_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_greet_trigger(CHAR_DATA *ch) {}
void oprog_get_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_examine_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_pull_trigger(CHAR_DATA *ch, void *obj) {}
void oprog_push_trigger(CHAR_DATA *ch, void *obj) {}

void rset_supermob(void *r) {}
void release_supermob(void) {}

/* tables.c (deferred — uses `goto` labels not yet supported) */
void load_commands(void) {}
void load_skill_table(void) {}
void load_herb_table(void) {}
void load_socials(void) {}
void load_tongues(void) {}
void load_classes(void) {}
void load_races(void) {}
void save_commands(void) {}
void save_skill_table(void) {}
void save_herb_table(void) {}
void save_socials(void) {}
void save_classes(void) {}
void sort_skill_table(void) {}
void remap_slot_numbers(void) {}


/* build.c — full file blocked on C99 VLA. Stub the entry points
 * referenced from elsewhere in the umbrella; real implementations
 * land when build.c parses cleanly (or VLA support arrives). */
void stop_editing(CHAR_DATA *ch) {}
void fold_area(AREA_DATA *tarea, char *filename, bool install) {}

/* db.c — temporarily stub the directory-walk init steps until the
 * runtime-data parsing path is debugged. These are fine to bypass for
 * a bootstrap run. */
void make_wizlist(void) {}
void make_retiredlist(void) {}

/* magic.c::skill_lookup — bypass while debugging the lookup path; the
 * real one segfaults inside bsearch_skill_exact's `skill_table[sn]->name`
 * load before the gsn assignments complete. Returning -1 just makes
 * each ASSIGN_GSN print a "skill not found" warning to stderr. */
int skill_lookup(const char *name)
{
    return -1;
}

/* magic.c::slot_lookup — abort()s during boot if the slot isn't in
 * skill_table, but our cut-down boot stubs load_skill_table so the
 * table is empty. Return -1 unconditionally; object loading callers
 * just skip the affect / spell-binding step. */
int slot_lookup(int slot)
{
    return -1;
}

/* number_range stub — the upstream version raises SIGFPE in some
 * runtime path during area_update; pin to `from` to keep boot moving */
int number_range(int from, int to)
{
    if (to <= from) return from;
    return from;
}





/* update.c — full file blocked on IRBuilder::coerce() unsupported. Stubs: */
void remove_portal(OBJ_DATA *portal) {}
void advance_level(CHAR_DATA *ch) {}
void gain_exp(CHAR_DATA *ch, int gain) {}
void gain_condition(CHAR_DATA *ch, int iCond, int value) {}
void mobile_update(void) {}
void char_update(void) {}
void obj_update(void) {}
void char_check(void) {}
void aggr_update(void) {}
void tele_update(void) {}
void auth_update(void) {}
void update_handler(void) {}
void reboot_check(int64_t reset) {}
void auction_update(void) {}

/* variables.c — full file blocked on IRBuilder::coerce() char*->string. */
void delete_variable(VARIABLE_DATA *vd) {}

/* house.c — defer; stub the load_*. */
void load_homedata(void) {}
void load_accessories(void) {}
void load_homebuy(void) {}
int get_secflag(char *flag) { return 0; }

/* build.c bulk stubs — full file blocked on C99 VLA */
int get_otype(char *type) { return -1; }
int get_aflag(char *flag) { return -1; }
int get_trapflag(char *flag) { return -1; }
int get_atype(char *type) { return -1; }
int get_npc_race(char *type) { return -1; }
int get_pc_race(char *type) { return -1; }
int get_wearloc(char *type) { return -1; }
int get_exflag(char *flag) { return -1; }
int get_pulltype(char *type) { return -1; }
int get_rflag(char *flag) { return -1; }
int get_mpflag(char *flag) { return -1; }
int get_oflag(char *flag) { return -1; }
int get_areaflag(char *flag) { return -1; }
int get_wflag(char *flag) { return -1; }
int get_actflag(char *flag) { return -1; }
int get_pcflag(char *flag) { return -1; }
int get_plrflag(char *flag) { return -1; }
int get_risflag(char *flag) { return -1; }
int get_trigflag(char *flag) { return -1; }
int get_partflag(char *flag) { return -1; }
int get_attackflag(char *flag) { return -1; }
int get_defenseflag(char *flag) { return -1; }
int get_langflag(char *flag) { return -1; }
int get_langnum(char *flag) { return -1; }
int get_dir(char *txt) { return -1; }
char *copy_buffer(CHAR_DATA *ch) { return ""; }
void start_editing(CHAR_DATA *ch, char *data) {}
EXTRA_DESCR_DATA *SetOExtra(OBJ_DATA *obj, char *keywords) { return NULL; }
EXTRA_DESCR_DATA *SetOExtraProto(OBJ_INDEX_DATA *obj, char *keywords) { return NULL; }
char *strip_cr(char *str) { return str; }
char *ext_flag_string(EXT_BV *bitvector, char *const flagarray[]) { return ""; }
char *flag_string(int bitvector, char *const flagarray[]) { return ""; }
void assign_area(CHAR_DATA *ch) {}
int get_cmdflag(char *flag) { return -1; }
void edit_buffer(CHAR_DATA *ch, char *argument) {}
void stop_editing(CHAR_DATA *ch) {}
RESET_DATA *parse_reset(AREA_DATA *tarea, char *argument, CHAR_DATA *ch) { return NULL; }
bool can_rmodify(CHAR_DATA *ch, ROOM_INDEX_DATA *room) { return TRUE; }
bool can_omodify(CHAR_DATA *ch, OBJ_DATA *obj) { return TRUE; }
bool can_oedit(CHAR_DATA *ch, OBJ_INDEX_DATA *obj) { return TRUE; }
bool can_mmodify(CHAR_DATA *ch, CHAR_DATA *mob) { return TRUE; }
bool can_medit(CHAR_DATA *ch, MOB_INDEX_DATA *mob) { return TRUE; }


/* Override to_channel from act_comm.c — its asmjit codegen produces a
 * frame that corrupts callers. Use a no-op until that's fixed. */
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }



/* Stub boot_log — its fclose(fpLOG) crashes because fpLOG is a global
 * we can't initialize from SMAUG.mad scope. */
void boot_log(const char *str, ...) { return; }

