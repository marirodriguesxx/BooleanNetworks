#include <Graph.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <chrono>
#include <algorithm> 
#include <fstream>
#include <random>
#include <omp.h>
#include <map>
#include <fstream>
#include <string>

using namespace std;
using namespace std::chrono;

void annealing(const vector<vector<int>>distanceMatrix, int nodes, int dim, int gridSize, int &cost, vector<int> &grid, vector<int> &positions,
vector<int>v_i, vector<int>v, vector<int> &A, vector<bool> io, vector<bool>borders, vector<bool>mults, vector<bool>others, double *randomvec){
    cout<<"Annealing Funcion"<<endl;
    vector<int> localGrid = grid;
    vector<int> localPositions = positions;
    int currentCost = cost, nextCost, swapCount=0, increment, distManhattanI, distManhattanJ, chess; 
    //random vector index
    double random, valor;
    int randomctrl = 0;
    double T=100;
    while(T>=0.00001){
        for(int i=0; i<gridSize; i++){
            for(int j=i+1; j<gridSize; j++){                
                //if we're looking at 2 empty spaces, skip                   
                if(localGrid[i]==255 && localGrid[j]==255)
                    continue;

                int node1 = localGrid[i], node2 = localGrid[j];
                nextCost = currentCost;
                int old1, old2;
                bool test = false;
                
                if(borders[i]==false && borders[j]==false) test = true;
                if(borders[i]==true && borders[j]==true) test = true;
                if((borders[i]==true && node1!=255) && borders[j]==false) test = false;
                if(borders[i]==false && (borders[j]==true && node2!=255)) test = false;
                
                if(test){
                    //remove cost from object edges                
                    if(node1!=255){
                        for(int i=0; i<v[node1]; i++){
                            int node = localPositions[node1]; 
                            int neighbor = localPositions[A[v_i[node1]+i]];
                            increment = distanceMatrix[node][neighbor];
                            nextCost -= increment; 
                        }
                    }
                    if(node2!=255){
                        for(int i=0; i<v[node2]; i++){
                            int node = localPositions[node2]; 
                            int neighbor = localPositions[A[v_i[node2]+i]];
                            increment = distanceMatrix[node][neighbor];
                            nextCost -= increment; 
                        }
                    }
                    //swap positions
                    // int old1, old2;
                    old1 = i;
                    old2 = j;
                    if(node1!=255) localPositions[node1] = old2;
                    if(node2!=255) localPositions[node2] = old1;
                    localGrid[j] = node1;
                    localGrid[i] = node2;
                    //recalculate cost
                    if(node1!=255){
                        for(int i=0; i<v[node1]; i++){
                            int node = localPositions[node1]; 
                            int neighbor = localPositions[A[v_i[node1]+i]];
                            increment = distanceMatrix[node][neighbor];
                            nextCost += increment;                         
                        }
                    }
                    if(node2!=255){
                        for(int i=0; i<v[node2]; i++){
                            int node = localPositions[node2]; 
                            int neighbor = localPositions[A[v_i[node2]+i]];
                            increment = distanceMatrix[node][neighbor];
                            nextCost += increment;   
                        }
                    }
                }

                //parameter for annealing probability
                valor = exp(-1*(nextCost - currentCost)/T);
                //random number between 0 and 1
                random = randomvec[randomctrl];
                randomctrl++;
                if(randomctrl==1000000) randomctrl=0;

                //if cost after changes is less than before or if cost is higher but we're in the annealing probanility range, return
                if(nextCost <= currentCost || random <= valor){
                    currentCost = nextCost;
                    swapCount++;
                }
                //else, undo changes and stay with previous cost
                else{
                    if(node1!=255) localPositions[node1] = old1;
                    if(node2!=255) localPositions[node2] = old2;
                    localGrid[j] = node2;
                    localGrid[i] = node1;
                }
            }
            if(cost==0) break;             
            T*=0.999;
        }   
        //aqui T*=0.999;
    }
    

    grid = localGrid;
    positions = localPositions; 
}

void printDistances(const vector<vector<int>> &distancesmatrix, const int gridsize){
    for(int i=0; i<gridsize; i++){
        for(int j=0; j<gridsize; j++){
            cout<<distancesmatrix[i][j]<<" ";
        }
         cout<<endl;
    }
}

void printGrid(const vector<int> &grid){
    for(int i=0; i<grid.size(); i++){
        cout<<grid[i]<<" ";
    }
    cout<<endl;
}
int main(int argc, char** argv){
    srand (time(NULL));
    //Lendo uma matriz de distâncias a partir de uma arquivo .txt
    ifstream inputFile;
    string dot = "", name = "";
	int value=0, gridsize=0;
    vector<int> distances;
    
    if(argc>1){
        inputFile.open(argv[1]);
        dot = argv[2];
    }else{
        cout<<"Erro:./main Arq.txt dot.dot"<<endl; return 1;
    }
	
	inputFile >> gridsize; 
    vector<vector<int>> distancesmatrix(gridsize,std::vector<int>(gridsize,255));

	while (inputFile >> value){
        distances.push_back(value);
	}	
    inputFile.close();
    for(int i=0; i<distances.size(); i++){
        distancesmatrix[i/gridsize][i%gridsize] = distances[i];
    }

	//printDistances(distancesmatrix,gridsize);
    //------------------------------------------------------

    double *randomvec;
    randomvec = new double[1000000];
    for(int i=0;i<1000000;i++){
        randomvec[i] = (double)rand() / (double)(RAND_MAX);
    }

    //Cria a estrutura do grafo com os vetores (A, v e v_i) à partir do grafo g recebido por linha de comando
    Graph g(dot);
    int n_nodes = g.num_nodes(); //numero de vertices do grafos
    cout<<"nodos: "<<n_nodes<<endl;
    int n_edges = g.num_edges(); //numero de arestas do grafos
    vector<int> h_edgeA(n_edges,0);
    vector<int> h_edgeB(n_edges,0);
    vector<pair<int,int>> edge_list = g.get_edges(); //lista e arestas do grafo
    vector<int> nodes_list = g.get_nodes(); //lista de nodos do grafo
    vector<int> A;
    vector<int>v(n_nodes,0);
    vector<int>v_i(n_nodes,0);
    vector<bool>io(n_nodes,false);
    vector<bool>mults(n_nodes,true);
    vector<bool>others(n_nodes,false);
    //------------------------------------------------------

    //Preenche a estrutura do grafo
    for(int i=0; i<edge_list.size(); i++){
        int n1 = edge_list[i].first;
        int n2 = edge_list[i].second;
        h_edgeA[i] = n1;
        h_edgeB[i] = n2;
        v[n1]++;
        if(n1!=n2) v[n2]++;
    }

    for(int i=1; i<n_nodes; i++){
        v_i[i] = v_i[i-1] + v[i-1];
    }

    for(int i=0; i<n_nodes; i++){
        for(int j=0; j<n_edges; j++){
            if (h_edgeA[j] != h_edgeB[j]) {
                if(h_edgeA[j]==i) A.push_back(h_edgeB[j]);
                if(h_edgeB[j]==i) A.push_back(h_edgeA[j]);
            } else {
                if(h_edgeA[j]==i) A.push_back(h_edgeB[j]);
            }
        }
    }
    //------------------------------------------------------

    //Variáveis para o placement
    int cost = 100000;
    int dim=sqrt(gridsize);
    vector<int> grid(gridsize,255);
    vector<bool> borders (gridsize,false);
    vector<int>positions (n_nodes,0);
    //------------------------------------------------------

    //Setting borders
    for(int i=0; i<gridsize; i++){
        if(i/dim==0) borders[i] = true;
        if(i/dim==dim-1) borders[i] = true;
        if(i%dim==0) borders[i] = true;
        if(i%dim==dim-1) borders[i] = true;      
    }
    //------------------------------------------------------

    //Marca quais nós são IO
    for(int i=0; i<n_nodes; i++)
        if(g.get_predecessors(i).size()==0 || g.get_sucessors(i).size()==0) 
            io[i] = true;    
    //------------------------------------------------------

    //Posicionando os nodos no grid e gerando um posicionamento aleatório
    for(int i=0; i<n_nodes; i++) 
        grid[i]=i; // função da biblioteca Algorithm
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle(grid.begin(), grid.end(), std::default_random_engine(seed));

    for(int i=0; i<gridsize; i++){
        for(int j=0; j<n_nodes; j++){
            if(j==grid[i]){
                positions[j]=i;
            }
        }
    }
    //------------------------------------------------------
    for(int i=0; i<edge_list.size(); i++) cout<<edge_list[i].first<<"->"<<edge_list[i].second<<endl;
    printGrid(grid);
    annealing(distancesmatrix, n_nodes, dim, gridsize, cost, grid, positions, v_i, v, A, io, borders, mults, others, randomvec);
    printGrid(grid);
    
    //delete randomvec;
    //annealing2M(distancesmatrix, n_nodes, dim, gridsize, cost, grid, 
    //positions, v_i, v, A, io, borders, mults, others, pattern, results, randomvec, tablemesh, table1hop, tablechess1hop, tablechessmesh, tablehex);
    //simulatted_annealing(distancesmatrix,gridsize);

    return 0;
    }