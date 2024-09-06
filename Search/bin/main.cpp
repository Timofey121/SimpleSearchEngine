#include <iostream>
#include <vector>

#include "../lib/Rank/Rank.h"

int main() {
    std::string path, req;
    std::cout << "Enter index path:\n";
    std::getline(std::cin, path);
    std::ifstream index(path, std::ios_base::binary);
    if (!index.is_open()) {
        std::cerr << "Error opening File for Searcher.\n" << std::endl;
        exit(-1);
    }
    std::cout << "Enter request:\n";
    std::getline(std::cin, req);
    auto request = BuildRequest(req);
    RefactorRequest(request);
    Rank rank(index, request.first);
    for (auto &term: request.second) {
        rank.AddTerm(term);
    }
    rank.Handle();
    std::string ans;
    std::cout << "Write K or 'ALL', if you need to output everything\n";
    std::cin >> ans;
    rank.Print(ans);
}