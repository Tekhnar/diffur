//
// Created by texnar on 11/11/2019.
//
#include "MyTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LEVEL_VERIFIC 1
#include "my_stack.h"

Tree::Tree(size_tree_t DEFAULT_LENGTH, size_tree_t DEFAULT_LENGHT_NAMES) :
free_ (0),
one_element ((elem*) calloc(DEFAULT_LENGTH, sizeof(one_element[0]) )),
all_names ((char *) calloc(DEFAULT_LENGHT_NAMES, sizeof(char))),
size_ (0),
size_names_ (0),
root_ (0),
length_names_ (DEFAULT_LENGHT_NAMES),
length_ (DEFAULT_LENGTH)
{
    fillingPoisonousValues();
}

void Tree::fillingPoisonousValues()
{
    for (size_tree_t i = size_ + 1; i < length_; i++){
//        data_[i] = 0;
        one_element[i].right_ = i + 1;
        one_element[i].parent_ = -1;
        one_element[i].type_ = -1;
        one_element[i].value_ = -1;
        one_element[i].name_ = nullptr;
    }
    one_element[length_ - 1].right_ = 0;
    free_ = size_ + 1;
}

Tree::~Tree()
{
    free(one_element);
    free(all_names);
    size_ = 0;
    length_ = 0;
    size_names_ = 0;
    root_ = 0;
    free_ = 0;
}

long ItLength(FILE* file)
{
    assert(file != nullptr);

    fseek(file, 0, SEEK_END);
    long result = ftell(file);
    fseek(file, 0, SEEK_SET);

    return result;
}

bool Tree::readTreeFromFile(const char* name_file) {
    FILE* file = fopen(name_file, "r+");
    long length_of_file = ItLength(file) + 1;
    char* text = (char*) calloc(length_of_file, sizeof(char));
    fread(text, sizeof(char), length_of_file - 1, file);
    text[length_of_file - 1] = '\0';

    loadingTree(text);
}

void Tree::loadingTree(char* text)
{
    assert(text != nullptr);

    if (text[0] == 0xEF && text[1] == 0xBB && text[2] == 0xBF)
        text += 3;
    skipSymbols(&text);
    readTextTree(text);
}

void Tree::readTextTree(char *read_now)
{
    if (*read_now == '(')
        readNewObject(&read_now);
    else
        printf("Error in read!\n");
}

void Tree::skipSymbols(char **read_now)
{
    assert(read_now != nullptr);
    while (isspace(**read_now))
        (*read_now)++;

//    while (**read_now != ')' && **read_now != '('
//            && **read_now != '\"' && **read_now != '\0') (*read_now)++;
}

size_tree_t Tree::readNewObject(char **read_now)
{
    (*read_now)++;
    size_tree_t left_temp = 0, right_temp = 0;

//-- read left branch --//
    if (**read_now == '(') {
        left_temp = readNewObject(read_now);

        skipSymbols(read_now);
        assert(**read_now == ')');
        (*read_now)++;
    }
//    if (**read_now == '(')
//-- read left branch --//

//    skipSymbols(read_now);

    char name[100] = {};
    readName(read_now, name);
    skipSymbols(read_now);


//-- read right branch --//
    if (**read_now == '(') {
        right_temp = readNewObject(read_now);

        skipSymbols(read_now);
        assert(**read_now == ')');
        (*read_now)++;
    }
//-- read right branch --//

    skipSymbols(read_now);
    if (**read_now == ')') {
        root_ = creatNewObject(name, left_temp, right_temp);
        return root_;
    }

    if (**read_now != ')' && **read_now != '\n'){
        printf("Error, don't found end of object!\n");
        abort();
    }
}

void Tree::readName(char **read_now, char *name)
{
//    (*read_now)++;
    skipSymbols(read_now);

    int read_symbol = 0;
    char num_read = 0;

    if (haveQuotes(read_now))
        num_read = (char) sscanf(*read_now, "\"%[^\"]\"%n", name, &read_symbol);
    else
        num_read = (char) sscanf(*read_now, " %[^() \n\t]%n", name, &read_symbol);

    if (num_read == 0) printf("Can't read name\n"); // should be test this function
    *read_now += read_symbol;

    skipSymbols(read_now);
}

size_tree_t Tree::creatNewObject(char name[], size_tree_t left, size_tree_t right, size_tree_t parent)
{
    size_tree_t new_index = free_;
    free_ = one_element[free_].right_;

    whatItIs(name, new_index);

    one_element[new_index].right_ = right;
    one_element[new_index].left_ = left;
    one_element[new_index].name_ = all_names + size_names_;

    if (one_element[new_index].left_)
        one_element[one_element[new_index].left_].parent_ = new_index;
    if (one_element[new_index].right_)
        one_element[one_element[new_index].right_].parent_ = new_index;

    if (one_element[new_index].parent_ == -1)
        one_element[new_index].parent_ = parent;
//    if (parent)
    int length = (int) (strlen(name) + 1);
    strcpy((all_names + size_names_), name);
    size_names_ += length;

   /* if (size_ == 0) {
        one_element[new_index].parent_ = 0;
    }*/
    size_++;
//    root_ = new_index;
    return new_index;
}

bool Tree::writeTreeInFile(const char *name_file)
{
    char *text = (char*) calloc(50000, sizeof(char));
    writeTree(text, root_);
    writeFulTreeInFile(text, name_file);
}

void Tree::writeFulTreeInFile (char* text, const char *name_file)
{
    FILE* file = fopen(name_file, "w");
    int length = strlen(text);
    fwrite(text, sizeof(char), length, file);
    fclose(file);
}

void Tree::writeTree (char* text, size_tree_t index)
{
//    char name[1000] = "{\"";
    strcat(text, "{\"");
    strcat(text, one_element[index].name_);
    strcat(text, "\"");

    if (one_element[index].left_ == 0 || one_element[index].right_ == 0){
        strcat(text, "{\"");
        strcat(text, one_element[index].name_);
        strcat(text, "\"}");
    }
    else {
        writeTree(text, one_element[index].left_);

        strcat(text, "\"");
        strcat(text, one_element[index].name_);
        strcat(text, "\"");

        writeTree(text, one_element[index].right_);
        strcat(text, "}");
    }
}

void Tree::dump()
{
    FILE* file = fopen("text_picture.dot", "wb");
    fprintf(file, "digraph structs {\n");
    fprintf(file, "rankdir=HR;\n");


    for (size_tree_t i = 1; i < length_; i++){
        if (one_element[i].parent_ != -1){
            fprintf(file, "Index%d [shape=record, label=\" <left> Yes: %d | '%s' | {Type: %d | Value: %lg} | <right> No: %d \",",
                i, one_element[i].left_, one_element[i].name_, one_element[i].type_,one_element[i].value_, one_element[i].right_);
            switch (one_element[i].type_){
                case TYPE_OPERATOR:
                    fprintf(file, "style=\"filled\", fillcolor=\"lightgrey\" ");
                    break;
                case TYPE_NUMBER:
                    fprintf(file, "style=\"filled\", fillcolor=\"yellow\" ");
                    break;
                case TYPE_VARIABLE:
                    fprintf(file, "style=\"filled\", fillcolor=\"lightblue\" ");
                    break;
                default:
                    break;
            }
            fprintf(file, "];\n");

        }
    }
    fprintf(file, "System [shape=record, label=\" <root> Root\" ];\n");
//    fprintf(file, "System [shape=record, label=\" <head> Head | <tail> Tail | <free> Free\" ];\n");
//    fprintf(file, "System: <head> -> Index%d : <index>;\n", head_);
//    fprintf(file, "System: <tail> -> Index%d : <index>;\n", tail_);
//    fprintf(file, "System: <free> -> Index%d : <index>;\n", free_);
    fprintf(file, "System: <root> -> Index%d;\n", root_);

    for (size_tree_t i = 1; i < length_; i++) {
        if (one_element[i].parent_ != -1) {
            if (one_element[i].left_ != 0)
                fprintf(file, "Index%d: <left> -> Index%d ;\n", i, one_element[i].left_);
            if (one_element[i].right_ != 0)
                fprintf(file, "Index%d: <right> -> Index%d ;\n", i, one_element[i].right_);
//            fprintf(file, "Index%d: <parent> -> Index%d ;\n", i, one_element[i].parent_);
        }
    }

 /*   size_list_t index = head_;
    for (size_list_t i = 1; i < length_; i++) {
        fprintf(file, "Index%d: <next> -> Index%d : <index>;\n", i, next_[i]);
        index = next_[index];
    }

    index = tail_;
    for (size_list_t i = 1; i < length_; i++) {
        if (prev_[i] != -1){
            fprintf(file, "Index%d: <prev> -> Index%d : <index>;\n", i, prev_[i]);
            index = prev_[index];
        }
    }*/



    fprintf(file, "}\n");
    fclose(file);
    system("dot text_picture.dot -T png -o test.png");
}

void Tree::play()
{

    writeInConsole((char*) "Введите команду для игры\n");
    char command[30] = {};
    scanf("%s", command);

    if (!strcmp(command, "игра"))
        searchPlay();
    if (!strcmp(command, "найти")) {
        writeInConsole((char*) "Что надо найти?\n");
        char read_name[100] = {};
        scanf("%s", read_name);
        writeInConsole((char*) "Индекс\n");
        size_tree_t index = checkName(read_name);
        if (index)
            printf("%d\n", checkName(read_name));
        else
            writeInConsole((char*) "Нет такого имени!\n");
    }
    if (!strcmp(command, "определение")) {
        writeInConsole((char*) "Что надо найти?\n");
        char read_name[100] = {};
        scanf("%s", read_name);
//        writeInConsole("Индекс\n");
//        printf("%d\n", checkName(read_name));
        size_tree_t index = checkName(read_name);
        if (index)
            definition(index);
        else
            writeInConsole((char*) "Нет такого имени!\n");
    }
}

void Tree::searchPlay()
{
    size_tree_t index = root_;
    while (one_element[index].left_ != 0 && one_element[index].right_ != 0) {
//        printf("%s\n", one_element[index].name_);
        writeInConsole(one_element[index].name_);
        bool answer = acceptAnswer();
        if (answer) index = one_element[index].left_;
        else index = one_element[index].right_;
    }

    say(2, one_element[index].name_);
    if (!acceptAnswer()){
        say(3);
        char new_name[100] = {};
        scanf(" %[^\n]", new_name);

        if (!checkName(new_name))
            addNewObjectInTree(new_name, index);
        else {
            say(5, new_name);
        }
    }
}

void Tree::say(int command, char* text)
{
//    assert(text);
    char full_text[200] = {};
//    char* full_text = nullptr;
    switch (command){
        case 1: {
            strcat(full_text, "Неизвестная команда, повторите ввод\n");
            writeInConsole(full_text);
            break;
        }
        case 2: {
            strcat(full_text,"Это ");
            strcat(full_text, text);
            strcat(full_text, "?\n");
            writeInConsole(full_text);
            break;
        }
        case 3: {
            strcat(full_text,"Тогда что это?\n");
            writeInConsole(full_text);
            break;
        }
        case 4: {
            strcat(full_text,"Чем он отличается от ");
            strcat(full_text, text);
            strcat(full_text, "?\n");
            writeInConsole(full_text);
            break;
        }
        case 5: {
            strcat(full_text,"Я уже знаю ");
            strcat(full_text, text);
            strcat(full_text, "\n");
            writeInConsole(full_text);
            break;
        }
        case 6: {
            strcat(full_text, text);
            strcat(full_text, "\b ");
            writeInConsole(full_text);
            break;
        }
        case 7: {
            strcat(full_text, "не ");
            strcat(full_text, text);
            strcat(full_text, "\b ");
            writeInConsole(full_text);
            break;
        }
        case 8: {
            strcat(full_text, text);
            strcat(full_text, " ");
            writeInConsole(full_text);
            break;
        }
        default: {
            writeInConsole((char*)"Неизвестная команда печати\n");
        }
    }
}

void Tree::writeInConsole(char *text)
{
    assert(text);
    printf("%s", text);

//    char full_text[200] = {};
//    strcat(full_text, "espeak -s 200 \"");
//    strcat(full_text, text);
//    strcat(full_text, "\"");
//    system(full_text);
}

bool Tree::acceptAnswer()
{
    while (true) {
        char answer[20] = {};
        scanf("%s", answer);
        if (!strcmp("да", answer)) {
            return true;
        } else if (!strcmp("нет", answer)) {
            return false;
        } else {
            say(1);
        }
    }
}
size_tree_t Tree::checkName(char *name)
{
    assert(name);
    return seeBranch(name, root_);
}
/*
bool Tree::seeBranch(char* name, size_tree_t index)
{
    if (!strcmp(one_element[index].name_, name)) return true;

    if (one_element[index].left_ != 0)
        if (seeBranch(name, one_element[index].left_)) return true;
    if (one_element[index].right_ != 0)
        if (seeBranch(name, one_element[index].right_)) return true;

    return false;
}*/

size_tree_t Tree::seeBranch(char* name, size_tree_t index)
{
    if (!strcmp(one_element[index].name_, name)) return index;

    size_tree_t new_index = 0;

    if (one_element[index].left_ != 0){
        new_index = seeBranch(name, one_element[index].left_);
        if (new_index) return new_index;
    }
    if (one_element[index].right_ != 0){
        new_index = seeBranch(name, one_element[index].right_);
        if (new_index) return new_index;
    }
    return 0;
}

void Tree::addNewObjectInTree(char *name, size_tree_t index)
{
    say(4, one_element[index].name_);
    char new_name[100] = {};
    scanf(" %[^\n]", new_name);
    strcat(new_name, "?");

    size_tree_t temp_index = one_element[index].parent_;
    size_tree_t new_element = creatNewObject(name, 0, 0);
    size_tree_t new_question = creatNewObject(new_name, new_element, index,
            one_element[index].parent_);

    if (one_element[temp_index].left_ == index)
        one_element[temp_index].left_ = new_question;
    if (one_element[temp_index].right_ == index)
        one_element[temp_index].right_ = new_question;
}

void Tree::definition(size_tree_t index)
{
    Stack_t stack = {};
    StackInit(&stack);

    searchParents(&stack, index);

    say(8, one_element[index].name_);

    size_tree_t new_index = 0, last_index = 0;
    StackPop(&stack, &new_index);
    last_index = new_index;
    while (true){
        StackPop(&stack, &new_index);
        if (new_index == one_element[last_index].left_ || new_index == root_) {
            say(6, one_element[last_index].name_);
        }
        else {
            say(7,one_element[last_index].name_);
        }
        last_index = new_index;
        if (index == new_index) break;
    }
//    writeInConsole(one_element[index].name_);
}

void Tree::searchParents(Stack_t *stack, size_tree_t index)
{
    while (true){
        StackPush(stack, index);
        if (index == root_) break;
        index = one_element[index].parent_;
    }
}

void Tree::whatItIs(char *name, size_tree_t index)
{
   isOperator(name, index);
   isNumber(name, index);
   isVariable(name, index);
}

bool Tree::isOperator (char *name, size_tree_t index)
{
#define OPER(str, num) if (!strcmp( #str , name)) {\
    one_element[index].type_ = TYPE_OPERATOR;\
    one_element[index].value_ = num;\
    return true;\
}

    OPER(+ , OPERATOR_ADD)
    OPER(- , OPERATOR_SUB)
    OPER(* , OPERATOR_MUL)
    OPER(/ , OPERATOR_DIV)
    OPER(^ , OPERATOR_POW)

/*
    if (!strcmp("+", name)) {
        one_element[index].type_ = TYPE_OPERATOR;
        one_element[index].type_ = OPERATOR_ADD;
    }
    if (!strcmp("-", name)) {
        one_element[index].type_ = TYPE_OPERATOR;
        one_element[index].type_ = OPERATOR_SUB;
    }
    if (!strcmp("*", name)) {
        one_element[index].type_ = TYPE_OPERATOR;
        one_element[index].type_ = OPERATOR_MUL;
    }
    if (!strcmp("/", name)) {
        one_element[index].type_ = TYPE_OPERATOR;
        one_element[index].type_ = OPERATOR_DIV;
    }
*/
#undef OPER
}

bool Tree::haveQuotes(char **read_now) {
    char symbol[3] = {};
    if (sscanf(*read_now, "%[\"]", symbol))
        return true;
    return false;
}

bool Tree::isNumber(char *name, size_tree_t index) {
    value_t number = 0;
    int num_read = 0;
    sscanf(name, "%lg%n", &number, &num_read);
    if (strlen(name) == num_read){
        one_element[index].type_ = TYPE_NUMBER;
        one_element[index].value_ = number;
        return true;
    }
    return false;
}

bool Tree::isVariable(char *name, size_tree_t index) {
#define VAR(str, num) if (!strcmp( #str , name)) {\
    one_element[index].type_ = TYPE_VARIABLE;\
    one_element[index].value_ = num;\
    return true;\
}

    VAR(x , 1)
    VAR(y , 2)
    VAR(z , 3)
    VAR(t , 4)

#undef VAR
    return false;
}
