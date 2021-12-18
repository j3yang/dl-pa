
#include <iostream>
#include <iterator>
#include "Utility.h"

using std::cout;
using std::endl;
using std::prev;

static const char *Formula_enum_str[] = { "fATOM", "fOR", "fNEG", "fTOP", "fDIAMOND" };
static const char *Program_enum_str[] = { "pGETS", "pTEST", "pSEQ", "pCUP", "pSTAR" };

void printValuation(const set<string> &valuation) {
    cout << "{ ";
    if (valuation.size() > 0) {
        for (auto it = valuation.begin(); it != prev(valuation.end()); it = next(it)) {
            cout << *it << ", ";
        }
        cout << *(--valuation.end());
    }
    cout << " }" << endl;
}

void printProgram(const string& prefix, const SProgram* node, bool isLeft) {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──" );
        switch (node->type) {
            case pGETS: {
                cout << Program_enum_str[node->type] << endl;
                SFormula *temp_node = createFAtom(strdup(node->name));
                printFormula( prefix + (isLeft ? "│   " : "    "), temp_node, true);
                deleteFormula(temp_node);
                printFormula( prefix + (isLeft ? "│   " : "    "), node->form, false);
                break;
            }
            case pTEST: {
                cout << Program_enum_str[node->type] << endl;
                printFormula( prefix + (isLeft ? "│   " : "    "), node->form, true);
                break;
            }
            default: {
                cout << Program_enum_str[node->type] << endl;
            }
        }
        printProgram( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printProgram( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printProgram(const SProgram* node) {
    printProgram("", node, false);    
}

void printFormula(const string& prefix, const SFormula* node, bool isLeft) {
    if (node != nullptr) {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──" );
        switch (node->type) {
            case fDIAMOND: {
                cout << Formula_enum_str[node->type] << " " << endl;
                printProgram(prefix + (isLeft ? "│   " : "    "), node->prog, true);
                break;
            }
            default: {
                cout << Formula_enum_str[node->type] << " " << (node->type == fATOM ? "atom name: " + string(node->name) : "") << endl;
            }
        }
        printFormula( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printFormula( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

void printFormula(const SFormula* node) {
    printFormula("", node, false);    
}
