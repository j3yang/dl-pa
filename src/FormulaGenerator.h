#ifndef __FORMULA_GENERATOR_H__
#define __FORMULA_GENERATOR_H__

#include "Formula.h"
#include "Program.h"
#include "Parameters.h"

void randomize();
SFormula *generate_atom(Parameters param);
SFormula *generate_formula(int depth, Parameters param);
SProgram* generate_program(int depth, Parameters param);
void print_formula(SFormula *form);
void print_inner_formula(SFormula *form);
void print_program(SProgram *prog);
void print_inner_program(SProgram *prog);

#endif