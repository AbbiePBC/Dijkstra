#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

int infinity = 100000; //max to represent no route found (yet)
int edge_weight[100];  //arbitrary max
int start;
int node_length;


typedef struct edge
{
    int index_second;
    int weight;
    struct edge* next;
}
edge;

typedef struct graph_node
{
    int index;
    char *node_name;
}
graph_node;

int node_compare(const void *a, const void *b)
{
    const struct graph_node *node_a = a;
    const struct graph_node *node_b = b;
    return strcmp(node_a->node_name, node_b->node_name);
}

 
typedef struct list_of_edges
{
   struct edge *head; 
}
list_of_edges;

typedef struct graph
{
    int number_of_nodes;
    struct list_of_edges* array;
}
graph;

char *find_node_word(int start, int max_length, char *links, char *node_name);
int find_node_length(int start, int max_length, char *links);
void get_routes(char* routes, int start_node_length, int end_node_length, int number_of_nodes, int cursor, struct graph_node *graph_node, struct graph* graph);
 

struct edge* new_edge(int index_second, int weight)
{
    struct edge* new_node =  malloc(sizeof(edge));
    new_node -> index_second = index_second;
    new_node -> weight = weight;
    new_node -> next = NULL;
    return new_node;
}
 

struct graph* createGraph(int number_of_nodes)
{
    struct graph* graph = malloc(sizeof(graph));
    graph -> number_of_nodes = number_of_nodes;
    graph -> array = malloc(number_of_nodes * sizeof(list_of_edges));
 
    for (int i = 0; i < number_of_nodes; ++i)
        graph->array[i].head = NULL;
 
    return graph;
}
 

void add_edge(struct graph* graph, int index_first, int index_second, int weight){
    
    struct edge* new_node = new_edge(index_second, weight);
    new_node -> next = graph -> array[index_first].head;
    graph -> array[index_first].head = new_node;
  
    //other direction
    new_node = new_edge(index_first, weight);
    new_node -> next = graph -> array[index_second].head;
    graph -> array[index_second].head = new_node;
}
 

typedef struct heap_node{
    int node_index;
    int dist;
}
heap_node;
 
typedef struct heap
{
    int heap_size;     
    int *heap_position;    
    struct heap_node **array;
}
heap;

struct heap_node* new_heap_node(int node_index, int dist)
{
    struct heap_node* heap_node = malloc(sizeof(heap_node));
    heap_node -> node_index = node_index;
    heap_node -> dist = dist;
    return heap_node;
}
 
 


void sort_heap(struct heap* heap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;
    
    if (left < heap  ->  heap_size && right < heap  ->  heap_size){

        if (heap -> array[left] -> dist < heap -> array[smallest] -> dist )
        smallest = left;
    
        if (heap -> array[right] -> dist < heap -> array[smallest] -> dist )
        smallest = right;
    
        if (smallest != idx) {

            struct heap_node *smallestNode = heap -> array[smallest];
            struct heap_node *idxNode = heap -> array[idx];
    
            // Swap heap_position
            heap -> heap_position[smallestNode -> node_index] = idx;
            heap -> heap_position[idxNode -> node_index] = smallest;

            //swap nodes
            struct heap_node* temp_node = heap -> array[smallest];
            heap -> array[smallest] = heap  ->  array[idx];
            heap -> array[idx] = temp_node;
            
            sort_heap(heap, smallest);
        }
    }
}
 
struct heap_node* extract_node(struct heap* heap)
{
    if (heap -> heap_size == 0)
        return NULL;
 
    struct heap_node* root = heap -> array[0];

    struct heap_node* lastNode = heap -> array[heap -> heap_size - 1];
    heap -> array[0] = lastNode;
 
    heap -> heap_position[root -> node_index] = heap -> heap_size-1;
    heap -> heap_position[lastNode -> node_index] = 0;
 
    heap -> heap_size = heap -> heap_size -1;
    sort_heap(heap, 0);
 
    return root;
}
 

void decrease_key(struct heap* heap, int node_index, int dist)
{
    int i = heap -> heap_position[node_index];

    heap -> array[i] -> dist = dist;
 
    while (i && heap -> array[i] -> dist < heap -> array[(i - 1) / 2] -> dist)
    {
        heap -> heap_position[heap -> array[i] -> node_index] = (i-1)/2;
        heap -> heap_position[heap -> array[(i-1)/2] -> node_index] = i;
        
        struct heap_node* temp_node = heap -> array[i];
        heap -> array[i] = heap  ->  array[(i-1)/2];
        heap  ->  array[(i-1)/2] = temp_node;
 
        i = (i - 1) / 2;
    }
}
 


void dijkstra(struct graph* graph, int start, int end, struct graph_node *graph_node)
{

    int number_of_nodes = graph -> number_of_nodes;
   
    int dist[number_of_nodes];     
 
    struct heap* heap = malloc(sizeof(heap));
    heap -> heap_position = malloc(number_of_nodes * sizeof(int));
    heap -> heap_size = 0;
    heap -> array = malloc(number_of_nodes *sizeof(heap_node*));
        
 
    for (int v = 0; v < number_of_nodes; ++v)
    {
        dist[v] = infinity;
        heap -> array[v] = new_heap_node(v, dist[v]);
        heap -> heap_position[v] = v;
    }
 
    heap -> array[start] = new_heap_node(start, dist[start]);
    heap -> heap_position[start] = start;
    dist[start] = 0;
    decrease_key(heap, start, dist[start]);
 
    heap -> heap_size = number_of_nodes;
 
    
    while (heap -> heap_size != 0)
    {
        struct heap_node* heap_node = extract_node(heap);
       
        int current_node = heap_node -> node_index;
 
        struct edge* pointer_to_edge = graph->array[current_node].head;
        while (pointer_to_edge != NULL)
        {
            int node_index = pointer_to_edge -> index_second;


            if ((heap -> heap_position[node_index] < heap -> heap_size) && dist[current_node] != infinity && 
              pointer_to_edge -> weight + dist[current_node] < dist[node_index])
            {
                dist[node_index] = dist[current_node] + pointer_to_edge -> weight;
 
                decrease_key(heap, node_index, dist[node_index]);
            }
            pointer_to_edge = pointer_to_edge -> next;
        }
    }
   
    if (start == end)
        printf ("%s to %s = already there (self referential)\n", graph_node[start].node_name, graph_node[end].node_name);
    
    else if (dist[end] == infinity)
        printf ("%s to %s = no route found\n", graph_node[start].node_name, graph_node[end].node_name);
    else
        printf("%s to %s = %d\n", graph_node[start].node_name, graph_node[end].node_name, dist[end]);
    
} 
 

int main(int argc, char *argv[])
{

    if (argc != 2){
         printf("need to pass in data");
         return 1;
     }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL){
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }
    char *first_line = NULL;
    size_t len = 0;
    getline(&first_line, &len, file);
    int number_of_nodes = atoi(first_line);
    
    
    struct graph* graph = createGraph(number_of_nodes);

    struct graph_node *graph_node = malloc(number_of_nodes * sizeof(struct graph_node));
    if (!graph_node) {
        printf("malloc failed, oh no!\n");
        return 1;
    }

    char* nodes = NULL;

    for (int i = 0; i < number_of_nodes; i++){
        int n = getline(&nodes, &len, file);
        nodes[n - 1] = '\0';
        graph_node[i].node_name = malloc(n * sizeof(char));
        strcpy(graph_node[i].node_name, nodes);
        graph_node[i].index = i;
    }

    char *edges_count = NULL;
    for (int j = number_of_nodes + 1; j < number_of_nodes + 3; j++){
        getline(&edges_count, &len, file);
    }
    int edges = atoi(edges_count);
    
    int current_line = number_of_nodes + 2;
    char *links = NULL;
    int index_first = 0; int index_second = 0;

    struct graph_node *sorted_nodes = malloc(number_of_nodes * sizeof(struct graph_node));
    if (!sorted_nodes) {
        printf("malloc failed, oh no!\n");
        return 1;
    }
    memcpy(sorted_nodes, graph_node, number_of_nodes * sizeof(struct graph_node));
    qsort(sorted_nodes, number_of_nodes, sizeof(struct graph_node), &node_compare);

    for (int j = current_line; j < current_line + edges; j++){
        int current_length = 0; 
        getline(&links, &len, file);
        int max_length = strlen(links);

        find_node_length(0, max_length, links);
        char *first_node = malloc((node_length + 1) * sizeof(char));
        find_node_word(start, max_length, links, first_node);
        current_length = node_length;

        struct graph_node first_node_to_find = {
            .node_name = first_node,
        };
        struct graph_node *found;
        found = bsearch(&first_node_to_find, sorted_nodes, number_of_nodes, sizeof(struct graph_node), &node_compare);
        if (found) {
            index_first = found->index;
        } else {
            printf("edge starts from nonexistent node\n");
            return 1;
        }

        current_length ++;
        int new_length = current_length; 
       
        char *second_node = malloc((node_length + 1) * sizeof(char));
        find_node_length(current_length, max_length, links);
        find_node_word(current_length, max_length, links, second_node);

        struct graph_node second_node_to_find = {
            .node_name = second_node,
        };
        found = bsearch(&second_node_to_find, sorted_nodes, number_of_nodes, sizeof(struct graph_node), &node_compare);
        if (found) {
            index_second = found->index;
        } else {
            printf("edge ends from nonexistent node\n");
            return 1;
        }

        current_length = node_length + new_length;

        find_node_length(current_length + 1, max_length - 1, links);
        int weight_length = node_length;
        char *weight = malloc(weight_length * sizeof(int));
        find_node_word(current_length + 1, max_length - 1, links, weight);

        edge_weight[node_length] = atoi(weight); 
        add_edge(graph, index_first, index_second,  edge_weight[node_length]);


        free(weight);
    }


    current_line = current_line + edges + 1;
    char *routes = NULL;
    int start_node_length = 0; int end_node_length = 0; int cursor = 0;
    getline(&routes, &len, file);
    
    while (getline(&routes, &len, file) != EOF)
        get_routes(routes, start_node_length, end_node_length, number_of_nodes, cursor, graph_node, graph);



    for (int i = 0; i < number_of_nodes; ++i)
    {
        free(graph -> array[i].head);
    }
    fclose(file);
   
 
    return 0;
}



void get_routes(char* routes, int start_node_length, int end_node_length, int number_of_nodes, int cursor, struct graph_node *graph_node, struct graph* graph){
    int max_length = strlen(routes);

        find_node_length(0, max_length, routes);
        start_node_length = node_length;

        find_node_length(start_node_length + 1, max_length, routes);
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

int find_node_length(int start, int max_length, char *links){
    node_length = 0;
    for (int i = 0; i < max_length; i ++)
        {
            if(links[i] != ' '){
                node_length ++;
            }
            else
            {
                break;
            }
        }
    return node_length;
}

char *find_node_word(int start, int max_length, char *links, char *node_name){
    node_length = 0;
    
    for (int i = start; i < max_length; i ++)
        {
            if(links[i] != ' '){
                node_name[node_length] = links[i];
                node_length ++;
            }
            else
            {
                break;
            }
        }
    return node_name;
}
