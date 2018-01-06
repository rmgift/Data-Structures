/* CS 261 Assignment 5 - Part 1
 * Name: Ryan Gift
 * Date: 05/28/17
 * Solution Description: This file implements the methods for the 
 * heap structure. The methods in this file add elements to the heap,
 * remove elements, return the 1st element and sort the heap. In order
 * to complete my adjust heap function I had to include an internal
 * function called indexSmallest which takes to indices, the heap,
 * and the compare function and returns the smallest index of the two.
 * This file should be combined with the dynamicyArray.h file, the 
 * tests.c file and the CuTest files.
 */
#include "dynamicArray.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define TESTING

#ifndef TESTING
static void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare);
static void buildHeap(DynamicArray* heap, compareFunction compare);
#endif

struct DynamicArray
{
    TYPE* data;
    int size;
    int capacity;
};


/*************************************************************************
 *                                                                       *
 *----------------------- Dynamic Array Functions -----------------------*
 *                                                                       *
 ************************************************************************/
static void setCapacity(DynamicArray* array, int capacity)
{
    TYPE* data = malloc(sizeof(TYPE) * capacity);
    for (int i = 0; i < array->size; i++)
    {
        data[i] = array->data[i];
    }
    free(array->data);
    array->data = data;
    array->capacity = capacity;
}

static void init(DynamicArray* array, int capacity)
{
    assert(capacity > 0);
    array->data = NULL;
    array->size = 0;
    setCapacity(array, capacity);
}

DynamicArray* dyNew(int capacity)
{
    DynamicArray* array = malloc(sizeof(DynamicArray));
    init(array, capacity);
    return array;
}

void dyDelete(DynamicArray* array)
{
    free(array->data);
    free(array);
}

void dyAdd(DynamicArray* array, TYPE value)
{
    if (array->size >= array->capacity)
    {
        setCapacity(array, 2 * array->capacity);
    }
    array->data[array->size] = value;
    array->size++;
}

void dyAddAt(DynamicArray* array, TYPE value, int position)
{
    assert(position <= array->size);
    dyAdd(array, value);
    for (int i = array->size - 1; i > position; i--)
    {
        dySwap(array, i, i - 1);
    }
}

void dyPut(DynamicArray* array, TYPE value, int position)
{
    assert(position < array->size);
    array->data[position] = value;
}

void dyRemoveAt(DynamicArray* array, int position)
{
    assert(position < array->size);
    for (int i = position; i < array->size - 1; i++)
    {
        array->data[position] = array->data[position + 1];
    }
    array->size--;
}

TYPE dyGet(DynamicArray* array, int position)
{
    assert(position < array->size);
    return array->data[position];
}

int dySize(DynamicArray* array)
{
    return array->size;
}

void dySwap(DynamicArray* array, int position1, int position2)
{
    assert(position1 < array->size);
    assert(position2 < array->size);
    TYPE temp = array->data[position1];
    array->data[position1] = array->data[position2];
    array->data[position2] = temp;
}



/*************************************************************************
*                                                                        *
*---------------------- Stack Interface Functions -----------------------*
*                                                                        *
*************************************************************************/
void dyStackPush(DynamicArray* stack, TYPE value)
{
    dyAdd(stack, value);
}

void dyStackPop(DynamicArray* stack)
{
    dyRemoveAt(stack, stack->size - 1);
}

TYPE dyStackTop(DynamicArray* stack)
{
    return dyGet(stack, stack->size - 1);
}

int dyStackIsEmpty(DynamicArray* stack)
{
    return stack->size == 0;
}



/*************************************************************************
*                                                                        *
*----------------------- Bag Interface Functions ------------------------*
*                                                                        *
*************************************************************************/
static int findFirst(DynamicArray* array, TYPE value, compareFunction compare)
{
    for (int i = 0; i < array->size; i++)
    {
        if (compare(value, array->data[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

void dyBagAdd(DynamicArray* bag, TYPE value)
{
    dyAdd(bag, value);
}

void dyBagRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = findFirst(bag, value, compare);
    if (position != -1)
    {
        dyRemoveAt(bag, position);
    }
}

int dyBagContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    return findFirst(bag, value, compare) != -1;
}


/*************************************************************************
*                                                                        *
*------------------- Ordered Bag Interface Functions --------------------*
*                                                                        *
*************************************************************************/
static int binarySearch(DynamicArray* array, TYPE value, compareFunction compare)
{
    int low = 0;
    int high = array->size - 1;
    while (low <= high)
    {
        int middle = (low + high) / 2;
        if (compare(value, array->data[middle]) < 0)
        {
            high = middle - 1;
        }
        else if (compare(value, array->data[middle]) > 0)
        {
            low = middle + 1;
        }
        else
        {
            return middle;
        }
    }
    return low;
}

void dyOrderedAdd(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    dyAddAt(bag,value, position);
}

void dyOrderedRemove(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    if (compare(value, bag->data[position]) == 0)
    {
        dyRemoveAt(bag, position);
    }
}

int dyOrderedContains(DynamicArray* bag, TYPE value, compareFunction compare)
{
    int position = binarySearch(bag, value, compare);
    return compare(value, dyGet(bag, position)) == 0;
}



/*************************************************************************
*                                                                        *
*----------------------- Heap Interface Functions -----------------------*
*                                                                        *
*************************************************************************/
/*
 * Internal routine to help adjustHeap function
 * takes heap and two index values to compare and then returns the smaller of the two
 */
int _indexSmallest(DynamicArray *heap, int i, int j, compareFunction compare)
{
	/* return index of smallest element */
	if (compare(dyGet(heap, i), dyGet(heap, j)) == -1) {
		return i;
	}
	else {
		return j;
	}
}

/*
 * Adjusts heap to maintain the heap property.
 * @param heap
 * @param last  index to adjust up to.
 * @param position  index where adjustment starts.
 * @param compare  pointer to compare function.
 */
void adjustHeap(DynamicArray* heap, int last, int position, compareFunction compare)
{
	/* last is the max position */
	/* position is the position to adjust from */
	int leftIdx = (position * 2) + 1;
	int rightIdx = (position * 2) + 2;
	int smallest;
	/* comparing root node to smallest of the two children */
	if (rightIdx < last) 
	{
		/* must have two children */
		/* get index of smallest */
		smallest = _indexSmallest(heap, leftIdx, rightIdx, compare);
		/* compare smallest child to pos */
		if (compare(dyGet(heap, smallest), dyGet(heap, position)) == -1)
		{
			/* if necessary swap and call adjustHeap() */
			dySwap(heap, position, smallest);
			adjustHeap(heap, last, smallest, compare);
		}
	}
	else if (leftIdx < last) 
	{
		/* check for two children */
		/* compare child to parent */
		if(compare(dyGet(heap, leftIdx), dyGet(heap, position)) == -1)
		{
			/* if necessary swap and call adjustHeap() */
			dySwap(heap, position, leftIdx);
			adjustHeap(heap, last, leftIdx, compare);
		}
	}
	/* else no children, at bottom and done */
}

/*
 * Builds a valid heap from an arbitrary array.
 * @param heap  array with elements in any order.
 * @param compare  pointer to compare function.
 */
void buildHeap(DynamicArray* heap, compareFunction compare)
{
	/* get the size of the array so we know where to start i */
	int max = dySize(heap);
	/* start the loop on the index that indicates first grouping of leaf nodes */
	for (int i = (max / 2) - 1; i >= 0; i--)
	{
		/* walk backwards down array and adjust nodes to make a heap */
		adjustHeap(heap, max, i, compare);
	}
}

/*
 * Adds an element to the heap.
 * @param heap
 * @param value  value to be added to heap.
 * @param compare  pointer to compare function.
 */
void dyHeapAdd(DynamicArray* heap, TYPE value, compareFunction compare)
{
	assert(heap != 0);

	int parent;
	/* pos is the size or next open spot */
	int pos = dySize(heap);
	/* adds to end – now need to adjust position */
	dyAdd(heap, value);

	while (pos != 0) 
	{
		/* compute the parent of the element we just added */
		parent = (pos - 1) / 2;

		/* compare value at pos to value at parent */
		if (compare(dyGet(heap, pos), dyGet(heap, parent)) == -1) 
		{
			/* if value at pos is less than value at parent, then swap*/
			dySwap(heap, parent, pos);
			/* repeat this by changing parent */
			pos = parent;
		}
		else 
		{
			return;
		}
	}
}

/*
 * Removes the first element, which has the min priority, form the heap.
 * @param heap
 * @param compare  pointer to the compare function.
 */
void dyHeapRemoveMin(DynamicArray* heap, compareFunction compare)
{
	/* Copy the last element to the first position */
	int last = ((heap->size) - 1);
	heap->data[0] = heap->data[last];
	/* Remove last element.*/
	dyRemoveAt(heap, last);
	/* Rebuild heap */
	adjustHeap(heap, last, 0, compare);
}

/*
 * Returns the first element, which has the min priority, from the heap.
 * @param heap
 * @return  Element at the top of the heap.
 */
TYPE dyHeapGetMin(DynamicArray* heap)
{
	/* return element 0 */
	assert(dySize(heap) > 0);
	return dyGet(heap, 0);
}

/*
 * Sorts arbitrary array in-place.
 * @param heap  array with elements in arbitrary order.
 * @param compare  pointer to the compare function.
 */
void dyHeapSort(DynamicArray* heap, compareFunction compare)
{
	/* first form the array into a heap */
	buildHeap(heap, compare);

	for (int i = dySize(heap) - 1; i > 0; i--)
	{
		/* the top of the heap is swapped with the last element */
		dySwap(heap, 0, i);
		/* size is reduced by 1 and readjusted */
		adjustHeap(heap, i, 0, compare);
	}
}



/*************************************************************************
*                                                                        *
*--------------------- Iterator Interface Functions ---------------------*
*                                                                        *
*************************************************************************/
DynamicArrayIterator* dyIteratorNew(DynamicArray* array)
{
    DynamicArrayIterator* iterator = malloc(sizeof(DynamicArrayIterator));
    iterator->array = array;
    iterator->current = 0;
    return iterator;
}

void dyIteratorDelete(DynamicArrayIterator* iterator)
{
    free(iterator);
}

int dyIteratorHasNext(DynamicArrayIterator* iterator)
{
    return iterator->current < iterator->array->size;
}

TYPE dyIteratorNext(DynamicArrayIterator* iterator)
{
    TYPE value = dyGet(iterator->array, iterator->current);
    iterator->current++;
    return value;
}

void dyIteratorRemove(DynamicArrayIterator* iterator)
{
    iterator->current--;
    dyRemoveAt(iterator->array, iterator->current);
}



/*************************************************************************
*                                                                        *
*-------------------------- Utility Functions ---------------------------*
*                                                                        *
*************************************************************************/
void dyPrint(DynamicArray* array, printFunction print)
{
    printf("\nsize: %d\ncapacity: %d\n[\n", array->size, array->capacity);
    for (int i = 0; i < array->size; i++)
    {
        printf("%d : ", i);
        print(array->data[i]);
        printf("\n");
    }
    printf("]\n");
}

void dyCopy(DynamicArray* source, DynamicArray* destination)
{
    free(destination->data);
    init(destination, source->capacity);
    for (int i = 0; i < source->size; i++)
    {
        destination->data[i] = source->data[i];
    }
    destination->size = source->size;
}