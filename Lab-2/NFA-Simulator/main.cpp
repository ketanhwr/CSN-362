/*
 * main.cpp
 * 
 * Author: Ketan Gupta <ketan19972010@gmail.com>
 *
 */

#include <iostream>
#include <cstring>
using namespace std;

int symbols;
char inputSymbol[50];

int dfastates;
char DFAState[50];

char startState;
int startStateIndex;

int countFinish;
char finishStates[50];
int finishStatesIndex[50];

int DFA[50][50];

char getInputSymbol(int index)
{
    return inputSymbol[index];
}

char getDFAState(int index)
{
    if (index == 49)
        return '!';
    return DFAState[index];
}

int getInputIndex(char input)
{
    for (int i = 0; i < symbols; i++)
        if (inputSymbol[i] == input)
            return i;
}

int getDFAIndex(char state)
{
    if (state == '!')
        return 49;
    for (int i = 0; i < dfastates; i++)
        if (DFAState[i] == state)
            return i;
}

bool isFinishing(int index)
{
    for (int i = 0; i < countFinish; i++)
        if (finishStatesIndex[i] == index)
            return true;
    return false;
}

void initialise()
{
    for (int i = 0; i < 50; ++i)
        DFA[49][i] = 49;

    cout << "Enter number of input symbols: ";
    cin >> symbols;

    cout << "Enter input symbols separated by space: ";
    for (int i = 0; i < symbols; i++)
        cin >> inputSymbol[i];
    
    cout << "Enter number of DFA states: ";
    cin >> dfastates;

    cout << "Enter DFA States separated by space: ";
    for (int i = 0; i < dfastates; i++)
        cin >> DFAState[i];
    
    cout << "Enter Start State: ";
    cin >> startState;
    startStateIndex = getDFAIndex(startState);

    cout << "Enter number of finishing states: ";
    cin >> countFinish;

    cout << "Enter Finish States separated by space: ";
    for (int i = 0; i < countFinish; i++) {
        cin >> finishStates[i];
        finishStatesIndex[i] = getDFAIndex(finishStates[i]);
    }
}

void createTable()
{
    for (int i = 0; i < dfastates; i++) {
        cout << "Enter transitions for " << DFAState[i] << " separated by space (in order of the input symbols provided): ";
        
        char trans;
        for (int j = 0; j < symbols; j++) {
            cin >> trans;
            DFA[i][j] = getDFAIndex(trans);
        }
    }
}

void simulate()
{
    char inputString[100];
    cout << "\nEnter Input String: ";
    cin >> inputString;

    int inputLength = strlen(inputString);
    int currentState = startStateIndex;

    cout << "Starting Simulation: ";
    cout << getDFAState(currentState);

    for (int i = 0; i < inputLength; i++) {
        int inputIndex = getInputIndex(inputString[i]);
        currentState = DFA[currentState][inputIndex];
        cout << " " << getDFAState(currentState);
    }

    if (isFinishing(currentState))
        cout << "\nDFA accepts this input!\n";
    else
        cout << "\nDFA doesn't accept this input!\n";
}

int main()
{
    initialise();

    createTable();

    int cnt;
    cout << "How many inputs do you want to simulate?: ";
    cin >> cnt;

    for (int i = 0;i < cnt; i++)
        simulate();

    return 0;
}
