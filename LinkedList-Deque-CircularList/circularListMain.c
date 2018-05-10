/* CS261- Assignment 3 - Part 3
 * Name: Ryan Gift
 * Date: 04/30/2017
 * Description: File tests a few simple functions of the circular
 * linked list utilizing the deque data structure.
 */
#include "circularList.h"
#include <stdio.h>

int main()
{	
	/* create deque and add some values */
	struct CircularList* deque = circularListCreate(); 
	circularListAddBack(deque, (TYPE)1);
	circularListAddBack(deque, (TYPE)2);
	circularListAddBack(deque, (TYPE)3);
	circularListAddFront(deque, (TYPE)4);
	circularListAddFront(deque, (TYPE)5);
	circularListAddFront(deque, (TYPE)6);
	/* print the values and the front and back */
	circularListPrint(deque);
	printf("%g\n", circularListFront(deque));
	printf("%g\n", circularListBack(deque));
	/* remove front and back, then print list again */
	circularListRemoveFront(deque);
	circularListRemoveBack(deque);
	circularListPrint(deque);
	/* reverse the list and print it again */
	circularListReverse(deque);
	circularListPrint(deque);
	circularListDestroy(deque);
	
	system("pause");
	return 0;
}