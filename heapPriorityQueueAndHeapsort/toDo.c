/* CS 261 Assignment 5 - Part 2
 * Name: Ryan Gift
 * Date: 05/28/17
 * Solution description: This file implements the methods to create
 * a to do list from user input. The methods in this file assist each 
 * other with the creation of a to-do list via the heap. Each to-do list
 * item contains a name and priority value assigned to it. This file 
 * should be combined with the Task files, DynamicArray files, and the 
 * todo.txt file if you wish to use an existing list.
 */
#include "dynamicArray.h"
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Loads into heap a list from a file with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listLoad(DynamicArray* heap, FILE* file)
{
    const int FORMAT_LENGTH = 256;
	//char format[FORMAT_LENGTH];
	char format[256];
    snprintf(format, FORMAT_LENGTH, "%%d, %%%d[^\n]", TASK_NAME_SIZE);
    
    Task temp;
    while (fscanf(file, format, &temp.priority, &temp.name) == 2)
    {
        dyHeapAdd(heap, taskNew(temp.priority, temp.name), taskCompare);
    }
}

/*
 * Writes to a file all tasks in heap with lines formatted like
 * "priority, name".
 * @param heap
 * @param file
 */
void listSave(DynamicArray* heap, FILE* file)
{
    for (int i = 0; i < dySize(heap); i++)
    {
        Task* task = dyGet(heap, i);
        fprintf(file, "%d, %s\n", task->priority, task->name);
    }
}

/*
 * Prints every task in heap.
 * @param heap
 */
void listPrint(DynamicArray* heap)
{
    DynamicArray* temp = dyNew(1);
    dyCopy(heap, temp);
    while (dySize(temp) > 0)
    {
        Task* task = dyHeapGetMin(temp);
        printf("\n");
        taskPrint(task);
        printf("\n");
        dyHeapRemoveMin(temp, taskCompare);
    }
    dyDelete(temp);
}

/*
 * Handles the given command.
 * @param list
 * @param command
 */
void handleCommand(DynamicArray* list, char command)
{
	/* MY NOTES ABOUT FILES: 
	 * before a file structure can be used it must be initialized
	 * FILE *fopen(const char *filename, const char *mode);
	
	 * FILE OPERATIONS:
	 * "r": read from file
	 * "w": write to file overwriting its contents, create the file if it doesn't exist
	 * "a": write to the file appending to the file, creating it if it doesn't exist 
	 * "r+": open for reading and writing, start at beginning 
	 * "w+": open for reading and writing (overwrite file) 
	 * "a+": open for reading and writing (append if file exists) 

	 * int getc(FILE* fp); will return current character from fp and advance to next character
	 * char* putc(const char* s, FILE* fp); will write one character to file
	 * int fclose(FILE *a_file); 
	**/
	struct Task *tempTask = malloc(sizeof(struct Task));
	char *filename[100];
	char *description[TASK_NAME_SIZE];
	char *nullPtr;
	int priority;

	switch (command)
	{
		/*  'l' to load to-do list from a file\n  */
		case 'l':
			printf("Please enter the filename: ");
			/* get the filename from the user up to the newline sequence */
			fgets(filename, sizeof(filename), stdin);
			/* set nullPtr to our newline sequence element in array */
			nullPtr = strchr(filename, '\n');
			/* if succeeded, replace newline sequence with 0 */
			if (nullPtr) 
			{
				*nullPtr = '\0';
			}
			/* open the file and assign it to loadFile */
			FILE *loadFile = fopen(filename, "r+");
			/* pass our list and loadFile to place to-do items from file into our list */
			listLoad(list, loadFile);
			printf("The list has been loaded from file successfully.\n");
			/* remember to close the file when we're done */
			fclose(loadFile);
			printf("\n");
			break;

		/*  's' to save to-do list to a file\n  */
		case 's':
			/* if your list contains to-do items, we can save it */
			if (dySize(list) > 0) 
			{
				printf("Please enter the filename: ");
				/* get the file name from the user up to the newline sequence */
				fgets(filename, sizeof(filename), stdin);
				/* set nullPtr to our newline sequence element in array */
				nullPtr = strchr(filename, '\n');
				/* if succeeded, replace newline sequence with 0 */
				if (nullPtr) 
				{
					*nullPtr = '\0';
				}
				/* open the file and assign it to saveFile*/
				FILE *saveFile = fopen(filename, "w+");
				/* call list save function to place to-do items into file */
				listSave(list, saveFile);
				printf("The list has been saved into the file successfully.\n");
				/* remember to close the file when you're done */
				fclose(saveFile);
				printf("\n");
			}
			else 
			{
				printf("Your to-do list is EMPTY!\n");
			}
			printf("\n");
			break;

		/*  'a' to add a new task\n  */
		case 'a':
			/* get the description of the task from the user */
			printf("Please enter the task description: ");
			fgets(description, 100, stdin);
			/* remove the newline escape sequence and replace it with a 0 */
			if (description[strlen(description) - 1] == '\n') 
			{
				description[strlen(description) - 1] = 0;
			}
			/* get the priority value as a reference */
			do 
			{
				printf("Please enter the task priority (0-999): ");
				scanf("%d", &priority);
				/* make sure value is in range of 0 to 999 */
			} while (!(priority >= 0 && priority <= 999));
			/* clear escape sequence from buffer */
			while (getchar() != '\n');
			/* assign tempTask the its priority and name */
			tempTask = taskNew(priority, description);
			/* add the task to our list/heap */
			dyHeapAdd(list, tempTask, taskCompare);
			printf("The task '%s' was added to your to-do list.\n", description);
			printf('\n');
			break;

		/*  'g' to get the first task\n  */
		case 'g':
			/* if our list contains to-do items, get it and print name */
			if (dySize(list) > 0) 
			{
				tempTask = dyHeapGetMin(list);
				printf("Your first task is: %s\n\n", tempTask->name);
			}
			/* our to-do list is empty */
			else {
				printf("Your to-do list is empty!\n\n");
			}
			break;

		/*  'r' to remove the first task\n  */
		case 'r':
			/* if our lists contains tasks, size is greater than 0 */
			if (dySize(list) > 0) 
			{
				/* get our first to-do item */
				tempTask = dyHeapGetMin(list);
				/* remove the item from our heap */
				dyHeapRemoveMin(list, taskCompare);
				printf("Your first task '%s' has been removed from the list.\n", tempTask->name);
				/*	delete task from the list */
				taskDelete(tempTask);
			}
			/* to-do list is empty, cannot remove items */
			else 
			{
				printf("Your to-do list is already empty!\n");
			}
			printf("\n");
			break;

		/*  'p' to print the list\n  */
		case 'p':
			/* make sure our list contains to-do items, then print */
			if (dySize(list) > 0) 
			{
				listPrint(list);
			}
			/* no to-do items in our list */
			else 
			{
				printf("You to-do list is empty!\n");
			}
			printf("\n");
			break;

		/*  'e' to exit the program\n  */
		case 'e':
			printf("Good bye!\n");
			break;
	}
}

int main()
{
    printf("\n\n** TO-DO LIST APPLICATION **\n\n");
    DynamicArray* list = dyNew(8);
    char command = ' ';
    do
    {
        printf("Press:\n"
               "'l' to load to-do list from a file\n"
               "'s' to save to-do list to a file\n"
               "'a' to add a new task\n"
               "'g' to get the first task\n"
               "'r' to remove the first task\n"
               "'p' to print the list\n"
               "'e' to exit the program\n"
        );
        command = getchar();	/* get input command from keyboard */
        // Eat newlines
        while (getchar() != '\n');	/* clear trailing newline character */
        handleCommand(list, command);	/* call function */
    }
    while (command != 'e');
	/* free dynamically allocated List pointers in array to avoid memory leaks */

    dyDelete(list);
    return 0;
}