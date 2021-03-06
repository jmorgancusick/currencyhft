#include "path.h"
#include <limits>
#include <utility>
#include <iostream>

using namespace std;

//construct a path based on a given graph, starting currency, and ending currency
Path::Path(Graph& graph, const string& start, const string& end, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit, API *db) {
  //find and store the optimal path
  DistanceEstimates dists = graph.FindOptimalPaths(start, ignoreCurrencies, exchangeLimit);
  path = graph.GetOptimalPath(dists, end);

  bank = graph.GetBankflag();

  //check that a path was actually found
  if (path.size() == 0) {
    cout << "Error: Path not possible" << endl;
    return;
  }

  CalcTotalRate(db);

  cout << "Found optimal path from " << start << " to " << end << endl;
}

//sets the total rate of converting from start to end
double Path::CalcTotalRate(API *db) {
  auto itrTo = path.begin();
  ++itrTo;
  auto itrFrom = path.begin();

  double result = 1.0;

  //multiply all forwards rates
  while (itrTo != path.end()) {
    double rate = 1.0;
    if (bank) {
      if ((*itrFrom).substr(0,3) != (*itrTo).substr(0,3)) {
        rate = db->GetBankRate((*itrFrom).substr(0,3),(*itrTo).substr(0,3),(*itrTo).substr(3,3));
      }
    }
    else {
      rate = db->GetForexRate(*itrFrom+*itrTo+"=X");
    }
    result *= rate;
    ++itrTo;
    ++itrFrom;
  }

  totalRate = result;
  cout << "Total Rate: " << totalRate << endl;
  return result;
}

