#include <iostream>
#include "FormulaGenerator.h"

using std::cout;
using std::endl;

void randomize() {
  uint32_t seed=0;
  FILE *devrnd = fopen("/dev/random","r");
  fread(&seed, 4, 1, devrnd);
  fclose(devrnd);
  srand(seed);
  rand(); rand(); rand();
}

SFormula *generate_atom(Parameters param) {
    int curr_case = param.roll_atom();
    switch (curr_case) {
        case fATOM: {
            int atom_number = param.roll_atom_number();
            return createFAtomInt(atom_number);
        }
        case fNEGATOM: {
            int atom_number = param.roll_atom_number();
            return createFAtomIntNeg(atom_number);
        }
        case fTOP: {
            return createFOperation(fTOP, NULL, NULL);
        }
        case fFALSE: {
            return createFOperation(fFALSE, NULL, NULL);
        }
        default: {
            cout << "Error: reached default in generate_atom" << endl;
            return NULL;
        }
    }
}

SFormula *generate_formula(int depth, Parameters param) {
    if (depth == param.get_max_depth()) return generate_atom(param);
    int curr_case = param.roll_formula_constructor();
    switch (curr_case) {
        case fATOM: {
            int atom_number = param.roll_atom_number();
            return createFAtomInt(atom_number);
        }
        case fOR: {
            return createFOperation(fOR, generate_formula(depth+1, param), generate_formula(depth+1, param));
        }
        case fNEG: {
            return createFOperation(fNEG, generate_formula(depth+1, param), NULL);
        }
        case fTOP: {
            return createFOperation(fTOP, NULL, NULL);
        }
        case fDIAMOND: {
            return createFDiamond(generate_program(depth+1, param), generate_formula(depth+1, param));
        }
        // Below not part of grammar
        case fNEGATOM: {
            int atom_number = param.roll_atom_number();
            return createFAtomIntNeg(atom_number);
        }
        case fAND: {
            return createFOperation(fAND, generate_formula(depth+1, param), generate_formula(depth+1, param));
        }
        case fFALSE: {
            return createFOperation(fFALSE, NULL, NULL);
        }
        case fBOX: {
            return createFOperationProg(fBOX, generate_formula(depth+1, param), NULL, generate_program(depth+1, param));
        }
        default: {
            cout << "Error: reached default in generate_formula" << endl;
            return NULL;
        }
    }
}

SProgram* generate_program(int depth, Parameters param) {
    if (depth == param.get_max_depth()) {
        return createPTest(createFOperation(fTOP, NULL, NULL));
    }
    int curr_case = param.roll_program_constructor();
    switch (curr_case) {
        case pGETS: {
            int atom_number = param.roll_atom_number();
            return createPGetsAtom(atom_number, generate_formula(depth+1, param));
        }
        case pTEST: {
            return createPTest(generate_formula(depth+1, param));
        }
        case pSEQ: {
            return createPOperation(pSEQ, generate_program(depth+1, param), generate_program(depth+1, param));
        }
        case pCUP: {
            return createPOperation(pCUP, generate_program(depth+1, param), generate_program(depth+1, param));
        }
        case pSTAR: {
            return createPOperation(pSTAR, generate_program(depth+1, param), NULL);
        }
        // Below not part of grammar
        case pIFTHEN: {
            return createPOperationForm(pIFTHEN, generate_program(depth+1, param), NULL, generate_formula(depth+1, param));
        }
        case pIFTHENELSE: {
            return createPOperationForm(pIFTHENELSE, generate_program(depth+1, param), generate_program(depth+1, param), generate_formula(depth+1, param));
        }
        case pWHILEDO: {
            return createPOperationForm(pWHILEDO, generate_program(depth+1, param), NULL, generate_formula(depth+1, param));
        }
        case pDOWHILENOT: {
            return createPOperationForm(pDOWHILENOT, generate_program(depth+1, param), NULL, generate_formula(depth+1, param));
        }
        default: {
            cout << "Error: reached def in generate_program" << endl;
            return NULL;
        }
    }
}

void print_formula(SFormula *form) {
    switch (form->type) {
        case fATOM: {
            cout << "atom" << form->atom_number;
            break;
        }
        case fOR: {       
            print_inner_formula(form->left);
            cout << " \\or "; 
            print_inner_formula(form->right);   
            break;
        }
        case fNEG: {           
            cout << "\\neg ";
            print_inner_formula(form->left);
            break;
        }
        case fTOP: {          
            cout << "\\top";            
            break;
        }
        case fDIAMOND: {    
            cout << "<";
            print_inner_program(form->prog);
            cout << ">";
            print_inner_formula(form->left);
            break;
        }
        // Below not part of grammar
        case fNEGATOM: {
            cout << "\\neg ";
            cout << "atom" << form->atom_number;
            break;
        }
        case fAND: {            
            cout << "\\neg ";
            cout << "(";
            cout << "\\neg ";
            print_inner_formula(form->left);
            cout << " \\or ";
            cout << "\\neg ";
            print_inner_formula(form->right);
            cout << ")"; 
            break;
        }
        case fFALSE: {
            cout << "\\neg ";
            cout << "\\top";
            break;
        }
        case fBOX: {
            cout << "\\neg ";
            cout << "<";
            print_inner_program(form->prog); 
            cout << ">";
            cout << "\\neg ";
            print_inner_formula(form->left);
            break;
        }
        default: {
            cout << "Error: found def in print_formula" << endl;
        }
    }
}

void print_inner_formula(SFormula *form) {
    switch (form->type) {
        case fATOM: {
            cout << "atom" << form->atom_number;
            break;
        }
        case fOR: {       
            cout <<  "(";
            print_inner_formula(form->left);
            cout << " \\or ";
            print_inner_formula(form->right);
            cout <<  ")";       
            break;
        }
        case fNEG: {           
            cout <<  "(";
            cout << "\\neg ";
            print_inner_formula(form->left);
            cout <<  ")";   
            break;
        }
        case fTOP: {          
            cout << " \\top";            
            break;
        }
        case fDIAMOND: {    
            cout <<  "(";
            cout << "<";
            print_inner_program(form->prog);
            cout << ">";
            print_inner_formula(form->left);
            cout <<  ")";     
            break;
        }
        // Below not part of grammar
        case fNEGATOM: {
            cout << "\\neg ";
            cout << "atom" << form->atom_number;
            break;
        }
        case fAND: {
            cout << "(";
            cout << "\\neg ";
            cout << "(";
            cout << "\\neg ";
            print_inner_formula(form->left);
            cout << " \\or ";
            cout << "\\neg ";
            print_inner_formula(form->right);
            cout << ")"; 
            cout << ")";
            break;
        }
        case fFALSE: {
            cout << "\\neg ";
            cout << "\\top";
            break;
        }
        case fBOX: {
            cout << "(";
            cout << "\\neg ";
            cout << "<";
            print_inner_program(form->prog); 
            cout << ">";
            cout << "\\neg ";
            print_inner_formula(form->left);
            cout << ")";
            break;
        }
        default: {
            cout << "Error: found def in print_inner_formula" << endl;
            break;
        }
    }
}

void print_program(SProgram *prog) {
    switch (prog->type) {
        case pGETS: {           
            cout << "atom" << prog->atom_number;            
            cout << " <- ";
            print_inner_formula(prog->form);
            break;
        }
        case pTEST: {     
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            break;
        }
        case pSEQ: {      
            print_inner_program(prog->left);
            cout << " \\seq ";
            print_inner_program(prog->right);
            break;
        }
        case pCUP: {        
            print_inner_program(prog->left);
            cout << " \\cup ";
            print_inner_program(prog->right);    
            break;
        }
        case pSTAR: {      
            print_inner_program(prog->left);
            cout << "\\star";            
            break;
        }
        // Below not part of grammar
        case pIFTHEN: { 
            cout << "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout << ")";
            cout << " \\cup ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << ")";
            break;
        }
        case pIFTHENELSE: {
            cout <<  "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << " \\cup ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->right);
            cout <<  ")";
            break;
        }
        case pWHILEDO: {
            cout <<  "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << "\\star";
            cout << " \\seq ";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            break;
        }
        case pDOWHILENOT: {
            print_inner_program(prog->left);
            cout << " \\seq ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << "\\star";
            cout << "\\seq";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            break;
        }
        default: {
            cout << "Error: found def in print_program" << endl;
            break;
        }
    }
}

void print_inner_program(SProgram *prog) {
    switch (prog->type) {
        case pGETS: {       
            cout << "(";    
            cout << "atom" << prog->atom_number;            
            cout << " <- ";
            print_inner_formula(prog->form);
            cout << ")";
            break;
        }
        case pTEST: {     
            cout << "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << ")";
            break;
        }
        case pSEQ: {      
            cout << "(";
            print_inner_program(prog->left);
            cout << " \\seq ";
            print_inner_program(prog->right);
            cout << ")";
            break;
        }
        case pCUP: {        
            cout << "(";
            print_inner_program(prog->left);
            cout << " \\cup ";
            print_inner_program(prog->right);    
            cout << ")";
            break;
        }
        case pSTAR: {      
            cout << "(";
            print_inner_program(prog->left);
            cout << "\\star";            
            cout << ")";
            break;
        }
        // Below not part of grammar
        case pIFTHEN: {
            cout << "(";
            cout << "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout << ")";
            cout << " \\cup ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << ")";
            cout << ")";
            break;
        }
        case pIFTHENELSE: {
            cout << "(";
            cout <<  "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << " \\cup ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->right);
            cout <<  ")";
            cout << ")";
            break;
        }
        case pWHILEDO: {
            cout << "(";
            cout <<  "(";
            cout << "?";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << "\\star";
            cout << " \\seq ";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << ")";
            break;
        }
        case pDOWHILENOT: {
            cout << "(";
            print_inner_program(prog->left);
            cout << " \\seq ";
            cout << "(";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << " \\seq ";
            print_inner_program(prog->left);
            cout <<  ")";
            cout << "\\star";
            cout << "\\seq";
            cout << "?";
            cout << "\\neg ";
            print_inner_formula(prog->form);
            cout << "?";
            cout << ")";
            break;
        }
        default: {
            cout << "Error: found def in print_inner_program" << endl;
            break;
        }
    }
}