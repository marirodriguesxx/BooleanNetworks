import json
import networkx as nx
import pygraphviz as pgv
import numpy as np
import scipy as sp

# Opening JSON file
f = open('cgra_mesh_4x4.json')
data = json.load(f)

#getting the matrix dimension
dimension = data['shape']
# print ("Dimension: ",dimension)
rows = dimension[0]
cols = dimension[1]


#finding de nodes and edges from json file
edges = list()
neighbors = list()
nodes = set()
for i in data['pe']:
    neighbor = i['neighbors']
    node = i['id']
    neighbors.append(neighbor)
    nodes.add(node)

#k = len(nodes)
k=rows*cols
print(k)

# finding the edges of the graph from its neighbors
for i in nodes:
  for j in  neighbors[i]:
    pair = (j,i)
    edges.append(pair)

edges.sort()

#creating a graph by edges from json using networkx libabry
G = nx.DiGraph()
G.add_edges_from(edges)
G.add_nodes_from(sorted(nodes))
length = dict(nx.all_pairs_shortest_path_length(G))


#generating the distance matrix
shortestDistance= [[0 for x in range(k)] for y in range(k)] 
for i in range(0,k):
    for j in range(0,k):
        shortestDistance[i][j] = length[i][j]

# print the list
for i in range(0,k):
    #print() 
    for j in range(0,k):
        print(shortestDistance[i][j],end=" ")
    print(end="\n")

f.close()

