#include "../Rank/Rank.h"


void Rank::Term::GetInfoAboutFile(const std::string &file) {
    std::string data, name;
    int64_t counter = 0;
    int64_t NumberOfWordsInDocument = 0;
    std::set<int64_t> lines;
    std::stringstream ss(file);
    ss >> data;
    ss >> data;
    while (data != "$") {
        DF_++;
        name = data;
        ss >> data;
        counter = std::stoi(data);
        ss >> data;
        NumberOfWordsInDocument = std::stoi(data);
        ss >> data;
        ss >> data;
        while (data != ")") {
            lines.insert(std::stoi(data));
            ss >> data;
        }
        docs.push_back({name, counter, NumberOfWordsInDocument, lines, 0});
        lines.clear();
        ss >> data;
    }
}

void Rank::Term::GetNeedRank() {
    for (auto &doc: docs) {
        int64_t k = 2;
        int64_t b = 1;
        auto dl = doc.NumberOfWordsInDocument_; // количество слов в текущем документе
        auto dl_avg = AvarageNumberOfWordsInDocument_; //среднее количество слов в документах
        auto n = DocNum_; // количество документов
        doc.Score_ = static_cast<double>(((doc.Counter_ * (k + 1)) / (doc.Counter_ + k * (1 - b + b * (dl / dl_avg)))) *
                                         std::log((n - DF_ + 0.5) / (DF_ + 0.5)));
    }
}

void Rank::Term::FindInfo(std::string &line) {
    BrokenFile_.seekg(0, std::ios_base::end);
    size_t FileSize = BrokenFile_.tellg();
    BrokenFile_.seekg(0, std::ios_base::beg);
    int64_t low = 0, high = FileSize;
    char symbol;
    bool found = false;
    while (low <= high) {
        int64_t mid = low + (high - low) / 2;
        BrokenFile_.seekg(mid);
        BrokenFile_.get(symbol);
        while (symbol != '$') {
            BrokenFile_.get(symbol);
            BrokenFile_.seekg(-2, std::ios_base::cur);
        }
        BrokenFile_.seekg(4, std::ios_base::cur);
        std::getline(BrokenFile_, line);
        std::stringstream ss(line);
        std::string word;
        ss >> word;
        if (word == Name_) {
            found = true;
            break;
        }
        if (word < Name_) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    if (!found) {
        std::cerr << "Word: '" << Name_ << "'  was not found in files\n";
        exit(-1);
    }
}

void Rank::Term::Start() {
    std::string data;
    BrokenFile_.seekg(0);
    BrokenFile_ >> data;
    AvarageNumberOfWordsInDocument_ = std::stoi(data);
    BrokenFile_ >> data;
    DocNum_ = std::stoi(data);
    FindInfo(data);
    GetInfoAboutFile(data);
    GetNeedRank();
}
