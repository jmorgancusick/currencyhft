#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <iostream>
#include "cycle.h"

using namespace std;

//adjacency matrix format using hash maps, outer string is "from node", inner string is "to node"
typedef unordered_map<string, unordered_map<string, double>> AdjacencyMatrix;

//distance estimates format using hash map, outer string is "to node", inner double is distance from
//start node to "to node", inner string is node immediately preceding "to node"
typedef unordered_map<string, pair <double, string>> DistanceEstimates;

/*
  Graph stores a directed graph of string nodes and double edge weights as an adjacency matrix.
  Reflex edges are not stored (assumed to have edge weight 0).
  Edge weights are represented as the negative natural log of the exchange rates.
*/
class Graph {

  public:
    //===========Constructors===========
    Graph(const vector<string>& currencies);

    //===========Setters================
    void InitializeCurrencies(const vector<string>& currencies);
    bool SetEdgeWeight(const string& startCurr, const string& endCurr, const double& rate);
    
    //===========Getters================
    unsigned int GetSize() const {return N;}
    double GetEdgeWeight(const string& startCurr, const string& endCurr) const;
    vector<string> GetCurrencies() const;
    vector<Cycle> GetCycles();

    //===========Helpers================
    bool CheckCurrency(const string& curr) const;

    //optimal path functions
    DistanceEstimates FindOptimalPaths(const string& start, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit);
    vector<string> GetOptimalPath(const DistanceEstimates& dists, const string& end) const;

  private:
    void BellmanFord(DistanceEstimates& dists, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit, const bool storeCycles);
    int CheckPath(const DistanceEstimates& dists, const string& start, const string& end, const bool storeCycles);
    double GetDistEstimate(const DistanceEstimates& dists, const string& node) const;
    string GetPrevNode(const DistanceEstimates& dists, const string& node) const;
    void FindCycles();

    AdjacencyMatrix graph;
    //number of nodes in graph
    unsigned int N;
    vector<Cycle> cycles;
};

#endif