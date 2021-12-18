#ifndef __FORMULA_H__
#define __FORMULA_H__

#include "Program.h"

/*
    We use a C interface for Formula.h and Program.h since our lexer (which is required to include these files) is in C.
    Note that the corresponding source files CAN include C++ code.
*/

typedef enum tagFOperationType {
    fATOM,
    fOR,
    fNEG,
    fTOP, 
    fDIAMOND,

    // Not part of grammar -- used in automatic test generation.
    fNEGATOM,
    fAND,
    fFALSE,
    fBOX,
    fDEF
} FOperationType;

typedef struct tagFormula {
    FOperationType type;
    // In the case where we have a unary operator, the single operand will be in the left branch.
    struct tagFormula *left;
    struct tagFormula *right;

    char *name; 
    int atom_number;
    
    struct tagProgram *prog; 
    
} SFormula;

SFormula *createFAtom(char *name);
SFormula *createFAtomInt(int atom_number);
SFormula *createFAtomIntNeg(int atom_number);
SFormula *createFDiamond(struct tagProgram *prog, SFormula *form);
SFormula *createFOperation(FOperationType type, SFormula *left, SFormula *right);
SFormula *createFOperationProg(FOperationType type, SFormula *left, SFormula *right, struct tagProgram *prog);
void deleteFormula(SFormula *b);

#endif