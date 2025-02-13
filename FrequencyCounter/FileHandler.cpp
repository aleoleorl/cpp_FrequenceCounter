#include "FileHandler.hpp"

string FileHandler::readData(const string& sourcePath)
{
    ifstream source(sourcePath);

    if (!source.is_open())
    {
        cerr << "Could not open the file: " << sourcePath << endl;
        return "";
    }

    stringstream buffer;
    buffer << source.rdbuf();
    if (source.is_open())
    {
        source.close();
    }

    return buffer.str();
}

vector<pair<string, int>> FileHandler::sort(const unordered_map<string, int>& wordCount)
{
    vector<pair<string, int>> vec(wordCount.begin(), wordCount.end());
    std::sort(vec.begin(), vec.end(), FileHandler::compare);
    return vec;
}

string FileHandler::pathCorrection(const string& sourcePath)
{
    string::size_type pos = sourcePath.find_last_of('.');
    if (pos == string::npos)
    {
        return sourcePath + "_info";
    }
    else
    {
        return sourcePath.substr(0, pos) + "_info" + sourcePath.substr(pos);
    }
}

void FileHandler::save(string sourcePath, vector<pair<string, int>> vec)
{
    string destPath = pathCorrection(sourcePath);
    ofstream outFile(destPath);
    if (!outFile.is_open())
    {
        cerr << "Could not open the file: " << destPath << endl;
        return;
    }
    outFile << "Data has been obtained from the following file: \n " << sourcePath << endl;

    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm timeInfo;
    localtime_s(&timeInfo, &now_time);

    outFile << "At:" << put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << endl;

    outFile << "Data:" << endl;
    for (const auto& pair : vec)
    {
        outFile << pair.first << ": " << pair.second << endl;
    }

    outFile.close();

    cout << "Data successfully saved to " << destPath << endl;
}