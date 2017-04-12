#include "../node/src/graph.h"
#include "../node/src/path.h"
#include "../node/src/sql/api.h"
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <iomanip>
#include <math.h>

using namespace std;


int main(int argc, char* argv[]) {


  API *db = new API();
  int retVal = db->connect();

  cout << "Getting currencies" << endl;

  vector<string> currencies = db->GetAllCurrencies();

  Graph g = Graph(currencies);

  for (auto it = currencies.begin(); it != currencies.end(); ++it) {
    //iterate through "to nodes"
    for (auto it2 = currencies.begin(); it2 != currencies.end(); ++it2) {
      //don't store reflex edges
      if (*it != *it2) {
        //all edges are initialized to infinity
        cout << *it << *it2 << endl;
        double rate = -log(db->GetForexRate(*it+*it2+"=X"));
        g.SetEdgeWeight(*it, *it2, rate);
      }
    }
  }

  vector<string> currs = g.GetCurrencies();

  cout << "Currencies:" << endl;
  for (unsigned int i = 0; i < currs.size(); ++i) {
    cout << "\t" << currs[i] << endl;
  }

  cout << "Beginning exchange simulations. Type 'exit' as a currency to end." << endl;

  while (true) {
    cout << endl;
    string start = "";
    string end = "";
    string amountString;
    double amount = 0.0;
    string limitExchangesString;
    int limitExchanges;
    unordered_set<string> ignoreCurrencies;

    cout << "Enter start currency: ";
    cin >> start;

    if (start == "exit") {
      break;
    }
    if (!g.CheckCurrency(start)) {
      continue;
    }

    cout  << "Enter end currency: ";
    cin >> end;

    if (end == "exit") {
      break;
    }
    if (!g.CheckCurrency(end)) {
      continue;
    }

    cout << "Enter amount to convert: ";
    cin >> amountString;

    try {
      amount = stod(amountString, NULL);
    }
    catch (invalid_argument &e) {
      cout << "Error: Invalid amount" << endl;
      continue;
    }

    cout << "Enter max path length, 0 for unlimited: ";
    cin >> limitExchangesString;

    try {
      limitExchanges = stoi(limitExchangesString, NULL);
    }
    catch (invalid_argument &e) {
      cout << "Error: Invalid amount" << endl;
      continue;
    }

    cout << "Enter something other than a currency to stop input" << endl;

    while (true) {
      string ignore;
      cout << "Enter currencies to ignore: ";
      cin >> ignore;
      if (find(currencies.begin(), currencies.end(), ignore) != currencies.end()) {
        ignoreCurrencies.insert(ignore);
      }
      else {
        break;
      }
    }

    cout << endl << "Finding path from " << start << " to " << end << endl;
    Path path = Path(g, start, end, ignoreCurrencies, limitExchanges);
    vector<string> *p = path.GetPath();
    for (auto it = (*p).begin(); it != (*p).end(); ++it) {
      cout << "\t" << *it << endl;
    }

    cout << "Converting " << amountString << " " << start << " to " << end << endl;
    double convert = path.ConvertStartAmount(amount);
    cout << amountString << " " << start << " = " << convert << " " << end << endl;

    cout << "Converting " << amountString << " " << end << " to " << start << endl;
    double convert2 = path.ConvertEndAmount(amount);
    cout << amountString << " " << end << " = " << convert2 << " " << start << endl;

    double rate = db->GetForexRate(start+end+"=X");
    double regularAmount = amount * rate;
    double difference = convert - regularAmount;
    cout << "Advantage over direct conversion: " << difference << endl;
  }

    delete db;
    return 0;
}
