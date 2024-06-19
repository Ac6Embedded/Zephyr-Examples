#include <zephyr/kernel.h>

int var_2 = 22;
extern int var_shared;

/* Thread functions for application B */
void app_b_threads(void *arg1, void *arg2, void *arg3) {
    printk("App B, Thread %d: can access var_2 = %d and var_shared = %d\n", (int) arg1, var_2, var_shared);
    printk("App B, Thread %d: cannot access var_1\n", (int) arg1);
    k_sleep(K_FOREVER);
}

K_THREAD_DEFINE(tid_app_b1, 1024, app_b_threads, (void*) 1, NULL, NULL, 10, K_USER, 0);
K_THREAD_DEFINE(tid_app_b2, 1024, app_b_threads, (void*) 2, NULL, NULL, 10, K_USER, 0);