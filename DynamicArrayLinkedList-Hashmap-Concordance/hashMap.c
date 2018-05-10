/* CS 261 Data Structures - Assignment 6
 * Name: Ryan Gift
 * Date: 06/04/17
 * Description: This file contains the methods to implement the hash map 
 * via an array. The hash map uses a table of buckets, that each contain 
 * a linked list of hash links. Each link stores a key value pair (string 
 * and integer) and a pointer to the next link. This file should should 
 * be combined with the hashMap.h file and another file containing main.
 */
#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#pragma warning(disable:4996) // used to disable _CRT_SECURE_NO_WARNINGS

/* HASH_FUNCTION accepts a char* key and computes almost unique 
 * value based on the ASCII value of each letter in the key passed.
 * Value returned from HASH_FUNCTION gets modulus with map capacity
 * to get the int index of the linked list bucket
 */
int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/* Creates a new hash table link with a copy of the key string.
 * param: key string to copy in the link.
 * param: value to set in the link.
 * param: next Pointer to set as the link's next.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/* Free the allocated memory for a hash table link created with hashLinkNew.
 * param: hash link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/* Initializes a hash table map, allocating memory for a link pointer table
 * with the given number of buckets.
 * param: map
 * param: capacity is the number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/* Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * param: map
 */
void hashMapCleanUp(HashMap* map)
{
	assert(map != 0);
	/* iterate through the array freeing each index from its list */
	for (int i = 0; i < map->capacity; i++)
	{
		HashLink *cur = map->table[i];
		/* while links exist in the index, delete it and go to next */
		while (cur != 0)
		{
			HashLink *temp = cur->next;
			hashLinkDelete(cur);
			cur = temp;
		}
		/* extra step but set index to 0 or null */
		map->table[i] = 0;
	}
	free(map->table);
	map->size = 0;
}

/* Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * param: capacity is the number of buckets.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/* Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * param: map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
	free(map);
}

/* Returns a pointer to the value of the link with the given key. Returns NULL
 * if no link with that key is in the table. Use HASH_FUNCTION(key) and the
 * map's capacity to find the index of the correct linked list bucket. Also
 * make sure to search the entire list.
 * param: map and key
 */
int* hashMapGet(HashMap* map, const char* key)
{
	assert(map != 0);
	assert(key != 0);
	/* compute hash value to find the correct bucket */
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	HashLink *bucketList = map->table[hashIndex];
	/* while our index still has list elements */
	while (bucketList != 0)
	{
		/* scan each list element key to see if it matches our key */
		if (strcmp(bucketList->key, key) == 0)
		{
			/* keys match, return address (aka pointer) of the value */
			return &(bucketList->value);
		}
		bucketList = bucketList->next;
	}
	return 0;
}

/* Resizes the hash table to have a number of buckets equal to the given capacity.
 * After allocating the new table, all of the links need to be rehashed into it
 * because the capacity has changed. Remember to free the old table and any old
 * links if you use hashMapPut to rehash them.
 * param: map
 * param: capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
	/* create a new hashTable equal to number of buckets given */
	int prevTableSize = (map->capacity);
	HashMap* newTable = hashMapNew(capacity);
	/* now copy in elements to new table */
	for (int i = 0; i < prevTableSize; i++) 
	{
		HashLink *tempLnk = map->table[i];
		/* while we have items in list copy them over to new inner list */
		while (tempLnk != 0) 
		{
			/* rehash links into table and get next element */
			hashMapPut(newTable, tempLnk->key, tempLnk->value);
			tempLnk = tempLnk->next;
		}
	}
	/* clean up and emove old table from map but keep link to map */
	hashMapCleanUp(map);
	/* now set map table to our new table and set size and capacity */
	map->table = newTable->table;
	map->size = newTable->size;
	map->capacity = newTable->capacity;
	free(newTable);
}

/* Updates the given key-value pair in the hash table. If a link with the given key
 * already exists, this will just update the value. Otherwise, it will create a new
 * link with the given key and value and add it to the table bucket's linked list.
 * You can use hashLinkNew to create the link. Use HASH_FUNCTION(key) and the map's
 * capacity to find the index of the correct linked list bucket. Also make sure to
 * search the entire list.
 * param: map, key, value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
	/* compute hash value to find the correct bucket and make sure we're dealing with postive arith */
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	if (hashIndex < 0)
	{
		hashIndex += (map->capacity);
	}

	/* if key already exists in map, only need to update value */
	if (hashMapContainsKey(map, key))
	{
		/* hashMapGet = Returns a pointer to the value of the link with the given key. */
		/* getVal points to (or holds the address of) the value we want to change */
		int *getVal = hashMapGet(map, key);
		/* dereference getVal and assign value */
		(*getVal) += value;
	}
	else
	{
		/* map does not contain the key so create a new link */
		HashLink *newVal = hashLinkNew(key, value, 0);
		/* if index is not empty we must iterate to find end of list */
		if (map->table[hashIndex] != 0)
		{
			HashLink *temp = map->table[hashIndex];
			/* while there's still a link, move to next */
			while (temp->next)
			{
				temp = temp->next;
			}
			/* at the end of our list add the new value */
			temp->next = newVal;
		}
		/* else index is empty simply add new link */
		else
		{
			map->table[hashIndex] = newVal;
		}
		map->size++;
	}
	/* check the load factor, returns average number of elements in each bucket */
	float loadFactor = hashMapTableLoad(map);
	/* if the load factor is greater than our defined limit, we must resize */
	if (loadFactor >= MAX_TABLE_LOAD)
	{
		resizeTable(map, map->capacity * 2);
	}
}

/* Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * param: map and key
 */
void hashMapRemove(HashMap* map, const char* key)
{
	assert(map != 0);
	assert(key != 0);
	/* compute hash value to find the correct bucket, then create temp links to walk list */
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	HashLink *cur = map->table[hashIndex];
	HashLink *prev = 0;
	/* if current index isn't empty, iterate through it */
	if (cur != 0)
	{
		/* if first index key matches passed key, set table index to next element in list */
		if (strcmp(cur->key, key) == 0)
		{
			map->table[hashIndex] = cur->next;
		}
		/* else we must iterate through list to remove link */
		else
		{
			while (strcmp(cur->key, key) != 0)
			{
				/* set temp links while walking down list */
				prev = cur;
				cur = cur->next;
			}
			/* found match, current holds link to remove, set prev to link beyound current */
			prev->next = cur->next;
		}
		/* delete the current link and decrement map's size */
		hashLinkDelete(cur);
		map->size--;
	}
}

/* Returns 1 if a link with the given key is in the table and 0 otherwise. Use HASH_FUNCTION(key)
 * and the map's capacity to find the index of the correct linked list bucket. Also make sure to
 * search the entire list.
 * param: map and key
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
	assert(map != 0);
	assert(key != 0);
	/* compute hash value to find the correct bucket, verify positive arith */
	int hashIndex = HASH_FUNCTION(key) % (map->capacity);
	if (hashIndex < 0)
	{
		hashIndex += (map->capacity);
	}

	/* create temp to walk down list and set to head of array element list */
	HashLink *temp = map->table[hashIndex];
	/* while there’s still elements in the list, search it */
	while (temp != 0) 
	{
		/* if the list element equals our test we found it */
		if (strcmp(temp->key, key) == 0) 
		{
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

/*
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    return (map->size);
}

/* Returns the number of buckets in the table. */
int hashMapCapacity(HashMap* map)
{
    return (map->capacity);
}

/* Returns the number of table buckets without any links. */
int hashMapEmptyBuckets(HashMap* map)
{
	int count = 0;
	/* iterate array searching for null or 0 buckets */
	for (int i = 0; i < (map->capacity); i++)
	{
		/* if our bucket/index is null, add to count */
		if (map->table[i] == 0)
		{
			count += 1;
		}
	}
	return count; 
}

/* Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * param: map
 */
float hashMapTableLoad(HashMap* map)
{
	return ((float)((map->size) / (map->capacity)));
}

/* Prints all the links in each of the buckets in the table. */
void hashMapPrint(HashMap* map)
{
    for (int i = 0; i < map->capacity; i++)
    {
        HashLink* link = map->table[i];
        if (link != NULL)
        {
            printf("\nBucket %i ->", i);
            while (link != NULL)
            {
                printf(" (%s, %d) ->", link->key, link->value);
                link = link->next;
            }
        }
    }
    printf("\n");
}