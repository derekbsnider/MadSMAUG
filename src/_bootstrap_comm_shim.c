/*
 * Bootstrap-only shim — four remaining stubs.
 *
 *   slot_lookup: returns -1 for any slot the caller can't find. NOT
 *     a SMAUG bug — gcc finds them all. Diagnosed as a madc
 *     parameter-passing issue: callers in db.c read `int value[6]`
 *     struct fields and pass them to slot_lookup as 32-bit values
 *     zero-extended into RDI rather than sign-extended. So a
 *     legitimately-stored -1 in value[i] arrives at slot_lookup as
 *     0x00000000FFFFFFFF — positive in 64-bit signed, which fails
 *     the `if (slot <= 0) return -1;` early-out and falls into the
 *     `if (fBootDb) abort()` path. Confirmed by tracing slot at
 *     entry: `slot(d)=-1 slot(lx)=ffffffff` — high 32 bits zero.
 *     The same code path works in standalone test (`int x = -1;
 *     check(x);` has full 0xFFFFFFFFFFFFFFFF in RDI), so it's
 *     specific to a struct-int-field load in the upstream context.
 *     Real fix is in madc's call-site codegen, not here.
 *
 *   act / to_channel / boot_log: empty. Variadic-format-arg
 *     pipeline corrupts the heap when un-stubbed.
 */
int  slot_lookup(int slot) { return -1; }
void act(sh_int AType, const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2, int type) {}
void to_channel(const char *argument, int channel, const char *verb, sh_int level) { return; }
void boot_log(const char *str, ...) { return; }
