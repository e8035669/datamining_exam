#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdio>
#include "Table.h"
#include "maths.h"

using namespace std;

struct ProbTable : public Table {
    ProbTable() {}

    void loadFile(ifstream&& ifs) {
        Table::loadFile(move(ifs), true, true);
        attrSets.resize(this->attrCount);
        for (auto& col : datas) {
            for(size_t i = 0; i < col.size(); ++i) {
                attrSets[i].push_back(col[i]);
            }
        }
        for (auto& item: attrSets) {
            std::sort(item.begin(), item.end());
            auto it = std::unique(item.begin(), item.end());
            item.erase(it, item.end());
            for (auto& i: item) {
                cout << i << ", ";
            }
            cout << endl;
        }
        clsSet = classes;
        std::sort(clsSet.begin(), clsSet.end());
        clsSet.erase(unique(clsSet.begin(), clsSet.end()), clsSet.end());
        for (auto& i: clsSet) {
            cout << i << ", ";
        }
        cout << endl;
    }

    void calcProbtable() {
        attrHits.resize(attrSets.size());
        for (size_t i = 0; i < attrSets.size(); ++i) {
            attrHits[i].resize(attrSets[i].size());
            for (size_t j = 0; j < attrSets[i].size(); j++) {
                attrHits[i][j].resize(clsSet.size());
            }
        }
        clsHit.resize(clsSet.size());

        for (size_t i = 0; i < attrHits.size(); i++) {
            for (size_t j = 0; j < attrHits[i].size(); j++) {
                for (size_t k = 0; k < attrHits[i][j].size(); k++) {
                    for (size_t row = 0; row < this->dataCount; row++) {
                        if (classes[row] == clsSet[k] && datas[row][i] == attrSets[i][j]) {
                            attrHits[i][j][k]++;
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < clsSet.size(); i++) {
            for (size_t row = 0; row < this->dataCount; row++) {
                if (clsSet[i] == classes[row]) {
                    clsHit[i]++;
                }
            }
        }
    }

    string showProbtable() {
        size_t colSize = 0;
        for (auto& i: clsSet) {
            colSize = max(colSize,i.size() + 4+4);
        }
        size_t attrSize = 0;
        for (size_t i = 0; i < attrCount; i++) {
            for (auto& j: attrSets[i]) {
                attrSize = max(attrSize, attrName[i].size() + j.size() + 3 + 4);
            }
        }

        char buf[200];
        stringstream ss;
        sprintf(buf, "%*s", (int)attrSize, "");
        ss << string(buf);
        for (size_t attr = 0; attr < clsSet.size(); attr++) {
            sprintf(buf, "%*s", (int)colSize, ("cls=" + clsSet[attr]).c_str());
            ss << string(buf);
        }
        ss << endl;


        for (size_t attrId = 0; attrId < attrCount; attrId++) {
            for (size_t setId = 0; setId < attrSets[attrId].size(); setId++) {
                sprintf(buf, "%*s", (int)attrSize,
                        (attrName[attrId] + " = " + attrSets[attrId][setId]).c_str());
                ss << string(buf);
                for (size_t clsId = 0; clsId < clsSet.size(); clsId++) {
                    sprintf(buf, "%*s", (int)colSize,
                            (to_string(attrHits[attrId][setId][clsId]) + " / " + to_string(clsHit[clsId])).c_str());
                    ss << string(buf);
                }
                ss << endl;
            }
        }

        sprintf(buf, "%*s", (int)attrSize, "All");
        ss << string(buf);
        for (size_t i = 0; i < clsSet.size(); i++) {
            sprintf(buf, "%*s", (int)colSize,
                    (to_string(clsHit[i]) + " / " + to_string(this->dataCount)).c_str());
            ss << string(buf);
        }
        ss << endl;

        return ss.str();
    }

    vector<vector<string>> attrSets;
    vector<string> clsSet;

    vector<vector<vector<size_t>>> attrHits;
    vector<size_t> clsHit;
};

int main(int argc, char** argv) {
    string file(argv[1]);
    ProbTable table;
    table.loadFile(ifstream(file));
    table.calcProbtable();
    cout << table.showProbtable() << endl;

    return 0;
}
