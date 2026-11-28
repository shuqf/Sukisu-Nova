# KPM private-buffer self-test

This module checks read/write behavior in a 512-byte static buffer owned by
the KPM itself (its `.bss` storage):

- Write and read back the first 256 bytes using a deterministic pattern
- Write and read back the second 256 bytes using a different pattern
- Report an overall PASS/FAIL result
- Re-run the same test through CONTROL0
- Log module exit when unloaded

The test deliberately does **not** call `kp_malloc`, `kp_realloc`,
`kp_free`, TLSF functions, or other allocator internals. The pinned SukiSU
KernelPatch build does not expose the allocator symbols needed by an external
KPM, so this test is not a dynamic-allocation test.

It does not call arbitrary kernel symbols, install inline/syscall hooks, or
modify kernel-owned data.

Build with the repository's **KPM Memory Allocator Test** GitHub Actions
workflow. Load `kpm-memory-selftest.kpm` in SukiSU Nova and inspect the kernel
log for `[KPM-MEMTEST]` lines. Use CONTROL0 to repeat the test, then unload
the module and confirm the exit log.
