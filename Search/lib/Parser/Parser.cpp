#include <stack>
#include "Parser.h"

std::pair<std::vector<std::string>, std::vector<std::string>> BuildRequest(const std::string &req) {
    std::vector<std::string> line, words;
    std::string word;
    for (char c: req) {
        if (isspace(c)) {
            if (!word.empty()) {
                if (word != "AND" && word != "OR") {
                    words.push_back(word);
                }
                line.push_back(word);
                word = "";
            }
        } else if (c == '(') {
            line.emplace_back("(");
        } else if (c == ')') {
            if (!word.empty()) {
                line.push_back(word);
                word = "";
            }
            line.emplace_back(")");
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
        line.push_back(word);
    }
    return {line, words};
}

void RefactorRequest(std::pair<std::vector<std::string>, std::vector<std::string>> &request) {
    std::stack<std::string> stack;
    std::vector<std::string> out;
    for (const auto &token: request.first) {
        if (token == "AND" || token == "(") {
            stack.push(token);
        } else if (token == "OR") {
            while (!stack.empty() && stack.top() != "OR" && stack.top() != "AND") {
                std::string line = stack.top();
                if (line != "(") {
                    out.push_back(line);
                }
                stack.pop();
            }
            stack.push(token);
        } else if (token == ")") {
            std::string line = stack.top();
            stack.pop();
            while (!stack.empty() && line != "(") {
                out.push_back(line);
                line = stack.top();
                stack.pop();
            }
        } else {
            out.push_back(token);
        }
    }
    while (!stack.empty()) {
        out.push_back(stack.top());
        stack.pop();
    }
    request.first = out;
}