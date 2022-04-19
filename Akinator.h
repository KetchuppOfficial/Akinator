#ifndef AKINATOR_H_INCLUDED
#define AKINATOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "Stack/Stack.h"
#include "../../My_Lib/My_Lib.h"

#define ESPEAK 1

#define MAX_ANSWER 50
#define BUFF_SIZE 1024

enum Node_Types
{
    LEAF,
    QUESTION,
    ROOT
};

enum Modes
{
    LEFT,
    RIGHT,
    NO_MODE
};

enum Answers
{
    NO,
    YES,
    REPRINT
};

enum Search_Status
{
    NOT_FOUND,
    FOUND
};

struct Node
{
    enum Node_Types type;
    char value[MAX_ANSWER];
    struct Node *left_son;
    struct Node *right_son;
    struct Node *parent;
};

typedef char* value_t;

//BASIC TREE FUNCTIONS
int Tree_Constructor (struct Node *root_ptr);
int Tree_Destructor  (struct Node *root_ptr);

int Print_Tree_In_File (struct Node *node_ptr, const char *file_name);
int Print_Node         (struct Node *node_ptr, FILE *tree_file, int level);

int  Read_Tree_From_File (struct Node *node_ptr, const char *file_name);
int  Read_Node           (struct Node *node_ptr, char *buffer, const int n_symbs);
void Delete_Extra_Spaces (char *line, const int cur_pos);
int  Tree_Verificator    (const char *const buffer, const long n_symbs);

//RELATED TO TREE DUMP
int  Tree_Dump   (struct Node *root_ptr, const char *dot_file_name);
int  Node_Dump   (struct Node *node_ptr, FILE *graph_file);
int  Arrows_Dump (struct Node *node_ptr, FILE *graph_file);
int  Print_Dump  (const char *dot_file_name);

//RELATED TO USING MODES
int Choose_Mode  (struct Node *root_ptr, const char *file_name, const char *pic_file_name);
int  Get_Variant (void);
int  Speak_To_Me (const char *phrase);
void Make_Speech (int parmN, ...);

int  Basic_Mode      (struct Node *node_ptr);
int  Add_Leaf        (struct Node *node_ptr, enum Modes mode, const value_t value, enum Node_Types node_type);
void Skip_Symbs      (void);
int  Read_Answer     (void);
int  Get_Text_Answer (char *buffer);

int Definition_Mode  (struct Node *root_ptr);
int Search_For_Name  (struct Node *node_ptr, const char *name, struct Stack *stack_ptr);
int Print_Definition (struct Stack *stack_ptr, const char *name);

int Comparison_Mode  (struct Node *root_ptr);
int Print_Comparison (struct Stack *stack_1, struct Stack *stack_2, const char *name_1, const char *name_2);
int Print_Difference (struct Stack *stack_ptr, const char *name, const int cur_pos);

#endif // AKINATOR_H_INCLUDED
