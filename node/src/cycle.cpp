#include "cycle.h"
#include "sql/api.h"

using namespace std;

//construct cycles based on a mapping of nodes
Cycle::Cycle(const unordered_map<string, string>& path, bool bankFlag) {
  unsigned int size = path.size();
  vector<string> keys;
  keys.reserve(size);
  cycle.reserve(size);

  for (auto it : path) {
    keys.push_back(it.first);
  }

  //find the first node in alphanumerical order
  string current = keys[0];
  for (unsigned int i = 1; i < size; ++i) {
    if (keys[i] < current) {
      current = keys[i];
    }
  }

  cycle.push_back(current);

  //add each other node in order, until start is reached again
  for (unsigned int i = 1; i < size; ++i) {
    current = (*(path.find(current))).second;
    cycle.push_back(current);
  }

  bank = bankFlag;
}

//construct cycles based on a vector of nodes
Cycle::Cycle(const vector<string>& path, bool bankFlag) {
  unsigned int size = path.size();
  cycle.reserve(size);

  //find the first node in alphanumerical order
  int start = 0;
  for (unsigned int i = 1; i < size; ++i) {
    if (path[i] < path[start]) {
      start = i;
    }
  }

  //add nodes in order of path from the start
  for (unsigned int i = start; i < size; ++i) {
    cycle.push_back(path[i]);
  }

  for (unsigned int i = 0; i < start; ++i) {
    cycle.push_back(path[i]);
  }

  bank = bankFlag;
}

//check if a mapping of nodes is equivalent to this cycle
//assumes that other is a valid cycle
bool Cycle::CheckEquivalent(const unordered_map<string, string>& other) {
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

//check if another is equivalent to this cycle
bool Cycle::CheckEquivalent(Cycle& other) {
  //first check if sizes are the same
  if (other.GetSize() != GetSize()) {
    return false;
  }

  vector<string>* otherCycle = other.GetCycle();

  unsigned int size = GetSize();

  for (unsigned int i = 0; i < size; ++i) {
    if (cycle[i] != (*otherCycle)[i]) {
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
    //choose appropriate rates for bank or forex
    if (bank) {
      totalRate *= db->GetBankRate((*itrFrom).substr(0,3),(*itrTo).substr(0,3),(*itrTo).substr(3,3));
    }
    else {
      totalRate *= db->GetForexRate(*itrFrom+*itrTo+"=X");
    }
    ++itrTo;
    ++itrFrom;
  }

  if (bank) {
    totalRate *= db->GetBankRate((*itrFrom).substr(0,3),(*cycle.begin()).substr(0,3),(*cycle.begin()).substr(3,3));
  }
  else {
    totalRate *= db->GetForexRate(*itrFrom+*cycle.begin()+"=X");
  }

  rate = totalRate;
  delete db;
  return totalRate;
}

void Cycle::UpdateDatabase() const {
  string expath = cycle[0];
  for (unsigned int j = 1; j < cycle.size(); ++j) {
    expath += "|" + cycle[j];
  }
  API *db = new API();
  int retVal = db->connect();
  db->UpdateProfitablePath(expath, cycle.size(), GetTotalRate());
  delete db;
}