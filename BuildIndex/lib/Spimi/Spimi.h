#ifndef SPIMI_H
#define SPIMI_H

#include <algorithm>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <queue>
#include <set>


class Spimi {
private:
    const int MaxWordsInBlock_ = 50;
    uint64_t DocNum_ = 0;
    uint64_t WordsNum_ = 0;
    struct Data {
        uint64_t Counter_ = 0;
        std::set<long long> Lines_;
    };
    struct Documents {
        std::unordered_map<std::string, Data> Docs_;
    };
    std::unordered_map<std::string, Documents> Data_;
    std::unordered_map<std::string, uint64_t> NumberOfWordsInDocument_;
    std::vector<std::string> Files_;
    std::vector<std::string> BrokenFiles_;

    void GetDirectoryFiles(const std::string &current_path);

    void CreateBrokenFiles();

    void WriteBrokenFile();

    void MergeBrokenFiles();

    void DeleteBrokenFiles();

public:
    void Start();
};

#endif
