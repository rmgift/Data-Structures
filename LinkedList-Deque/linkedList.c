/* CS261- Assignment 3 - Part 1 */
/* Name: Ryan Gift
 * Date: 04/30/2017
 * Description: This program implements the deque and bag ADT with 
 * two sentinels. The methods below add elements to both data types, 
 * remove elements, print the elements, destroy structure, indicate 
 * if its empty, and if it contains the element. This file should be 
 * combined with the linkedList.h file and the linkedListMain.c file.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"

/* Double link */
struct Link
{
	TYPE value;
	struct Link* next;
	struct Link* prev;
};

/* Double linked list with front and back sentinels */
struct LinkedList
{
	int size;
	struct Link* frontSentinel;
	struct Link* backSentinel;
};


/*************************************************************************
 *                                                                       *
 *--------------------- Deque Linked List Functions ---------------------*
 *                                                                       *
 ************************************************************************/
/* Allocates the list's sentinel and sets the size to 0.
 * The sentinels' next and prev should point to eachother or NULL
 * as appropriate.
 */
static void init(struct LinkedList* list) {
	/* allocate front and back sentinels */
	list->frontSentinel = malloc(sizeof(struct Link));
	assert(list->frontSentinel != 0);
	list->backSentinel = malloc(sizeof(struct Link));
	assert(list->backSentinel != 0);
	/* assign front and back sentinel to appropriate locations */
	list->frontSentinel->prev = 0;
	list->frontSentinel->next = list->backSentinel;
	list->backSentinel->next = 0;
	list->backSentinel->prev = list->frontSentinel;
	/* set list size to zero as nothings in the list */
	list->size = 0;
}

/* Adds a new link with the given value before the given link and
 * increments the list's size.
 */
static void addLinkBefore(struct LinkedList* list, struct Link* link, TYPE value)
{
	/* Diagram for manipulation
	 * oldPrevLink  -  newLink  -  link (given)
	 */
	
	/* create a newLink to be added and save the given links previous */
	struct Link * newLink = (struct Link*)malloc(sizeof(struct Link));
	struct Link * currPrevLink = link->prev;
	/* assign all next and previous links appropriately */
	link->prev = newLink;
	currPrevLink->next = newLink;
	newLink->prev = currPrevLink;
	newLink->next = link;
	/* insertion complete, assign value to newLink and increment size */
	newLink->value = value;
	list->size++;
}

/* Removes the given link from the list and decrements the list's size. */
static void removeLink(struct LinkedList* list, struct Link* link)
{
	assert(!linkedListIsEmpty(list));
	/* reassign next and prev appropriately */
	link->prev->next = link->next;
	link->next->prev = link->prev;
	/* free link and reduce the size */
	free(link);
	list->size--;
}

/* Allocates and initializes a list. */
struct LinkedList* linkedListCreate()
{
	struct LinkedList* newDeque = malloc(sizeof(struct LinkedList));
	init(newDeque);
	return newDeque;
}

/* Deallocates every link in the list including the sentinels,
 * and frees the list itself.
 */
void linkedListDestroy(struct LinkedList* list)
{
	/* while our list is not empty remove front element */
	while (!linkedListIsEmpty(list))
	{
		linkedListRemoveFront(list);
	}
	/* free both sentinels then the list */
	free(list->frontSentinel);
	free(list->backSentinel);
	free(list);
}

/* Adds a new link with the given value to the front of the deque. */
void linkedListAddFront(struct LinkedList* list, TYPE value)
{
	addLinkBefore(list, list->frontSentinel->next, value);
}

/* Adds a new link with the given value to the back of the deque. */
void linkedListAddBack(struct LinkedList* list, TYPE value)
{
	addLinkBefore(list, list->backSentinel, value);
}

/* Returns the value of the link at the front of the deque. */
TYPE linkedListFront(struct LinkedList* list)
{
	/* assert list is not empty as to not return front sentinel */
	assert(!linkedListIsEmpty(list));
	return ((list->frontSentinel)->next)->value;
}

/* Returns the value of the link at the back of the deque. */
TYPE linkedListBack(struct LinkedList* list)
{
	/* assert list is not empty as to not return back sentinel */
	assert(!linkedListIsEmpty(list));
	return ((list->backSentinel)->prev)->value;
}

/* Removes the link at the front of the deque. */
void linkedListRemoveFront(struct LinkedList* list)
{
	/* assert not empty as to not remove back sentinel */
	assert(!linkedListIsEmpty(list));
	removeLink(list, list->frontSentinel->next);
}

/* Removes the link at the back of the deque. */
void linkedListRemoveBack(struct LinkedList* list)
{
	/* assert not empty as to not remove front sentinel */
	assert(!linkedListIsEmpty(list));
	removeLink(list, list->backSentinel->prev);
}

/* Returns 1 if the deque is empty and 0 otherwise. */
int linkedListIsEmpty(struct LinkedList* list)
{
	return list->size == 0;
}

/* Prints the values of the links in the deque from front to back. */
void linkedListPrint(struct LinkedList* list)
{
	struct Link *temp = list->frontSentinel->next;
	/* while not the back sentinel, print our values */
	while (temp != list->backSentinel)
	{
		printf("%d->", temp->value);
		/* temp gets next position */
		temp = temp->next;
	}
	printf("\n");
}



/*************************************************************************
 *                                                                       *
 *------------------------- Deque Bag Functions -------------------------*
 *                                                                       *
 ************************************************************************/
/* Adds a link with the given value to the bag. */
void linkedListAdd(struct LinkedList* list, TYPE value)
{
	/* assert list is not empty and add value to end */
	assert(list != 0);
	linkedListAddBack(list, value);
}

/* Returns 1 if a link with the value is in the bag and 0 otherwise. */
int linkedListContains(struct LinkedList* list, TYPE value)
{
	/* assert list is not empty */
	assert(list != 0);
	assert(!linkedListIsEmpty(list));
	struct Link *newLnk = list->frontSentinel->next;
	/* iterate the list and return true if we found the value */
	while (newLnk != list->backSentinel)
	{
		if (EQ(newLnk->value, value))
		{
			return 1;
		}
		newLnk = newLnk->next;
	}
	return 0;
}

/* Removes the first occurrence of a link with the given value. */
void linkedListRemove(struct LinkedList* list, TYPE value)
{
	/* assert list is not empty */
	assert(list != 0);
	assert(!linkedListIsEmpty(list));

	struct Link *temp = list->frontSentinel->next;
	/* iterate list and if value found, remove it*/
	while (temp != list->backSentinel)
	{
		if (EQ(temp->value, value))
		{
			removeLink(list, temp);
			return;	/* only remove the one copy */
		}
		temp = temp->next;
	}
}