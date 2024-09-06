#ifndef RANK_H
#define RANK_H

#include <stack>
#include <utility>
#include <vector>

#include "../Parser/Parser.h"
#include "../Terms/Terms.h"


class Rank {
private:
    struct Term {
        std::ifstream &BrokenFile_;
        int64_t DF_ = 0;
        int64_t DocNum_ = 0;
        int64_t AvarageNumberOfWordsInDocument_ = 0;
        struct Document {
            std::string Name_;
            int64_t Counter_;
            int64_t NumberOfWordsInDocument_;
            std::set<int64_t> Text_;
            double Score_;
        };
        std::string Name_;
        std::vector<Document> docs;

        void GetInfoAboutFile(const std::string &file);

        void GetNeedRank();

        void FindInfo(std::string &line);

        void Start();

        Term(std::string req, std::ifstream &ind) : Name_(std::move(req)), BrokenFile_(ind) {
            Start();
        }
    };

    std::ifstream &BrokenFile_;
    std::vector<std::string> &Line_;
    std::unordered_map<std::string, Term *> Terms_;

    struct Data {
        std::set<int64_t> Text_;
        double Score_;
        bool Deleted_ = false;
    };
    std::vector<std::pair<std::string, Data>> FirstDocs_;
    std::stack<Term *> SecondDocs_;

    void HandleAndOr(std::vector<std::pair<std::string, Data>> &docs, Term *term, const std::string &line);

    void Delete(std::vector<std::pair<std::string, Data>> &docs, const std::string &key);

public:
    void Handle();

    void AddTerm(const std::string &word) {
        Terms_[word] = new Term(word, BrokenFile_);
    }

    void Print(const std::string& ans);

    explicit Rank(std::ifstream &ind, std::vector<std::string> &input) : BrokenFile_(ind), Line_(input) {

    }

    ~Rank() {
        for (auto &term: Terms_) {
            delete term.second;
        }
    }
};


#endif
