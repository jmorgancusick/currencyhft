#ifndef CYCLE_H
#define CYCLE_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/*
  Cycle stores a path of nodes that from a negative cycle
  The starting node of the cycle is selected to be the first node in alphanumerical order
  Each node after the first follows the previous node, with the last node looping back to the start
*/
class Cycle {

  public:
    //===========Constructors===========
    Cycle(const unordered_map<string, string>* path);

    //===========Getters================
    vector<string>* GetCycle() {return &cycle};
    unsigned int const GetSize() const {return cycle.size()};
    double GetTotalRate() const {return rate;}

    //===========Helpers================
    bool CheckEquivalent(const unordered_map<string, string>& other) const;
    void UpdateDatabase() const;

  private:
    //===========Setters================
    double CalcRate() const;

    vector<string> cycle;
    int rate;
}

#endif