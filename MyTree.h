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


    explicit Tree (size_tree_t DEFAULT_LENGHT = 50, size_tree_t DEFAULT_LENGHT_NAMES = 1000);
    ~Tree ();
    void dump ();
    void latex (const char* name);

    bool readTreeFromFile (const char name_file[]);
    bool writeTreeInFile (const char name_file[]);

    void play ();

    void fullDifferential (Tree *diff_tree);

    void definition (size_tree_t index);

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

    static void startPrintLatex (FILE* file);
    void writeTexInText (char * text, size_tree_t index);
    static void endPrintLatex (FILE* file);
    int priorityFunction (size_tree_t index);
    void writeNameInTexText (char* text, size_tree_t index);

    void deleteLastBracket (char *text);

    bool haveQuotes (char** read_now);

    void copyTree (Tree& copy);

    static bool isConstBranch (Tree* tree, size_tree_t index);

    size_tree_t diff (Tree *diff_tree,const size_tree_t index);
    size_tree_t differentialOfAddSub (bool isAdd, Tree *diff_tree, size_tree_t left, size_tree_t right);
    size_tree_t differentialOfMul (Tree *diff_tree, size_tree_t left, size_tree_t right);
    static size_tree_t differentialOfDiv(Tree *diff_tree, size_tree_t left, size_tree_t right);
    static size_tree_t differentialOfPow(Tree *diff_tree, size_tree_t left, size_tree_t right);

    size_tree_t createNewNode (Tree *diff_tree, size_tree_t type, value_t value);
    size_tree_t copyNode (Tree *diff_tree, size_tree_t index);
    size_tree_t copyBranch (Tree *diff_tree, size_tree_t index);
    bool isVariable (char* name, size_tree_t index);

    void whatItIs (char* name, size_tree_t index);
    bool isOperator (char *name, size_tree_t index);
    bool isNumber (char* name, size_tree_t index);
    void searchParents (Stack_t* stack, size_tree_t index);

    void fillingPoisonousValues();

    size_tree_t seeBranch (char* name, size_tree_t index);
    size_tree_t checkName (char* name);
    void addNewObjectInTree (char* name, size_tree_t index);

    void writeInConsole (char* text);
    bool acceptAnswer ();
    void say (int command, char* = nullptr);
    void searchPlay ();

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
