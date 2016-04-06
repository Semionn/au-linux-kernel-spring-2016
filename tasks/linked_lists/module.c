#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/errno.h>

#include "stack.h"
#include "assert.h"

static void __init test_stack(void)
{
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
            create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
}

static int __init print_processes_backwards(void)
{
    int result = 0;
    LIST_HEAD(process_stack);
    struct task_struct *task = NULL;
    stack_entry_t* stack_entry = NULL;
    char* process_file_name = NULL;
    
    for_each_process(task)
    {
        process_file_name = (char*)kmalloc(sizeof(task->comm), GFP_KERNEL);
        if (!process_file_name)
        {
            result = -ENOMEM;
            goto exit;
        }

        get_task_comm(process_file_name, task);

        stack_entry = create_stack_entry((void*) process_file_name);
        if (!stack_entry)
        {
            result = -ENOMEM;
            kfree(process_file_name);
            goto exit;
        }

        stack_push(&process_stack, stack_entry);
    }

exit:

    while (!stack_empty(&process_stack))
    {
        stack_entry = stack_pop(&process_stack);
        process_file_name = STACK_ENTRY_DATA(stack_entry, char*);
        delete_stack_entry(stack_entry);
        printk(KERN_ALERT "%s\n", process_file_name);
        kfree(process_file_name);
    }

    return result;
}

static int __init ll_init(void)
{
    printk(KERN_ALERT "Hello, linked_lists\n");
    test_stack();
    print_processes_backwards();
    return 0;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
