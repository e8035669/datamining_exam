#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

using namespace std;

void getSumRowCol(vector<vector<double>>& matrix, vector<double>& attrSum,
                  vector<double>& clsSum, double& allSum) {
    size_t size = matrix.size();
    attrSum.clear();
    attrSum.resize(size);
    clsSum.clear();
    clsSum.resize(size);
    allSum = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            attrSum[j] += matrix[i][j];
            clsSum[i] += matrix[i][j];
            allSum += matrix[i][j];
        }
    }
}

double calc_entropy(vector<vector<double>>& matrix) {
    vector<double> attrSum;
    vector<double> clsSum;
    double allSum;
    getSumRowCol(matrix, attrSum, clsSum, allSum);

    size_t size = matrix.size();
    double ret = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            if (matrix[i][j] > 0) {
                double num = (-matrix[i][j] * std::log2(matrix[i][j]));
                ret += num;
            }
        }
        if (attrSum[i] > 0) {
            ret += (attrSum[i] * log2(attrSum[i]));
        }
    }
    ret /= allSum;
    return ret;
}

double calc_Estart(vector<vector<double>>& matrix) {
    vector<double> attrSum;
    vector<double> clsSum;
    double allSum;
    getSumRowCol(matrix, attrSum, clsSum, allSum);

    size_t size = matrix.size();
    double Estart = 0;
    for (size_t i = 0; i < size; i++) {
        if (clsSum[i] > 0) {
            double p = clsSum[i] / allSum;
            Estart += (-p * log2(p));
        }
    }
    return Estart;
}

double calc_IG(vector<vector<double>>& matrix) {
    double Estart = calc_Estart(matrix);
    double Eavg = calc_entropy(matrix);
    return Estart - Eavg;
}

double calc_GiniIndex(vector<vector<double>>& matrix) {
    vector<double> attrSum;
    vector<double> clsSum;
    double allSum;
    getSumRowCol(matrix, attrSum, clsSum, allSum);
    size_t size = matrix.size();
    double ret = 0;
    for (size_t col = 0; col < size; col++) {
        double num = 0;
        for (size_t row = 0; row < size; row++) {
            num += (matrix[row][col] * matrix[row][col]);
        }
        if (attrSum[col] > 0) {
            ret += (num / attrSum[col]);
        }
    }
    ret = 1. - ret / allSum;
    return ret;
}

double calc_Chisquare(vector<vector<double>>& matrix) {
    vector<double> attrSum;
    vector<double> clsSum;
    double allSum;
    getSumRowCol(matrix, attrSum, clsSum, allSum);
    size_t size = matrix.size();
    double ret = 0;
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            double expect = attrSum[j] * clsSum[i] / allSum;
            if (expect > 0) {
                ret += (matrix[i][j] - expect) * (matrix[i][j] - expect) /
                       (expect);
            }
        }
    }
    return ret;
}

double calc_Split(vector<vector<double>>& matrix) {
    vector<double> attrSum;
    vector<double> clsSum;
    double allSum;
    getSumRowCol(matrix, attrSum, clsSum, allSum);
    size_t size = matrix.size();
    double ret = 0;
    for (size_t i = 0; i < size; i++) {
        if (attrSum[i] > 0) {
            double num = attrSum[i] / allSum;
            ret += (-num * log2(num));
        }
    }
    return ret;
}

double calc_GainRatio(vector<vector<double>>& matrix) {
    return calc_IG(matrix) / calc_Split(matrix);
}

int main(int argc, char** argv) {
    string buf;
    vector<vector<double>> input;
    while (getline(cin, buf)) {
        vector<double> tmpInput;
        stringstream ss(buf);
        string buf2;
        while (getline(ss, buf2, ' ')) {
            tmpInput.push_back(std::stod(buf2));
        }
        input.push_back(tmpInput);
        if (tmpInput.size() != input[0].size())
            throw runtime_error("Length mismatch.");
        if (tmpInput.size() == input.size()) {
            cout << "Matrix Complete." << endl;
            break;
        }
    }

    cout << "Entropy:   " << calc_entropy(input) << " Small better" << endl;
    cout << "IG:        " << calc_IG(input) << " Big better" << endl;
    cout << "Gini:      " << calc_GiniIndex(input) << " Small better" << endl;
    cout << "ChiSquare: " << calc_Chisquare(input) << " Big better" << endl;
    cout << "GainRatio: " << calc_GainRatio(input) << " Big better" << endl;

    return 0;
}
