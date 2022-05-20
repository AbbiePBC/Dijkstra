#include "graph_functions.h"

int infinity = 100000; //max to represent no route found (yet)
int edge_weight[100];  //arbitrary max
int start;
int node_length;

int node_compare(const void *a, const void *b)
{
    const struct GraphNode *node_a = a;
    const struct GraphNode *node_b = b;
    return strcmp(node_a->node_name, node_b->node_name);
}


struct Edge* new_edge(int index_second, int weight)
{
    struct Edge* new_node =  malloc(sizeof(Edge));
    new_node->index_second = index_second;
    new_node->weight = weight;
    new_node->next = NULL;
    return new_node;
}
 

struct Graph* create_graph(int number_of_nodes)
{
    struct Graph* graph = malloc(sizeof(Graph));
    graph->number_of_nodes = number_of_nodes;
    graph->array = malloc(number_of_nodes * sizeof(ListOfEdges));
 
    for (int i = 0; i < number_of_nodes; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 

void add_edge(struct Graph* graph, int index_first, int index_second, int weight){
    
    struct Edge* new_node = new_edge(index_second, weight);
    new_node->next = graph->array[index_first].head;
    graph->array[index_first].head = new_node;
  
    //other direction
    new_node = new_edge(index_first, weight);
    new_node->next = graph->array[index_second].head;
    graph->array[index_second].head = new_node;
}
 


struct HeapNode* new_heap_node(int node_index, int dist)
{
    struct HeapNode* heap_node = malloc(sizeof(heap_node));
    heap_node->node_index = node_index;
    heap_node->dist = dist;
    return heap_node;
}
 


void sort_heap(struct Heap* heap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    
    if (left < heap->heap_size && right < heap->heap_size){

        if (heap->array[left]->dist < heap->array[smallest]->dist )
        smallest = left;
    
        if (heap->array[right]->dist < heap->array[smallest]->dist )
        smallest = right;
    
        if (smallest != idx) {

            struct HeapNode *smallestNode = heap->array[smallest];
            struct HeapNode *idxNode = heap->array[idx];
    
            // Swap heap_position
            heap->heap_position[smallestNode->node_index] = idx;
            heap->heap_position[idxNode->node_index] = smallest;

            //swap nodes
            struct HeapNode* temp_node = heap->array[smallest];
            heap->array[smallest] = heap->array[idx];
            heap->array[idx] = temp_node;
            
            sort_heap(heap, smallest);
        }
    }
}
 
struct HeapNode* extract_node(struct Heap* heap)
{
    if (heap->heap_size == 0)
        return NULL;
 
    struct HeapNode* root = heap->array[0];

    struct HeapNode* lastNode = heap->array[heap->heap_size - 1];
    heap->array[0] = lastNode;
 
    heap->heap_position[root->node_index] = heap->heap_size-1;
    heap->heap_position[lastNode->node_index] = 0;
 
    heap->heap_size = heap->heap_size -1;
    sort_heap(heap, 0);
 
    return root;
}
 

void decrease_key(struct Heap* heap, int node_index, int dist)
{
    int i = heap->heap_position[node_index];

    heap->array[i]->dist = dist;
 
    while (i && heap->array[i]->dist < heap->array[(i - 1) / 2]->dist)
    {
        heap->heap_position[heap->array[i]->node_index] = (i-1)/2;
        heap->heap_position[heap->array[(i-1)/2]->node_index] = i;
        
        struct HeapNode* temp_node = heap->array[i];
        heap->array[i] = heap->array[(i-1)/2];
        heap->array[(i-1)/2] = temp_node;
 
        i = (i - 1) / 2;
    }
}
 


void dijkstra(struct Graph* graph, int start, int end, struct GraphNode *graph_node)
{

    int number_of_nodes = graph->number_of_nodes;
   
    int dist[number_of_nodes];     
 
    struct Heap* heap = malloc(sizeof(heap));
    heap->heap_position = malloc(number_of_nodes * sizeof(int));
    heap->heap_size = 0;
    heap->array = malloc(number_of_nodes *sizeof(HeapNode*));
        
 
    for (int v = 0; v < number_of_nodes; ++v)
    {
        dist[v] = infinity;
        heap->array[v] = new_heap_node(v, dist[v]);
        heap->heap_position[v] = v;
    }
 
    heap->array[start] = new_heap_node(start, dist[start]);
    heap->heap_position[start] = start;
    dist[start] = 0;
    decrease_key(heap, start, dist[start]);
 
    heap->heap_size = number_of_nodes;
 
    
    while (heap->heap_size != 0)
    {
        struct HeapNode* heap_node = extract_node(heap);
       
        int current_node = heap_node->node_index;
 
        struct Edge* pointer_to_edge = graph->array[current_node].head;
        while (pointer_to_edge != NULL)
        {
            int node_index = pointer_to_edge->index_second;


            if ((heap->heap_position[node_index] < heap->heap_size) && dist[current_node] != infinity && 
              pointer_to_edge->weight + dist[current_node] < dist[node_index])
            {
                dist[node_index] = dist[current_node] + pointer_to_edge->weight;
 
                decrease_key(heap, node_index, dist[node_index]);
            }
            pointer_to_edge = pointer_to_edge->next;
        }
    }
   
    if (start == end)
        printf ("%s to %s = already there (self referential)\n", graph_node[start].node_name, graph_node[end].node_name);
    
    else if (dist[end] == infinity)
        printf ("%s to %s = no route found\n", graph_node[start].node_name, graph_node[end].node_name);
    else
        printf("%s to %s = %d\n", graph_node[start].node_name, graph_node[end].node_name, dist[end]);
    
} 
 



void get_routes(char* routes, int start_node_length, int end_node_length, int number_of_nodes, int cursor, struct GraphNode *graph_node, struct Graph* graph){
    int max_length = strlen(routes);

    find_node_word_length(0, max_length, routes);
    start_node_length = node_length;

    find_node_word_length(start_node_length + 1, max_length, routes);
    end_node_length = node_length;

    char *start_node = malloc((start_node_length + 1) * sizeof(char));
    char *end_node = malloc((end_node_length  + 1) * sizeof(char));

    find_node_word(0, start_node_length, routes, start_node);
    cursor = node_length;        
    find_node_word(cursor + 1, max_length - 1, routes, end_node);
    
    int start_index = 0; int end_index = 0; 
    for (int x = 0; x < number_of_nodes; x++ ){
        if (strcasecmp(start_node, graph_node[x].node_name) == 0){
            start_index = graph_node[x].index;
        }
        if (strcasecmp(end_node, graph_node[x].node_name)== 0){
            end_index = graph_node[x].index;
            
        }
    }
    dijkstra(graph, start_index, end_index, graph_node);

    free(start_node);
    free(end_node);
        
}

// avoid using strtok which would modify the string
int find_node_word_length(int start, int max_length, char *edges){
    node_length = 0;
    for (int i = 0; i < max_length; i ++)
    {
        if(edges[i] != ' '){
            node_length ++;
        }
        else
        {
            break;
        }
    }
    return node_length;
}

char *find_node_word(int start, int max_length, char *edges, char *node_name){
    node_length = 0;
    
    for (int i = start; i < max_length; i ++)
        {
            if(edges[i] != ' '){
                node_name[node_length] = edges[i];
                node_length ++;
            }
            else
            {
                break;
            }
        }
    return node_name;
}
