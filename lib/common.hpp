#pragma once
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Split a string into a vector by a specific token
 * @param input Input string
 * @param token Token used to split string
 * @return Vector of strings
 */
vector<string> split(string input, string token);

/**
 * @brief Transform a vector of strings to a vector of integers
 * @param inputVector Input vector of strings
 * @return Vector of integers
 */
vector<int> stringVectorToIntVector(vector<string> inputVector);

/**
 * @brief Formats collected 2D vector to output string
 */
string formatCollectedValues(vector<vector<int>> collectedValues);