#include "sched.h"
#include "mm.h"

static int nr_threads = 0;
static struct list_head runqueue;

extern void switch_to(struct task_struct *prev, struct task_struct *next);

struct task_struct *get_current()
{
    struct task_struct *current;
    asm volatile("mv %0, tp" : "=r"(current));
    return current;
}

struct task_struct *find_task(int pid)
{
    struct list_head *pos;
    list_for_each(pos, &runqueue) {
        struct task_struct *task = list_entry(pos, struct task_struct, list);
        if (task->pid == pid)
            return task;
    }
    return 0;
}

void schedule()
{
    // TODO: Disable interrupt?
    // TODO: Implement list_next_entry_circular
    struct task_struct *next = 0;
    switch_to(get_current(), next);
}

void kill_zombies()
{
    struct list_head *pos, *tmp;
    list_for_each_safe(pos, tmp, &runqueue) {
        struct task_struct *task = list_entry(pos, struct task_struct, list);
        if (task->state == TASK_ZOMBIE) {
            list_del_init(&task->list);
            kfree((void *)task->kernel_sp);
            kfree((void *)task->user_sp);
            kfree(task);
        }
    }
}

void idle()
{
    while (1) {
        kill_zombies();
        schedule();
    }
}

void kthread_init()
{
    INIT_LIST_HEAD(&runqueue);
    struct task_struct *init = kthread_create(idle);
    asm volatile("mv tp, %0" : : "r"(init));
}

struct task_struct *kthread_create(void (*threadfn)())
{
    struct task_struct *task = kmalloc(sizeof(struct task_struct));
    task->pid = nr_threads++;
    task->state = TASK_RUNNING;
    task->kernel_sp = (long)kmalloc(STACK_SIZE);
    task->user_sp = (long)kmalloc(STACK_SIZE);
    task->context.ra = (unsigned long)threadfn;
    task->context.sp = task->kernel_sp + STACK_SIZE;
    list_add_tail(&task->list, &runqueue);
    return task;
}

void kthread_stop(struct task_struct *task)
{
    task->state = TASK_ZOMBIE;
    schedule();
}

void kthread_exit()
{
    get_current()->state = TASK_ZOMBIE;
    schedule();
}
