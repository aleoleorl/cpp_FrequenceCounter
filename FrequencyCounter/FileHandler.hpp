#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP
#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>
#include "Counter.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;

class FileHandler
{
private:
	static bool compare(const pair<string, int>& a, const pair<string, int>& b)
	{
		return a.second > b.second;
	}
	string pathCorrection(const string& sourcePath);
public:
	string readData(const string& sourcePath);
	vector<pair<string, int>> sort(const unordered_map<string, int>& wordCount);
	void save(string sourcePath, vector<pair<string, int>> vec);
};

#endif //FILEHANDLER_HPP