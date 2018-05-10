/* CS261- Assignment 4 - Part 1 */
/* Name: Ryan Gift
 * Date: 05/14/2017
 * Description: This file contains the compare method and print method for the 
 * binary search tree. The two methods cast pointers to then compare the pointers 
 * and/or print the pointers value data. This file should be combined with the 
 * structs.h file, bst.c file, and bst.h file.
 */
#include <stdio.h>
#include <assert.h>
#include "bst.h"
#include "structs.h"

/*----------------------------------------------------------------------------
 very similar to the compareTo method in java or the strcmp function in c. it
 returns an integer to tell you if the left value is greater than, less than, or
 equal to the right value. you are comparing the number variable, letter is not
 used in the comparison.

 if left < right return -1
 if left > right return 1
 if left = right return 0
 */

 /*Define this function, type casting the value of void * to the desired type.
  The current definition of TYPE in bst.h is void*, which means that left and
  right are void pointers. To compare left and right, you should first cast
  left and right to the corresponding pointer type (struct data *), and then
  compare the values pointed by the casted pointers.

  DO NOT compare the addresses pointed by left and right, i.e. "if (left < right)",
  which is really wrong.
 */
int compare(TYPE left, TYPE right)
{
	struct data *castLeft = (struct data *) left;
	struct data *castRight = (struct data *) right;

	/* if left < right, return -1 */
	if (castLeft->number < castRight->number)
	{
		return -1;
	}
	/* else if left > right, return 1 */
	else if (castLeft->number > castRight->number)
	{
		return 1;
	}
	/* else left == right, return 0 */
	else
	{
		return 0;
	}
}

/*Define this function, type casting the value of void * to the desired type*/
void print_type(TYPE curval)
{
	/* assert our current value is not null */
	assert(curval != NULL);
	/* cast curval to desired type */
	struct data *newCurVal = (struct data *) curval;

	/* Print number part of struct data */
	printf("%d", newCurVal->number);
}