#include "mavalloc.h"
#include "mem_list.h"

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  if (size < 0)    return -1;

  size_t requested_size = ALIGN4( size );
  
  pool = (void *) malloc(requested_size);

  // malloc returns NULL when unable to allocate memory
  if (pool == NULL) return -1;
  
  MEM_ALGO = algorithm;

  root = insert_node(NULL,H,pool,requested_size,NULL);

  return 0;
}

struct mem_list *early_viable_fit(size_t size, struct mem_list *start_pos){
  struct mem_list *trav;
  trav = start_pos;
  while (trav)
  {   
      if (trav->type==H && trav->size >= size)
      {
          struct mem_list* temp = insert_node(trav,H,trav->start_addr+size,trav->size-size,trav->next);
          trav->type = P;
          trav->size = size;
          return trav;
      }
      trav = trav->next;
  }
  printf("Requested amount of memory is UNAVAILABLE\n");
  return NULL;
}

struct mem_list *comparative_fit(size_t size, bool borw){
  struct mem_list *trav;
  struct mem_list *position;

  trav = root;
  position = NULL;

  while (trav)
  {
      if (trav->type==H && trav->size >= size)
      {
         if((!position) || (borw&(trav->size < position->size))|((!borw)&(trav->size > position->size))){
           position = trav;
         }
      }
      trav = trav->next;
  }
  if (position)
  {
    struct mem_list* temp = insert_node(position,H,position->start_addr+size,position->size-size,position->next);
    position->type = P;
    position->size = size;
    return position;
  }
  return NULL;
}

void * mavalloc_alloc( size_t size )
{
  size = ALIGN4(size);

  static struct mem_list* start_pos;

  switch (MEM_ALGO)
  {
    case FIRST_FIT:
      return early_viable_fit(size,root);
    case NEXT_FIT:
      if (!start_pos)
      {
        start_pos = root;
      }
      start_pos = early_viable_fit(size,start_pos);
      return start_pos;
    case BEST_FIT:
      return comparative_fit(size,true); // borw=true for best fit
    case WORST_FIT:
      return comparative_fit(size,false);
    default:
      printf("Invalid Memory Allocation Algorithm");
      break;
  }

  // only return NULL on failure
  return NULL;
}

void mavalloc_free( void * ptr )
{
  //coalescing

  struct mem_list* node = ptr;
  
  if(!node)    return;

  size_t size = node->size;
  void *start_addr = node->start_addr;

  bool is_prev_hole = false;
  bool is_next_hole = false;
  if(node->prev && node->prev->type == H){
    is_prev_hole = true;
    size += node->prev->size;
    start_addr = node->prev->start_addr;
    if(root==node->prev)  root=node;
    delete_node(node->prev);
  }
  if(node->next && node->next->type == H){
    is_next_hole = true;
    size += node->next->size;
    delete_node(node->next);
  }

  node->type = H;
  node->size = size;
  node->start_addr = start_addr;

  return;
}

int mavalloc_size( )
{
  return list_size(root);
}

void mavalloc_destroy( )
{
  free_list(root);
  free(pool);
  return;
}