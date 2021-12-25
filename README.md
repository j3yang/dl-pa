# DL-PA
## What is it?
A [model checker](https://en.wikipedia.org/wiki/Model_checking) for the [Dynamic Logic of Propositional Assignments (DL-PA)](https://hal.archives-ouvertes.fr/hal-01240721/document) with solving and parameterized random formula generation functionalities.

A brief introduction to model checking and DL-PA can be found here: [www.j3yang.dev/post/dlpa/](https://www.j3yang.dev/post/dlpa/).

## Requirements
To compile the main program, you will need:
- make
- g++ (with support for C++17)
- gcc
- Flex
- Bison

To run testing, you will need:
- Python 3
## Building
Running `make` creates the `build/` directory, intermediate `build/*.o` files and the final `build/dlpa` executable. It also uses `flex` to create the `src/lex.yy.c` file and uses `bison` to create the `src/Parser.cpp` file. Running `make clean` removes all generated files.
```bash
make          # Generate intermediate files and the final executable.
make clean    # Remove all generated finals and the final executable.
```

## Usage
It is necessary to specify whether the program is to run in solve, check or generate mode. If no input/output file is provided, then `stdin`/`stdout` are used respectively.
```
./build/dlpa {-c|-s|-g} [-v] [-f input_file_path] [-o output_file_path]
```

### Options
```
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
```

## Testing
```
python3 tests/runtests.py [test_suite ...]
```
If no test suites are given then all test suites are used. Note that only model checking is tested.

## Input Format
If running `dl-pa` in solving or checking mode, then the input consists of an optional valuation and a DL-PA formula.
```
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

Atom format:
    [a-zA-Z][a-zA-Z0-9]*"("[ ,a-zA-Z0-9]*")"
    [a-zA-Z][a-zA-Z0-9]* 

Example:
    {atom1, atom2, atom(x,y)} <?atom1? \seq atom2 <- \neg atom2> atom2 \or atom(x,y)
```
If running `dl-pa` in generating mode, the input consists of a list of integers describing the parameters. If all weights are 0, then all weights are set to 1 by default.
```
9 integers for formula weights (fATOM,fOR,fNEG,fTOP,fDIAMOND,fNEGATOM,fAND,fFALSE,fBOX).
9 integers for program weights (pGETS,pTEST,pSEQ,pCUP,pSTAR,pIFTHEN,pIFTHENELSE,pWHILEDO,pDOWHILENOT).
1 integer for max_depth.
1 integer for max_atoms.
```

## License
This project is licensed under the terms of the BSD-2-Clause License.
