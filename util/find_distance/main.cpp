#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "Table.h"
#include "maths.h"

using namespace std;

struct NumberTable : public Table {
    NumberTable() {}

    void loadFile(ifstream&& ifs) {
        Table::loadFile(std::move(ifs), true, true);
        for (auto& item : this->datas) {
            vector<double> tmpValue(item.size());
            std::transform(item.begin(), item.end(), tmpValue.begin(),
                           [](auto& s) { return std::stod(s); });
            values.push_back(tmpValue);
        }
    }

    void findEulDis(vector<double>& point, vector<size_t>& order,
                    vector<double>& distance) {
        if (point.size() != attrCount)
            throw std::runtime_error("length mismatch");
        order.resize(dataCount);
        distance.resize(dataCount);

        for (size_t i = 0; i < dataCount; ++i) {
            double dis = 0;
            for (size_t j = 0; j < attrCount; ++j) {
                double err = point[j] - values[i][j];
                dis += (err * err);
            }
            dis = pow(dis, 1.0 / attrCount);
            distance[i] = dis;
        }
        order = sort_indexes(distance);
    }

    void findCityDis(vector<double>& point, vector<size_t>& order,
                    vector<double>& distance) {
        if (point.size() != attrCount)
            throw std::runtime_error("length mismatch");
        order.resize(dataCount);
        distance.resize(dataCount);

        for (size_t i = 0; i < dataCount; ++i) {
            double dis = 0;
            for (size_t j = 0; j < attrCount; ++j) {
                dis += std::fabs(point[j] - values[i][j]);
            }
            distance[i] = dis;
        }
        order = sort_indexes(distance);
    }

    vector<vector<double>> values;
};

int main(int argc, char** argv) {
    string fileName = argv[1];
    string method = argv[2];

    NumberTable table;
    table.loadFile(ifstream(fileName));
    cout << table.showData() << endl;

    vector<double> input(table.attrCount);
    for (size_t i = 0; i < table.attrCount; ++i) {
        input[i] = stod(string(argv[i + 3]));
    }
    vector<double> distances;
    vector<size_t> indexes;

    if (method == "eul") {
        table.findEulDis(input, indexes, distances);
    } else if (method == "city") {
        table.findCityDis(input, indexes, distances);
    } else {
        throw runtime_error("Method must be \"eul\" or \"city\"");
    }

    for (size_t i = 0; i < table.dataCount; ++i) {
        cout << "Rank " << i << '\t' << "id: " << indexes[i] << '\t'
             << "Dis: " << distances[indexes[i]] << '\t' << "(";
        for (size_t j = 0; j < table.attrCount; ++j) {
            cout << table.datas[indexes[i]][j] << " ";
        }
        cout << ")" << '\t' << table.classes[indexes[i]] << endl;
    }

    return 0;
}
