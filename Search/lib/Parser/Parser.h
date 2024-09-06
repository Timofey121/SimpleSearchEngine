#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

std::pair<std::vector<std::string>, std::vector<std::string>> BuildRequest(const std::string &req);
void RefactorRequest(std::pair<std::vector<std::string>, std::vector<std::string>> &request);

#endif // PARSER_H