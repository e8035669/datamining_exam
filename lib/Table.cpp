#include "Table.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace std;

void Table::loadFile(std::istream&& ifs, bool hasTitle, bool hasClass) {
    std::string buf, buf2;

    if (hasTitle) {
        if (std::getline(ifs, buf)) {
            stringstream ss(buf);
            while (std::getline(ss, buf2, ',')) {
                attrName.push_back(buf2);
            }
            if (hasClass) {
                className = attrName.back();
                attrName.pop_back();
            }
        }
    }

    while (std::getline(ifs, buf)) {
        stringstream ss(buf);
        vector<string> tmpData;

        while (getline(ss, buf2, ',')) {
            tmpData.push_back(buf2);
        }

        if (hasClass) {
            string cls = tmpData.back();
            classes.push_back(cls);
            tmpData.pop_back();
        }

        datas.push_back(tmpData);
    }
    dataCount = datas.size();
    if (dataCount) {
        attrCount = datas[0].size();
    }
}

std::string Table::showData() {
    char buf[200];
    vector<size_t> maxSizes(attrCount);
    if (!attrName.empty()) {
        std::transform(attrName.begin(), attrName.end(), maxSizes.begin(),
                       [](auto& v) { return v.size(); });
    }

    for (auto& item : datas) {
        std::transform(
            item.begin(), item.end(), maxSizes.begin(), maxSizes.begin(),
            [](string& s, size_t& m) { return std::max(s.size(), m); });
    }

    size_t clsSize = className.size();
    for (auto& item: classes) {
        clsSize = std::max(clsSize, item.size());
    }

    stringstream ss;
    if (attrName.empty()) {
        ss << "No titles." << endl;
    } else {
        for (size_t i = 0; i < attrName.size(); ++i) {
            sprintf(buf, "%*s", (int)maxSizes[i], attrName[i].c_str());
            ss << string(buf);
        }
    }

    if (!className.empty()) {
        sprintf(buf, "%*s", (int)clsSize, className.c_str());
        ss << string(buf);
    }
    ss << endl;

    for (size_t i = 0; i < dataCount; ++i) {
        for (size_t j = 0; j < attrCount; ++j) {
            sprintf(buf, "%*s", (int)maxSizes[j], datas[i][j].c_str());
            ss << string(buf);
        }
        if (clsSize) {
            sprintf(buf, "%*s", (int)clsSize, classes[i].c_str());
            ss << string(buf);
        }
        ss << endl;
    }

    return string(buf);
}
