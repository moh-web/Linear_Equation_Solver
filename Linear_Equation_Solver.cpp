#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
using namespace std;

const int MAX = 100;

int    n;
int    numVars;
string varName[MAX];
float  co[MAX][MAX];
float  rhs[MAX];

string removeSpaces(string s) {
    string res = "";
    for (int i = 0; i < s.size(); i++)
        if (s[i] != ' ')
            res = res + s[i];
    return res;
}

string numToStr(float v) {
    if (v == (int)v)
        return to_string((int)v);

    string s = to_string(v);
    int dot = s.find('.');
    int last = s.size() - 1;
    while (last > dot && s[last] == '0') last--;
    if (last == dot) last--;
    return s.substr(0, last + 1);
}

int findVar(string name) {
    for (int i = 0; i < numVars; i++)
        if (varName[i] == name)
            return i;
    return -1;
}

int addVar(string name) {
    int idx = findVar(name);
    if (idx != -1) return idx;
    varName[numVars] = name;
    numVars++;
    return numVars - 1;
}

void sortVars() {
    for (int i = 0; i < numVars - 1; i++) {
        for (int j = 0; j < numVars - i - 1; j++) {
            if (varName[j] > varName[j+1]) {

                string tmp = varName[j];
                varName[j] = varName[j+1];
                varName[j+1] = tmp;

                for (int eq = 0; eq < n; eq++) {
                    float t = co[eq][j];
                    co[eq][j] = co[eq][j+1];
                    co[eq][j+1] = t;
                }
            }
        }
    }
}

void parseEquation(string line, int eqIdx) {
    line = removeSpaces(line);

    int eqPos = line.find('=');
    string lhs    = line.substr(0, eqPos);
    float  rhsVal = atof(line.substr(eqPos + 1).c_str());

    string terms[MAX];
    int    numTerms = 0;
    string cur = "";

    for (int i = 0; i < lhs.size(); i++) {
        char c = lhs[i];
        if ((c == '+' || c == '-') && i != 0) {
            terms[numTerms] = cur;
            numTerms++;
            cur = "";
        }
        cur = cur + c;
    }
    terms[numTerms] = cur;
    numTerms++;

    for (int t = 0; t < numTerms; t++) {
        string term = terms[t];

        int xPos = -1;
        for (int i = 0; i < term.size(); i++) {
            if (term[i] == 'x') { xPos = i; break; }
        }

        if (xPos == -1) {
            rhsVal = rhsVal - atof(term.c_str());
        } else {
            string coeffStr = term.substr(0, xPos);
            string vName    = term.substr(xPos);

            float coeff;
            if (coeffStr == "" || coeffStr == "+") coeff =  1.0;
            else if (coeffStr == "-")              coeff = -1.0;
            else                                   coeff = atof(coeffStr.c_str());

            int idx = addVar(vName);
            co[eqIdx][idx] = co[eqIdx][idx] + coeff;
        }
    }

    rhs[eqIdx] = rhsVal;
}

void printEq(float c[], float r) {
    bool first = true;
    for (int j = 0; j < numVars; j++) {
        if (c[j] == 0) continue;
        if (!first && c[j] > 0) cout << "+";
        cout << numToStr(c[j]) << varName[j];
        first = false;
    }
    if (first) cout << "0";
    cout << "=" << numToStr(r) << "\n";
}

float calcDet(float mat[][MAX], int size) {
    float tmp[MAX][MAX];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            tmp[i][j] = mat[i][j];

    float det  = 1.0;
    int   sign = 1;

    for (int col = 0; col < size; col++) {

        int pivotRow = -1;
        for (int row = col; row < size; row++) {
            if (fabs(tmp[row][col]) > 0.000001) {
                pivotRow = row;
                break;
            }
        }

        if (pivotRow == -1) return 0;

        if (pivotRow != col) {
            for (int k = 0; k < size; k++) {
                float t = tmp[col][k];
                tmp[col][k] = tmp[pivotRow][k];
                tmp[pivotRow][k] = t;
            }
            sign = sign * -1;
        }

        det = det * tmp[col][col];

        for (int row = col + 1; row < size; row++) {
            float factor = tmp[row][col] / tmp[col][col];
            for (int k = col; k < size; k++)
                tmp[row][k] = tmp[row][k] - factor * tmp[col][k];
        }
    }

    return det * sign;
}

int main() {

    cin >> n;
    cin.ignore();
    numVars = 0;

    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            co[i][j] = 0;

    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        parseEquation(line, i);
    }

    sortVars();

    string cmd;
    while (cin >> cmd) {
        cmd = removeSpaces(cmd);

        if (cmd == "quit") {
            break;
        }
        else if (cmd == "num_vars") {
            cout << numVars << "\n";
        }
        else if (cmd == "equation") {
            int idx;
            cin >> idx;
            printEq(co[idx-1], rhs[idx-1]);
        }
        else if (cmd == "column") {
            string vName;
            cin >> vName;
            vName = removeSpaces(vName);
            int idx = findVar(vName);
            for (int i = 0; i < n; i++) {
                if (idx == -1 || co[i][idx] == 0)
                    cout << "0\n";
                else
                    cout << numToStr(co[i][idx]) << "\n";
            }
        }
        else if (cmd == "add") {
            int i, j;
            cin >> i >> j;
            i--; j--;

            float res[MAX];
            float resRhs = rhs[i] + rhs[j];
            for (int k = 0; k < numVars; k++)
                res[k] = co[i][k] + co[j][k];

            printEq(res, resRhs);
        }
        else if (cmd == "subtract") {
            int i, j;
            cin >> i >> j;
            i--; j--;

            float res[MAX];
            float resRhs = rhs[i] - rhs[j];
            for (int k = 0; k < numVars; k++)
                res[k] = co[i][k] - co[j][k];

            printEq(res, resRhs);
        }
        else if (cmd == "substitute") {
            string vName;
            int dst, src;
            cin >> vName >> dst >> src;
            vName = removeSpaces(vName);
            dst--; src--;

            int vIdx = findVar(vName);

            float res[MAX];
            float resRhs = rhs[dst];
            for (int k = 0; k < numVars; k++)
                res[k] = co[dst][k];

            if (vIdx != -1 && co[dst][vIdx] != 0 && co[src][vIdx] != 0) {
                float scale = co[dst][vIdx] / co[src][vIdx];
                resRhs = resRhs - scale * rhs[src];
                for (int k = 0; k < numVars; k++)
                    res[k] = res[k] - scale * co[src][k];
            }

            printEq(res, resRhs);
        }
        else if (cmd == "D") {
            string rest;
            getline(cin, rest);
            rest = removeSpaces(rest);

            float mat[MAX][MAX];
            for (int i = 0; i < n; i++)
                for (int j = 0; j < numVars; j++)
                    mat[i][j] = co[i][j];

            if (rest != "") {
                int vIdx = findVar(rest);
                if (vIdx != -1)
                    for (int i = 0; i < n; i++)
                        mat[i][vIdx] = rhs[i];
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < numVars; j++) {
                    if (j > 0) cout << " ";
                    cout << numToStr(mat[i][j]);
                }
                cout << "\n";
            }
        }
        else if (cmd == "D_value") {
            int sz = n < numVars ? n : numVars;

            float mat[MAX][MAX];
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++)
                    mat[i][j] = co[i][j];

            cout << numToStr(calcDet(mat, sz)) << "\n";
        }
        else if (cmd == "solve") {
            int sz = n < numVars ? n : numVars;

            float mat[MAX][MAX];
            for (int i = 0; i < sz; i++)
                for (int j = 0; j < sz; j++)
                    mat[i][j] = co[i][j];

            float dval = calcDet(mat, sz);

            if (dval == 0) {
                cout << "No Solution\n";
            } else {
                for (int v = 0; v < sz; v++) {
                    float cramer[MAX][MAX];
                    for (int i = 0; i < sz; i++)
                        for (int j = 0; j < sz; j++)
                            cramer[i][j] = co[i][j];

                    for (int i = 0; i < sz; i++)
                        cramer[i][v] = rhs[i];

                    float dv = calcDet(cramer, sz);
                    cout << varName[v] << "=" << numToStr(dv / dval) << "\n";
                }
            }
        }

    }

    return 0;
}
