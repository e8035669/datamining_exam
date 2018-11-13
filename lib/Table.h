#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <istream>
#include <vector>
#include <iostream>

struct Table {
    Table() {}

    void loadFile(std::istream&& ifs, bool hasTitle, bool hasClass);

    std::string showData();

    size_t attrCount;
    size_t dataCount;

    std::vector<std::string> attrName;
    std::string className;

    std::vector<std::vector<std::string>> datas;
    std::vector<std::string> classes;
};


#endif
