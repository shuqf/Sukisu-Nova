# KPM Symbol Lookup Test

This diagnostic KPM uses the SukiSU KernelPatch ABI-v2 source pinned in the workflow.
It only calls the KernelPatch-exported `kallsyms_lookup_name` resolver and reports whether
selected kernel symbols resolve. It does not call the resolved symbols, install hooks,
or modify kernel state.

Build with the GitHub Actions workflow **KPM Symbol Lookup Test**. Load
`kpm-symbol-lookup-test.kpm` from the KPM manager, then inspect:

```sh
dmesg | grep KPM-SYMTEST
```

Unload the module after collecting the output.
