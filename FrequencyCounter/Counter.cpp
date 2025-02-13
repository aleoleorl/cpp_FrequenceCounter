#include "Counter.hpp"

bool Counter::isNotAWordSymbol(char ch, bool isItShouldBeSpechialSymbol)
{
    if (isItShouldBeSpechialSymbol)
    {
        if (ch == ' ' || ch == '.' || ch == ',' || ch == '-' || ch == ':')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (ch != ' ' && ch != '.' && ch != ',' && ch != '-' && ch != ':')
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

int Counter::checkEndIndex(size_t curEnd, string& str)
{
    for (size_t i = curEnd; i < str.size(); i++)
    {
        char ch = str[i];
        if (isNotAWordSymbol(ch))
        {
            return i;
        }
        if (i == str.size() - 1)
        {
            return str.size() - 1;
        }
    }
}

int Counter::checkStartIndex(size_t curStart, string& str)
{
    int count = 0;
    bool isText = false;
    for (size_t i = curStart; i > 0; i--)
    {
        char ch = str[i];
        if (isText && isNotAWordSymbol(ch))
        {
            count++;
            isText = false;
        }
        if (!isText && isNotAWordSymbol(ch, false))
        {
            count++;
            isText = true;
        }
        if (count == 2)
        {
            return i;
        }
    }
    return 0;
}

void Counter::wordCleaning(string& word)
{
    size_t i = 0;
    while (i < word.size())
    {
        if (word[i] == '\n' || word[i] == '\0')
        {
            word.erase(i, 1);
            continue;
        }
        word[i] = tolower(word[i]);
        i++;
    }
}

bool Counter::hashChecker(string str)
{
    string hash = SHA256::hash(str);
    char lastChar = hash.back();
    return (lastChar >= '0' && lastChar <= '7');
}

void Counter::blockPart(size_t curStart, size_t curEnd, string& str, unordered_map<string, int>& wordCount, mutex& mtx)
{
    int start = checkStartIndex(curStart, ref(str));
    int end = checkEndIndex(curEnd, ref(str));

    string word1;
    string word2;
    int isNotFirst = false;

    string word;
    for (size_t i = start; i <= end; ++i)
    {
        char ch = str[i];
        if (isNotAWordSymbol(ch))
        {
            wordCleaning(word2);
            if (!word2.empty())
            {
                if (isNotFirst && hashChecker(word1 + word2))
                {
                    unique_lock<mutex> lock(mtx);
                    wordCount[word2]++;
                    lock.unlock();
                }
                else
                {
                    isNotFirst = true;
                }
                word1 = word2;
                word2.clear();
            }
        }
        else
        {
            word2 += ch;
        }
        if (i == end && isNotFirst && hashChecker(word1 + word2))
        {
            wordCleaning(word2);
            if (!word2.empty())
            {
                unique_lock<mutex> lock(mtx);
                wordCount[word2]++;
                lock.unlock();
            }
        }
    }
}

void Counter::makeCount(string& str, unordered_map<string, int>& wordCount, int strSize)
{    
    if (strSize > 100)
    {
        stringSize = strSize;
    }

    int size = str.size();
    int curStart = 0;
    int curEnd = 0;
    
    mutex mtx;
    vector<thread> threads;
        
    while (curEnd < size)
    {
        curStart = curEnd;
        curEnd = curEnd + stringSize <= size ? curEnd + stringSize : size;
        if (curEnd + stringSize > size)
        {
            curEnd = size;
        }
        threads.emplace_back(
        [this, curStart, curEnd, &str, &wordCount, &mtx]() 
        {
            this->blockPart(curStart, curEnd, str, wordCount, mtx);
        });
    }

    for (auto& th : threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}