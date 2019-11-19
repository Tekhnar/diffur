//
// Created by texnar on 11/11/2019.
//

#ifndef AKINATOR_MYtree_H
#define AKINATOR_MYtree_H

#include "my_stack.h"
#include <ctype.h>

const char TYPE_OPERATOR = 1, TYPE_NUMBER = 2, TYPE_VARIABLE = 3;

enum {OPERATOR_ADD = 1, OPERATOR_SUB, OPERATOR_MUL, OPERATOR_DIV, OPERATOR_POW};

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

    bool readTreeFromFile (const char name_file[]);
    bool writeTreeInFile (const char name_file[]);

    void play ();

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

    bool haveQuotes (char** read_now);

    void whatItIs (char* name, size_tree_t index);
    bool isVariable (char* name, size_tree_t index);
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

    void writeTree (char* text, size_tree_t index);
    void loadingTree (char text[]);
    void readTextTree (char* read_now);

    void skipSymbols (char** read_now);
    size_tree_t readNewObject (char** read_now);
    void readName (char** read_now, char name[]);
    size_tree_t creatNewObject (char name[], size_tree_t left, size_tree_t right, size_tree_t parent = 0);

    void writeFulTreeInFile (char* text, const char *name_file);
};

#endif //AKINATOR_MYtree_H
