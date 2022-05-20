#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

int edge_weight[100];  //arbitrary max
int start;
int node_word_length;

typedef struct Edge
{
    int index_second;
    int weight;
    struct Edge* next;
}
Edge;

typedef struct GraphNode
{
    int index;
    char *node_name;
}
GraphNode;

typedef struct HeapNode{
    int node_index;
    int dist;
}
HeapNode;
 
typedef struct Heap
{
    int heap_size;     
    int *heap_position;    
    struct HeapNode **array;
}
Heap;

typedef struct ListOfEdges
{
   struct Edge *head; 
}
ListOfEdges;


typedef struct Graph
{
    int number_of_nodes;
    struct ListOfEdges* array;
}
Graph;



int node_compare(const void *a, const void *b);
int find_node_word_length(int start, int max_length, char *links);
void get_routes(char* routes, int start_node_word_length, int end_node_word_length, int number_of_nodes, int cursor, struct GraphNode *GraphNode, struct Graph* graph);
 

struct Edge* new_edge(int index_second, int weight);
struct Graph* create_graph(int number_of_nodes);
void add_edge(struct Graph* graph, int index_first, int index_second, int weight);
struct HeapNode* new_heap_node(int node_index, int dist);
void sort_heap(struct Heap* heap, int idx);
struct HeapNode* extract_node(struct Heap* heap);
void decrease_key(struct Heap* heap, int node_index, int dist);
void dijkstra(struct Graph* graph, int start, int end, struct GraphNode *graph_node);
char *find_node_word(int start, int max_length, char *links, char *node_name);
