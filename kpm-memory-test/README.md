# KPM allocator self-test

This module tests the SukiSU KernelPatch allocator API from the pinned
KernelPatch source revision:

- `kp_malloc(256)`
- Write/read verification of a private 256-byte buffer
- `kp_realloc(..., 512)`
- Verification that the original bytes survived reallocation
- Write/read verification of the expanded region
- `kp_free()`

It does not call the allocated memory as code, modify kernel-owned data, resolve
or call arbitrary kernel symbols, or install inline/syscall hooks.

Build with the repository's **KPM Memory Allocator Test** GitHub Actions workflow.
Load `kpm-memory-selftest.kpm` in SukiSU Nova and inspect the kernel log for
`[KPM-MEMTEST]` lines. Unload the module afterward and confirm the exit log.
