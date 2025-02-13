#include <iostream>
#include "Counter.hpp"
#include "FileHandler.hpp"

using namespace std;

bool isNumber(const std::string& s) 
{
    char* end = nullptr;
    long value = std::strtol(s.c_str(), &end, 10);
    return end != s.c_str() && *end == '\0';
}

void messInfo()
{
    cout << "Information:" << endl;
    cout << "Write the path of the text file as a first argument to make a words SHA-256 frequence calculation" << endl;
    cout << "Write the number as a second argument if you want to change the number of symbols that will be used inside each thread of calculation. Default value is 100 and can't be less." << endl;
    cout << "Have a good day!" << endl;
}

void processCalculation(string path, int num)
{
    FileHandler fHandler;
    string content = fHandler.readData(path);

    unordered_map<string, int> wordCount;
    Counter counter;
    counter.makeCount(ref(content), ref(wordCount), num);

    if (wordCount.size() > 0)
    {
        vector<pair<string, int>> vec = fHandler.sort(wordCount);
        fHandler.save(path, vec);
    }
}

int main(int argc, char* argv[])
{
    locale::global(locale(""));

    if (argc <= 1)
    {
        cout << "Write -info as an argument of the file to get the information." << endl;
    }
    else
    {
        string s = argv[1];
        string inf = "-info";
        cout << argv[1] << endl;
        if (argv[1] == inf)
        {
            messInfo();
        }
        else
        {
            int num = 100;
            if (argc >= 3)
            {
                if (isNumber(argv[2]))
                num = stoi(argv[2]);
            }
            processCalculation(argv[1], num);           
        }
    }

    return 0;
}