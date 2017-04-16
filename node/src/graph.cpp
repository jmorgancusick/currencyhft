#include "graph.h"
#include "sql/api.h"
#include <limits>
#include <algorithm>

using namespace std;

//construct a Graph based on the list of currencies to store
//currencies must contain every currency that will be stored in the Graph
Graph::Graph(const vector<string>& currencies) {
  InitializeCurrencies(currencies);
}

//initialize Graph nodes and weights based on input currencies
void Graph::InitializeCurrencies(const vector<string>& currencies) {
  N = currencies.size();
  graph.reserve(N);

  cout << "Constructing graph of " << N << " currencies" << endl;

  //iterate through "from nodes"
  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    unordered_map<string, double> innerMap;
    innerMap.reserve(N-1);
    //iterate through "to nodes"
    for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
      //don't store reflex edges
      if (*it != *it2) {
        //all edges are initialized to infinity
        innerMap[*it2] = numeric_limits<double>::infinity();
      }
    }
    graph[*it] = innerMap;
  }
}

//set the edge weight, i.e. the rate from startCurr to endCurr
//assumes that rate is the negative natural log of the actual rate
bool Graph::SetEdgeWeight(const string& startCurr, const string& endCurr, const double& rate) {
  if (!CheckCurrency(startCurr) || !CheckCurrency(endCurr)) {
    return false;
  }

  graph[startCurr][endCurr] = rate;

  return true;
}

//gets the edge weight from startCurr to endCurr
double Graph::GetEdgeWeight(const string& startCurr, const string& endCurr) const {
  //check if start and end nodes are actually in graph, say edge weight is infinity if either is not
  if (!CheckCurrency(startCurr) || !CheckCurrency(endCurr)) {
    return numeric_limits<double>::infinity();
  }

  //const safe way to grab edge weights
  AdjacencyMatrix::const_iterator start = graph.find(startCurr);
  unordered_map<string, double>::const_iterator weight = (*start).second.find(endCurr);
  return (*weight).second;
}

//gets a vector of all the currencies in the graph, in no particular order
vector<string> Graph::GetCurrencies() const {
  vector<string> currencies;
  currencies.reserve(N);

  for (auto it = graph.begin(); it != graph.end(); ++it) {
    currencies.push_back(it->first);
  }
  return currencies;
}

//checks if a currency is currently stored in the graph or not
bool Graph::CheckCurrency(const string& curr) const {
  if (graph.find(curr) == graph.end()) {
    cout << "Currency " << curr << " not found in graph" << endl;
    return false;
  }
  return true;
}

//find distance estimats from start to other nodes, ignoring negative cycles
DistanceEstimates Graph::FindOptimalPaths(const string& start, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit) {
  cout << "Finding optimal paths from " << start <<  endl;

  DistanceEstimates dists;
  dists.reserve(N);

  vector<string> currencies = GetCurrencies();

  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    if (ignoreCurrencies.find(*it) == ignoreCurrencies.end()) {
      //first element of pair is the distance estimate to the node, initalized to infinity
      //second element is the previous node to reach this one, initialized to empty string
      dists[*it] = make_pair(numeric_limits<double>::infinity(), "");
    }
  }

  //starting node has 0 distance to itself, and its previous node is set to special value HEAD
  dists[start].first = 0;
  dists[start].second = "HEAD";

  //perform the (modified) Bellman-Ford algorithm to find shortest paths
  BellmanFord(dists, ignoreCurrencies, exchangeLimit, false);
  return dists;
}

//runs a modified Bellman-Ford algorithm to find shortest paths without entering cycles
//assumes that the start node has distance estimate 0 and prev node HEAD
void Graph::BellmanFord(DistanceEstimates& dists, const unordered_set<string>& ignoreCurrencies, const int exchangeLimit, const bool storeCycles) {
  cout << "Beginning Bellman-Ford algorithm" << endl;
  int count = 1;

  vector<string> currenciesAll = GetCurrencies();
  vector<string> currencies;
  currencies.reserve(N - ignoreCurrencies.size());

  for (auto it = currenciesAll.begin(); it != currenciesAll.end(); ++it) {
    if (ignoreCurrencies.find(*it) == ignoreCurrencies.end()) {
      currencies.push_back(*it);
    }
  }

  //running V-1 iterations ensures shortest paths are found
  //regular Bellman-Ford requires an additional iteration to check for negative cycles,
  //but as negative cycles are checked for and ignored during edge relaxations, this is not needed
  for (unsigned int v = 0; v < N - 1; ++v) {
    //flag for if any distance estimate changed during the iteration
    bool distsChanged = false;

    //iterate through "start nodes"
    for (auto it = currencies.begin(); it != currencies.end(); ++it) {
      string current = *it;
      double distStart = dists[current].first;
      //relaxing edges from a node that does not yet have a distance estimate never has an effect, so skip them
      if (distStart != numeric_limits<double>::infinity()) {
        //iterate through "to nodes"
        for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
          string next = *it2;
          //reflex edges have weight 0, so skip them
          if (current != next) {
            double edgeWeight = GetEdgeWeight(current, next);
            double distEnd = dists[next].first;
            //relaxation of the edge
            //if using the current distance estimate of the "from node" added to its weight to the "to node"
            //is less than the current distance estimate of the "to node", then use that as the new path
            if (distStart + edgeWeight < distEnd) {
              //check if path update enters a cycle or exceeds length limit, and abort the update if it does
              int length = CheckPath(dists, current, next, storeCycles);
              if (length == -1 || (exchangeLimit > 0 && length > exchangeLimit)) {
                continue;
              }

              //perform the distance update update
              dists[next].first = distStart + edgeWeight;
              dists[next].second = current;
              distsChanged = true;
            }
          }
        }
      }
    }
    //if no distance estimates change during an iteration, then the shortest paths have been found
    if (!distsChanged) {
      break;
    }
    ++count;
  }

  cout << "Bellman-Ford completed in " << count << " iterations" << endl;
  //note: at completion of Bellman-Ford, optimal path from start node to all other nodes has been found,
  //not just to the specified ending node
  //results could be cached to allow for fast retrieval of optimal paths from this start node
}

//checks if adding link from start to end introduces a cycle in the current list of distance elements
//also finds length of the path
int Graph::CheckPath(const DistanceEstimates& dists, const string& start, const string& end, const bool storeCycles) {
  int length = 0;
  string currNode = start;
  vector<string> path;
  //loop backwards until the starting node is reached
  while (currNode != "HEAD") {
    if (storeCycles) {
      path.push_back(currNode);
    }
    //if the previous node of a node is the ending node, then this is a cycle
    if (GetPrevNode(dists, currNode) == end) {
      if (storeCycles) {
        reverse(path.begin(), path.end());

        Cycle cycle = Cycle(path);
        for (unsigned int i = 0; i < cycles.size(); ++i) {
          if (cycle.CheckEquivalent(cycles[i])) {
            return -1;
          }
        }

        cycles.push_back(cycle);

        cout << "Cycle Found:" << endl;

        for (int i = 0; i < path.size(); i++) {
          cout << path[i] << endl;
        }
        cout << endl;
      }

      return -1;
    }
    currNode = GetPrevNode(dists, currNode);
    ++length;
  }
  return length;
}

//returns the optimal path to take from start node to end
//assumes that a path actually exists, which should always be the case for a currency exchange rate graph
vector<string> Graph::GetOptimalPath(const DistanceEstimates& dists, const string& end) const {
  //generate the path from start to end nodes
  cout << "Retrieving optimal path to " << end << endl;
  vector<string> path;
  string currNode = end;
  //loop backwards until the starting node is reached
  while (currNode != "HEAD") {
    path.push_back(currNode);
    currNode = GetPrevNode(dists, currNode);
    //if no prev node is found, path does not exist
    if (currNode == "") {
      return {};
    }
    //if path grows longer than number of nodes, must be in cycle
    if (path.size() > N) {
      cout << "Error: encountered cycle in optimal path" << endl;
      return {};
    }
  }

  reverse(path.begin(), path.end());
  return path;
}

//returns the distance estimate to a node
double Graph::GetDistEstimate(const DistanceEstimates& dists, const string& node) const {
  //if the requested node isn't known, say that is has infinite distance
  if (!CheckCurrency(node)) {
    return numeric_limits<double>::infinity();
  }
  return (*(dists.find(node))).second.first;
}

//returns the previous node in the current estimated path to a node
string Graph::GetPrevNode(const DistanceEstimates& dists, const string& node) const {
  //if the requested node isn't known, say that the prev node is null
  if (!CheckCurrency(node)) {
    return NULL;
  }
  return (*(dists.find(node))).second.second;
}

//finds and stores the negative cycles in the graph
void Graph::FindCycles() {
  DistanceEstimates dists;
  dists.reserve(N);

  vector<string> currencies = GetCurrencies();

  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    //first element of pair is the distance estimate to the node, initalized to infinity
    //second element is the previous node to reach this one, initialized to empty string
    dists[*it] = make_pair(numeric_limits<double>::infinity(), "");
  }

  string start = "USD";

  //starting node has 0 distance to itself, and its previous node is set to special value HEAD
  dists[start].first = 0;
  dists[start].second = "HEAD";

  //perform the (modified) Bellman-Ford algorithm to find shortest paths
  unordered_set<string> ignoreCurrencies;
  BellmanFord(dists, ignoreCurrencies, 0, true);
}

//returns the cycles that have been found
vector<Cycle> Graph::GetCycles() {
  return cycles;
}

//update the DB with found cycles
void Graph::UpdateCyclesDB() {
  for (unsigned int i = 0; i < cycles.size(); ++i) {
    vector<string>* cycle = cycles[i].GetCycle();
    string expath = (*cycle)[0];
    for (unsigned int j = 1; j < cycle->size(); ++j) {
      expath += "|" + (*cycle)[j];
    }
    API *db = new API();
    int retVal = db->connect();
    db->UpdateProfitablePath(expath, cycles[i].GetSize(), cycles[i].GetTotalRate());
  }
}