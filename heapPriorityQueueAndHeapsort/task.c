/* CS 261 Assignment 5 - Part 2
 * Name: Ryan Gift
 * Date: 05/28/17
 * Solution description: This file implements the task functions
 * to create a task and return it and compare two tasks by their
 * priority. This file should be combined with the task.h file,
 * dynamicArray files, and toDo.c file.
 */
#include "task.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Creates a new task with the given priority and name.
 * @param priority
 * @param name
 * @return The new task.
 */
Task* taskNew(int priority, char* name)
{
	/* create a new task struct to return */
	struct Task *newTask = malloc(sizeof(struct Task));
	/* assign priority and name to our new task*/
	newTask->priority = priority;
	strcpy(newTask->name, name);
	/* return the new task */
    return newTask;
}

/*
 * Frees a dynamically allocated task.
 * @param task
 */
void taskDelete(Task* task)
{
    free(task);
}

/*
 * Casts left and right to Task pointers and returns
 * -1 if left's priority < right's priority,
 *  1 if left's priority > right's priority,
 *  0 if left's priority == right's priority.
 * @param left  Task pointer.
 * @param right  Task pointer.
 * @return 
 */
int taskCompare(void* left, void* right)
{
	/* create casts to compare values and not pointers */
	struct Task *castLeft = left;
	struct Task *castRight = right;
	/* if left < right, return -1 */
	if (castLeft->priority < castRight->priority)
	{
		return -1;
	}
	/* if left > right, return 1 */
	if (castLeft->priority > castRight->priority)
	{
		return 1;
	}
	/* else left == right, return 0 */
	else
	{
		return 0;
	}
}

/*
 * Prints a task as a (priority, name) pair.
 * @param value  Task pointer.
 */
void taskPrint(void* value)
{
    Task* task = (Task*)value;
    printf("(%d, %s)", task->priority, task->name);
}
