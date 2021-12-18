#include <iostream>
#include "Parameters.h"

using std::cout;
using std::endl;

Parameters::Parameters(int f_weights[], int p_weights[], int m_atoms, int m_depth) {
    // Read in formula weights and set everything to 1 if all weights 0
    for (int i = 0; i < F_WEIGHTS_LEN; i++) {
        formula_weights[i] = f_weights[i];
        total_formula_weights += f_weights[i];
    }
    if (total_formula_weights == 0) {
        for (int i = 0; i < F_WEIGHTS_LEN - 1; i++) {
            formula_weights[i] = 1;
            total_formula_weights++;
        }
    }
    // Process atom weights and set atom weights to 1 if all atom weights originally 0
    atom_weights[fTOP] = formula_weights[fTOP];
    atom_weights[fFALSE] = formula_weights[fFALSE];
    atom_weights[fATOM] = formula_weights[fATOM];
    atom_weights[fNEGATOM] = formula_weights[fNEGATOM];
    for (int i = 0; i < F_WEIGHTS_LEN; i++) {
        total_atom_weights += atom_weights[i];
    }
    if (total_atom_weights == 0) {
        formula_weights[fTOP] = 1;
        total_atom_weights++;
        formula_weights[fFALSE] = 1;
        total_atom_weights++;
        formula_weights[fATOM] = 1;
        total_atom_weights++;
        formula_weights[fNEGATOM] = 1;
        total_atom_weights++;
    }
    // Read in program weights and set everything to 1 if all weights 0
    for (int i = 0; i < P_WEIGHTS_LEN; i++) {
        program_weights[i] = p_weights[i];
        total_program_weights += p_weights[i];
    }
    if (total_program_weights == 0) {
        for (int i = 0; i < P_WEIGHTS_LEN - 1; i++) {
            program_weights[i] = 1;
            total_formula_weights++;
        }
    }
    
    max_atoms = m_atoms;
    max_depth = m_depth;
}

int Parameters::roll_formula_constructor() {
    int total = total_formula_weights;
    int rnd = rand()%total;
    for (int i = 0; i < F_WEIGHTS_LEN; i++) {
        if (rnd < formula_weights[i]) return i;
        else rnd -= formula_weights[i];
    }
    cout << "Error: returning fDEF in roll_formula_constructor" << endl;
    return fDEF;
}

int Parameters::roll_program_constructor() {
    int total = total_program_weights;
    int rnd = rand()%total;
    for (int i = 0; i < P_WEIGHTS_LEN; i++) {
        if (rnd < program_weights[i]) return i;
        else rnd -= program_weights[i];
    }
    cout << "Error: returning pDEF in roll_program_constructor" << endl;
    return pDEF;
}

int Parameters::roll_atom_number() { 
    return (rand()%max_atoms);
}

int Parameters::roll_atom() {
    // roll fTOP, fFALSE, fATOM or fNEGATOM based on their weights
    int temp_formula_weights[F_WEIGHTS_LEN] = {0};
    temp_formula_weights[fTOP] = formula_weights[fTOP];
    temp_formula_weights[fFALSE] = formula_weights[fFALSE];
    temp_formula_weights[fATOM] = formula_weights[fATOM];
    temp_formula_weights[fNEGATOM] = formula_weights[fNEGATOM];
    int total = total_atom_weights;
    int rnd = rand()%total;
    for (int i = 0; i < F_WEIGHTS_LEN; i++) {
        if (rnd < temp_formula_weights[i]) return i;
        else rnd -= temp_formula_weights[i];
    }
    cout << "Error: returning fDEF in roll_atom" << endl;
    return fDEF;
}

int Parameters::get_max_atoms() {
    return max_atoms;
}
        
int Parameters::get_max_depth() {
    return max_depth;
}