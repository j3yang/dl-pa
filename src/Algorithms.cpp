#include <iostream>
#include <algorithm>
#include <iterator>
#include "Algorithms.h"

using std::cout;
using std::endl;
using std::set_difference;
using std::inserter;
using std::next;

int evaluateFormula(const SFormula *const e, const set<string> &curr_valuation, int universe_size) {
    int result = 0;
    switch (e->type) {
        case fATOM: {
            result = (curr_valuation.find(string(e->name)) != curr_valuation.end()) ? 1 : 0;
            break;
        }
        case fOR: {
            result = evaluateFormula(e->left, curr_valuation, universe_size) || evaluateFormula(e->right, curr_valuation, universe_size);
            break;
        }
        case fNEG: {
            result = !evaluateFormula(e->left, curr_valuation, universe_size);
            break;
        }
        case fTOP: {
            result = 1;
            break;
        }
        case fDIAMOND: {
            set<set<string> > singleton_curr_valuation = {curr_valuation}; 
            set<set<string> > new_valuations = run_program_sets(e->prog, singleton_curr_valuation, universe_size);
            for (set<string> valuation : new_valuations) {
                int form_true = evaluateFormula(e->left, valuation, universe_size);
                if (form_true) {
                    result = 1;
                    break;
                }
            }
            break;
        }
        default: {
            cout << "Error: reached default in evaluateFormula" << endl;
            return 0;
        }
    }
    return result;
}


set<set<string> > run_program_sets(const SProgram *p, const set<set<string> > &input_valuations, int universe_size) {
    // Each individual valuation will produce a set of valuations
    // all_output_valuations is the union of all sets of valuations
    set<set<string> > all_output_valuations = {};
    int form_true;
    for (set<string> valuation : input_valuations) {    
        set<set<string> > curr_output_valuations = {};
        set<string> curr_valuation = valuation;
        set<set<string> > singleton_curr_valuation = {curr_valuation};
        switch (p->type) {
            case pGETS: {
                form_true = evaluateFormula(p->form, curr_valuation, universe_size);
                if (form_true) curr_valuation.insert(p->name);
                else curr_valuation.erase(p->name);
                curr_output_valuations.insert(curr_valuation);
                break;
            }
            case pTEST: {
                form_true = evaluateFormula(p->form, curr_valuation, universe_size);
                if (form_true) {
                    curr_output_valuations.insert(curr_valuation);
                } 
                break;
            }
            case pSEQ: {
                curr_output_valuations = run_program_sets(p->left, singleton_curr_valuation, universe_size);
                curr_output_valuations = run_program_sets(p->right, curr_output_valuations, universe_size);
                break;
            }
            case pCUP: {
                set<set<string> > left_output_valuations = run_program_sets(p->left, singleton_curr_valuation, universe_size);
                curr_output_valuations.insert(left_output_valuations.begin(), left_output_valuations.end());
                set<set<string> > right_output_valuations = run_program_sets(p->right, singleton_curr_valuation, universe_size);
                curr_output_valuations.insert(right_output_valuations.begin(), right_output_valuations.end());
                break;
            }
            case pSTAR: {
                // Merge output, use next output as input
                set<set<string> > temp_input_valuations = singleton_curr_valuation;
                set<set<string> > seen_input_valuations;
                set<set<string> > temp_output_valuations;
                curr_output_valuations.insert(curr_valuation); // Corresponds to running the program 0 times
                for (int i = 0; i <= (1 << universe_size); i++) {
                    seen_input_valuations.insert(temp_input_valuations.begin(), temp_input_valuations.end());
                    temp_output_valuations = run_program_sets(p->left, temp_input_valuations, universe_size);                    
                    curr_output_valuations.insert(temp_output_valuations.begin(), temp_output_valuations.end());
                    set<set<string> > diff {};
                    set_difference(temp_output_valuations.begin(),temp_output_valuations.end(),seen_input_valuations.begin(), seen_input_valuations.end(), inserter(diff, diff.begin())); // set difference: temp_output_valuation - seen_input_valuation
                    if (diff.size() == 0) break;
                    temp_input_valuations = diff;
                }
                break;
            }
            default: {
                cout << "Error: reached default in run_program_sets" << endl;
            }
        }
        all_output_valuations.insert(curr_output_valuations.begin(), curr_output_valuations.end());
    } 
    return all_output_valuations;
}


void find_all_solutions(set<set<string> > &solutions, const struct tagFormula *e, set<string>::iterator it, int depth, int universe_size) {
    static set<string> val_subset = {};
    if (depth == universe_size + 1) {
        if (evaluateFormula(e, val_subset, universe_size)) solutions.insert(val_subset);
    } else {
        val_subset.insert(*it);
        find_all_solutions(solutions, e, next(it), depth+1, universe_size);
        val_subset.erase(*it);
        find_all_solutions(solutions, e, next(it), depth+1, universe_size);
    }
}