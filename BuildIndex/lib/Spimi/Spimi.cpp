#include "Spimi.h"

void Spimi::Start() {
    std::string directory;
    std::cout << "Enter your directory:\n";
    std::cin >> directory;
    GetDirectoryFiles(directory);
    CreateBrokenFiles();
    DeleteBrokenFiles();
}

struct Comparator {
    bool operator()(const std::pair<std::string, uint64_t> &lhs,
                    const std::pair<std::string, uint64_t> &rhs) const {
        std::stringstream s1(lhs.first), s2(rhs.first);
        std::string a, b;
        s1 >> a;
        s2 >> b;
        uint64_t minLength = std::min(a.length(), b.length());
        for (uint64_t i = 0; i < minLength; ++i) {
            if (a[i] > b[i]) {
                return true;
            } else if (a[i] < b[i]) {
                return false;
            }
        }
        return a.length() > b.length();
    }
};

void Spimi::GetDirectoryFiles(const std::string &current_path) {
    try {
        for (const auto &file: std::filesystem::directory_iterator(current_path)) {
            if (is_directory(file)) {
                GetDirectoryFiles(file.path().string());
            } else {
                ++DocNum_;
                Files_.push_back(file.path().string());
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
        exit(-1);
    }
}


void Spimi::CreateBrokenFiles() {
    uint64_t current_words = 0;
    for (const auto &filename: Files_) {
        std::ifstream file(filename);
        if (current_words > MaxWordsInBlock_) {
            WordsNum_ += current_words;
            current_words = 0;
            WriteBrokenFile();
        }
        std::string line;
        uint64_t line_number = 0;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                ++line_number;
                std::stringstream ss(line);
                std::string word;
                while (ss >> word) {
                    std::transform(word.begin(),
                                   word.end(),
                                   word.begin(),
                                   ::tolower);
                    std::string result;
                    bool hasAlpha = false;
                    for (char c: word) {
                        if (isalpha(c)) {
                            hasAlpha = true;
                            result += c;
                        } else if (hasAlpha) {
                            break;
                        }
                    }
                    if (!hasAlpha) {
                        continue;
                    }
                    ++current_words;
                    ++NumberOfWordsInDocument_[filename];
                    Data_[word].Docs_[filename].Counter_++;
                    Data_[word].Docs_[filename].Lines_.insert(line_number);
                }
            }
            file.close();
        } else {
            std::cerr << "Unable to open file " << filename << '\n';
            exit(-1);
        }
    }
    if (current_words > 0) {
        WordsNum_ += current_words;
        WriteBrokenFile();
    }
    MergeBrokenFiles();
}


void Spimi::WriteBrokenFile() {
    std::string mini = "../NeedForSearcherFiles/BrokenFile_" + std::to_string(BrokenFiles_.size() + 1) + ".txt";
    BrokenFiles_.push_back(mini);
    std::ofstream out(mini);
    if (out.is_open()) {
        std::vector<std::string> sorted_terms;
        for (const auto &term: Data_) {
            sorted_terms.push_back(term.first);
        }
        std::sort(sorted_terms.begin(), sorted_terms.end());
        for (const auto &term: sorted_terms) {
            out << term << " ";
            for (const auto &doc: Data_[term].Docs_) {
                out << doc.first << " " << doc.second.Counter_ << " " << NumberOfWordsInDocument_[doc.first] << " ( ";
                for (const auto &line: doc.second.Lines_) {
                    out << line << " ";
                }
                out << ") ";
            }
            out << '\n';
        }
        out.close();
    } else {
        std::cerr << "Unable to open BrokenFile file: " << mini << '\n';
        exit(-1);
    }
    Data_.clear();
}

void Spimi::MergeBrokenFiles() {
    std::vector<std::ifstream> files(BrokenFiles_.size());
    std::priority_queue<std::pair<std::string, uint64_t>, std::vector<std::pair<std::string, uint64_t>>, Comparator> heap;
    std::string term;
    std::ofstream out("../NeedForSearcherFiles/FileForSearcher.txt");
    out << std::to_string(WordsNum_ / DocNum_) << " " << std::to_string(DocNum_) << " *\n";
    for (uint64_t i = 0; i < files.size(); i++) {
        files[i].open(BrokenFiles_[i]);
        if (std::getline(files[i], term)) {
            heap.emplace(term, i);
        }
    }
    std::string prev;
    std::string current;
    while (!heap.empty()) {
        auto [value, index] = heap.top();
        heap.pop();
        current = value.substr(0, value.find(' '));
        if (prev.empty()) {
            out << value;
        } else if (current == prev) {
            out << value.substr(value.find(' '), value.length());
        } else {
            out << '$' << '\n' << value;
        }
        prev = current;
        if (std::getline(files[index], value)) {
            heap.emplace(value, index);
        }
    }
    for (auto &stream: files) {
        if (stream.is_open()) {
            stream.close();
        }
    }
    out.close();
}


void Spimi::DeleteBrokenFiles() {
    for (const auto &files: BrokenFiles_) {
        remove(files.c_str());
    }
}
