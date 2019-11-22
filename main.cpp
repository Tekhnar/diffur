#include <stdio.h>
#include "MyTree.h"

int main() {
    Tree diff;
    diff.readTreeFromFile("tree_test.txt");
    diff.writeTreeInFile("test.txt");

    Tree diff_tree;
    diff.fullDifferential(&diff_tree);

    diff_tree.latex("dump_latex.tex");

    diff_tree.dump();
//    diff.dump();

    return 0;
}