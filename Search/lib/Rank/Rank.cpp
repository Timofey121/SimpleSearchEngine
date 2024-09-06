#include "Rank.h"

void Rank::HandleAndOr(std::vector<std::pair<std::string, Data>> &docs, Term *term, const std::string &line) {
    for (auto &doc: term->docs) {
        bool found = false;
        for (auto &item: docs) {
            if (item.second.Deleted_) {
                continue;
            }
            if (item.first == doc.Name_) {
                found = true;
                if (line == "OR") {
                    item.second.Score_ += doc.Score_;
                    for (auto &l: doc.Text_) {
                        item.second.Text_.insert(l);
                    }
                }
            }
            if (line == "AND") {
                item.second.Score_ += doc.Score_;
                for (auto &l: doc.Text_) {
                    item.second.Text_.insert(l);
                }
            }
        }
        if (!found && line == "AND") {
            Delete(docs, doc.Name_);
        } else if (line == "OR") {
            docs.emplace_back(doc.Name_, Data(doc.Text_, doc.Score_));
        }
    }
}

void Rank::Delete(std::vector<std::pair<std::string, Data>> &docs, const std::string &key) {
    for (auto &doc: docs) {
        if (doc.first == key) {
            doc.second.Deleted_ = true;
            return;
        }
    }
}

void Rank::Handle() {
    bool first_calc = true;
    if (Line_.size() == 1) {
        Term *term = Terms_[Line_[0]];
        for (auto &doc: term->docs) {
            FirstDocs_.emplace_back(doc.Name_, Data(doc.Text_, doc.Score_));
        }
        return;
    }
    for (auto &line: Line_) {
        if (line == "AND") {
            if (first_calc) {
                first_calc = false;
                Term *term2 = SecondDocs_.top();
                SecondDocs_.pop();
                Term *term1 = SecondDocs_.top();
                SecondDocs_.pop();
                for (auto &doc: term1->docs) {
                    FirstDocs_.emplace_back(doc.Name_, Data(doc.Text_, doc.Score_));
                }
                HandleAndOr(FirstDocs_, term2, "AND");
            } else {
                Term *term = SecondDocs_.top();
                SecondDocs_.pop();
                HandleAndOr(FirstDocs_, term, "AND");
            }
        } else if (line == "OR") {
            if (first_calc) {
                first_calc = false;
                Term *term2 = SecondDocs_.top();
                SecondDocs_.pop();
                Term *term1 = SecondDocs_.top();
                SecondDocs_.pop();
                for (auto &doc: term1->docs) {
                    FirstDocs_.emplace_back(doc.Name_, Data(doc.Text_, doc.Score_));
                }
                HandleAndOr(FirstDocs_, term2, "OR");
            } else {
                Term *term = SecondDocs_.top();
                SecondDocs_.pop();
                HandleAndOr(FirstDocs_, term, "OR");
            }
        } else {
            SecondDocs_.emplace(Terms_[line]);
        }
    }

}

void Rank::Print(const std::string& ans) {
    std::sort(FirstDocs_.begin(), FirstDocs_.end(),
              [](const std::pair<std::string, Data> &lhs, const std::pair<std::string, Data> &rhs) {
                  return lhs.second.Score_ > rhs.second.Score_;
              });
    int64_t k;
    if (ans == "ALL") {
        k = FirstDocs_.size();
    } else {
        k = stoi(ans);
        if (k > FirstDocs_.size()) {
            std::cerr << "Number of documents is less than Amount\n";
            std::cerr << "Now K is Max. K = " << FirstDocs_.size() << "\n";
            k = FirstDocs_.size();
        }
    }
    for (int64_t i = 0; i < k; i++) {
        std::cout << i + 1 << ") " << "File: " << FirstDocs_[i].first << ". Lines: (";
        std::vector<uint64_t> extra;
        for (auto &line: FirstDocs_[i].second.Text_) {
            extra.push_back(line);
        }
        for (int j = 0; j < extra.size() - 1; ++j) {
            std::cout << extra[j] << ", ";
        }
        std::cout << extra.back() << ")\n";
    }
}
