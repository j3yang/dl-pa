#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "Formula.h"

/*
    We use a C interface for Formula.h and Program.h since our lexer (which is required to include these files) is in C.
    Note that the corresponding source files CAN include C++ code.
*/

typedef enum tagPOperationType {
    pGETS,
    pTEST,
    pSEQ,
    pCUP,
    pSTAR,

    // Not part of grammar -- used in automatic test generation.
    pIFTHEN,
    pIFTHENELSE,
    pWHILEDO,
    pDOWHILENOT,
    pDEF
} POperationType;

typedef struct tagProgram {
    POperationType type;
    // In the case where we have a unary operator, the single operand will be in the left branch.
    struct tagProgram *left;
    struct tagProgram *right;

    char *name;
    int atom_number;

    struct tagFormula *form;
    
} SProgram;


SProgram *createPGets(char *name, struct tagFormula *form);
SProgram *createPGetsAtom(int atom_number, struct tagFormula *form);
SProgram *createPTest(struct tagFormula *form);
SProgram *createPOperation(POperationType type, SProgram *left, SProgram *right);
SProgram *createPOperationForm(POperationType type, SProgram *left, SProgram *right, struct tagFormula *form);
void deleteProgram(SProgram *b);

#endif