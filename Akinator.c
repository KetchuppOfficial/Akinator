#include "Akinator.h"
#include "../My_Lib/My_Lib.h"

const char *GRAPH_FILE = "Graph.dot";

const int POISON = 2004;
const void *const DEAD_PTR = NULL;

//RELATED TO CONSTRUCTING AND DESTRUCTING A TREE
//************************************************************************
int Tree_Constructor (struct Node *root_ptr)
{
    MY_ASSERT (root_ptr, "struct Node *root_ptr", NULL_PTR, ERROR);

    root_ptr->type = ROOT;
    memmove (root_ptr->value, "Unknown who", strlen ("Unknown who"));
    root_ptr->left_son  = NULL;
    root_ptr->right_son = NULL;
    root_ptr->parent    = NULL;

    return NO_ERRORS;
}

int Tree_Destructor (struct Node *node_ptr)
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    if (node_ptr->left_son)
        Tree_Destructor (node_ptr->left_son);

    if (node_ptr->right_son)
        Tree_Destructor (node_ptr->right_son);

    node_ptr->type = (enum Node_Types)POISON;

    node_ptr->left_son  = (struct Node *)DEAD_PTR;
    node_ptr->right_son = (struct Node *)DEAD_PTR;

    if (node_ptr->parent)
        free (node_ptr);

    return NO_ERRORS;
}
//************************************************************************

//RELATED TO READING TREE FROM FILE
//************************************************************************
int Read_Tree_From_File (struct Node *node_ptr, const char *file_name)
{
    MY_ASSERT (node_ptr,  "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (file_name, "const char *file_name", NULL_PTR, ERROR);

    FILE *tree_file = Open_File (file_name, "rb");

    long n_symbs = Define_File_Size (tree_file);
    char *buffer = Make_Buffer (tree_file, n_symbs);

    if (Tree_Verificator (buffer, n_symbs) == ERROR)
        return ERROR;

    Read_Node (node_ptr, buffer, n_symbs);

    free (buffer);

    Close_File (tree_file, file_name);

    return NO_ERRORS;
}

int Read_Node (struct Node *node_ptr, char *buffer, const int n_symbs)
{
    MY_ASSERT (node_ptr,    "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (buffer,      "char *buffer",          NULL_PTR, ERROR);
    MY_ASSERT (n_symbs > 0, "const long n_symbs",    POS_VAL,  ERROR);

    static bool root_found = false;
    static int symb_i = 0;

    int node_i = 0;

    for (; symb_i < n_symbs; symb_i++)
    {

        if (buffer[symb_i] == '{')
        {
            symb_i++;
            while (isspace (buffer[symb_i]))
                symb_i++;

            char line[MAX_ANSWER] = "";

            int i = 0;
            while (buffer[symb_i] != '}' && buffer[symb_i] != '{' && buffer[symb_i] != '\r' && buffer[symb_i] != '\n')
                line[i++] = buffer[symb_i++];

            Delete_Extra_Spaces (line, i);

            while (buffer[symb_i] != '}' && buffer[symb_i] != '{')
                symb_i++;

            if (!root_found)
            {
                for (int i = 0; i < MAX_ANSWER; i++)
                    node_ptr->value[i] = '\0';

                memmove (node_ptr->value, line, strlen (line));
                root_found = true;
                Read_Node (node_ptr, buffer, n_symbs);
                break;
            }
            else
            {
                enum Node_Types node_type = QUESTION;

                if (buffer[symb_i] == '{')
                    node_type = QUESTION;
                else
                {
                    node_type = LEAF;
                    symb_i++;
                }

                switch (node_i)
                {
                    case 0:
                        Add_Leaf (node_ptr, LEFT, line, node_type);
                        if (node_type == QUESTION)
                            Read_Node (node_ptr->left_son, buffer, n_symbs);

                        node_i++;
                        continue;
                    case 1:
                        Add_Leaf (node_ptr, RIGHT, line, node_type);
                        if (node_type == QUESTION)
                            Read_Node (node_ptr->right_son, buffer, n_symbs);

                        break;
                    default:
                        MY_ASSERT (false, "int node_i", UNEXP_VAL, ERROR);
                }
                break;
           }
        }
    }

    return NO_ERRORS;
}

void Delete_Extra_Spaces (char *line, const int cur_pos)
{
    int shift = 0;
    while (isspace (line[cur_pos - shift]) || line[cur_pos - shift] == '\0')
    {
        line[cur_pos - shift] = '\0';
        shift++;
    }
}

#define Brace_Check                         \
do                                          \
{                                           \
    if (old_l_counter < l_counter)          \
    {                                       \
        old_l_counter = l_counter;          \
        if (letters == 0)                   \
            return ERROR;                   \
    }                                       \
    else if (old_r_counter < r_counter)     \
    {                                       \
        old_r_counter = r_counter;          \
        if (letters != 0)                   \
            return ERROR;                   \
    }                                       \
}                                           \
while (0)

int Tree_Verificator (const char *const buffer, const long n_symbs)
{
    MY_ASSERT (buffer,      "const char *const buffer", NULL_PTR, ERROR);
    MY_ASSERT (n_symbs > 0, "const long n_symbs",       POS_VAL,  ERROR);

    int l_counter     = 0, r_counter     = 0;
    int old_l_counter = 0, old_r_counter = 0;
    long symb_i = 0;

    while (symb_i < n_symbs)
    {
        int letters = 0;

        if (isalpha (buffer[symb_i]) && l_counter == 0)
            return ERROR;

        if (buffer[symb_i] == '{')
        {
            l_counter++;
            symb_i++;
        }
        if (buffer[symb_i] == '}')
        {
            r_counter++;
            symb_i++;
        }

        if (r_counter > l_counter)
            return ERROR;

        while (symb_i < n_symbs && buffer[symb_i] != '{' && buffer[symb_i] != '}')
        {
            if (l_counter == r_counter && l_counter > 0 && !isspace (buffer[symb_i]))
                return ERROR;

            if (!isspace (buffer[symb_i]))
                letters++;
            symb_i++;
        }

        if (symb_i < n_symbs && buffer[symb_i] == '{')
            Brace_Check
        else if (symb_i < n_symbs && buffer[symb_i] == '}')
            Brace_Check
    }

    if (l_counter != r_counter)
        return ERROR;

    return NO_ERRORS;
}

//************************************************************************

//RELATED TO PRINTING TREE IN FILE
//************************************************************************
int Print_Tree_In_File (struct Node *node_ptr, const char *file_name)
{
    MY_ASSERT (node_ptr,  "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (file_name, "const char *file_name", NULL_PTR, ERROR);

    FILE *tree_file = Open_File (file_name, "wb");

    Print_Node (node_ptr, tree_file, 0);

    Close_File (tree_file, file_name);

    return NO_ERRORS;
}

#define Print_Shift                     \
do                                      \
{                                       \
    for (int i = 0; i < level; i++)     \
            fprintf (tree_file, "\t");  \
}                                       \
while (0)

int Print_Node (struct Node *node_ptr, FILE *tree_file, int level)
{
    MY_ASSERT (node_ptr,   "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (tree_file,  "FILE *tree_file",       NULL_PTR, ERROR);

    if (node_ptr->type == LEAF)
    {
        Print_Shift;
        fprintf (tree_file, "{ %s }\n", node_ptr->value);
    }
    else
    {
        Print_Shift;
        fprintf (tree_file, "{\n");
        Print_Shift;
        fprintf (tree_file, "%s\n", node_ptr->value);
    }

    if (node_ptr->left_son)
        Print_Node (node_ptr->left_son, tree_file, level + 1);

    if (node_ptr->right_son)
        Print_Node (node_ptr->right_son, tree_file, level + 1);

    if (node_ptr->type != LEAF)
    {
        Print_Shift;
        fprintf (tree_file, "}\n");
    }

    return NO_ERRORS;
}
//************************************************************************

int Choose_Mode (struct Node *root_ptr, const char *file_name, const char *dot_file_name)
{
    MY_ASSERT (root_ptr,  "struct Node *root_ptr",     NULL_PTR, ERROR);
    MY_ASSERT (file_name, "const char *file_name",     NULL_PTR, ERROR);
    MY_ASSERT (file_name, "const char *dot_file_name", NULL_PTR, ERROR);

    Speak_To_Me ("Choose one mode (write its number):\n");
    Speak_To_Me ("1) Akinator mode;\n");
    Speak_To_Me ("2) Definition mode;\n");
    Speak_To_Me ("3) Comparison mode\n");

    int choice = Get_Variant ();

    switch (choice)
    {
        case 1:
            if (Basic_Mode (root_ptr) == REPRINT)
            {
                Print_Tree_In_File (root_ptr, file_name);
                Tree_Dump (root_ptr, dot_file_name);
            }
            break;
        case 2:
            Definition_Mode (root_ptr);
            break;
        case 3:
            Comparison_Mode (root_ptr);
            break;
        default:
            MY_ASSERT (false, "int choice", UNEXP_VAL, ERROR);
    }

    return NO_ERRORS;
}

int Get_Variant (void)
{
    int num = 0;

    for (;;)
    {
        if (scanf ("%d", &num) != 1 || !(1 <= num && num <= 3))
            Skip_Symbs ();
        else
        {
            getchar ();
            break;
        }
    }

    return num;
}

//RELATED TO BASIC MODE
//************************************************************************
int Basic_Mode (struct Node *node_ptr)
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    int tree_status = 0;

    if (node_ptr->type != LEAF)
    {
        Make_Speech (2, node_ptr->value, "?");

        if (Read_Answer () == YES)
            tree_status = Basic_Mode (node_ptr->left_son);
        else
            tree_status = Basic_Mode (node_ptr->right_son);

        if (tree_status == REPRINT)
            return REPRINT;
    }
    else
    {

        Make_Speech (2, "The right answer is ", node_ptr->value);

        Speak_To_Me ("Am I right?\n");

        if (Read_Answer () == YES)
            Speak_To_Me ("I know what you are thinking about!!! Ha-ha, ha-ha\n");
        else
        {
            char old_variant[MAX_ANSWER] = "";
            memmove (old_variant, node_ptr->value, MAX_ANSWER);

            Speak_To_Me ("What's the right answer?\n");
            char right_ans[MAX_ANSWER] = "";
            Get_Text_Answer (right_ans);

            Make_Speech (5, "How does ", right_ans, " differs from ", old_variant, "?");

            char difference[MAX_ANSWER] = "";
            Get_Text_Answer (difference);

            node_ptr->type = QUESTION;

            memmove (node_ptr->value, difference, MAX_ANSWER);

            Add_Leaf (node_ptr, LEFT,  right_ans,   LEAF);
            Add_Leaf (node_ptr, RIGHT, old_variant, LEAF);

            return REPRINT;
        }
    }

    return NO_ERRORS;
}

int Get_Text_Answer (char *buffer)
{
    MY_ASSERT (buffer, "char *buffer", NULL_PTR, ERROR);

    fgets (buffer, MAX_ANSWER, stdin);

    int i = 0;
    while (buffer[i] != '\n' && buffer[i] != '\0')
        i++;
    if (buffer[i] == '\n')
        buffer[i] = '\0';
    else
        while (getchar () != '\n') {;}

    return NO_ERRORS;
}

#define Initialize_Node(son_ptr)                                    \
do                                                                  \
{                                                                   \
    son_ptr = (struct Node *)calloc (1, sizeof (struct Node));      \
                                                                    \
    son_ptr->type = node_type;                                      \
    memmove (son_ptr->value, value, strlen (value));                \
    son_ptr->left_son = NULL;                                       \
    son_ptr->right_son = NULL;                                      \
    son_ptr->parent = node_ptr;                                     \
}                                                                   \
while (0)
int Add_Leaf (struct Node *node_ptr, enum Modes mode, const value_t value, enum Node_Types node_type)
{
    MY_ASSERT (node_ptr, "struct Node *node_ptr", NULL_PTR, ERROR);

    switch (mode)
    {
        case LEFT:
            if (!node_ptr->left_son)
                Initialize_Node (node_ptr->left_son);
            break;
        case RIGHT:
            if (!node_ptr->right_son)
                Initialize_Node (node_ptr->right_son);
            break;
        default:
            MY_ASSERT (false, "Modes mode", UNEXP_VAL, ERROR);
    }

    return NO_ERRORS;
}

int Read_Answer (void)
{
    char ans = 0;
    enum Answers yes_no = NO;

    for (;;)
    {
        for (;;)
        {
            ans = getchar ();
            if (ans == 'y')
            {
                yes_no = YES;
                break;
            }
            else if (ans == 'n')
            {
                yes_no = NO;
                break;
            }

            else
                Skip_Symbs ();
        }
        char after = getchar ();
        if (after == '\n')
            return yes_no;
        else
            Skip_Symbs ();
    }
}

void Skip_Symbs (void)
{
    Speak_To_Me ("There is no such variant. Try again\n");
    while (getchar () != '\n') {;}
}
//************************************************************************

//RELATED TO DEFINITION MODE
//************************************************************************
int Definition_Mode (struct Node *root_ptr)
{
    MY_ASSERT (root_ptr, "struct Node *root_ptr", NULL_PTR, ERROR);

    struct Stack stack = {};

    Stack_Ctor (&stack);

    Speak_To_Me ("Write the name of a person you want to get definition of\n");
    char name[MAX_ANSWER] = "";
    Get_Text_Answer (name);

    if (Search_For_Name (root_ptr, name, &stack) == FOUND)
        Print_Definition (&stack, name);
    else
        Speak_To_Me ("I'm sorry. I don't have this person in my database :( \n");

    Stack_Dtor (&stack);

    return NO_ERRORS;
}

#define Search(side)                                                            \
do                                                                              \
{                                                                               \
    if (node_ptr->side##_son)                                                   \
    {                                                                           \
        if (Search_For_Name (node_ptr->side##_son, name, stack_ptr) == FOUND)   \
            return FOUND;                                                       \
        else                                                                    \
            Stack_Pop (stack_ptr, NULL);                                        \
    }                                                                           \
}                                                                               \
while (0)

int Search_For_Name (struct Node *node_ptr, const char *name, struct Stack *stack_ptr)
{
    MY_ASSERT (node_ptr,  "struct Node *node_ptr",   NULL_PTR, ERROR);
    MY_ASSERT (name,      "const char *name",        NULL_PTR, ERROR);
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_PTR, ERROR);

    if (!strncmp (node_ptr->value, name, MAX_ANSWER))
    {
        Stack_Push (stack_ptr, node_ptr);
        return FOUND;
    }
    else
    {
        Stack_Push (stack_ptr, node_ptr);

        Search (left);
        Search (right);
    }

    return NOT_FOUND;
};

#define Universal_Switch(condition, if_yes, if_no)  \
do                                                  \
{                                                   \
    switch (condition)                              \
    {                                               \
        case true:                                  \
            if_yes;                                 \
            break;                                  \
        case false:                                 \
            if_no;                                  \
            break;                                  \
    }                                               \
}                                                   \
while (0)

int Print_Definition (struct Stack *stack_ptr, const char *name)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_PTR, ERROR);
    MY_ASSERT (name,      "const char *name",        NULL_PTR, ERROR);

    struct Node *node_ptr = NULL, *next_node_ptr = NULL;

    Make_Speech (2, name, ": ");

    for (int i = 0; i < stack_ptr->size - 1; i++)
    {
        node_ptr      = (struct Node *)(stack_ptr->data[i]);
        next_node_ptr = (struct Node *)(stack_ptr->data[i + 1]);

        Universal_Switch (next_node_ptr == node_ptr->right_son,
                          Make_Speech (2, "NOT ", node_ptr->value),
                          Make_Speech (1, node_ptr->value));
    }

    return NO_ERRORS;
}
//************************************************************************

//RELATED TO COMPARISON MODE
//************************************************************************
int Comparison_Mode (struct Node *root_ptr)
{
    MY_ASSERT (root_ptr, "struct Node *root_ptr", NULL_PTR, ERROR);

    struct Stack stack_1 = {};
    struct Stack stack_2 = {};

    Stack_Ctor (&stack_1);
    Stack_Ctor (&stack_2);

    Speak_To_Me ("Write names of two people you want to compare\n");

    printf ("Person 1: ");
    char name_1[MAX_ANSWER] = "";
    Get_Text_Answer (name_1);

    printf ("Person 2: ");
    char name_2[MAX_ANSWER] = "";
    Get_Text_Answer (name_2);

    int status_1 = Search_For_Name (root_ptr, name_1, &stack_1);
    int status_2 = Search_For_Name (root_ptr, name_2, &stack_2);

    if (status_1 == FOUND && status_2 == FOUND)
        Print_Comparison (&stack_1, &stack_2, name_1, name_2);
    else
        Speak_To_Me ("I'm sorry. One or two of persons you've written isn't presented in my database :( \n");

    Stack_Dtor (&stack_1);
    Stack_Dtor (&stack_2);

    return NO_ERRORS;
}

int Print_Comparison (struct Stack *stack_1, struct Stack *stack_2, const char *name_1, const char *name_2)
{
    MY_ASSERT (stack_1, "struct Stack *stack_1", NULL_PTR, ERROR);
    MY_ASSERT (name_1,  "const char *name_1",    NULL_PTR, ERROR);
    MY_ASSERT (stack_2, "struct Stack *stack_2", NULL_PTR, ERROR);
    MY_ASSERT (name_2,  "const char *name_2",    NULL_PTR, ERROR);

    long max_size = (stack_1->size < stack_2->size) ? stack_2->size : stack_1->size;

    struct Node *next_node_1 = NULL, *next_node_2 = NULL;
    struct Node *node_ptr = NULL;

    for (int i = 0; i < max_size - 1; i++)
    {
        next_node_1 = (struct Node *)(stack_1->data[i + 1]);
        next_node_2 = (struct Node *)(stack_2->data[i + 1]);
        node_ptr    = (struct Node *)(stack_1->data[i]);

        if (!strcmp (next_node_1->value, next_node_2->value))
        {
            Universal_Switch (next_node_1 == node_ptr->right_son,
                              Make_Speech (2, "BOTH NOT ", node_ptr->value),
                              Make_Speech (2, "BOTH ",     node_ptr->value));
        }
        else
        {
            Print_Difference (stack_1, name_1, i);
            Print_Difference (stack_2, name_2, i);

            break;
        }
    }

    return NO_ERRORS;
}

int Print_Difference (struct Stack *stack_ptr, const char *name, const int cur_pos)
{
    MY_ASSERT (stack_ptr, "struct Stack *stack_ptr", NULL_PTR, ERROR);
    MY_ASSERT (name,      "const char *name",        NULL_PTR, ERROR);

    Make_Speech (2, name, ": ");

    for (int i = cur_pos; i < stack_ptr->size - 1; i++)
    {
        Universal_Switch ((struct Node *)(stack_ptr->data[i + 1]) == ((struct Node *)(stack_ptr->data[i]))->right_son,
                          Make_Speech (2, "NOT ", ((struct Node *)(stack_ptr->data[i]))->value),
                          Make_Speech (1, ((struct Node *)(stack_ptr->data[i]))->value));
    }


    return ERROR;
}
//************************************************************************

//RELATED TO GRAPHIC DUMP
//************************************************************************
int Tree_Dump (struct Node *root_ptr, const char *dot_file_name)
{
    MY_ASSERT (root_ptr,      "struct Node *root_ptr",     NULL_PTR, ERROR);
    MY_ASSERT (dot_file_name, "const char *dot_file_name", NULL_PTR, ERROR);

    FILE *graph_file = Open_File (dot_file_name, "wb");

    fprintf (graph_file, "digraph List\n"
                         "{\n"
                         "\trankdir = TB;\n"
                         "\tnode [style = rounded];\n\n");

    Node_Dump (root_ptr, graph_file);

    Arrows_Dump (root_ptr, graph_file);

    fprintf (graph_file, "}\n");

    Close_File (graph_file, dot_file_name);

    Print_Dump (dot_file_name);

    return NO_ERRORS;
}

int Node_Dump (struct Node *node_ptr, FILE *graph_file)
{
    MY_ASSERT (node_ptr,   "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (graph_file, "FILE *graph_file",      NULL_PTR, ERROR);

    if (node_ptr->type == LEAF)
        fprintf (graph_file, "\tnode%p\t[style = filled, fillcolor = green, label = \"%s\"];\n\n", node_ptr, node_ptr->value);
    else
        fprintf (graph_file, "\tnode%p\t[shape = box, style = filled, fillcolor = yellow, label = \"%s?\"];\n\n", node_ptr, node_ptr->value);

    if (node_ptr->left_son)
        Node_Dump (node_ptr->left_son, graph_file);

    if (node_ptr->right_son)
        Node_Dump (node_ptr->right_son, graph_file);

    return NO_ERRORS;
}

#define Print_Arrow(side)                                                               \
do                                                                                      \
{                                                                                       \
    if (node_ptr->side##_son)                                                           \
    {                                                                                   \
        fprintf (graph_file, "node%p -> node%p;\n", node_ptr, node_ptr->side##_son);    \
        Arrows_Dump (node_ptr->side##_son, graph_file);                                 \
    }                                                                                   \
}                                                                                       \
while (0)

int Arrows_Dump (struct Node *node_ptr, FILE *graph_file)
{
    MY_ASSERT (node_ptr,   "struct Node *node_ptr", NULL_PTR, ERROR);
    MY_ASSERT (graph_file, "FILE *graph_file",      NULL_PTR, ERROR);

    Print_Arrow (left);
    Print_Arrow (right);

    if (node_ptr->parent)
    {
        fprintf (graph_file, "node%p -> node%p [color = \"dimgray\"];\n", node_ptr, node_ptr->parent);
    }

    return NO_ERRORS;
}

int Print_Dump (const char *dot_file_name)
{
    MY_ASSERT (dot_file_name, "const char *dot_file_name", NULL_PTR, ERROR);
    
    char print_dump[BUFF_SIZE] = "";
    sprintf (print_dump, "dot -Tpng %s -o Graphic_Dump/Akinator.png", dot_file_name);
    system (print_dump);

    return NO_ERRORS;
}
//************************************************************************

int Speak_To_Me (const char *phrase)
{
    MY_ASSERT (phrase, "const char *phrase", NULL_PTR, ERROR);

    printf ("%s", phrase);

    #if ESPEAK == 1
    char print_dump[BUFF_SIZE] = "";
    sprintf (print_dump, "espeak %s", phrase);

    system (print_dump);
    #endif

    return NO_ERRORS;
}

void Make_Speech (int parmN, ...)
{
    char *buffer = (char *)calloc (parmN * MAX_ANSWER, sizeof (char));

    va_list ap;
    va_start (ap, parmN);

    for (int i = 0; i < parmN; i++)
        strcat (buffer, va_arg (ap, char *));
    strcat (buffer, "\n");

    va_end (ap);

    Speak_To_Me (buffer);

    free (buffer);
}
