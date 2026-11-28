// SPDX-License-Identifier: GPL-2.0-or-later
#include <compiler.h>
#include <kpmodule.h>
#include <kallsyms.h>
#include <linux/printk.h>

KPM_NAME("kpm-symbol-lookup-test");
KPM_VERSION("1.0.0");
KPM_LICENSE("GPL v2");
KPM_AUTHOR("Sukisu-Nova test");
KPM_DESCRIPTION("Read-only KernelPatch kallsyms lookup diagnostic");

static void check_symbol(const char *name)
{
    unsigned long addr;

    addr = kallsyms_lookup_name(name);
    pr_info("[KPM-SYMTEST] %s: %s\n", name, addr ? "FOUND" : "NOT_FOUND");
}

static void run_checks(void)
{
    if (!kallsyms_lookup_name) {
        pr_err("[KPM-SYMTEST] KernelPatch kallsyms_lookup_name export is NULL\n");
        return;
    }

    pr_info("[KPM-SYMTEST] lookup API: AVAILABLE\n");
    check_symbol("kallsyms_lookup_name");
    check_symbol("schedule");
    check_symbol("do_exit");
    check_symbol("printk");
    check_symbol("_text");
    check_symbol("init_task");
    check_symbol("__arm64_sys_getpid");
    pr_info("[KPM-SYMTEST] checks complete; no kernel functions were called\n");
}

static long symbol_test_init(const char *args, const char *event, void *reserved)
{
    pr_info("[KPM-SYMTEST] init event=%s\n", event ? event : "(null)");
    run_checks();
    return 0;
}

static long symbol_test_control(const char *args, char *__user out_msg, int outlen)
{
    if (args && args[0] == 'r') {
        run_checks();
    }
    return 0;
}

static long symbol_test_exit(void *reserved)
{
    pr_info("[KPM-SYMTEST] exit\n");
    return 0;
}

KPM_INIT(symbol_test_init);
KPM_CTL0(symbol_test_control);
KPM_EXIT(symbol_test_exit);
