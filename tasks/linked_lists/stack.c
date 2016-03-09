#include "stack.h"
#include <linux/slab.h>
#include <linux/gfp.h>

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* result = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    if (result)
    {
        INIT_LIST_HEAD(&(result->lh));
        result->data = data;
    }
    return result;
}

void delete_stack_entry(stack_entry_t *entry)
{
    list_del(&(entry->lh));
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
   list_add(&(entry->lh), stack); 
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    if (list_empty(stack))
        return NULL;
    return list_first_entry(stack, stack_entry_t, lh);
}
