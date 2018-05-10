/* CS 261 Assignment 5 - Part 2
 * Name: Ryan Gift
 * Date: 05/28/17
 * Description: This file implements the methods to create a to do list 
 * from user input. The methods in this file assist each other with the 
 * creation of a to-do list via the heap. Each to-do list item contains 
 * a name and priority value assigned to it. This file should be combined 
 * with the Task files, DynamicArray files, and the todo.txt file if you 
 * wish to use an existing list.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "dynamicArray.h"
#include "task.h"
#pragma warning(disable:4996) // used to disable _CRT_SECURE_NO_WARNINGS

/* Loads into heap a list from a file with lines formatted like "priority, name".
 * param: heap
 * param: file
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

/* Writes to a file all tasks in heap with lines formatted like "priority, name".
 * param: heap
 * param: file
 */
void listSave(DynamicArray* heap, FILE* file)
{
    for (int i = 0; i < dySize(heap); i++)
    {
        Task* task = dyGet(heap, i);
        fprintf(file, "%d, %s\n", task->priority, task->name);
    }
}

/* Prints every task in heap.
 * param: heap
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

/* Handles the given command.
 * param: list
 * param: command
 */
void handleCommand(DynamicArray* list, char command)
{
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
			/* get the filename up to the newline char then set ptr to newline char in array */
			fgets(filename, sizeof(filename), stdin);
			nullPtr = strchr(filename, '\n');
			/* replace newline sequence with null terminator */
			if (nullPtr) 
			{
				*nullPtr = '\0';
			}
			/* open file and place items in file into list */
			FILE *loadFile = fopen(filename, "r+");
			listLoad(list, loadFile);
			printf("The list has been loaded from file successfully.\n\n");
			fclose(loadFile);
			break;

		/*  's' to save to-do list to a file\n  */
		case 's':
			/* if your list contains to-do items, we can save it */
			if (dySize(list) > 0) 
			{
				printf("Please enter the filename: ");
				/* get the file name and replace newline char with null term */
				fgets(filename, sizeof(filename), stdin);
				nullPtr = strchr(filename, '\n');
				if (nullPtr) 
				{
					*nullPtr = '\0';
				}
				/* open the file and write list into it */
				FILE *saveFile = fopen(filename, "w+");
				listSave(list, saveFile);
				printf("The list has been saved into the file successfully.\n\n");
				fclose(saveFile);
			}
			else 
			{
				printf("Your to-do list is EMPTY!\n");
			}
			printf("\n");
			break;

		/*  'a' to add a new task\n  */
		case 'a':
			/* get the description and priority of the task from the user */
			printf("Please enter the task description: ");
			fgets(description, sizeof(description), stdin);
			nullPtr = strchr(description, '\n');
			if (nullPtr)
			{
				*nullPtr = '\0';
			}

			do 
			{
				/* make sure value is in range of 0 to 999 */
				printf("Please enter the task priority (0-999): ");
				scanf("%d", &priority);
			} while (!(priority >= 0 && priority <= 999));

			while (getchar() != '\n');
			/* assign new task with priority and description then add it to our list */
			tempTask = taskNew(priority, description);
			dyHeapAdd(list, tempTask, taskCompare);
			printf("The task '%s' was added to your to-do list.\n\n", description);
			break;

		/*  'g' to get the first task\n  */
		case 'g':
			/* if our list contains to-do items, get it and print name */
			if (dySize(list) > 0)
			{
				tempTask = dyHeapGetMin(list);
				printf("Your first task is: %s\n\n", tempTask->name);
			}
			else 
			{
				printf("Your to-do list is empty!\n\n");
			}
			break;

		/*  'r' to remove the first task\n  */
		case 'r':
			/* if our lists contains tasks, size is greater than 0 */
			if (dySize(list) > 0) 
			{
				/* get our first to-do item, remove it from the list, and delete it */
				tempTask = dyHeapGetMin(list);
				dyHeapRemoveMin(list, taskCompare);
				printf("Your first task '%s' has been removed from the list.\n", tempTask->name);
				taskDelete(tempTask);
			}
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
		
		/* get input command from keyboard, clearn newline char, call function */
        command = getchar();
        while (getchar() != '\n');
        handleCommand(list, command);
	} while (command != 'e');
	
	/* free dynamically allocated List pointers in array to avoid memory leaks */
    dyDelete(list);

	system("pause");
    return 0;
}