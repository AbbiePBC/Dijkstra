#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "graph_functions.h"


void read_nodes_from_file(char* nodes, int number_of_nodes, struct GraphNode *graph_node, 
                          size_t len, FILE *file);

int read_edge_count_from_file(int number_of_nodes, size_t len, FILE *file);

