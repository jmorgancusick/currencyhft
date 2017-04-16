#include "path.h"
#include "sql/api.h"
#include <limits>
#include <utility>
#include <iostream>

using namespace std;

//construct a path based on a given graph, starting currency, and ending currency
Path::Path(Graph& graph, const string& start, const string& end, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit) {
  //find and store the optimal path
  DistanceEstimates dists = graph.FindOptimalPaths(start, ignoreCurrencies, exchangeLimit);
  path = graph.GetOptimalPath(dists, end);

  //check that a path was actually found
  if (path.size() == 0) {
    cout << "Error: Path not possible" << endl;
    return;
  }

  CalcTotalRate();

  cout << "Found optimal path from " << start << " to " << end << endl;
}

//sets the total rate of converting from start to end
double Path::CalcTotalRate() {
  API *db = new API();
  int retVal = db->connect();

  auto itrTo = path.begin();
  ++itrTo;
  auto itrFrom = path.begin();

  double result = 1.0;

  //multiply all forwards rates
  while (itrTo != path.end()) {
    double rate = db->GetForexRate(*itrFrom+*itrTo+"=X");
    result *= rate;
    ++itrTo;
    ++itrFrom;
  }

  delete db;

  totalRate = result;
  cout << "Total Rate: " << totalRate << endl;
  return result;
}

