```
A model checker for the Dynamic Logic of Propositional Assignments (DL-PA) with solving and parameterized random formula generation functionalities.

Compilation:
    make

Usage:
    ./build/dlpa {-c|-s|-g} [-v] [-f input_file_path] [-o output_file_path]

Options:
    -c check model
        Given a DL-PA formula and valuation, check if the valuation models the formula.
        Atoms not present in the valuation set are false by default.
    -s solve formula
        Given a DL-PA formula, find all valuations which model the formula.
    -g generate formula
        Given a set of parameter weights, generate a random DL-PA formula.
    -v verbose
    -f input file
        Input file contains a DL-PA formula if checking/solving, or parameter weights if generating.
        If no input file is given, read from stdin.
    -o output file
        If no output file is given, write to stdout.

Bison Grammar:   
    Valuation
        input_valuation
        : "{" valuation "}"
        |
        ;

        valuation
        : "atom" valuation 
        |
        ;
    
    Formula
        form
        : "(" form[F] ")"           
        | form[L] "\\or" form[R]    // Disjunction
        | "\\neg" form[F]           // Negation
        | "\\top"                   // True
        | "<" prog[P] ">" form[F]   // Diamond
        | "atom"                    // Atom
        ;

    Program
        prog
        : "(" prog[P] ")"           
        | "atom" "<-" form[F]       // Assignment
        | "?" form[F] "?"           // Test
        | prog[L] "\\seq" prog[R]   // Sequential composition
        | prog[L] "\\cup" prog[R]   // Non-deterministic composition
        | prog[L] "\\star"          // Finite iteration (Kleene star)
        ;

    Flex Atom format:
        [a-zA-Z][a-zA-Z0-9]*"("[ ,a-zA-Z0-9]*")"
        [a-zA-Z][a-zA-Z0-9]* 

    Example:
        {atom1, atom2, atom(x,y)} <?atom1? \seq atom2 <- \neg atom2> atom2 \or atom(x,y)
    
    
Parameters input format if generating:
    9 integers for formula weights (fATOM,fOR,fNEG,fTOP,fDIAMOND,fNEGATOM,fAND,fFALSE,fBOX)
    9 integers for program weights (pGETS,pTEST,pSEQ,pCUP,pSTAR,pIFTHEN,pIFTHENELSE,pWHILEDO,pDOWHILENOT)
    1 integer for max_depth
    1 integer for max_atoms

    If all weights are 0 then set all weights to 1 by default

Testing:
    Usage:
        python3 tests/runtests.py [test_suite ...]
    If no test suites are given then all test suites are used. 
    Only model checking is checked.
```