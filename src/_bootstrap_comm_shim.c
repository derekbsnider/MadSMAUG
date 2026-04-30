/*
 * Bootstrap shim — all stubs removed.
 *
 * SMAUG 1.8 boots end-to-end and runs as a network MUD on madc:
 *   - Telnet greeting
 *   - Full character creation (name → password → color → sex →
 *     class → race)
 *   - Stats roll, MOTD, room entry
 *   - In-game commands: look, inventory, movement (n/s/e/w/u/d),
 *     say, who, quit
 *   - Returning-player reconnect (and `Reconnecting.` flow)
 *
 * The slot_lookup un-stub blocker (high 32 bits of an `int`
 * parameter arriving as zero instead of sign-extended) was a
 * cascade of three separate madc codegen issues, fixed in develop
 * 2026-04-30:
 *
 *   1. scanf-family `%d` writes only 4 bytes into a madc int slot
 *      (which is 8 bytes by design), leaving the high half stale.
 *      Fixed by __madc_sscanf/__madc_fscanf wrappers that rewrite
 *      `%d` -> `%ld` so libc writes the full 8 bytes.
 *
 *   2. `stat()` and friends are dlsym'd with a 4-byte int return,
 *      but madc was reading the 64-bit RAX without sign-extending
 *      EAX. Added them to the int32-returner whitelist that emits
 *      movsxd after the call.
 *
 *   3. The lexer parsed `\033` as `\0` (NUL terminator) plus
 *      literal "33", silently truncating the format string and
 *      causing make_color_sequence's sprintf to write 0 bytes —
 *      then `buf[ln-1] = 'm'` wrote 'm' (0x6D) to buf[-1], an OOB
 *      write into a caller's stack frame. Fixed by adding
 *      multi-digit octal and `\xHH` hex escape handling to the
 *      lexer.
 *
 * The fgetc-into-char-array idiom in act_comm.c (send_*_title)
 * and db.c (show_file/show_file_vnum) doesn't terminate in madc
 * because the chained `(arr[i] = fgetc(fp)) != EOF` expression
 * value isn't truncated-and-extended back to char before the
 * compare. The upstream sources have been patched to use an int
 * intermediate. Fixing the chained-subscript-assignment-as-
 * expression value at the madc compiler level is queued — the
 * straightforward TokenVar variant works (covered by
 * tests/testsignextend).
 */
