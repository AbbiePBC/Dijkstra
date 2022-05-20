#include <assert.h> //to do: use assert in some places instead of ret vals
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#include "read_input.h"

int edge_weight[100];  //arbitrary max
int start;
int node_word_length;

int parse_file(char *filename);


int main(int argc, char *argv[])
{
    if (argc != 2){
        printf("need to pass in data");
        return 1;
    }
    if (parse_file(argv[1]) == 0){
        return 0;
    }
}    


bool add_node_to_graph(int *index, int number_of_nodes, char *first_node, 
                  struct GraphNode *sorted_nodes){
    struct GraphNode first_node_to_find = {
        .node_name = first_node,
    };
    struct GraphNode *found;
    // if we sort the graph as we're making it, we have a sorted graph, 
    // which is then easier to search
    found = bsearch(&first_node_to_find, sorted_nodes, number_of_nodes, 
            sizeof(struct GraphNode), &node_compare);
    if (found) {
        *index = found->index;
    } else {
        printf("edge starts from nonexistent node\n");
    }
    return found;

}

int read_edges_add_to_graph(int current_line, int num_edges, size_t len, FILE* file, 
    struct Graph* graph, int number_of_nodes, struct GraphNode *sorted_nodes ){
    
    int index_first = 0; int index_second = 0;
    char *edges = NULL;

    for (int j = current_line; j < current_line + num_edges; j++){
        getline(&edges, &len, file);
        int max_length = strlen(edges);

        find_node_word_length(0, max_length, edges);
        char *first_node = malloc((node_word_length + 1) * sizeof(char));
        find_node_word(start, max_length, edges, first_node);
        int current_idx_through_line = node_word_length;
        add_node_to_graph(&index_first, number_of_nodes, first_node, sorted_nodes);
        current_idx_through_line ++;
        int new_length = current_idx_through_line; // to do: fix this
       
        char *second_node = malloc((node_word_length + 1) * sizeof(char));
        find_node_word_length(current_idx_through_line, max_length, edges);
        find_node_word(current_idx_through_line, max_length, edges, second_node);
        add_node_to_graph(&index_second, number_of_nodes, second_node, sorted_nodes);

        current_idx_through_line = node_word_length + new_length;

        find_node_word_length(current_idx_through_line + 1, max_length - 1, edges);
        int weight_length = node_word_length;
        char *weight = malloc(weight_length * sizeof(int));
        find_node_word(current_idx_through_line + 1, max_length - 1, edges, weight);

        edge_weight[node_word_length] = atoi(weight); 
        add_edge(graph, index_first, index_second,  edge_weight[node_word_length]);

        free(weight);
    }

    return 0;
}

struct GraphNode* new_node(int number_of_nodes){
    struct GraphNode* graph_node = malloc(number_of_nodes * sizeof(struct GraphNode));
    if (!graph_node) {
        printf("malloc failed, oh no!\n");
        exit(1); //todo: this is a temp solution
    }
    return graph_node;
}

void free_graph(int number_of_nodes, struct Graph* graph){
    for (int i = 0; i < number_of_nodes; ++i)
    {
        free(graph->array[i].head);
    }
}

int parse_file(char *filename)
{    
    FILE *file = fopen(filename, "r");

    if (file == NULL){
        printf("Could not open %s.\n", filename);
        return 1;
    }
    char *first_line = NULL;
    size_t len = 0;
    getline(&first_line, &len, file);
    int number_of_nodes = atoi(first_line);

    struct Graph* graph = create_graph(number_of_nodes);
    
    struct GraphNode *graph_node = new_node(number_of_nodes); // logic here? 

    char* nodes = NULL;

    read_nodes_from_file(nodes, number_of_nodes, graph_node, len, file);
    int num_edges = read_edge_count_from_file(number_of_nodes, len, file);

    int current_line = number_of_nodes + 2;

    struct GraphNode *sorted_nodes = new_node(number_of_nodes);

    // for a graph with many nodes, it becomes more efficient to sort the nodes in the tree
    // before they are compared with the edge length and that data is filled in. 

    memcpy(sorted_nodes, graph_node, number_of_nodes * sizeof(struct GraphNode));
    qsort(sorted_nodes, number_of_nodes, sizeof(struct GraphNode), &node_compare);

    int edges_read_unsuccessfully = read_edges_add_to_graph(current_line, num_edges, len, file, graph, number_of_nodes, sorted_nodes);
    if (edges_read_unsuccessfully){
        free_graph(number_of_nodes, graph); 
        printf("error in reading edges.\n");
    }

    char *routes = NULL;
    getline(&routes, &len, file);

    int start_node_word_length = 0; int end_node_word_length = 0; int cursor = 0;
    
    while (getline(&routes, &len, file) != EOF)
        get_routes(routes, start_node_word_length, end_node_word_length, number_of_nodes, cursor, graph_node, graph);

    free_graph(number_of_nodes, graph);    

    fclose(file);
   
}
