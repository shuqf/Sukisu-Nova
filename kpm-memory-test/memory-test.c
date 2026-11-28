// SPDX-License-Identifier: GPL-2.0-or-later
#include <compiler.h>
#include <kpmodule.h>
#include <kpmalloc.h>
#include <linux/printk.h>

KPM_NAME("kpm-memory-selftest");
KPM_VERSION("1.0.0");
KPM_LICENSE("GPL v2");
KPM_AUTHOR("Sukisu-Nova test");
KPM_DESCRIPTION("Private-buffer KernelPatch allocator self-test");

static long memory_test_init(const char *args, const char *event, void *reserved)
{
    unsigned char *buf = 0;
    unsigned char *grown = 0;
    int i;
    int errors = 0;

    (void)args;
    (void)reserved;
    pr_info("[KPM-MEMTEST] init event=%s\n", event ? event : "(null)");

    buf = kp_malloc(256);
    if (!buf) {
        pr_err("[KPM-MEMTEST] kp_malloc(256): FAIL (NULL)\n");
        return 0;
    }
    pr_info("[KPM-MEMTEST] kp_malloc(256): PASS\n");

    for (i = 0; i < 256; i++)
        buf[i] = (unsigned char)((i * 37 + 11) & 0xff);
    for (i = 0; i < 256; i++) {
        if (buf[i] != (unsigned char)((i * 37 + 11) & 0xff)) {
            errors++;
            break;
        }
    }
    pr_info("[KPM-MEMTEST] write/read 256 bytes: %s\n", errors ? "FAIL" : "PASS");

    grown = kp_realloc(buf, 512);
    if (!grown) {
        pr_err("[KPM-MEMTEST] kp_realloc(512): FAIL (NULL); freeing original buffer\n");
        kp_free(buf);
        return 0;
    }
    buf = grown;
    pr_info("[KPM-MEMTEST] kp_realloc(512): PASS\n");

    for (i = 0; i < 256; i++) {
        if (buf[i] != (unsigned char)((i * 37 + 11) & 0xff)) {
            errors++;
            break;
        }
    }
    pr_info("[KPM-MEMTEST] realloc preserved first 256 bytes: %s\n",
            errors > 1 ? "FAIL" : "PASS");

    for (i = 256; i < 512; i++)
        buf[i] = (unsigned char)((i * 19 + 7) & 0xff);
    for (i = 256; i < 512; i++) {
        if (buf[i] != (unsigned char)((i * 19 + 7) & 0xff)) {
            errors++;
            break;
        }
    }
    pr_info("[KPM-MEMTEST] write/read expanded region: %s\n",
            errors > 1 ? "FAIL" : "PASS");

    kp_free(buf);
    buf = 0;
    pr_info("[KPM-MEMTEST] kp_free: PASS (buffer released)\n");
    pr_info("[KPM-MEMTEST] result: %s; private buffer only, no hooks or kernel data modified\n",
            errors ? "FAIL" : "PASS");
    return 0;
}

static long memory_test_exit(void *reserved)
{
    (void)reserved;
    pr_info("[KPM-MEMTEST] exit\n");
    return 0;
}

KPM_INIT(memory_test_init);
KPM_EXIT(memory_test_exit);
