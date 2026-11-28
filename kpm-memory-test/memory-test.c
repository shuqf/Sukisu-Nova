// SPDX-License-Identifier: GPL-2.0-or-later
#include <compiler.h>
#include <kpmodule.h>
#include <linux/printk.h>

KPM_NAME("kpm-memory-selftest");
KPM_VERSION("1.2.0");
KPM_LICENSE("GPL v2");
KPM_AUTHOR("Sukisu-Nova test");
KPM_DESCRIPTION("KPM initialized .data buffer read/write diagnostic");

/*
 * Non-zero initializer places this object in .data rather than .bss.
 * volatile forces each test store/load to access the backing memory.
 */
static volatile unsigned char test_buffer[512] = { 0xA5 };

static int check_region(int start, int end, int multiplier, int addend,
                        const char *label)
{
    int i;

    for (i = start; i < end; i++)
        test_buffer[i] = (unsigned char)((i * multiplier + addend) & 0xff);

    for (i = start; i < end; i++) {
        unsigned char expected =
            (unsigned char)((i * multiplier + addend) & 0xff);
        unsigned char actual = test_buffer[i];

        if (actual != expected) {
            pr_err("[KPM-MEMTEST] %s: FAIL offset=%d expected=0x%02x actual=0x%02x\n",
                   label, i, expected, actual);
            return 0;
        }
    }

    pr_info("[KPM-MEMTEST] %s: PASS\n", label);
    return 1;
}

static int run_buffer_test(void)
{
    int first_ok, second_ok;

    first_ok = check_region(0, 256, 37, 11, "initialized .data first 256 bytes");
    second_ok = check_region(256, 512, 19, 7, "initialized .data second 256 bytes");

    pr_info("[KPM-MEMTEST] result: %s; initialized volatile .data buffer; no allocator, hooks, or kernel data modified\n",
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
    pr_info("[KPM-MEMTEST] CONTROL0: rerunning initialized .data test\n");
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
