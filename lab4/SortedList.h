/*
 * SortedList (and SortedListElement)
 *
 *A doubly linked list, kept sorted by a specified key.
 *This structure is used for a list head, and each element
 *of the list begins with this structure.
 *
 *The list head is in the list, and an empty list contains
 *only a list head.  The list head is also recognizable because
 *it has a NULL key pointer.
 */
struct SortedListElement {
  struct SortedListElement *prev;
  struct SortedListElement *next;
  const char *key;
};
typedef struct SortedListElement SortedList_t;
typedef struct SortedListElement SortedListElement_t;


extern int opt_yield;
#define INSERT_YIELD 0x01// yield in insert critical section
#define DELETE_YIELD 0x02// yield in delete critical section
#define SEARCH_YIELD 0x04// yield in lookup/length critical section

/**
 * SortedList_insert ... insert an element into a sorted list
 *
 *The specified element will be inserted in to
 *the specified list, which will be kept sorted
 *in ascending order based on associated keys
 *
 * @param SortedList_t *list ... header for the list
 * @param SortedListElement_t *element ... element to be added to the list
 *
 * Note: if (opt_yield & INSERT_YIELD)
 *call pthread_yield in middle of critical section
 */
void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{

  SortedListElement_t *curr = list->next;
  SortedListElement_t *p;
  while(curr != list)
    {
      if(strcmp(element->key, curr->key) <= 0)
	{
	  break;
	}
      curr = curr->next;
    }
  if (opt_yield & INSERT_YIELD)
    pthread_yield();
  p = curr->prev;
  element->next = curr;
  element->prev = p;
  curr->prev = element;
  p->next = element;  
 
}

/**
 * SortedList_delete ... remove an element from a sorted list
 *
 *The specified element will be removed from whatever
 *list it is currently in.
 *
 *Before doing the deletion, we check to make sure that
 *next->prev and prev->next both point to this node
 *
 * @param SortedListElement_t *element ... element to be removed
 *
 * @return 0: element deleted successfully, 1: corrtuped prev/next pointers
 *
 * Note: if (opt_yield & DELETE_YIELD)
 *call pthread_yield in middle of critical section
 */
int SortedList_delete( SortedListElement_t *element)
{
  SortedListElement_t * n = element->next;
  SortedListElement_t * p = element->prev;
  if(opt_yield & DELETE_YIELD)
    pthread_yield();

  if(p->next != element || n->prev != element)
    return 1;
  
  n->prev = p;
  p->next = n;
  element->next = NULL;
  element->prev = NULL;
  return 0;
}

/**
 * SortedList_lookup ... search sorted list for a key
 *
 *The specified list will be searched for an
 *element with the specified key.
 *
 * @param SortedList_t *list ... header for the list
 * @param const char * key ... the desired key
 *
 * @return pointer to matching element, or NULL if none is found
 *
 * Note: if (opt_yield & SEARCH_YIELD)
 *call pthread_yield in middle of critical section
 */
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
  SortedListElement_t *curr = list->next;
  if (opt_yield & SEARCH_YIELD)
    pthread_yield();
  while(curr != list)
    {
      if(strcmp(key, curr->key)==0)
	return curr;
      curr = curr->next;
    }
  return NULL;
}

/**
 * SortedList_length ... count elements in a sorted list
 *While enumeratign list, it checks all prev/next pointers
 *
 * @param SortedList_t *list ... header for the list
 *
 * @return int number of elements in list (excluding head)
 *   -1 if the list is corrupted
 *
 * Note: if (opt_yield & SEARCH_YIELD)
 *call pthread_yield in middle of critical section
 */
int SortedList_length(SortedList_t *list)
{
  int count = 0;
  SortedListElement_t *curr = list->next;
  if (opt_yield & SEARCH_YIELD)
    pthread_yield();
  while(curr != list)
    {
      count++;
      curr = curr->next;
    }
  return count;
}
