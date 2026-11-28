// SPDX-License-Identifier: GPL-2.0-or-later
#include <compiler.h>
#include <kpmodule.h>
#include <linux/printk.h>

KPM_NAME("kpm-memory-selftest");
KPM_VERSION("1.1.0");
KPM_LICENSE("GPL v2");
KPM_AUTHOR("Sukisu-Nova test");
KPM_DESCRIPTION("KPM private BSS buffer read/write self-test");

static unsigned char test_buffer[512];

static int run_buffer_test(void)
{
    int i;
    int first_ok = 1;
    int second_ok = 1;

    for (i = 0; i < 256; i++)
        test_buffer[i] = (unsigned char)((i * 37 + 11) & 0xff);

    for (i = 0; i < 256; i++) {
        if (test_buffer[i] != (unsigned char)((i * 37 + 11) & 0xff)) {
            first_ok = 0;
            break;
        }
    }
    pr_info("[KPM-MEMTEST] write/read first 256 bytes: %s\n",
            first_ok ? "PASS" : "FAIL");

    for (i = 256; i < 512; i++)
        test_buffer[i] = (unsigned char)((i * 19 + 7) & 0xff);

    for (i = 256; i < 512; i++) {
        if (test_buffer[i] != (unsigned char)((i * 19 + 7) & 0xff)) {
            second_ok = 0;
            break;
        }
    }
    pr_info("[KPM-MEMTEST] write/read second 256 bytes: %s\n",
            second_ok ? "PASS" : "FAIL");

    pr_info("[KPM-MEMTEST] result: %s; static KPM .bss buffer only; no allocator, hooks, or kernel data modified\n",
            (first_ok && second_ok) ? "PASS" : "FAIL");

    return (first_ok && second_ok) ? 0 : -1;
}

static long memory_test_init(const char *args, const char *event, void *reserved)
{
    (void)args;
    (void)reserved;
    pr_info("[KPM-MEMTEST] init event=%s\n", event ? event : "(null)");
    run_buffer_test();
    return 0;
}

static long memory_test_ctl0(const char *ctl_args, char *__user out_msg, int outlen)
{
    (void)ctl_args;
    (void)out_msg;
    (void)outlen;
    pr_info("[KPM-MEMTEST] CONTROL0: rerunning private-buffer test\n");
    run_buffer_test();
    return 0;
}

static long memory_test_exit(void *reserved)
{
    (void)reserved;
    pr_info("[KPM-MEMTEST] exit\n");
    return 0;
}

KPM_INIT(memory_test_init);
KPM_CTL0(memory_test_ctl0);
KPM_EXIT(memory_test_exit);
