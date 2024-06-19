#include <zephyr/kernel.h>

int var_1 = 11;
extern int var_shared;

/* Thread functions for application A */
void app_a_threads(void *arg1, void *arg2, void *arg3) {
    printk("App A, Thread %d: can access var_1 = %d and var_shared = %d\n", (int) arg1, var_1, var_shared);
    printk("App A, Thread %d: cannot access var_2\n", (int) arg1);
    k_sleep(K_FOREVER);
}

K_THREAD_DEFINE(tid_app_a1, 1024, app_a_threads, (void*) 1, NULL, NULL, 10, K_USER, 0);
K_THREAD_DEFINE(tid_app_a2, 1024, app_a_threads, (void*) 2, NULL, NULL, 10, K_USER, 0);
