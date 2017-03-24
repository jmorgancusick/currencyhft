#include "path.h"
#include <limits>
#include <utility>
#include <iostream>

using namespace std;

//construct a path based on a given graph, starting currency, and ending currency
Path::Path(const Graph& graph, const string& start, const string& end) {
  //find and store the optimal path
  DistanceEstimates dists = graph.FindOptimalPaths(start);
  path = graph.GetOptimalPath(dists, end);

  cout << "Found optimal path from " << start << " to " << end << ":" << endl;
  for (auto it = path.begin(); it != path.end(); ++it) {
    cout << "\t" << *it << endl;
  }
}

/*
double Path::ConvertStartAmount(const double& start) {
  
}
*/