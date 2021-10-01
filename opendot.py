import json
import networkx as nx
import pygraphviz as pgv
import numpy as np
import scipy as sp
M = pgv.AGraph()    #create an empty graph
M = pgv.AGraph(strict=False, directed=True) 
M = pgv.AGraph("ApoptosisNetwork.dot") #import data from dot file
M.layout(prog="dot")  #defining the graph layout
#print("-----------------edges: -------------------")
n_edges=M.number_of_edges()
#print(n_edges)
edges = M.edges()
edges.sort()
#print(edges) #Return list of edges in the graph.
print("-------------------Nodes: -------------------")
n_nodes = M.number_of_nodes()
#print(n_nodes)
nodes = M.nodes()
nodes.sort()
#print(nodes) #Return list of edges in the graph.

#print("-------------------Creating adjacency matrix: -------------------")
G = nx.DiGraph()
G.add_edges_from(edges)
G.add_nodes_from(nodes)
length = dict(nx.all_pairs_shortest_path_length(G))

#generating the distance matrix
shortestDistance= [[0 for x in range(n_nodes)] for y in range(n_nodes)] 
for i in range(0,n_nodes):
    for j in range(0,n_nodes):
        shortestDistance[i][j] = length[i][j]

# print the list
print("Distance Matrix:")
for i in range(0,n_nodes):
    #print() 
    for j in range(0,n_nodes):
        print(shortestDistance[i][j],end=" ")
    print(end="\n")




