/*
 *   linkedListQueue.c 
 *
 *   Linked list implementation of an abstract queue
 *
 *   Created by Sally Goldin, 22 January 2013, for CPE 113
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "abstractQueue.h"

/* Structure that represents one item of the list */
typedef struct _listitem
{
    void* pData;               /* Pointer to the data for this node */
    struct _listitem * pNext;  /* Link to the next item in the list */
} LISTITEM_T;


/* global head and tail pointers */
LISTITEM_T * head = NULL;    /* first item in list */  
LISTITEM_T * tail = NULL;    /* last item in the list */

int count = 0;               /* for convenience, keep track of current size */

/**
 * Add a data item to the queue (end of the list)
 * Arguments:
 *   data      -   Pointer to generic data we want to add to queue   
 * Returns 1 if successful, 0 if we have run out of space.
 */
int enqueue(void* data)
{
   int bOk = 1;
   LISTITEM_T * pNew = calloc(1,sizeof(LISTITEM_T));
   /* If allocation was successful */
   if (pNew != NULL)
      {
      pNew->pData = data; /* copy the data to the list item */
      if (head == NULL)   /* queue is empty */
         {
	 head = pNew;
         }
      else
         {
         tail->pNext = pNew; /* add to end of queue */
	 }
      tail = pNew;           /* and update the tail */
      count++;
      }
   else
      {
      bOk = 0;
      }
   return bOk;
}


/* Get the next item in the queue. This is the element 
 * at the front of the queue.
 * Returns the data stored at the front of the queue.
 * Also removes that item from the queue.
 * Returns NULL if the queue is empty.
 */
void * dequeue()
{
   void * returnData = NULL;
   if (count > 0)
      {
      LISTITEM_T* pFirst = head;  /* we will dequeue the first item */
      returnData = pFirst->pData;
      head = pFirst->pNext;       /* after dequeue we'll have a new head */
      if (head == NULL)           /* if this was last item in queue */
	  tail = NULL;
      count--;
      free(pFirst);
      }
   return returnData;
}


/* Find out how many items are currently in the queue.
 * Return number of items in the queue (could be zero)
 */
int queueSize()
{
   return count;
}


/* Clear so we can reuse 
 */
void queueClear()
{
    LISTITEM_T* pCurrent = head;  
    LISTITEM_T* pTrash = NULL;
    while(pCurrent != NULL)
       {
       pTrash = pCurrent;
       pCurrent = pCurrent->pNext;
       free(pTrash);
       }
    /* reset head, tail and count */
    head = NULL;
    tail = NULL;
    count = 0;
}


/** DEBUGGING FUNCTION PRINTS SOME INFO ABOUT THE QUEUE **/
void printDebug()
{
   printf("linkedListQueue: count = %d\n",count);
   if (count > 0)
      {
      printf("Contents: \n");
      LISTITEM_T* pCurrent = head;  
      while (pCurrent != NULL)
          {
	    printf("\t\t%s\n", (char*) pCurrent->pData);
	  pCurrent = pCurrent->pNext;
          } 
      }
}
