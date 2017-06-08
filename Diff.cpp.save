

#define PROVERKA( symb ) if (strcmp(pointer->content, symb ) != 0) { printf("wrong symbol %S \n", pointer->content);   assert(0);}

#define MAX_STR_LEN 100
#undef _DEBUG

#ifdef _DEBUG
    #define DEBUG( op )  { op; }
#else
    #define DEBUG( op )  {}
#endif

#define DEBUG(...) { __VA_ARGS__ ;}




char* s = NULL;

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

Node* pointer = NULL;






Node* GetG0(Node* first_node)
{
    pointer = first_node;

    DEBUG( printf("FUCK THE DEBUG \n") );
    assert(pointer);

    PROVERKA( "{" )
    pointer = pointer->next;

    Node* last_code = Get_code();
    Node* ret = last_code;
    Node* last = NULL;
    while (last_code)
    {
        last = last_code;                                        //связывает ; соседних get_code
        last_code->right = Get_code();
        last_code = last_code->right;
    }

    PROVERKA( "}" )
    pointer = pointer->next;

    last->right = Get_func_op();
    last_code = last->right;
    while(last_code)
    {
        last = last_code;
        last_code->right = Get_func_op();
        last_code = last_code->right;
    }



    if (!pointer->next)
    {
        assert(!"ERROR no \\0 that was waited \n");
    }

    return ret;

}

Node* Get_func_op()
{
    if (pointer->type == OP_FUNC )
    {
        Node* fun = pointer;
        pointer = pointer->next;

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

            if (pointer->type == OP_RET)
            {
                pointer = pointer->next;
                Node* e = Get_e();
            }
            else return 0;

            PROVERKA( ";" )
            pointer = pointer->next;

            Node* point1 = code;
            while (point1->right)
            {
                point1 = point1->right;
            }

            point1->right = newNode( OP_TZ, ";");
            point1 = point1->right;

            point1->left = newNode( OP_RET, "return");
            point1->left->left = e;

            PROVERKA( "}" )
            pointer = pointer->next;

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
        first = Get_if();
    }
    if (pointer->type == VAR)
    {
        first = Get_a();
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
   Node* var = Get_var();

   PROVERKA( "=" )
   Node* ass = pointer;
   pointer = pointer->next;

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
    DEBUG( printf("CALLING T \n") );
    Node* node1 = GetT();
    Node* znak1 = NULL;
    Node* node2 = NULL;

    while (pointer->type == OP_ADD || pointer->type == OP_SUB )
        {
            DEBUG( printf("I'm im + or -  \n") );
            operation = pointer;

            DEBUG( printf("CALLING T from while \n") );
            node2 = GetT();

            strange_insert(operation, node1, node2, OP_ADD, OP_SUB);

            node1 = operation;
        }
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
    Node_cpy(new1, tonode);
    Node_cpy(tonode, op);

    tonode->left = &new1;
    tonode->right = node2;

}

void Node_cpy(Node* node1, Node* node2)
{
    node1->type = node2->type;
    strcpy(node1->content, node2->content);
}


Node* GetT()
{
    DEBUG( printf("CALLING D \n") );
    Node* node1 = GetP();
    Node* znak1 = NULL;
    Node* node2 = NULL;

    while (pointer->type == OP_MUL || pointer->type == OP_DIV )
        {
            DEBUG( printf("I'm im * or /  \n") );
            operation = pointer;

            DEBUG( printf("CALLING D from while \n") );
            node2 = GetP();

            strange_insert(operation, node1, node2, OP_MUL, OP_DIV);

            node1 = operation;
        }
    return node1;
}





Node* GetP()
{
    if (strcmp(pointer->content, "("))
    {
       pointer= pointer->next;
       Node* node1 = Get_e();
       PROVERKA( ")" )
       pointer = pointer->next;

       return node1;
    }
    else
    {
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
        return node1;
    {
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

