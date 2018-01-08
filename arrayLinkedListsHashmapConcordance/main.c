/* CS 261 Data Structures
 * Assignment 6
 * Name: Ryan Gift
 * Date: 06/04/17
 * Solution Description: This file contains the methods to open a .txt
 * file, count the number of instances of each word in the file. Each
 * instance is added to a hash map. It then prints out the totals for
 * each word in the document. This file must be combined with the 
 * hashMap.c and hashMap.h files.
 */
#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#pragma warning(disable:4996) // used to disable _CRT_SECURE_NO_WARNINGS

/*
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);

        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
		return NULL;
    }
    word[length] = '\0';
    return word;
}

/* 
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    const char* fileName = "input1.txt";
	if (argc > 1)
    {
        fileName = argv[1];
    }
    printf("Opening file: %s\n", fileName);
    
    clock_t timer = clock();
    /* this creates a hashMap with 10 indices */
    HashMap* map = hashMapNew(10);
    
	
	// --- Concordance code begins here ---
	/* open the file with the fileName and populate it with the words */
	FILE *concordFile = fopen(fileName, "r");
	
	/* assign first word to newWord if it exists */
	char* newWord = nextWord(concordFile);

	/* iterate the concordance until the end of the file is reached */
	while (newWord != NULL)
	{
		/* each hashLink stores a word from the document as the key */
		/* the number of times the word appears is the hashLink value */

		/* STEP 1: check if words in list, if true then increment count
		           else put the word in the hash map and increment count */
		/* void hashMapPut(HashMap* map, const char* key, int value)
		 * Updates the given key-value pair in the hash table.
		 * If a link with key already exists, this will just update the value.
		 * Else create new link with key & value then add to table's bucket linked list.
		 */
		hashMapPut(map, newWord, 1);
		/* STEP 2: now free the current word */
		free(newWord);
		/* STEP 3: get the next word  */
		newWord = nextWord(concordFile);
	}

	// print key : value
	for (int i = 0; i < (map->capacity); i++) {
		/* get the current index position */
		HashLink* temp = map->table[i];
		/* if the current index has links, iterate list */
		if (temp != 0){
			/* while we still have links, iterate the list and print */
			while(temp != 0){
				printf("\n %s: %d", temp->key, temp->value);
				temp = temp->next;
			}
		}

	} 
	printf("\n");
	// Be sure to free the word after you are done with it here.
	fclose(concordFile);
	// --- Concordance code ends here ---
    
	
	//hashMapPrint(map);
    
	timer = clock() - timer;
	printf("\nRan in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
	printf("Empty buckets: %d\n", hashMapEmptyBuckets(map));
	printf("Number of links: %d\n", hashMapSize(map));
	printf("Number of buckets: %d\n", hashMapCapacity(map));
	printf("Table load: %f\n", hashMapTableLoad(map));
    
	hashMapDelete(map);

	system("pause");

	return 0;
}
