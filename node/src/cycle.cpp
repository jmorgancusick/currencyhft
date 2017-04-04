#include "cycle.h"
#include "sql/api.h"

using namespace std;

//construct cycles based on a mapping of nodes
Cycle::Cycle(const unordered_map<string, string>& path) {
  unsigned int size = path.size();
  vector<string> keys;
  keys.reserve(size);
  cycle.reserve(size);

  for (auto it : path) {
    keys.push_back(it.first);
  }

  //find the first node in alphanumerical order
  sort(keys.begin(), keys.end());
  string current = keys[0];
  cycle.push_back(current);

  //add each other node in order, until start is reached again
  for (unsigned int i = 1; i < size; ++i) {
    current = (*(path.find(current))).second;
    cycle.push_back(current);
  }
}

//check if a mapping of nodes is equivalent to this cycle
//assumes that other is a valid cycle
bool Cycle::CheckEquivalent(const unordered_map<string, string>& other) const {
  //first check if sizes are the same
  if (other.size() != GetSize()) {
    return false;
  }

  unsigned int size = other.size();

  vector<string> keys;
  keys.reserve(size);

  for (auto it : other) {
    keys.push_back(it.first);
  }

  sort(keys.begin(), keys.end());
  string current = keys[0];

  //check if each node is the same
  if (current != cycle[0]) {
    return false;
  }

  for (unsigned int i = 1; i < size; ++i) {
    current = (*(other.find(current))).second;
    if (current != cycle[i]) {
      return false;
    }
  }

  return true;
}

//calculates the total rate of one loop of the cycle, from start node back to itself
double Cycle::CalcRate() {
  API *db = new API();
  int retVal = db->connect();

  auto itrTo = cycle.begin();
  ++itrTo;
  auto itrFrom = cycle.begin();

  double totalRate = 1;

  while (itrTo != cycle.end()) {
    totalRate *= db->GetForexRate(*itrFrom+*itrTo+"=X");
  }

  totalRate *= db->GetForexRate(*itrTo+*cycle.begin()+"=X");
  rate = totalRate;
  return totalRate;
}

void Cycle::UpdateDatabase() const {

}