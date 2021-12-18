#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include <set>
#include <string>
#include "Formula.h"
#include "Program.h"

using std::set;
using std::string;

int evaluateFormula(const struct tagFormula *e, const set<string> &curr_val, int universe_size);
set<set<string> > run_program_sets(const SProgram *p, const set<set<string> > &input_val, int universe_size);
void find_all_solutions(set<set<string> > &solutions, const struct tagFormula *e, set<string>::iterator it, int depth, int universe_size);

#endif