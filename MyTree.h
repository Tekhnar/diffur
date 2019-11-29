//
// Created by texnar on 11/11/2019.
//

#ifndef AKINATOR_MYtree_H
#define AKINATOR_MYtree_H

#include "my_stack.h"
#include <ctype.h>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

const char TYPE_OPERATOR = 1, TYPE_NUMBER = 2, TYPE_VARIABLE = 3;

enum {OPERATOR_ADD = 1, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV, OPERATOR_POW,
        OPERATOR_SIN, OPERATOR_COS, OPERATOR_LN};

enum {VARIABLE_X = 1, VARIABLE_Y, VARIABLE_Z, VARIABLE_T};

//#include <zconf.h>
//#include "my_stack.h"
typedef int size_tree_t;
typedef double value_t;
//static size_tree_t DEFAULT_LENGHT = 100;

struct elem {
    char *name_;
    size_tree_t type_;
    value_t value_;

    size_tree_t left_;
    size_tree_t right_;
    size_tree_t parent_;
};

struct Tree {


    explicit Tree (size_tree_t DEFAULT_LENGHT = 50, size_tree_t DEFAULT_LENGTH_NAMES = 20);
    ~Tree ();
    void dump ();
    void latex (Tree* origin,const char* name);

    bool readTreeFromFile (const char name_file[]);
    bool writeTreeInFile (const char name_file[]);

    void fullDifferential (Tree *diff_tree);

    Tree(const Tree& that) = delete;
private:
    elem *one_element;
    char *all_names;
    size_tree_t size_;
    size_tree_t length_;
    size_tree_t size_names_;
    size_tree_t length_names_;
    size_tree_t root_;
    size_tree_t free_;

    char* point_read_;


    //----recursive descent----//
    size_tree_t getG (char* text);
    size_tree_t getE ();
    size_tree_t getT ();
    size_tree_t getO ();

    size_tree_t getP ();

    size_tree_t getN ();
    size_tree_t getId ();

    //----recursive descent----//

    void recordExpression (char* text);
    void writeFunExplanations (char* text, int num_action);

    void allSimplifications (char* text);
    int optimisationOfConstants (char* text);
    int optimisationUnusedMembers(char* text);

    void optimisationOfUnusedNodeAddSub (size_tree_t index);
    void optimisationOfUnusedNodeMul (size_tree_t index);
    void optimisationOfUnusedNodePow (size_tree_t index);

    size_tree_t searchUnusedNode (Tree* tree , size_tree_t index);
    bool checkUnusedNode (Tree* tree, size_tree_t index);
    void optimisationOfUnusedNode (size_tree_t index);


    static void startPrintLatex (FILE* file);
    void writeTexInText (char * text, size_tree_t index);
    static void endPrintLatex (FILE* file);

    void priorityStrcat (char* text, const char* string, bool priority, size_tree_t index);
    int priorityFunction (size_tree_t index);
    bool comparePriority (size_tree_t first, size_tree_t second);
    void writeNameInTexText (char* text, size_tree_t index);

    bool exceptionOperators (char* text, size_tree_t index);


    void deleteLastBracket (char *text);

    bool haveQuotes (char** read_now);

    void copyTree (Tree& copy);

    static bool isConstBranch (Tree* tree, size_tree_t index);
    size_tree_t searchConstNode (Tree* tree, size_tree_t index);
    void optimisationOfConstNode (size_tree_t index);
    void clearNode (size_tree_t index);
    void clearBranch (size_tree_t index);

    size_tree_t diff (Tree *diff_tree,const size_tree_t index);
    size_tree_t differentialOfAddSub (bool isAdd, Tree *diff_tree, size_tree_t left, size_tree_t right);
    size_tree_t differentialOfMul (Tree *diff_tree, size_tree_t left, size_tree_t right);
    static size_tree_t differentialOfDiv(Tree *diff_tree, size_tree_t left, size_tree_t right);
    static size_tree_t differentialOfPow(Tree *diff_tree, size_tree_t left, size_tree_t right);


    void autoLengthIncrease(int factor = 2);
    void autoLengthNamesIncrease(int factor = 2);
    size_tree_t createNewNode (Tree *diff_tree, size_tree_t type, value_t value);
    size_tree_t copyNode (Tree *diff_tree, size_tree_t index);
    size_tree_t createNumber(Tree* tree, value_t value);

    size_tree_t copyBranch (Tree *diff_tree, size_tree_t index);
    bool isVariable (char* name, size_tree_t index);

    void whatItIs (char* name, size_tree_t index);
    bool isOperator (char *name, size_tree_t index);
    bool isNumber (char* name, size_tree_t index);
    void searchParents (Stack_t* stack, size_tree_t index);

    void fillingPoisonousValues();

    size_tree_t seeBranch (char* name, size_tree_t index);
    size_tree_t checkName (char* name);


    static void writeNameInTextFromTree (char* text, char* name);
    void writeTree (char* text, size_tree_t index);
    void loadingTree (char text[]);
    void readTextTree (char* read_now);

    void skipSymbols (char** read_now);
    size_tree_t readNewObject (char** read_now);
    void readName (char** read_now, char name[]);
    size_tree_t createNewObject (char name[], size_tree_t left, size_tree_t right, size_tree_t parent = 0);

    void writeFulTreeInFile (char* text, const char *name_file);
};

#endif //AKINATOR_MYtree_H
