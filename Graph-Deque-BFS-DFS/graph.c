/* CS 261 Data Structure
 * Assignment 7
 * Name: Ryan Gift
 * Date: 06/11/17
 * Description: This file contains the implemented methods to search a tree via 
 * the iterative methods of Depth-First Search (DFS) and Breadth-First Search (BFS). 
 * This file should be combined with the graph.h file and both deque.h and deque.c 
 * files as the lists for the graph are implemented via the deque.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "graph.h"
#include "deque.h"
#pragma warning(disable:4996) // used to disable _CRT_SECURE_NO_WARNINGS

/* Clears isVisited in all nodes in the graph. */
static void clearVisited(Graph* graph)
{
    for (int i = 0; i < graph->numVertices; ++i)
    {
        graph->vertexSet[i].isVisited = 0;
    }
}

/* Recursive helper function for DfsRecursive. Determines if there is a path from the 
 * given vertex to the destination using a recursive depth-first search.
 * param: graph, vertex, and destination
 * return: 1 if there is a path, 0 otherwise.
 */
static int DfsRecursiveHelper(Graph* graph, Vertex* vertex, Vertex* destination)
{
    vertex->isVisited = 1;
    if (vertex == destination)
    {
        return 1;
    }
    for (int i = 0; i < vertex->numNeighbors; ++i)
    {
        Vertex* neighbor = vertex->neighbors[i];
        if (!neighbor->isVisited)
        {
            if (DfsRecursiveHelper(graph, neighbor, destination) == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}

/* Determines if an edge (v1, v2) exists.
 * param: v1 and v2
 * return: 1 if the edge exists, 0 otherwise.
 */
static int isAdjacent(Vertex* v1, Vertex* v2)
{
    if (v1 == v2)
    {
        return 1;
    }
    for (int i = 0; i < v1->numNeighbors; ++i)
    {
        if (v1->neighbors[i] == v2)
        {
            return 1;
        }
    }
    return 0;
}

/* Connects two vertices by adding each other to their neighbors lists.
 * param: v1 and v2
 */
static void createEdge(Vertex* v1, Vertex* v2)
{
    v1->neighbors = realloc(v1->neighbors, sizeof(Vertex*) * (v1->numNeighbors + 1));
    v2->neighbors = realloc(v2->neighbors, sizeof(Vertex*) * (v2->numNeighbors + 1));
    v1->neighbors[v1->numNeighbors] = v2;
    v2->neighbors[v2->numNeighbors] = v1;
    ++(v1->numNeighbors);
    ++(v2->numNeighbors);
}

/* Determines if there is a path from the source to the destination using a recursive 
 * depth-first search starting at the source. You can use this function to test the 
 * correctness of the others.
 * param: graph, source and destination
 * return: 1 if there is a path, 0 otherwise.
 */
int dfsRecursive(Graph* graph, Vertex* source, Vertex* destination)
{
    clearVisited(graph);
    return DfsRecursiveHelper(graph, source, destination);
}

/* Determines if there is a path from the source to the destination using an iterative 
 * depth-first search starting at the source. Remember to call clearVisited() before 
 * starting the search.
 * param: graph, source and destination
 * return: 1 if there is a path, 0 otherwise.
 */
int dfsIterative(Graph* graph, Vertex* source, Vertex* destination)
{
	/* clear visited before starting our search */
	clearVisited(graph);
	/* if the source is our destination found it */
	if (source == destination)
	{
		return 1;
	}
	/* depth first search utilizes a stack, must push and pop the same location */
	/* create a container for vertices known to be reachable and add starting vertex */
	Deque *stk = dequeNew();
	dequePushFront(stk, source);
	/* iterate across graph while stk is not empty */
	while (!(dequeIsEmpty(stk)))
	{
		/* assign first entry from container to vertex, remove it from stack and mark as visited */
		Vertex* v = dequeFront(stk);
		dequePopFront(stk);
		v->isVisited = 1;

		/* if the vertex (node) is our destination */
		if (v == destination)
		{
			/* delete the container to avoid memory leaks and return path is valid */
			dequeDelete(stk);
			return 1;
		}

		/* iterate to get neighbors of current vertex (node) */
		for(int i = 0; i < v->numNeighbors; i++)
		{
			/* if neighbors of vertex are not already in the set of reachable, push it onto stack */
			if (!(v->neighbors[i]->isVisited))
			{
				dequePushFront(stk, v->neighbors[i]);
			}
		}
	}
	/* no path found, delete stack and return 0 */
	dequeDelete(stk);
	return 0;
}

/* Determines if there is a path from the source to the destination using an iterative 
 * breadth-first search starting at the source. Remember to call clearVisited() before 
 * starting the search.
 * param: graph, source and destination
 * return: 1 if there is a path, 0 otherwise.
 */
int bfsIterative(Graph* graph, Vertex* source, Vertex* destination)
{
	/* clear visited before starting our search */
	clearVisited(graph);
	/* if the source is our destination found it */
	if(source == destination)
	{
		return 1;
	}

	/* breadth first search utilizes a queue, must push to back and pop from front, LIFO */
	/* create a container for vertices known to be reachable, add start vertex to end of queue */
	Deque *queue = dequeNew();
	dequePushBack(queue, source);

	/* iterate across graph while the queue is not empty */
	while(!(dequeIsEmpty(queue)))
	{
		/* assign first entry from container to vertex, remove it and mark it as visited */
		Vertex *v = dequeFront(queue);
		dequePopFront(queue);
		v->isVisited = 1;

		/* if the vertex (node) is our destination */
		if(v == destination)
		{
			/* delete the container to avoid memory leaks, return valid path */
			dequeDelete(queue);
			return 1;
		}

		/* iterate to get neighbors of current vertex (node) */
		for(int i = 0; i < v->numNeighbors; i++)
		{
			/* if neighbors of vertex are not already in the set of reachable, push into queue */
			if(!(v->neighbors[i]->isVisited))
			{
				dequePushBack(queue, v->neighbors[i]);
			}
		}
	}
	/* no path found, delete queue and return 0 */
	dequeDelete(queue);
	return 0;
}


typedef struct Edge Edge;

struct Edge
{
    int i;
    int j;
};

/* Generates a set of random unique edges of size numEdges sampled from the set of all possible edges.
 * param: numVertices and numEdges
 * return: An array of numEdges edges.
 */
Edge* randomEdges(int numVertices, int numEdges)
{
    assert(numVertices > 0);
    int maxEdges = numVertices * (numVertices - 1) / 2;
    assert(numEdges >= 0);
    assert(numEdges <= maxEdges);
    
    /* Generate all possible edges */
    Edge* edges = malloc(sizeof(Edge) * maxEdges);
    int k = 0;
    for (int i = 0; i < numVertices; ++i)
    {
        for (int j = i + 1; j < numVertices; ++j)
        {
            edges[k].i = i;
            edges[k].j = j;
            ++k;
        }
    }

    /* Shuffle edges */
    for (int i = maxEdges - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        Edge temp = edges[i];
        edges[i] = edges[j];
        edges[j] = temp;
    }
    
    // Take only the number of edges needed
    edges = realloc(edges, sizeof(Edge) * numEdges);
    return edges;
}

/* Given a number of vertices and a number of edges, generates a graph connecting random pairs 
 * of vertices. The edges are unique, and thus their is a maximum number of edges allowed in 
 * proportion to the number of vertices. numEdges must be in the interval 
 * [0, numVertices * (numVertices + 1) / 2].
 * param: numVertices and numEdges 
 */
Graph* randomGraph(int numVertices, int numEdges)
{
    assert(numVertices > 0);
    assert(numEdges >= 0);
    assert(numEdges <= numVertices * (numVertices - 1) / 2);
    
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = numEdges;
    graph->vertexSet = malloc(sizeof(Vertex) * numVertices);
    
    /* Initialize vertices */
    for (int i = 0; i < graph->numVertices; ++i)
    {
        Vertex* vertex = &graph->vertexSet[i];
        vertex->label = i;
        vertex->isVisited = 0;
        vertex->numNeighbors = 0;
        vertex->neighbors = NULL;
    }
    
    /* Randomly connect vertices */
    Edge* edges = randomEdges(numVertices, numEdges);
    for (int i = 0; i < numEdges; ++i)
    {
        Vertex* v1 = &graph->vertexSet[edges[i].i];
        Vertex* v2 = &graph->vertexSet[edges[i].j];
        createEdge(v1, v2);
    }
    free(edges);

    return graph;
}

/* Loads a graph from the given file. The file's first line must be the number of vertices 
 * in the graph and each consecutive line must be a list of numbers separated by spaces. The 
 * first number is the next vertex and the following numbers are its neighbors.
 */
Graph* loadGraph(const char* fileName)
{
    FILE* file = fopen(fileName, "r");
    char buffer[512];
    
    /* Get the number of vertices */
    fgets(buffer, sizeof buffer, file);
    int numVertices = (int) strtol(buffer, NULL, 10);
    Graph* graph = malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->numEdges = 0;
    
    /* Initialize vertices */
    graph->vertexSet = malloc(sizeof(Vertex) * numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        Vertex* vertex = &graph->vertexSet[i];
        vertex->isVisited = 0;
        vertex->label = i;
        vertex->neighbors = NULL;
        vertex->numNeighbors = 0;
    }
    
    /* Create edges */
    while (fgets(buffer, sizeof buffer, file) != NULL)
    {
        char* begin = buffer;
        char* end = NULL;
        
        /* Get vertex */
        int i = (int) strtol(begin, &end, 10);
        Vertex* vertex = &graph->vertexSet[i];
        begin = end;
        
        /* Create edges */
        for (int i = (int) strtol(begin, &end, 10);
            end != begin;
            i = (int) strtol(begin, &end, 10))
        {
            Vertex* neighbor = &graph->vertexSet[i];
            if (!isAdjacent(vertex, neighbor))
            {
                createEdge(vertex, neighbor);
                ++(graph->numEdges);
            }
            begin = end;
        }
    }

    fclose(file);
    return graph;
}

/* Frees all memory allocated for a graph and the graph itself. */
void freeGraph(Graph* graph)
{
    for (int i = 0; i < graph->numVertices; ++i)
    {
        free(graph->vertexSet[i].neighbors);
    }
    free(graph->vertexSet);
    free(graph);
}

/* Prints the vertex count, edge count, and adjacency list for each vertex. */
void printGraph(Graph* graph)
{
    printf("Vertex count: %d\n", graph->numVertices);
    printf("Edge count: %d\n", graph->numEdges);
    for (int i = 0; i < graph->numVertices; ++i)
    {
        Vertex* vertex = &graph->vertexSet[i];
        printf("%d :", vertex->label);
        for (int j = 0; j < vertex->numNeighbors; ++j)
        {
            printf(" %d", vertex->neighbors[j]->label);
        }
        printf("\n");
    }
}