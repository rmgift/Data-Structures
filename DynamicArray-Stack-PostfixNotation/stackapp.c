/* CS261- Assignment 2 - Part 3
 * Name: Ryan Gift
 * Date: 04/23/2017
 * Description: This program implements the ADT stack and checks for 
 * balanced parentheses, braces and brackets of an expression. This 
 * file should be combined with the dynArray.h file 
 * (must change #define TYPE to char) and the dynamicArray.c file.
 * stack.c: Stack application. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dynArray.h"


/* **************************************************************** *
 * --------Using stack to check for unbalanced parentheses--------- *
 * **************************************************************** */
/* Returns the next character of the string, returns '0' at end
 * param: s pointer to a string 	
 * pre: s is not null
 */
char nextChar(char* s, int i)
{
	//static int i = -1;	
	char c;
	//++i;	
	c = *(s + i);			
	if (c == '\0')
	{
		return '\0';
	}

	return c;
}

/* checks whether both current characters are matching
 * param: character from stack & current character in string
 */
int isMatchingPair(char char1, char char2)
{
	if (char1 == '(' && char2 == ')')
	{
		return 1;
	}
	else if (char1 == '{' && char2 == '}')
	{
		return 1;
	}
	else if (char1 == '[' && char2 == ']')
	{
		return 1;
	}
	return 0;
}

/* Checks whether the (), {}, and [] are balanced or not
 * param: s pointer to a string 	
 * pre: s is not null	
 * post:	
 */
int isBalanced(char* s)
{
	/* create dynamic array struct for evaluation process and get first char */
	struct DynArr *newS;
	newS = newDynArr(25);
	int currIdx = 0;
	char current = nextChar(s, currIdx++);
	/* while we've not reached the end of the string, loop */
	while (current != '\0')
	{
		/* if value is opening parenthesis, push on to stack */
		if (current == '(' || current ==  '[' || current == '{')
		{
			pushDynArr(newS, current);
		}
		/* if value is closing parenthesis, check if balanced */
		else if (current == ')' || current ==  ']' || current == '}')
		{
			/* test more than balance, if also palindrome use this statement */
			if (!isMatchingPair(topDynArr(newS), current))
			{
				deleteDynArr(newS);
				return 0;
			}
			/* balanced so pop stack */
			popDynArr(newS);
		}
		else
		{
			deleteDynArr(newS);
			return 0;
		}
		/* character has been evaluated or ignored, get next character and loop */
		current = nextChar(s, currIdx++);
	}

	/* if stack is empty after all characters have been read, expression is balanced */ 
	int returnVal = isEmptyDynArr(newS);
	deleteDynArr(newS);
	/* stack is not empty, expression is not balanced */
	return returnVal;
}

/* Prints result of string being tested, uncomment else for failures
 */
void printResult(int val, char* str)
{
	if (val)
	{
		printf("The string %s is balanced\n", str);
	}
	/*
	else
	{
		printf("The string %s IS NOT balanced\n", str);
	}
	*/
}

void hardCodedTests(void)
{
	char* s1 = "[{([()])}]"; 
	char* s2 = "[{([(]))}]";
	char* s3 = "[(())}";
	char* s4 = "[(())]";
	char* s5 = "";
	char* s6 = "){}";
	char* s7 = "{}";
	char* s8 = "[({({}[()])[]})]";
	char* s9 = "[({(";
	int res = 0;
	res = isBalanced(s1);
	printResult(res, s1);
	res = isBalanced(s2);
	printResult(res, s2);
	res = isBalanced(s3);
	printResult(res, s3);
	res = isBalanced(s4);
	printResult(res, s4);
	res = isBalanced(s5);
	printResult(res, s5);
	res = isBalanced(s6);
	printResult(res, s6);
	res = isBalanced(s7);
	printResult(res, s7);
	res = isBalanced(s8);
	printResult(res, s8);
	res = isBalanced(s9);
	printResult(res, s9);
	return;
}

int main(int argc, char* argv[])
{	
	printf("Assignment 2\n");
	//char* s = argv[1]; // use this line for command line strings
	hardCodedTests();
	/* local vars to setup random number generation and string */
	char* possible = "{}[]()";
	srand(time(NULL));
	char* str = NULL;
	int index;
	for (index = 0; index < 100000; index++)
	{
		/* get the size of new string and setup memory */
		int size = (rand() % 25) + 1;
		str = (char*)malloc((size + 1) * sizeof(char));
		memset(str, '\0', size + 1);
		int curr = 0;
		/* fill the new string with our possible brackets */
		while (curr < size)
		{
			int temp = rand() % ((int)(sizeof(possible) - 1));
			str[curr] = possible[temp];
			curr++;
		}
		/* check if its balanced and print result, then free current string */
		int res = isBalanced(str);
		//printf("%d -> ", index + 1);
		printResult(res, str);
		free(str);
	}

	system("pause");
	return 0;	
}