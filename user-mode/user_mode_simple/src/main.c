#include <zephyr/kernel.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/sys/libc-hooks.h>

/* Memory partitions definitions */
K_APPMEM_PARTITION_DEFINE(partition1);
K_APPMEM_PARTITION_DEFINE(partition2);
K_APPMEM_PARTITION_DEFINE(partition_shared);

/* Memory domains declarations */
struct k_mem_domain domain_a;
struct k_mem_domain domain_b;

/* Variables in specific memory partitions */
K_APP_DMEM(partition1) int var_1 = 11;
K_APP_DMEM(partition2) int var_2 = 22;
K_APP_DMEM(partition_shared) int var_shared = 99;

/* Thread functions for application A */
void app_a_threads(void *arg1, void *arg2, void *arg3) {
    printk("App A, Thread %d: can access var_1 = %d and var_shared = %d\n", (int) arg1, var_1, var_shared);
    printk("App A, Thread %d: cannot access var_2\n", (int) arg1);
    k_sleep(K_FOREVER);
}

/* Thread functions for application B */
void app_b_threads(void *arg1, void *arg2, void *arg3) {
    printk("App B, Thread %d: can access var_2 = %d and var_shared = %d\n", (int) arg1, var_2, var_shared);
    printk("App B, Thread %d: cannot access var_1\n", (int) arg1);
    k_sleep(K_FOREVER);
}

/* Kernel thread function */
void kernel_thread(void *arg1, void *arg2, void *arg3) {
    printk("Kernel Thread: can access var_1 = %d, var_2 = %d, and var_shared = %d\n", var_1, var_2, var_shared);
    k_sleep(K_FOREVER);
}

/* Threads definition for different applications */
K_THREAD_DEFINE(tid_app_a1, 1024, app_a_threads, (void*) 1, NULL, NULL, 10, K_USER, 0);
K_THREAD_DEFINE(tid_app_a2, 1024, app_a_threads, (void*) 2, NULL, NULL, 10, K_USER, 0);
K_THREAD_DEFINE(tid_app_b1, 1024, app_b_threads, (void*) 1, NULL, NULL, 10, K_USER, 0);
K_THREAD_DEFINE(tid_app_b2, 1024, app_b_threads, (void*) 2, NULL, NULL, 10, K_USER, 0);

/* Kernel thread */
K_THREAD_DEFINE(tid_kernel, 1024, kernel_thread, NULL, NULL, NULL, 10, 0, K_FP_REGS);

/* Memory partition configuration arrays */
struct k_mem_partition *app_a_partitions[] = {
#ifdef CONFIG_CPU_CORTEX_M
    &z_libc_partition,
#endif
    &partition1,
    &partition_shared
};

struct k_mem_partition *app_b_partitions[] = {
#ifdef CONFIG_CPU_CORTEX_M
    &z_libc_partition,
#endif
    &partition2,
    &partition_shared
};

/* Main function to initialize domains and add threads to them */
int main(void)
{
    /* Initialize and assign partitions to domains */
    k_mem_domain_init(&domain_a, ARRAY_SIZE(app_a_partitions), app_a_partitions);
    k_mem_domain_init(&domain_b, ARRAY_SIZE(app_b_partitions), app_b_partitions);

    /* Add app1 threads to domain a */
    k_mem_domain_add_thread(&domain_a, tid_app_a1);
    k_mem_domain_add_thread(&domain_a, tid_app_a2);
    
    /* Add app2 threads to domain b */
    k_mem_domain_add_thread(&domain_b, tid_app_b1);
    k_mem_domain_add_thread(&domain_b, tid_app_b2);

    return 0;
}
