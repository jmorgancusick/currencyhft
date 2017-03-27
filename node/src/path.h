#ifndef OPTIMALPATH_H
#define OPTIMALPATH_H

#include <unordered_map>
#include <string>
#include <vector>

#include "graph.h"

using namespace std;

/*
  Path stores the optimal path from a giving start node to an end node on a Graph.
  Nodes appear at most once in the path, avoiding negative cycles.
*/
class Path {
  public:
    //===========Constructors===========
    Path(const Graph& graph, const string& start, const string& end);

    //===========Setters================
    double CalcTotalRate();
    //===========Getters================
    vector<string>* GetPath() {return &path;}
    double GetTotalRate() const {return totalRate;}

    //===========Helpers================
    //TODO: convert an amount from starting currency to ending currency using optimal path
    double ConvertStartAmount(const double& amount) const {return amount*totalRate;}
    double ConvertEndAmount(const double& amount) const {return amount/totalRate;}

  private:
    //optimal path, stored as vector of nodes from start to end in order
    vector<string> path;
    double totalRate;
};

#endif