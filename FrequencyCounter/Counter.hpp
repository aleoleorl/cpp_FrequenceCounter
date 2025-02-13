#ifndef COUNTER_HPP
#define COUNTER_HPP
#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "SHA256.hpp"

using namespace std;

class Counter
{
private:
	int stringSize = 100;
	bool isNotAWordSymbol(char ch, bool isItShouldBeSpechialSymbol = true);
	int checkEndIndex(size_t curEnd, string& str);
	int checkStartIndex(size_t curStart, string& str);
	void wordCleaning(string& word);
	bool hashChecker(string str);
	void blockPart(size_t curStart, size_t curEnd, string& str, unordered_map<string, int>& wordCount, mutex& mtx);
public:
	void makeCount(string& str, unordered_map<string, int>& wordCount, int strSize = 100);
};

#endif // COUNTER_HPP