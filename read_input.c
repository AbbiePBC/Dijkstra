#include "read_input.h"

void read_nodes_from_file(char* nodes, int number_of_nodes, struct GraphNode *graph_node, size_t len, FILE *file){
    for (int i = 0; i < number_of_nodes; i++){
        int n = getline(&nodes, &len, file);
        nodes[n - 1] = '\0';
        graph_node[i].node_name = malloc(n * sizeof(char));
        strcpy(graph_node[i].node_name, nodes);
        graph_node[i].index = i;
    }
}


int read_edge_count_from_file(int number_of_nodes, size_t len, FILE *file){
    char *edges_count = NULL;
    for (int j = number_of_nodes + 1; j < number_of_nodes + 3; j++){
        getline(&edges_count, &len, file);
    }
    return atoi(edges_count);
}
