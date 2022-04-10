#include "mavalloc.h"
#include "mem_list.h"

int mavalloc_init(size_t size, enum ALGORITHM algorithm)
{
    if (size < 0)
        return -1;

    // size is 4-byte aligned
    size_t requested_size = ALIGN4(size);

    // allocating memory pool
    pool = (void *)malloc(requested_size);

    // malloc returns NULL when unable to allocate memory
    if (pool == NULL)
        return -1;

    MEM_ALGO = algorithm;

    // initialisation of the memory allocation linked list
    root = insert_node(NULL, H, pool, requested_size, NULL);

    return 0;
}

/**
 * FIRST_FIT and NEXT_FIT memory allocation algorithms
 * result in early viability of a memory block
 * since these algorithms do not compare all the available spaces.
 *
 * The following function implements the general algorithm behind the two allocation schemes
 * */
struct mem_list *early_viable_fit(size_t size, struct mem_list *start_pos)
{
    struct mem_list *trav;
    // starting position of the list varies according to the algorithm
    trav = start_pos;

    // traverse the list to find a Hole that can fit the memory request and insert node
    while (trav)
    {
        if (trav->type == H && trav->size >= size) // Hole that can fit the memory request
        {
            struct mem_list *temp =
                insert_node(
                    trav, H,
                    trav->start_addr + size,
                    trav->size - size,
                    trav->next);
            trav->type = P;
            trav->size = size;
            return trav; // returns the first fitting instance
        }
        trav = trav->next;
    }
    printf("Requested amount of memory is UNAVAILABLE\n");
    return NULL;
}

/**
 * BEST_FIT and WORST_FIT memory allocation algorithms
 * compare all the available memory blocks before allocating
 *
 * The following function implements the general algorithm behind the two allocation schemes
 * */
struct mem_list *comparative_fit(size_t size, bool borw)
// borw stands for BEST_FIT or WORST_FIT -- borw=true for BEST; false otherwise
{
    struct mem_list *trav;
    struct mem_list *position;

    trav = root;
    position = NULL;

    // whole list is traversed in deciding the position to insert the node
    while (trav)
    {
        if (trav->type == H && trav->size >= size) // Hole that can fit the memory request
        {
            if ((!position) || (borw & (trav->size < position->size)) |       // for BEST_FIT
                                   ((!borw) & (trav->size > position->size))) // for WORST_FIT
            {
                position = trav;
            }
        }
        trav = trav->next;
    }
    if (position)
    {
        // node insertion
        struct mem_list *temp = insert_node(
            position, H, position->start_addr + size,
            position->size - size, position->next);
        position->type = P;
        position->size = size;
        return position; // returns fitting position according to the algorithm
    }
    // on failure in allocation
    return NULL;
}

void *mavalloc_alloc(size_t size)
{
    // size is 4-byte aligned
    size = ALIGN4(size);

    /*
    start position determines where the next hole should be
    looked from in NEXT_FIT allocation algorithm.
    It is a static field in order to strore the previous position
    */
    static struct mem_list *start_pos;

    /* calling memory allocation scheme with appropriate parameters */
    switch (MEM_ALGO)
    {

    /*
    An incoming process gets allocated into a Hole of size greater than or equal to it.
    Searching for Holes starts from the beginning (root) of the allocation list.
    */
    case FIRST_FIT:
        return early_viable_fit(size, root);

    /*
    An incoming process gets allocated into a Hole of size greater than or equal to it.
    Searching for Holes starts from where the previous NEXT_FIT search ended.
    */
    case NEXT_FIT:
        if (!start_pos)
        {
            start_pos = root;
        }
        start_pos = early_viable_fit(size, start_pos);
        return start_pos;

    /*
    BEST_FIT search finds the least sized Hole capable of holding the incoming process.
    */
    case BEST_FIT:
        return comparative_fit(size, true); // borw=true for BEST_FIT

    /*
    WORST_FIT search finds the largest Hole and allocates the incoming process to it.
    */
    case WORST_FIT:
        return comparative_fit(size, false); // borw=false for WORST_FIT

    default:
        printf("Invalid Memory Allocation Algorithm");
        break;
    }

    // only return NULL on failure
    return NULL;
}

void mavalloc_free(void *ptr)
{
    // if node to be freed is NULL do nothing
    struct mem_list *node = ptr;
    if (!node)
        return;

    /*
        COALESCING - If there are two consecutive blocks free then combine (coalesce) them.
    */
    size_t size = node->size;            // size of block to be coalesced
    void *start_addr = node->start_addr; // starting address of block to be coalesced

    // if previous block is a Hole - to be combined to the new hole
    // updating new hole parameters and deleting the previous node
    if (node->prev && node->prev->type == H)
    {
        size += node->prev->size;
        start_addr = node->prev->start_addr;
        if (root == node->prev)
            root = node;
        delete_node(node->prev);
    }
    // if next block is a Hole - to be combined to the new hole
    // updating new hole parameters and deleting the next node
    if (node->next && node->next->type == H)
    {
        size += node->next->size;
        delete_node(node->next);
    }

    // assigning new hole parameters to the existing node
    // essentially transforming it into the new Hole
    node->type = H;
    node->size = size;
    node->start_addr = start_addr;

    return;
}

int mavalloc_size()
{
    return list_size(root);
}

void mavalloc_destroy()
{
    /* Delete the memory allocation list */
    free_list(root);
    /* Free the memory held by the pool */
    free(pool);
    return;
}