//
//  main.cpp
//  KragerMinCut
//
//  Created by Abhishek Bhatia on 12/9/17.
//  Copyright © 2017 Abhishek Bhatia. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <random>

using namespace std;
default_random_engine generator;

class Node {
public:
    int id;
    vector<int> edges;

};

class Edge {
public:
    int v1, v2;
    
};

class Graph {

public:
    vector<Node> Nodes;
    vector<Edge> Edges;
    int num_nodes;

};

/*
vector<int> line_to_vec(string line) {

    vector<int> vec;
    
    
    char * temp_str;
    char cstr[line.length()];
    line.copy(cstr, line.length()-2); // removing trailing \t and \r characters
    temp_str = strtok(cstr, " \t");
    //cout << line << endl;
    while (temp_str != NULL) {
        vec.push_back(stoi(temp_str));
        //cout << "#####" << endl;
        //cout << stoi(temp_str) << endl;
        temp_str = strtok(NULL, " \t");
    }
    return vec;
}
 */

Graph Initialize_Graph (ifstream &File) {
    
    string line;
    Graph Mygraph;
    Mygraph.num_nodes = 0;
    if (File.is_open()) {
        while(!File.eof())
        {
            getline(File,line);
            
            if (!line.empty()) {
                //cout << line << endl;
                stringstream ss(line);
                //vector<int> list_vec;
                //list_vec = line_to_vec(line);
                Node _node;
                //_node.id = list_vec[0]; // Vertex
                ss >> _node.id;
                int edge_n;
                //for (int i= 1; i < list_vec.size(); i++) {
                while(ss >> edge_n ) {
                    _node.edges.push_back(edge_n);
                    //cout << " Adding edge num " << edge_n << endl;
                    if (edge_n > _node.id) { // To record edges only once
                        Edge e;
                        e.v1 = _node.id;
                        e.v2 = edge_n;
                        Mygraph.Edges.push_back(e);
                    }
                }
                Mygraph.Nodes.push_back(_node);
                Mygraph.num_nodes++;
                
            }
            
            
        }
        File.close();
    }
    else {
        cout << "file not found" << endl;
    }
    
    return Mygraph;
}

void RandomContraction(Graph &G) {
    
    // Pick a random edge from G
    
    
    uniform_int_distribution<int> distribution(0,G.Edges.size()-1);
    int edge_num = distribution(generator);
    //cout << "Picking edge num " << edge_num << endl;
    Edge E = G.Edges[edge_num];
    
    // Now merge the two nodes of the edge, keep v1 decrease node number, i.e. the edges connected to v2 should be connected to v1(v2).
    
    vector<Edge> new_edges;
    for (int j=0; j<G.Edges.size();j++) {
        Edge new_edge;
        new_edge.v1 = G.Edges[j].v1;
        new_edge.v2 = G.Edges[j].v2;
        if (new_edge.v1 == E.v2) {
            new_edge.v1 = E.v1;
        }
        if (new_edge.v2 == E.v2) {
            new_edge.v2 = E.v1;
        }
        
        if(new_edge.v1 != new_edge.v2) {
            new_edges.push_back(new_edge);
            
        }
    }
    G.Edges = new_edges;
    // Decrease number of nodes in the graph
    G.num_nodes--;
    
    
    
}
int KragerMinCut (Graph &G) {
    // Number of trials should be n^2logn to increase success rate to 1/n
    int num_trials;
    num_trials = 10000; //pow(G.Nodes.size(),2) * log(G.Nodes.size());
    
    int min_cut_best = G.Edges.size();
    
    for (int n=0; n<num_trials; n++) {
        
        Graph G_copy = G;
        
        while (G_copy.num_nodes > 2) {
            RandomContraction(G_copy);
        }
        
        if (G_copy.Edges.size() < min_cut_best) {
            min_cut_best = G_copy.Edges.size();
        }
    }
    
    
    return min_cut_best;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    
    

    ifstream File;
    File.open("/Users/abhatia/Documents/Algorithms/KargerMincut/kargerMinCut.txt");
    //File.open("/Users/abhatia/Documents/Algorithms/stanford-algs/testCases/course1/assignment4MinCut/input_random_40_200.txt");
    
    Graph Mygraph  = Initialize_Graph(File);
    int min_cut = KragerMinCut(Mygraph);
    cout << "Min cut = " << min_cut << endl;
    
    return 0;
}
