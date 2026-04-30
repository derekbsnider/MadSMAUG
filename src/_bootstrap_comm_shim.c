/*
 * Bootstrap shim — one stub remaining.
 *
 *   slot_lookup: returns -1 unconditionally. Un-stub blocker is a
 *   real madc codegen bug that only manifests inside the SMAUG
 *   umbrella context.
 *
 *   Diagnosed 2026-04-30 (post-act-unstub):
 *
 *     - Upstream slot_lookup at magic.c:357 starts
 *           `if ( slot <= 0 ) return -1;`
 *     - Un-stubbing makes that guard fail to early-return for the
 *       FIRST -1 input during gods.are area loading. Function
 *       falls through the for loop and hits the
 *       `if (fBootDb) { bug(...); abort(); }` block.
 *
 *   Probes inside slot_lookup confirm:
 *     - top_sn = 310 (skill_table is fully loaded — not a missing
 *       skills bug).
 *     - sizeof(slot) = 8 (madc's `int` is 64-bit by design —
 *       DataDefINT in include/datadef.h).
 *     - slot read via `%d` prints -1 (low 32 = 0xFFFFFFFF).
 *     - (long)slot prints 0xFFFFFFFF — the high 32 are ZERO,
 *       NOT sign-extended.
 *     - (slot <= 0) evaluates to 0 — guard reads the underlying
 *       64-bit storage as 0x00000000FFFFFFFF (positive 4B), not
 *       as int32 -1. Compare to 0 yields false.
 *
 *   So the int parameter arrives at the function with the low 32
 *   correct (-1) but the high 32 zero, while sizeof says it's
 *   8 bytes. A 64-bit signed compare on that storage doesn't see
 *   -1.
 *
 *   Cannot reproduce in isolation. An exact-copy slot_lookup body
 *   in a small .mad with a stub skill_table sign-extends slot
 *   correctly (high 32 = 0xFFFFFFFF) and the guard returns -1.
 *   The bug is specific to the SMAUG umbrella build (5800+
 *   functions, intricate dedupe / extern resolution / asmjit
 *   register-allocator pressure).
 *
 *   Likely hypotheses for next session:
 *     (1) Some forward-decl path registers slot_lookup's param
 *         with a different type than the definition, and dedupe
 *         picks a call-site signature that emits a 32-bit
 *         zero-extending mov for the int arg.
 *     (2) The caller (db.c's load_obj loop reading
 *         `pObjIndex->value[i]`) emits a 32-bit zero-extending
 *         mov even though the struct member is int64 in madc's
 *         layout — IRBuilder Mem-load makes a different decision
 *         under whatever conditions hold there.
 *     (3) Function-local `extern bool fBootDb;` (the second line
 *         of the function body) somehow disturbs the parameter
 *         symbol-table entry for `slot`.
 *
 *   Workaround: stub returns -1 unconditionally. Callers see
 *   "skill not found" — which is a legitimate fall-through path
 *   in the upstream code. Boot completes, runtime is fully
 *   interactive (boot_db end-to-end, telnet greeting, character
 *   creation through password prompt).
 *
 *   (act / to_channel / boot_log all un-stub cleanly — the
 *   earlier "variadic-format-arg pipeline corrupts the heap"
 *   finding was a layout-shift symptom of separate codegen bugs
 *   that landed in madc develop this session.)
 */
int slot_lookup(int slot) { return -1; }
