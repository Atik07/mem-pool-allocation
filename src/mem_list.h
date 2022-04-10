#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum allocation_type
{
    H = 0,
    P = 1
};

struct mem_list
{
    struct mem_list *prev;
    enum allocation_type type;
    void *start_addr;
    size_t size;
    struct mem_list *next;
};

struct mem_list* root;

struct mem_list* create_node(
    enum allocation_type type,
    void *start_addr,
    size_t size)
{
  struct mem_list* node = (struct mem_list*)malloc(sizeof(struct mem_list));
  node->prev=NULL;
  node->type=type;
  node->start_addr=start_addr; 
  node->size=size;
  node->next= NULL;
  return node;
}

struct mem_list* insert_node(
    struct mem_list* prev,
    enum allocation_type type,
    void *start_addr,
    size_t size,
    struct mem_list* next)
{
    struct mem_list* node = create_node(type,start_addr,size);
    node->prev = prev;
    if(node->prev) node->prev->next = node;
    node->next = next;
    if(node->next) node->next->prev = node;
    return node;
}

void delete_node(struct mem_list *node)
{   
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

void print_list(struct mem_list *root)
{
    printf("-------------------\n");
    struct mem_list *trav = root;
    while (trav)
    {
        printf("%d\t%p\t%lu\n", trav->type, (void *)trav->start_addr, trav->size);
        trav = trav->next;
    }
    printf("********************\n");
    return;
}

void free_list(struct mem_list* root){
    struct mem_list* temp;

    while (root != NULL)
    {
        temp = root;
        root = root->next;
        free(temp);
    }
    return;
}