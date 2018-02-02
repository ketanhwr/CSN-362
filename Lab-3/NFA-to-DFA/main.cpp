/*
 * main.cpp
 * 
 * Author: Ketan Gupta <ketan19972010@gmail.com>
 *
 */

#include <bits/stdc++.h>
using namespace std;

vector<int> nfa[20][20];
int states;
int inputs;
int startSymbol;
int countEndSymbols;
int endSymbols[20];

int dfa[200][20];
int dfaStates[200];
int cntdfa;
set<int> checked;

int magic(int state, int input)
{
	if (state == -1)
		return -1;

	set<int> ms;

	while (state > 0) {
		int temp = state%10;

		for (int i = 0; i < nfa[temp][input].size(); i++)
			ms.insert(nfa[temp][input][i]);

		state /= 10;
	}

	if (ms.size() == 0)
		return -1;

	int ret = 0;

	for (set<int>::iterator it = ms.begin(); it != ms.end(); ++it) {
		ret = ret * 10 + *it;
	}

	return ret;
}

int stateExists(int state)
{
	for (int i = 0; i < cntdfa; ++i)
		if (dfaStates[i] == state)
			return i;

	return -1;
}

string output(int num)
{
	if (num == -1)
		return string("E");

	string s;
	while (num > 0) {
		int temp = num % 10;
		s += (char)('0' + temp - 1);
		num /= 10;
	}

	return string(s.rbegin(), s.rend());
}

bool containsEndState(int num)
{
	if (num == -1)
		return false;

	while (num > 0) {
		int temp = num % 10;
		for (int i = 0; i < countEndSymbols; ++i) {
			if (endSymbols[i] == temp)
				return true;
		}
		num /= 10;
	}

	return false;
}

int main()
{
	cout << "Enter number of states: ";
	cin >> states;

	cout << "Enter number of inputs: ";
	cin >> inputs;

	cout << "Enter start symbol: ";
	cin >> startSymbol;
	startSymbol++;

	cout << "Enter number of end symbols: ";
	cin >> countEndSymbols;

	cout << "Enter end symbols separated by space: ";
	for (int i = 0; i < countEndSymbols; ++i) {
		cin >> endSymbols[i];
		endSymbols[i]++;
	}

	cout << "\nEnter the NFA\n";
	cout << "For transitions, enter number of transitions followed by transitions" << endl;

	for (int i = 1; i < states + 1; ++i) {
		for (int j = 0; j < inputs; ++j) {
			cout << "Enter transitions for " << i-1 << " on " << (char)('a' + j) << ": ";
			
			int temp;
			cin >> temp;
			nfa[i][j].resize(temp);

			for (int k = 0; k < temp; ++k) {
				cin >> nfa[i][j][k];
				nfa[i][j][k]++;
			}
		}
	}

	queue<int> wow;
	wow.push(startSymbol);
	cntdfa = 1;
	dfaStates[0] = startSymbol;

	while (!wow.empty()) {
		int node = wow.front();
		wow.pop();
		checked.insert(node);

		// cout << "Current checking: " << node << endl;

		int currentIndex = stateExists(node);

		for (int i = 0; i < inputs; ++i) {
			int transition = magic(node, i);

			// cout << "Input " << i << " : " << transition << endl;

			int it = stateExists(transition);

			if (it == -1) {
				dfaStates[cntdfa] = transition;
				cntdfa++;
				dfa[currentIndex][i] = cntdfa - 1;
			} else {
				dfa[currentIndex][i] = it;
			}

			if (checked.find(transition) == checked.end()) {
				wow.push(transition);
			}
		}
	}

	cout << "\nDFA Generated:\n" << endl;

	for (int i = 0; i < cntdfa; ++i) {
		cout << output(dfaStates[i]) << ": ";
		for (int j = 0; j < inputs; ++j) {
			cout << output(dfaStates[dfa[i][j]]) << " ";
		}
		cout << endl;
	}

	cout << "\nStart State: " << startSymbol - 1 << endl;

	cout << "\nEnd States: ";

	for (int i = 0; i < cntdfa; ++i) {
		if (containsEndState(dfaStates[i]))
			cout << output(dfaStates[i]) << " ";
	}

	cout << endl;

	return 0;
}
