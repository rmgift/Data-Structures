/* CS261- Assignment 3 - Part 3
 * Name: Ryan Gift
 * Date: 04/30/2017
 * Solution description: This program implements the Deque ADT
 * with a Circularly-Doubly-Linked List with 1 sentinel. The methods below
 * add elements to the list, remove elements, print the list, reverse the
 * list, destroy the list, and indicate if the list is empty. This file should
 * be combined with the circularList.h file and the circularListMain.c file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "circularList.h"

// Double link
struct Link
{
	TYPE value;
	struct Link * next;
	struct Link * prev;
};

struct CircularList
{
	int size;
	struct Link* sentinel;
};

/*
 * Allocates the list's sentinel and sets the size to 0.
 * The sentinel's next and prev should point to the sentinel itself.
 */
static void init(struct CircularList* list)
{
	list->sentinel = malloc(sizeof(struct Link));
	assert(list->sentinel != 0);

	list->sentinel->next = list->sentinel;
	list->sentinel->prev = list->sentinel;
	list->sentinel->value = 0;
	list->size = 0;
}

/* Creates a link with the given value and NULL next and prev pointers. */
static struct Link* createLink(TYPE value)
{
	/* create link and assign value */
	struct Link *newLink = malloc(sizeof(struct Link));
	newLink->value = value;

	/* assign next and prev to NULL and return for assignments */
	newLink->next = NULL;
	newLink->prev = NULL;
	return newLink;
}

/*
 * Adds a new link with the given value after the given link and
 * increments the list's size.
 */
static void addLinkAfter(struct CircularList* list, struct Link* link, TYPE value)
{
	/* call createLink to create a link with the given value */
	struct Link *newLink = createLink(value);
	/* reassign next and prev based on link */
	newLink->next = link->next;
	newLink->prev = link->next->prev;
	newLink->next->prev = newLink;
	newLink->prev->next = newLink;
	// increment size for new value
	list->size++;
}

/* Removes the given link from the list and decrements the list's size. */
static void removeLink(struct CircularList* list, struct Link* link)
{
	assert(!circularListIsEmpty(list));
	/* reassign prev and next to remove link connection */
	link->prev->next = link->next;
	link->next->prev = link->prev;
	/* free link/memory and reduce size by 1 */
	free(link);
	list->size--;
}

/* Allocates and initializes a list. */
struct CircularList* circularListCreate()
{
	/* allocate size for list, initialize it and return list */
	struct CircularList* list = malloc(sizeof(struct CircularList));
	init(list);
	return list;
}

/* Deallocates every link in the list and frees the list pointer. */
void circularListDestroy(struct CircularList* list)
{
	/* while the list is not empty delete first position data */
	while (!circularListIsEmpty(list))
	{
		removeLink(list, list->sentinel->next);
	}
	/* all data removed, free sentinel, then list */
	free(list->sentinel);
	free(list);
}

/* Adds a new link with the given value to the front of the deque. */
void circularListAddFront(struct CircularList* list, TYPE value)
{
	addLinkAfter(list, list->sentinel->next, value);
}

/* Adds a new link with the given value to the back of the deque. */
void circularListAddBack(struct CircularList* list, TYPE value)
{
	addLinkAfter(list, list->sentinel->prev, value);
}

/* Returns the value of the link at the front of the deque. */
TYPE circularListFront(struct CircularList* list)
{
	/* assert as to not return sentinel value */
	assert(!circularListIsEmpty(list));
	return ((list->sentinel)->next)->value;
}

/* Returns the value of the link at the back of the deque. */
TYPE circularListBack(struct CircularList* list)
{
	/* assert as to not return sentinel value */
	assert(!circularListIsEmpty(list));
	return ((list->sentinel)->prev)->value;
}

/* Removes the link at the front of the deque. */
void circularListRemoveFront(struct CircularList* list)
{
	/* assert as to not remove sentinel */
	assert(!circularListIsEmpty(list));
	removeLink(list, list->sentinel->next);
}

/* Removes the link at the back of the deque. */
void circularListRemoveBack(struct CircularList* list)
{
	/* assert as to not remove sentinel */
	assert(!circularListIsEmpty(list));
	removeLink(list, list->sentinel->prev);
}

/* Returns 1 if the deque is empty and 0 otherwise. */
int circularListIsEmpty(struct CircularList* list)
{
	return list->size == 0;
}

/* Prints the values of the links in the deque from front to back. */
void circularListPrint(struct CircularList* list)
{
	/* create temp Link for position */
	struct Link *temp = list->sentinel->next;
	/* while not the sentinel, print our values */
	while (temp != list->sentinel)
	{
		printf("%g->", temp->value);
		/* temp gets next position */
		temp = temp->next;
	}

	printf("\n");
}

/* Reverses the deque. */
void circularListReverse(struct CircularList* list)
{
	/* assert as to not indicate false positive of empty list reversing */
	assert(!circularListIsEmpty(list));
	/* create temp for sentinel to iterate through list */
	struct Link *temp = list->sentinel;
	/* iterate across all elements and slide sentinel to opposite end of list */
	for (int i = 0; i < (list->size + 1); i++)
	{
		/* set current data point */
		struct Link *currData = temp->next;
		/* set next data to prev data (slide our sentinel) */
		temp->next = temp->prev;
		/* set prev data to saved data */
		temp->prev = currData;
		/* move temp forward to sentinel, 1 position ahead now */
		temp = temp->next;
	}
}