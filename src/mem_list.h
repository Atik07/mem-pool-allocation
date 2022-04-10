/*
To maintain a linked list of allocated
and free memory segments of the memory pool

* * NOTE: memory allocation list consumes extraneous space
* * allocated memory pool is not to be used for memory requirements of the allocation list
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
A segment in the memory pool is either allocated to a process or is an empty hole.
*/
enum allocation_type
{
    H = 0,
    P = 1
};

/*
Data Structure to store linked list of allocated and free memory segments.
Implemented using a doubly linked list. Each entry in the list specifies
- a pointer to previous item,
- a hole (H) or process allocation (P),
- the address at which it starts,
- the length,
- and a pointer to the next item.
*/
struct mem_list
{
    struct mem_list *prev;
    enum allocation_type type;
    void *start_addr;
    size_t size;
    struct mem_list *next;
};

/*
Initialisation of the linked list that stores the
allocated and free memory segments of the memory pool
*/
struct mem_list *root;

/*
Creates a node of the memory list type. Does NOT insert into the list.
*/
struct mem_list *create_node(
    enum allocation_type type,
    void *start_addr,
    size_t size)
{
    struct mem_list *node = (struct mem_list *)malloc(sizeof(struct mem_list));
    node->prev = NULL;
    node->type = type;
    node->start_addr = start_addr;
    node->size = size;
    node->next = NULL;
    return node;
}

/*
Creates and inserts a node in the memory list.
*/
struct mem_list *insert_node(
    struct mem_list *prev,
    enum allocation_type type,
    void *start_addr,
    size_t size,
    struct mem_list *next)
{
    struct mem_list *node = create_node(type, start_addr, size);
    node->prev = prev;
    if (node->prev)
        node->prev->next = node;
    node->next = next;
    if (node->next)
        node->next->prev = node;
    return node;
}

/*
Deletes a node from the list, provided pointer to the node
*/
void delete_node(struct mem_list *node)
{
    if (!node)
    {
        return;
    }
    if (node->prev)
    {
        node->prev->next = node->next;
    }
    if (node->next)
    {
        node->next->prev = node->prev;
    }
    free(node);
    return;
}

/*
Returns size of the memory allocation list
*/
int list_size(struct mem_list *root)
{
    struct mem_list *trav = root;
    int size = 0;
    while (trav)
    {
        size += 1;
        trav = trav->next;
    }
    return size;
}

/*
Prints the memory allocation list
*/
void print_list(struct mem_list *root)
{
    printf("---------------------------\n");
    struct mem_list *trav = root;
    while (trav)
    {
        printf("%d\t%p\t%lu\n",
               trav->type,
               (void *)trav->start_addr,
               trav->size);

        trav = trav->next;
    }
    printf("***************************\n");
    return;
}

/*
Deletes and Frees the memory of all the elements in the allocation list
*/
void free_list(struct mem_list *root)
{
    struct mem_list *temp;

    while (root != NULL)
    {
        temp = root;
        root = root->next;
        free(temp);
    }
    return;
}