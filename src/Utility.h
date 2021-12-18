#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <set>
#include <string>
#include "Formula.h"
#include "Program.h"

using std::set;
using std::string;

void printProgram(const string& prefix, const SProgram* node, bool isLeft);
void printProgram(const SProgram* node);
void printFormula(const string& prefix, const struct tagFormula* node, bool isLeft);
void printFormula(const struct tagFormula* node);
void printValuation(const set<string> &valuation);

#endif