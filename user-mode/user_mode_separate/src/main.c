#include <zephyr/kernel.h>
#include <zephyr/app_memory/app_memdomain.h>
#include <zephyr/sys/libc-hooks.h>

/* Memory partitions definitions */
K_APPMEM_PARTITION_DEFINE(partition1);
K_APPMEM_PARTITION_DEFINE(partition2);
K_APPMEM_PARTITION_DEFINE(partition_shared);

/* Memory domains declarations */
struct k_mem_domain domain_a, domain_b;

extern int var_1;
extern int var_2;
extern int var_shared;

extern k_tid_t tid_app_a1, tid_app_a2, tid_app_b1, tid_app_b2;

/* Kernel thread function */
void kernel_thread(void *arg1, void *arg2, void *arg3) {
    printk("Kernel Thread: can access var_1 = %d, var_2 = %d, and var_shared = %d\n", var_1, var_2, var_shared);
    k_sleep(K_FOREVER);
}

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

