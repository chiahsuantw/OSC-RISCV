/* Circular doubly linked list implementation */

#pragma once

#include <stddef.h>

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr: the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 * WARNING: any const qualifier of @ptr is lost.
 */
#define container_of(ptr, type, member)              \
    ({                                               \
        void *__mptr = (void *)(ptr);                \
        ((type *)(__mptr - offsetof(type, member))); \
    })

struct list_head {
    struct list_head *prev;
    struct list_head *next;
};

#define LIST_HEAD(name) struct list_head name = { &(name), &(name) }

/**
 * INIT_LIST_HEAD - Initialize a list_head structure
 * @list: list_head structure to be initialized.
 *
 * Initializes the list_head to point to itself.  If it is a list header,
 * the result is an empty list.
 */
static inline void INIT_LIST_HEAD(struct list_head *head)
{
    head->next = head;
    head->prev = head;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_add(struct list_head *node, struct list_head *prev,
                              struct list_head *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added.
 * @head: list head to add it after.
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *node, struct list_head *head)
{
    __list_add(node, head, head->next);
}

/**
 * list_add_tail - add a new entry
 * @new: new entry to be added.
 * @head: list head to add it before.
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *node, struct list_head *head)
{
    __list_add(node, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

/**
 * list_is_head - tests whether @list is the list @head
 * @list: the entry to test.
 * @head: the head of the list.
 */
static inline int list_is_head(const struct list_head *list,
                               const struct list_head *head)
{
    return list == head;
}

/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

/**
 * list_for_each - iterate over a list
 * @pos: the &struct list_head to use as a loop cursor.
 * @head: the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; !list_is_head(pos, (head)); pos = pos->next)

/**
 * list_entry - get the struct for this entry
 * @ptr: the &struct list_head pointer.
 * @type: the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
 * list_count_nodes - count nodes in the list
 * @head: the head for your list.
 */
static inline int list_count_nodes(struct list_head *head)
{
    struct list_head *pos;
    int count = 0;
    list_for_each(pos, head)
        count++;
    return count;
}