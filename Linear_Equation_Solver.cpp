#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <cmath>
using namespace std;

struct Equation {
	double* a;
	double b;
};

void parseEquation(string input, Equation& eq, int n) {

	for (int i = 0; i < n; i++)
		eq.a[i] = 0;

	eq.b = 0;

	string s = "";
	for (char c : input)
		if (!isspace(c))
			s += c;

	int i = 0;
	int side = 1;

	while (i < s.size()) {

		if (s[i] == '=') {
			side = -1;
			i++;
			continue;
		}

		int sign = 1;

		if (s[i] == '+') i++;
		else if (s[i] == '-') {
			sign = -1;
			i++;
		}

		sign *= side;

		string number = "";
		while (i < s.size() && (isdigit(s[i]) || s[i] == '.')) {
			number += s[i];
			i++;
		}

		double value = number.empty() ? 1.0 : stod(number);
		value *= sign;

		if (i < s.size() && s[i] == 'x') {

			i++;
			string varIndex = "";

			while (i < s.size() && isdigit(s[i])) {
				varIndex += s[i];
				i++;
			}

			if (varIndex.empty())
				throw runtime_error("Missing variable index.");

			int index = stoi(varIndex) - 1;

			if (index < 0 || index >= n)
				throw runtime_error("Variable index out of range.");

			eq.a[index] += value;
		}
		else {
			eq.b -= value;
		}
	}
}

int main() {

	int n;
	cin >> n;
	cin.ignore();

	Equation* eq = new Equation[n];

	for (int i = 0; i < n; i++)
		eq[i].a = new double[n];

	for (int i = 0; i < n; i++) {

		string input;
		getline(cin, input);

		try {
			parseEquation(input, eq[i], n);
		}
		catch (exception& e) {
			cout << "Error: " << e.what() << endl;
			return 1;
		}
	}

	const double EPS = 1e-9;

	for (int i = 0; i < n; i++) {

		int maxRow = i;
		for (int k = i + 1; k < n; k++)
			if (fabs(eq[k].a[i]) > fabs(eq[maxRow].a[i]))
				maxRow = k;

		if (maxRow != i)
			swap(eq[i], eq[maxRow]);

		if (fabs(eq[i].a[i]) < EPS) {
			cout << "No unique solution.\n";
			return 1;
		}

		for (int j = i + 1; j < n; j++) {

			double factor = eq[j].a[i] / eq[i].a[i];

			for (int k = i; k < n; k++)
				eq[j].a[k] -= factor * eq[i].a[k];

			eq[j].b -= factor * eq[i].b;
		}
	}

	double* x = new double[n];

	for (int i = n - 1; i >= 0; i--) {

		x[i] = eq[i].b;

		for (int j = i + 1; j < n; j++)
			x[i] -= eq[i].a[j] * x[j];

		x[i] /= eq[i].a[i];
	}

	for (int i = 0; i < n; i++)
		cout << "x" << i + 1 << " = " << x[i] << endl;

	for (int i = 0; i < n; i++)
		delete[] eq[i].a;

	delete[] eq;
	delete[] x;

	return 0;
}

