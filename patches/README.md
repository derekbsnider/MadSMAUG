# Upstream patches

After extracting the SMAUG 1.8 tarball into `upstream/smaug1.8/`,
apply the patches in this directory:

```sh
cd /workspace/MadSMAUG
for p in patches/*.patch; do
  patch -p1 < "$p"
done
```

## madc-fgetc-loop.patch

Replaces the chained `(buf[i] = fgetc(fp)) != EOF` idiom in
`act_comm.c` (4 sites in `send_*_title`) and `db.c` (2 sites in
`show_file` / `show_file_vnum`) with an int-intermediate loop so
the EOF compare actually terminates under madc.

madc's int is 64-bit by design, but a chained subscript assignment
to a char element returns the unbound RHS register's full int —
not the truncated-and-extended char value the C standard requires.
The loop ran past EOF, walked off `BUFF`, and eventually crashed
inside libc's fgetc on a corrupted FILE* pointer.

The straightforward `(c = fgetc(fp))` variant where `c` is a TokenVar
local works correctly under madc develop ≥ c35824f. The
TokenSubscript / TokenSubscriptExpr variants are queued for a
follow-up codegen fix; until then this patch keeps SMAUG bootable.
