# Dijkstra's shortest path algorithm

## Context

When I was initially learning to code, Dijkstra's shortest path algorithm was one of the first algorithms I wrote, and I wrote it badly!
This repo was created to document the initial state of the code as well as to (hopefully) document my improvements as I learn. 


### Compilaton

```gcc -o <bin> main.c graph_functions.c```

### Usage

```./bin.out <path/to/input.txt> ```

The input file should be of the form:
- number of nodes
- list of nodes, separated by ```\n```
- new line
- number of edges
- list of edges, as: node1, node2, distance
- new line
- list of paths to find as: startNode, endNode

