#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



#define PROVERKA( symb )                    \
 printf("proverka  %s \n", symb);            \
if (strcmp(pointer->content, symb ) != 0)     \
{                                              \
printf("wrong symbol %S \n", pointer->content); \
   assert(0);                                    \
}

#define MAX_STR_LEN 100
#define _DEBUG

#ifdef _DEBUG
    #define DEBUG( op )  { op; }
#else
    #define DEBUG( op )  {}
#endif



enum types
{
    NUM = 1,
    ID,
    CHAR,
    BRACKET,
    VAR,
    FUNCTION,

    #define DEF_OP(arg1, arg2) arg1,
    #include "DEF_OPERATIONS.cpp"
    #undef DEF_OP

    CMD_DUMMY
};

typedef struct Node
{
    Node* prev = 0;
    Node* left = 0;
    int type = 0;
    int num = 0;
    char* content = 0;
    Node* right = 0;
    Node* next = 0;
} ;

Node* new_Node(int type, char* content);
bool isHead(Node* lx);
bool isTail(Node* lx);
void connect(Node* prev, Node* next);
void connect_left(Node* target, Node* source);
void connect_right(Node* target, Node* source);
Node* get_num();
Node* get_id();
Node* get_chr();
Node* get_brk();
char* dump(Node* lx);
void expand_lex_type(Node* lx);
Node* Nodes(char* s);
int type(Node* lx);
const char* content(Node* lx);
int filesize (FILE * file);
char* file_v_buf (FILE *reading);
Node* GetG0(Node* first_node);
Node* Get_e();
Node* Get_a();
Node* GetT();
Node* GetP();
Node* GetN();
Node* Get_func_op();
Node* Get_code();
Node* Get_if();
Node* Get_eq();
void strange_insert(Node* op, Node* node1, Node* node2, int const1, int const2);
void Node_cpy(Node* node1, Node* node2);
Node* Get_nen();
Node* Get_var();
void id_to_var(Node* lx);
void Dotter_easy_show (Node* This);
void Dotter_show(Node* nod);

char* _s = 0;
int _curn = 0;
Node* pointer = NULL;
FILE* dotter_easy = NULL;

int main()
{
    dotter_easy = fopen("dotter.txt", "w");
    FILE* reading = fopen("proga.txt" , "r");
    assert(reading);

    char* buf_pointer = file_v_buf (reading);

    Node* first = Nodes(buf_pointer);
    Node* nod = first;
    while (nod->next)
    {
        id_to_var(nod);
        nod = nod->next;
    }


    nod = GetG0(first);

    Dotter_show(nod);

    return 0;
}




Node* new_Node(int type, char* content)
{
    Node* res = (Node*) calloc(1, sizeof(Node));
    res -> content = (char*) calloc(strlen(content) + 1, sizeof(char));
    strcpy(res -> content, content);
    res -> type = type;
    return res;
}

bool isHead(Node* lx)
{
    return !(lx -> prev);
}

bool isTail(Node* lx)
{
    return !(lx -> next);
}

void connect(Node* prev, Node* next)
{
    assert(prev != 0);
    assert(next != 0);
    Node* rp = next -> prev;
    Node* ln = prev -> next;
    next -> prev = prev;


    prev -> next = next;

}

void connect_left(Node* target, Node* source)
{
    target -> left = source;
}

void connect_right(Node* target, Node* source)
{
    target -> right = source;
}




Node* get_num()
{
    while(isspace(*_s))
    {
        _s++;
    }

    char* s0 = _s;
    while(isdigit(*_s))
    {
        _s++;
    }
    if(*_s == '.')
    {
        _s++;
        while(isdigit(*_s))
        {
            _s++;
        }
    }
    if(s0 != _s)
    {
        char buf = *_s;
        *_s = '\0';
        Node* res = new_Node(NUM, s0);
        *_s = buf;
        return res;
    }
    else
    {
        return 0;
    }
}

Node* get_id()
{
    while(isspace(*_s))
    {
        _s++;
    }

    char* s0 = _s;
    while(isdigit(*_s) || isupper(*_s) || islower(*_s) || *_s == '_')
    {
        _s++;
    }
    if(s0 != _s)
    {
        char buf = *_s;
        *_s = '\0';
        Node* res = new_Node(ID, s0);
        *_s = buf;
        return res;
    }
    else
    {
        return 0;
    }
}

Node* get_chr()
{
    while(isspace(*_s))
    {
        _s++;
    }

    char* s0 = _s;
    while(ispunct(*_s) && *_s != '(' && *_s != '{' && *_s != '[' && *_s != ')' && *_s != '}' && *_s != ']')
    {
        _s++;
    }
    if(s0 != _s)
    {
        char buf = *_s;
        *_s = '\0';
        Node* res = new_Node(CHAR, s0);
        *_s = buf;
        return res;
    }
    else
    {
        return 0;
    }
}

Node* get_brk()
{
    while(isspace(*_s))
    {
        _s++;
    }

    char* s0 = _s;
    if(*_s == '(' || *_s == '{' || *_s == '[' || *_s == ')' || *_s == '}' || *_s == ']')
    {
        _s++;
        char buf = *_s;
        *_s = '\0';
        Node* res = new_Node(BRACKET, s0);
        *_s = buf;
        return res;
    }
    else
    {
        return 0;
    }
}


char* dump(Node* lx)
{
    char* res = (char*) calloc(strlen(lx -> content) + 5, sizeof(char));
    strcpy(res, lx -> content);
    sprintf(res + strlen(lx -> content), " %d", lx -> type);
    return res;
}

char* get_str()
{
    char* res = 0;
    while(isspace(*_s))
    {
        _s++;
    }

    if(*_s == '\"')
    {
        _s++;
        char* s0 = _s;
        while(*_s != '\"' && *_s)
        {
            _s++;
        }
        char buf = *_s;
        *_s = '\0';
        res = (char*) calloc(strlen(s0) + 1, sizeof(char));
        strcpy(res, s0);
        *_s = buf;
    }
    return res;
}




void expand_lex_type(Node* lx)
{
    #define DEF_OP(arg1, arg2) \
    if (!strcmp(lx->content, #arg2 )) lx->type = arg1 ;\

    #include "DEF_OPERATIONS.cpp"

    #undef DEF_OP

}

void id_to_var(Node* lx)
{
    if (lx->type == ID)
    {
        if (lx->next)
        {
            if (strcmp(lx->next->content, "(") == 0)
            {
                DEBUG( printf("ID -> FUNCTION \n") );
                lx->type = FUNCTION ;
            }
            else
            {
                DEBUG( printf("ID -> VAR \n") );
                lx->type = VAR ;
            }
        }
        else
        {
            DEBUG( printf("ID -> VAR \n") );
            lx->type = VAR ;
        }
    }
}

Node* Nodes(char* s)
{
    _s = s;
    Node* res = new_Node(0,"start");
    Node* cur = res;
    Node* got = (Node*)1;
    printf("begin parsing\n");
    while(got)
    {
        got = 0;
        printf("getting chr\n");
        got = get_chr();
        if(!got) got = get_brk();
        printf("getting num\n");
        if(!got) got = get_num();
        printf("getting id\n");
        if(!got) got = get_id();
        if(got)
        {
            expand_lex_type(got);
            printf("got result %s\n", dump(got));
            connect(cur, got);
            printf("connected\n");
            cur = cur -> next;
        }
    }
    if(*_s == '\0')
    {
        return res -> next;
    }
    else
    {
        fprintf(stderr, "error: wrong symbol %c\n", *_s);
        return 0;
    }
}

int type(Node* lx)
{
    if(lx)
    {
        return lx -> type;
    }
    else
    {
        return 0;
    }
}

const char* content(Node* lx)
{
    if(lx)
    {
        return lx -> content;
    }
    else
    {
        return 0;
    }
}




char* file_v_buf(FILE *reading)
    {

        int file_size = 0;
        file_size = filesize(reading);
        rewind(reading);
        char* file_string = (char*)calloc(1, file_size);
        fread(file_string, file_size, 1, reading);
        return file_string;
    }


int filesize(FILE *file)
    {
        FILE* Minifile = file ;
        long int file_size=0;

        fseek( file, 0, SEEK_END );
        file_size = ftell( file );

        printf("filesize %d\n", file_size);
        return (file_size );
    }



Node* GetG0(Node* first_node)
{
    pointer = first_node;

    DEBUG( printf("FUCK THE DEBUG \n") );
    assert(pointer);

    PROVERKA( "{" )
    pointer = pointer->next;

    Node* last_code = Get_code();
    printf("ha\n");
    Node* ret = last_code;
    Node* last = NULL;
    DEBUG( printf("WHErE IS MY CODE\n") );
    while (last_code)
    {
        DEBUG( printf("CONNECTED CODE ; \n") );
        last = last_code;                                        //связывает ; соседних get_code
        last_code->right = Get_code();
        last_code = last_code->right;
    }

    PROVERKA( "}" )

    if (pointer->next)
    {
        pointer = pointer->next;
        DEBUG( printf("In function descriptors\n") );
        Node* first_function = Get_func_op();
        Node* first = NULL;
        last->right = first_function;

        while(first_function)
        {
            first = first_function;
            first_function->right = Get_func_op();

            first_function = first_function->right;

        }
    }

    if (pointer != NULL)
    {
        printf("%s\n", pointer->content);
        assert(!"ERROR no \\0 that was waited \n");
    }

    return ret;

}

Node* Get_func_op()
{
    if (pointer == NULL) return 0;
    printf("get_func_op\n");
    if (pointer->type == OP_FUNC )
    {
        Node* fun = pointer;
        pointer = pointer->next;
        printf("function\n");
        if (pointer->type == FUNCTION)
        {
            Node* node_f = pointer;
            pointer = pointer->next;

            PROVERKA( "(" )
            pointer = pointer->next;

            Node* node_arg = Get_var();

            node_f->left = node_arg;

            PROVERKA( ")" )
            pointer = pointer->next;

            PROVERKA( "{" )
            pointer = pointer->next;

            Node* code = Get_code();

            Node* e = NULL;
            printf("return \n");
            if (pointer->type == OP_RET)
            {
                pointer = pointer->next;
                e = Get_e();
            }
            else return 0;

            PROVERKA( ";" )
            pointer = pointer->next;

            Node* point1 = code;
            while (point1->right)
            {
                point1 = point1->right;
            }

            point1->right = new_Node( OP_TZ, ";");
            point1 = point1->right;

            point1->left = new_Node( OP_RET, "return");
            point1->left->left = e;

            PROVERKA( "}" )
            pointer = pointer->next;
            printf("0x%d\n", pointer);
            fun->left = node_f;
            node_f->right = code;


            return fun;
        }
        else return 0;

    }
    else
    {
        return 0;
    }
}

Node* Get_code()
{
    Node* first = 0;
    if (pointer->type == OP_IF)
    {
        DEBUG( printf("in Get_if \n") );
        return Get_if();
    }
    if (pointer->type == VAR)
    {
        DEBUG( printf("in Get_a \n") );
        return  Get_a();
    }

    return first;

}


Node* Get_if()
{
    PROVERKA( "if" )
    Node* iff = pointer;
    pointer = pointer->next;

    PROVERKA( "(" )
    pointer = pointer->next;

    Node* cond = Get_eq();

    PROVERKA( ")" )
    pointer = pointer->next;

    PROVERKA( "{" )
    pointer = pointer->next;

    Node* code = Get_code();

    PROVERKA( "}" )
    pointer = pointer->next;

    iff->left = cond;
    iff->right = code;

    Node* tz = new_Node( OP_TZ , ";");
    tz->left = iff;

    return tz;
}

Node* Get_a()
{
    DEBUG( printf("in Get_var \n") );
    Node* var = Get_var();

    PROVERKA( "=" )
    Node* ass = pointer;
    pointer = pointer->next;

    DEBUG( printf("in Get_e \n") );
    Node* e = Get_e();

    PROVERKA( ";" )
    Node* ret = pointer;
    pointer = pointer->next;

    ret->left = ass;
    ass->left = var;
    ass->right = e;

    return ret;

}


Node* Get_eq()
{
   Node* e1 = Get_e();

   Node* op = pointer;
   pointer = pointer->next;

   Node* e2 = Get_e();

   op->left = e1;
   op->right = e2;

   return op;
}




Node* Get_e()
{
    DEBUG( printf("in GetT \n") );
    Node* node1 = GetT();
    Node* znak1 = NULL;
    Node* node2 = NULL;

    while (pointer->type == OP_ADD || pointer->type == OP_SUB )
        {
            pointer = pointer->next;
            DEBUG( printf("I'm im + or -  \n") );
            Node* operation = pointer;

            DEBUG( printf("CALLING T from while \n") );
            node2 = GetT();

            strange_insert(operation, node1, node2, OP_ADD, OP_SUB);

            node1 = operation;
        }
    DEBUG( printf("Out of Get_e \n") );
    return node1;
}

void strange_insert(Node* op, Node* node1, Node* node2, int const1, int const2)
{
    Node* tonode = node1;
    while (tonode->type == const1 || tonode->type == const2)
    {
        tonode = tonode->right;
    }
    Node new1 = {};
    Node_cpy(&new1, tonode);

    Node_cpy(tonode, op);
    tonode->left = &new1;
    tonode->right = node2;

}

void Node_cpy(Node* node1, Node* node2)
{
    node1->type = node2->type;
    node1->content = (char*) calloc(1, sizeof(node2->content));
    strcpy(node1->content, node2->content);
}


Node* GetT()
{
    DEBUG( printf("in GetP \n") );
    Node* node1 = GetP();
    Node* znak1 = NULL;
    Node* node2 = NULL;

    while (pointer->type == OP_MUL || pointer->type == OP_DIV )
        {
            DEBUG( printf("I'm im * or /  \n") );
            Node* operation = pointer;

            DEBUG( printf("CALLING D from while \n") );
            node2 = GetP();

            strange_insert(operation, node1, node2, OP_MUL, OP_DIV);

            node1 = operation;
        }
    DEBUG( printf("returned from GetT \n") );
    return node1;
}





Node* GetP()
{
    if (strcmp(pointer->content, "(") == 0)
    {
       pointer = pointer->next;
       DEBUG( printf("in Get_e \n") );
       Node* node1 = Get_e();

       PROVERKA( ")" )
       pointer = pointer->next;

       return node1;
    }
    else
    {
        DEBUG( printf("in Get_nen \n") );
        return Get_nen();
    }
}

Node* Get_nen()
{
    if (pointer->type == NUM)
    {
        return GetN();
    }

    if (pointer->type == VAR)
    {
        return Get_var();
    }

    if (pointer->type == FUNCTION)
    {
        Node* node_f = pointer;
        pointer = pointer->next;

        PROVERKA( "(" )
        pointer = pointer->next;

        Node* node_arg = Get_e();

        node_f->left = node_arg;

        return node_f;
    }
    return 0;
}

Node* Get_var()
{
    Node* node1 = pointer;
    if (pointer->type == VAR)
    {
        pointer = pointer->next;
        return node1;
    }
    else
    {
        return 0;
    }
}


Node* GetN()
{
    if (pointer->type == NUM)
    {
        Node* node1 = pointer;
        pointer = pointer->next;

        return node1;
    }
    else
    {
        return 0;
    }
}


void Dotter_easy_show (Node* This)
    {
        if (This == NULL) { printf("Ha\n");return ;}

        if (This->left != NULL)
            {

                fprintf (dotter_easy, "\"0x%d \n %s \n type = %i \" -> \"0x%d \n %s \n type = %i \" \n",  This, This->content, This->type, This->left, This->left->content, This->left->type );
            }

        if (This->right != NULL)
            {

                fprintf (dotter_easy, "\"0x%d \n %s \n type = %i \"-> \"0x%d \n %s \n type = %i \" \n",  This, This->content, This->type, This->right, This->right->content, This->right->type );
            }

        Dotter_easy_show (This->left);
        Dotter_easy_show (This->right);

    }

void Dotter_show(Node* nod)
{
    fprintf(dotter_easy, "digraph { \n");
    Dotter_easy_show(nod);
    fprintf(dotter_easy, "\n }");
}

