#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include "Formula.h"
#include "Program.h"

static const int F_WEIGHTS_LEN = fDEF+1;
static const int P_WEIGHTS_LEN = pDEF+1;

class Parameters {
        int max_atoms = 50;
        int max_depth = 50;
        int formula_weights[F_WEIGHTS_LEN] = {0};
        int atom_weights[F_WEIGHTS_LEN] = {0};
        int program_weights[P_WEIGHTS_LEN] = {0};
        int total_formula_weights = 0;
        int total_atom_weights = 0;
        int total_program_weights = 0;
        int atom_counter = 0;

    public:    
        Parameters(int f_weights[], int p_weights[], int m_atoms, int m_depth);
        int roll_formula_constructor();
        int roll_program_constructor();
        int roll_atom_number();
        int roll_atom();
        int get_max_atoms();
        int get_max_depth();
};

#endif