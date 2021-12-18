#include <stdlib.h>
#include <stdio.h>
#include "Formula.h"

static SFormula *allocateFormula() {
    SFormula *b = (SFormula *)malloc(sizeof(SFormula));
    if (b == NULL) {
        printf("Insufficient memory for allocateFormula\n");
        return NULL;
    }
    b->type = fDEF;
    b->atom_number = -1;
    b->name = NULL;
    b->prog = NULL;
    b->left = NULL;
    b->right = NULL;
    return b;
}

SFormula *createFAtom(char *name) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = fATOM;
    b->name = name;
    return b;
}

SFormula *createFAtomInt(int atom_number) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = fATOM;
    b->atom_number = atom_number;
    return b;
}

SFormula *createFAtomIntNeg(int atom_number) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = fNEGATOM;
    b->atom_number = atom_number;
    return b;
}

SFormula *createFDiamond(SProgram *prog, SFormula *form) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = fDIAMOND;
    b->prog = prog;
    b->left = form;
    return b;
}

SFormula *createFOperation(FOperationType type, SFormula *left, SFormula *right) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = type;
    b->left = left;
    b->right = right;
    return b;
}

SFormula *createFOperationProg(FOperationType type, SFormula *left, SFormula *right, SProgram *prog) {
    SFormula *b = allocateFormula();
    if (b == NULL) return NULL;
    b->type = type;
    b->left = left;
    b->right = right;
    b->prog = prog;
    return b;
}

void deleteFormula(SFormula *b) {
    if (b == NULL) return;
    if (b->name != NULL) free(b->name);
    if (b->prog != NULL) deleteProgram(b->prog);
    if (b->left != NULL) deleteFormula(b->left);
    if (b->right != NULL) deleteFormula(b->right);
    free(b);
}
