%{
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include "Formula.h"
#include "Program.h"
#include "Algorithms.h"
#include "Utility.h"
#include "Parameters.h"
#include "FormulaGenerator.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::set;
using std::string;
using std::prev;


set<string> initial_valuation;
set<string> atom_universe {};
SFormula *main_formula;

extern "C" {
    int yyparse(void);
    int yylex(void);  
    int yywrap() {
        return 1;
    }
}

void yyerror (char const *s) {
    fprintf (stderr, "%s\n", s);
}
%}

%union {
    char *name;
    SFormula *formula;
    SProgram *program;
}

%type <formula> form
%type <program> prog

// Propositional variables
%token <name> TOKEN_ATOM "atom" 

// Formulas
%token TOKEN_LPAREN   "("
%token TOKEN_RPAREN   ")"
%token TOKEN_OR       "\\or"     
%token TOKEN_NEG      "\\neg"          
%token TOKEN_TRUE     "\\top"

// Programs
%token TOKEN_LANGLE   "<"
%token TOKEN_RANGLE   ">"
%token TOKEN_GETS     "<-"             
%token TOKEN_TEST     "?"             
%token TOKEN_SEQ      "\\seq"  
%token TOKEN_CUP      "\\cup" 
%token TOKEN_STAR     "\\star"

// Valuation
%token TOKEN_LBRACE   "{"
%token TOKEN_RBRACE   "}"

/* Precedence (increasing) and associativity */
// Programs
%left "?"
%left "<-"
%left "\\seq"
%left "\\cup"
%left "\\star"

// Formulas
%left "\\or"
%left ">"
%left "\\neg"

%%

input
    : input_valuation form { main_formula = $2; }
    ;

form
    : "(" form[F] ")"           { $$ = $F; }
    | form[L] "\\or" form[R]    { $$ = createFOperation( fOR, $L, $R ); }
    | "\\neg" form[F]           { $$ = createFOperation( fNEG, $F, NULL ); }
    | "\\top"                   { $$ = createFOperation( fTOP, NULL, NULL); }
    | "<" prog[P] ">" form[F]   { $$ = createFDiamond($P, $F); }
    | "atom"                    { $$ = createFAtom($1); atom_universe.insert(string($1)); }
    ;

prog
    : "(" prog[P] ")"           { $$ = $P; }
    | "atom" "<-" form[F]       { $$ = createPGets($1, $F); atom_universe.insert(string($1)); } 
    | "?" form[F] "?"           { $$ = createPTest($F); }
    | prog[L] "\\seq" prog[R]   { $$ = createPOperation(pSEQ, $L, $R); }
    | prog[L] "\\cup" prog[R]   { $$ = createPOperation(pCUP, $L, $R); }
    | prog[L] "\\star"          { $$ = createPOperation(pSTAR, $L, NULL); }
    ;

input_valuation
    : "{" valuation "}"         { ; }
    |
    ;

valuation
    : "atom" valuation          { initial_valuation.insert(string($1)); }
    |
    ;

%%

int main(int argc, char *argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    string input_file_path;
    string output_file_path;
    bool expecting_input_file = false;
    bool expecting_output_file = false;
    bool verbose = false;
    bool check_mode = false;
    bool solve_mode = false;
    bool generate_mode = false;

    for (string arg : args) {
        if (arg == "-v") verbose = true;
        else if (arg == "-c" && !(solve_mode || generate_mode)) check_mode = true;
        else if (arg == "-s" && !(generate_mode || check_mode)) solve_mode = true;
        else if (arg == "-g" && !(check_mode || solve_mode)) generate_mode = true;
        else if (expecting_input_file) {
            if (std::filesystem::exists(arg)) {
                input_file_path = arg;
                freopen(input_file_path.c_str(), "r", stdin);
                expecting_input_file = false;
            } else {
                cout << "Input file does not exist" << endl;
                return 0;
            }
        }
        else if (expecting_output_file) {
            output_file_path = arg;
            freopen(output_file_path.c_str(), "w", stdout);
            expecting_output_file = false;
        }
        else if (arg == "-f") expecting_input_file = true;
        else if (arg == "-o") expecting_output_file = true;
        else {
            cout << "Usage: ./dlpa_solve {-s|-c|-g} [-v] [-f input_file_path] [-o output_file_path]" << endl;
            return 0;
        }
    } 

    if (generate_mode) {
        if (verbose) cout << "=====Generating=====" << endl;
        randomize();
        int f_weights[F_WEIGHTS_LEN] = {0};
        int p_weights[P_WEIGHTS_LEN] = {0};
        int m_atoms;
        int m_depth;
        for (int i = 0; i < F_WEIGHTS_LEN - 1; i++) cin >> f_weights[i];
        for (int i = 0; i < P_WEIGHTS_LEN - 1; i++) cin >> p_weights[i];
        cin >> m_atoms;
        cin >> m_depth;
        Parameters param(f_weights, p_weights, m_atoms, m_depth);
        SFormula *root = generate_formula(0, param);
        print_formula(root);
        deleteFormula(root);
    } else {
        if (verbose) cout << "=====Constructing AST=====" << endl;
        yyparse();
        if (verbose) printFormula(main_formula);

        if (check_mode) {
            if (verbose) cout << "=====Checking=====" << endl;
            int sol = evaluateFormula(main_formula, initial_valuation, atom_universe.size());
            cout << (sol ? "True" : "False") << endl;
        } else if (solve_mode) {
            set<set<string> > solutions {};
            if (verbose) cout << "=====Solving=====" << endl;
            find_all_solutions(solutions, main_formula, atom_universe.begin(), 1, atom_universe.size());
            for (set<string> soln_val : solutions) {
                printValuation(soln_val);
            }
            deleteFormula(main_formula);
        }
    }
    return 0;
}
