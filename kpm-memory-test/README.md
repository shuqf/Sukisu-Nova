# KPM initialized .data buffer self-test

This module checks basic read/write behavior using a 512-byte, initialized,
volatile static buffer. Its non-zero initializer places the buffer in the ELF
`.data` section rather than `.bss`.

- Writes and reads back two 256-byte regions.
- Reports the first mismatching offset, expected byte, and actual byte.
- CONTROL0 reruns the same test.
- EXIT reports module unload.
- Does not use a dynamic allocator, install hooks, call kernel functions
  through resolved addresses, or intentionally modify kernel data.

This is a diagnostic for initialized KPM data storage. It is **not** a test of
the `.bss` section or of KernelPatch's dynamic allocator. A PASS does not
establish that all KPM memory facilities work.
