# Upstream patches

After extracting the SMAUG 1.8 tarball into `upstream/smaug1.8/`,
apply the patches in this directory:

```sh
cd /workspace/MadSMAUG
for p in patches/*.patch; do
  patch -p1 < "$p"
done
```

## smaug-echo-color-prompt.patch

Upstream SMAUG sends `IAC WONT ECHO` in `CON_GET_WANT_RIPANSI` (after
the user types) instead of at the end of `CON_CONFIRM_NEW_PASSWORD`
(before the prompt).  Move the IAC sequence so the colour-preference
selection echoes for the user.
