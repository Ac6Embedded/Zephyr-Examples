#include <zephyr/kernel.h>

/* Define the semaphore (kernel object) */
K_SEM_DEFINE(my_sem, 0, 1);

/* User thread1 entry function */
void user_thread1(void *p1, void *p2, void *p3) {
    if (k_sem_take(&my_sem, K_FOREVER) == 0) {
        printk("User thread1: Successfully accessed the semaphore.\n");
    }
}

/* User thread2 entry function */
void user_thread2(void *p1, void *p2, void *p3) {
    /* It will fail here as user thread2 doesn't have the permission to access my_sem */
    k_sem_take(&my_sem, K_FOREVER);
}

/* Kernel thread entry function */
void kernel_thread(void *p1, void *p2, void *p3) {
    printk("Kernel thread: Access any kernel object without explicit permission.\n");
    k_sem_give(&my_sem);
}

/* Fatal error handler */
void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf) {
	if(reason == K_ERR_KERNEL_OOPS) {
		printk("Kernel OOPS in : %s\n", k_thread_name_get(k_current_get()));
    }
}

/* Define the threads */
K_THREAD_DEFINE(kernel_thread_id, 1024, kernel_thread, NULL, NULL, NULL, 7, 0, 0);
K_THREAD_DEFINE(user_thread1_id, 1024, user_thread1, NULL, NULL, NULL, 5, K_USER, 0);
K_THREAD_DEFINE(user_thread2_id, 1024, user_thread2, NULL, NULL, NULL, 6, K_USER, 0);

int main(void) {   
    k_object_access_grant(&my_sem, user_thread1_id);
    printk("Main function: Access granted to user thread1.\n");
    
    k_thread_name_set(user_thread2_id, "User Thread 2");
    return 0;
}