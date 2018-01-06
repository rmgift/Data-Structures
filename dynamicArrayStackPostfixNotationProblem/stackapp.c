/* CS261- Assignment 2 - Part 3
 * Name: Ryan Gift
 * Date: 04/23/2017
 * Solution description: This program implements the ADT stack
 * and checks for balanced parentheses, braces and brackets of 
 * an expression. This file should be combined with the dynArray.h
 * file (must change #define TYPE to char) and the dynamicArray.c file.
 * stack.c: Stack application. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "dynArray.h"


/* ***************************************************************
Using stack to check for unbalanced parentheses.
***************************************************************** */
/* 
 * Returns the next character of the string, once reaches end return '0' (zero)
 * param: s pointer to a string 	
 * pre: s is not null		
*/
char nextChar(char* s)
{
	static int i = -1;	
	char c;
	++i;	
	c = *(s + i);			
	if ( c == '\0' )
		return '\0';	
	else 
		return c;
}

/* 
 * Checks whether the (), {}, and [] are balanced or not
 * param: s pointer to a string 	
 * pre: s is not null	
 * post:	
 */
int isBalanced(char* s)
{
	/* create dynamic array struct for evaluation process */
	struct DynArr *newS;
	newS = newDynArr(10);

	/* Each character is read in turn */
	char c = nextChar(s);

	/* if string is empty, loop will not iterate */
	while (c != '\0')
	{
		/* if value is opening parenthesis, push on to stack */
		if (c == '(' || c ==  '[' || c == '{')
		{
			pushDynArr(newS, c);
		}
		/* if value is closing parenthesis, check if balanced */
		if (c == ')' || c ==  ']' || c == '}')
		{
			/* ( = 40 and ) = 41 */
			/* if top of stack is ( then pop, else not balanced */
			if ((c == ')') && (topDynArr(newS) != c - 1))
			{
				return 0;
			}
			/* if top of stack is [ or { then pop, else not balanced */
			/* [ = 91 and ] = 93  &&  { = 123 and } = 125 */
			if ((c == ']' || c == '}') && (topDynArr(newS) != c - 2))
			{
				return 0;
			}
			/* balanced so pop stack*/
			popDynArr(newS);
		}
		/* character has been evaluated or ignored, get next character and loop */
		c = nextChar(s);
	}

	/* if stack is empty after all characters have been read, expression is balanced */ 
	if(isEmptyDynArr(newS))
	{
		return 1;
	}
	
	/* stack is not empty, expression is not balanced */
	return 0;
}

int main(int argc, char* argv[])
{	
	//char* s = argv[1]; // use this line for command line strings
	char* s = "[{([()])}]"; // else create a hard coded string
	printf("Assignment 2\n");

	int res;
	res = isBalanced(s);

	if (res)
		printf("The string %s is balanced\n",s);
	else 
		printf("The string %s is not balanced\n",s);
	
	system("pause");

	return 0;	
}