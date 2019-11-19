#include <stdio.h>
#include "MyTree.h"

int main() {
    Tree diff;
    diff.readTreeFromFile("tree.txt");
    diff.dump();

    return 0;
}