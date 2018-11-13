#include <iostream>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "maths.h"

using namespace std;

struct Database {
    Database() {}

    void loadFile(istream&& ifs) {
        string buf;
        string buf2;
        if (std::getline(ifs, buf)) {
            stringstream ss(buf);
            while (std::getline(ss, buf2, ',')) {
                attrName.push_back(buf2);
            }
            className = attrName.back();
            attrName.pop_back();
        }
        attrCount = attrName.size();

        cout << "AttrName: ";
        for (auto& item: attrName) {
            cout << item << ", ";
        }
        cout << endl;

        cout << "ClassName: " << className << endl;
        while (std::getline(ifs, buf)) {
            stringstream ss(buf);
            vector<double> tmpData;
            for (size_t i = 0; i < attrCount; ++i) {
                if (getline(ss, buf2, ',')) {
                    tmpData.push_back(std::stod(buf2));
                } else {
                    throw std::runtime_error("Cannot get float number.");
                }
            }
            datas.push_back(tmpData);
            if (getline(ss, buf2, ',')) {
                classes.push_back(buf2);
            }
        }
        dataCount = datas.size();

        cout << "datas" << endl;
        for (size_t i = 0; i < dataCount; ++i) {
            for (size_t j = 0; j < attrCount; ++j) {
                cout << datas[i][j] << ", ";
            }
            cout << classes[i] << endl;
        }
    }

    void findEulDis(vector<double>& point, vector<size_t>& order, vector<double>& distance) {
        if (point.size() != attrCount) throw std::runtime_error("length mismatch");
        order.resize(dataCount);
        distance.resize(dataCount);

        for (size_t i = 0; i < dataCount; ++i) {
            double dis = 0;
            for (size_t j = 0; j < attrCount; ++j) {
                double err = point[j] - datas[i][j];
                dis += (err * err);
            }
            dis = pow(dis, 1.0/attrCount);
            distance[i] = dis;
        }
        order = sort_indexes(distance);
    }

    size_t attrCount;
    size_t dataCount;

    vector<string> attrName;
    string className;

    vector<vector<double>> datas;
    vector<string> classes;
};


int main(int argc, char** argv) {
    string fileName = argv[1];

    Database db;
    db.loadFile(ifstream(fileName));

    vector<double> input(db.attrCount);
    for (size_t i = 0; i < db.attrCount; ++i) {
        input[i] = stod(string(argv[i + 2]));
    }
    vector<double> distances;
    vector<size_t> indexes;

    db.findEulDis(input, indexes, distances);

    for (size_t i = 0; i < db.dataCount; ++i) {
        cout << "Rank " << i << '\t' << "id: " << indexes[i] << '\t' << "Dis: " << distances[indexes[i]] << '\t' << "(";
        for (size_t j = 0; j < db.attrCount; ++j) {
            cout << db.datas[indexes[i]][j] << " ";
        }
        cout << ")" << '\t' << db.classes[indexes[i]] << endl;
    }

	return 0;
}
