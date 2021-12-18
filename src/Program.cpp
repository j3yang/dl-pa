#include <stdlib.h>
#include <stdio.h>
#include "Program.h"

static SProgram *allocateProgram() {
    SProgram *b = (SProgram *)malloc(sizeof(SProgram));
    if (b == NULL) {
        printf("Insufficient memory for allocateProgram\n");
        return NULL;
    }
    b->type = pDEF;
    b->atom_number = -1;
    b->name = NULL;
    b->form = NULL;
    b->left = NULL;
    b->right = NULL;
    return b;
}

SProgram *createPGets(char *name, SFormula *form) {
    SProgram *b = allocateProgram();
    if (b == NULL) return NULL;
    b->type = pGETS;
    b->name = name;
    b->form = form;
    return b;
}

SProgram *createPGetsAtom(int atom_number, SFormula *form) {
    SProgram *b = allocateProgram();
    if (b == NULL) return NULL;
    b->type = pGETS;
    b->atom_number = atom_number;
    b->form = form;
    return b;
}


SProgram *createPTest(SFormula *form) {
    SProgram *b = allocateProgram();
    if (b == NULL) return NULL;
    b->type = pTEST;
    b->form = form;
    return b;
}

SProgram *createPOperation(POperationType type, SProgram *left, SProgram *right) {
    SProgram *b = allocateProgram();
    if (b == NULL) return NULL;
    b->type = type;
    b->left = left;
    b->right = right;
    return b;
}

SProgram *createPOperationForm(POperationType type, SProgram *left, SProgram *right, SFormula *form) {
    SProgram *b = allocateProgram();
    if (b == NULL) return NULL;
    b->type = type;
    b->left = left;
    b->right = right;
    b->form = form;
    return b;
}

void deleteProgram(SProgram *b) {
    if (b == NULL) return;
    if (b->name != NULL) free(b->name);
    if (b->form != NULL) deleteFormula(b->form);
    if (b->left != NULL) deleteProgram(b->left);
    if (b->right != NULL) deleteProgram(b->right);
    free(b);
}
