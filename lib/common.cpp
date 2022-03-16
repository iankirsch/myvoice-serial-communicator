#include "common.hpp"

vector<string> split(string str, string token) {
  vector<string> result;

  while (str.size() > 0) {
    int index = str.find(token);

    if (index != string::npos) {
      result.push_back(str.substr(0, index));
      str = str.substr(index + token.size());
      if (str.size() == 0) {
        result.push_back(str);
      }
    } else {
      result.push_back(str);
      break;
    }
  }
  return result;
}

vector<int> stringVectorToIntVector(vector<string> inputVector) {
  vector<int> result;

  for (int i = 0; i < inputVector.size(); i++) {
    int currentValue = atoi(inputVector[i].data());
    result.push_back(currentValue);
  }

  return result;
}

string formatCollectedValues(vector<vector<int>> collectedValues) {
  string result = "";
  for (int t = 0; t < collectedValues.size(); t++) {
    for (int i = 0; i < collectedValues[t].size(); i++) {
      result += to_string(collectedValues[t][i]);
      if (i != collectedValues[t].size() - 1) {
        result += " ";
      }
    }
    if (t != collectedValues.size() - 1) {
      result += "\n";
    }
  }
  return result;
}