//#include <stdio.h>
#include "MyTree.h"

int main() {
    Tree diff;
    diff.readTreeFromFile("rec_test.txt");
    diff.writeTreeInFile("test.txt");
    diff.dump();
    Tree diff_tree;
    diff.fullDifferential(&diff_tree);
    diff_tree.latex(&diff, "dump_tex.tex");
    diff_tree.dump();

//    diff_tree.latex("dump_latex.tex");

//    diff.dump();

    return 0;
}