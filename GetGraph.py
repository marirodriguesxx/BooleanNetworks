import json
import networkx as nx
import pygraphviz as pgv
import numpy as np
import scipy as sp

M = pgv.AGraph()    #create an empty graph
M = pgv.AGraph(strict=False, directed=True) 
M = pgv.AGraph("dot2.dot") #import data from dot file
M.layout(prog="dot")  #defining the graph layout
#print("-----------------edges: -------------------")
n_edges=M.number_of_edges()
print(n_edges)
edges = M.edges()
edges.sort()
print(edges) #Return list of edges in the graph.

#print("-------------------Nodes: -------------------")
n_nodes = M.number_of_nodes()
print(n_nodes)
nodes = M.nodes()
nodes.sort()
print(nodes) #Return list of edges in the graph.