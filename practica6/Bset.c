#include "Bset.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


struct stNode
{
    Type data;
    struct stNode* left;
    struct stNode* right;
    struct stNode* father;
};

typedef struct stNode* Node;


struct stSet
{
    Node root;
    int size;
    CompareFunc comparefunc;
};
//prototipos de funciones locales
bool set_contains(Type X, Node n, Set s);
bool set_remove(Type t, Node n, Set s,int opc);
Type set_remove_min(Node n, Set s  );
Type set_remove_max(Node n, Set s  );
void set_printPreO(Node n);
void set_printPosO (Node n);
void set_printInO(Node n);
void set_insert(Node New_Node, Node father, Set s );
void set_Destroy(Node n, Set s);


//variables globales


Set set_create(CompareFunc cf)
{
    Set s = malloc(sizeof(struct stSet));
    s->root = NULL;
    s->size = 0;
    s->comparefunc = cf;

    return s;
}

int set_size(Set s)
{
    return s->size;
}

bool set_add(Set s, Type t)
{
    //creando el nodo, y almacenando su información
    Node new_node = malloc(sizeof(struct stNode));
    new_node->data = t;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->father = NULL;

    int nivel = 0;

    if (s->root == NULL)
    {
        s->root = new_node;
        s->size ++;
        return true;
    }
    else{
        set_insert(new_node,s->root,s);
    }
}

void set_insert(Node new_node, Node father, Set s ){
    int cmp;
    cmp = s->comparefunc(new_node->data,father->data);
    if(cmp == 0){
        printf("Elemento Duplicado");
    } else if(cmp < 0){
        if(father->left== NULL) {
            new_node->father = father;
            father->left = new_node;
        }else{
            set_insert(new_node,father->left,s);
        }
    } else if(cmp > 0){
        if(father->right == NULL){
            new_node->father = father;
            father->right = new_node;
        }else{
            set_insert(new_node,father->right,s);
        }
    }
    return;
}

int compareInts(Type t1, Type t2)
{
    int *i1, *i2;
    i1 = (int*) t1;
    i2 = (int*) t2;
    return *i1 - *i2;
}

bool set_contains(Type X, Node n, Set s){
    if(n==NULL) return false;
    else if (s->comparefunc(X,n->data)==0)
        return true;
    else if (s->comparefunc(X,n->data)<0)
        return set_contains(X,n->left,s);
    else if(s->comparefunc(X,n->data)>0)
        return set_contains(X,n->right,s);
    return false;

}

bool set_containsValue(Type X, Set s){
    return set_contains(X,s->root,s);
}

Type set_remove_min(Node n, Set s ){
    if(n->left ==NULL)
    {
        Type data;
        data = n->data;
        if(n->right)
        {
            //decirle al hijo quien es su padre antes de eliminar
            n->right->father = n->father;
            //decirle a n que hijo es de su padre
            if (s->comparefunc(n->data,n->father->data)<0)
                n->father->left = n->right;
            else n->father->right = n->right;
        }
        else{
            //ver al hijo para decirle al padre que ya no tiene hijo
            if (s->comparefunc(n->data,n->father->data)<0)
                n->father->left = NULL;
            else
                n->father->right=NULL;

        }
        free(n);
        return data;
    } else
        return set_remove_min(n->left,s);

}

bool set_remove(Type t, Node n, Set s,int opc)
{
    if (n != NULL)
    {
        if (s->comparefunc(t,n->data) > 0)
            return set_remove(t,n->right,s,opc);
        else if (s->comparefunc(t,n->data) < 0)
            return set_remove(t,n->left,s,opc);
        else
        {

            //si llegó a este punto, significa que encontró el valor
            //caso 1, no tiene hijos
            if (n->left == NULL && n->right == NULL)
            {
                //ver qué hijo es
                if(s->comparefunc(n->data,n->father->data)<0)// es hijo izq
                {
                    n->father->left = NULL;//avisarle al padre que se queda sin hijo
                    free(n);
                }
                else
                {
                    n->father->right = NULL;//avisarle al padre que se queda sin hijo
                    free(n);
                }
                s->size --;
                return true;
            }
                //caso donde tenga hijo derecho
            else if (n->left == NULL)
            {
                //presentar al hijo a su nuevo padre
                n->right->father = n->father;

                //ver n qué hijo es de su padre
                if(s->comparefunc(n->data,n->father->data)<0)// es hijo izq
                    //presentar al padre con el hijo izq
                    n->father->left = n->right;
                else // es hijo der
                    n->father->right = n->right;
                free(n);
                s->size --;
                return true;

            }
                //caso donde tenga hijo izquierdo
            else if (n->right == NULL)
            {
                //presentar al hijo a su nuevo padre
                n->left->father = n->father;

                //ver n qué hijo es de su padre
                if(s->comparefunc(n->data,n->father->data)<0)// es hijo izq
                    //presentar al padre con el hijo izq
                    n->father->left = n->left;
                else // es hijo der
                    n->father->right = n->left;

                free(n);

                s->size --;
                return true;
            }
                //caso donde tenga dos hijos
            else
            {
                if(opc==1){
                    n->data = set_remove_min(n->right, s);
                    s->size --;
                    return true;
                } else{
                    n->data = set_remove_max(n->right, s);
                    s->size --;
                    return true;
                }

            }
        }
    }
    else
    {
        return false;
    }
}

bool set_removeValueminder(Type t, Set s)
{
    return set_remove(t, s->root,s,1);
}
bool set_removeValuemaxizq(Type t, Set s)
{
    return set_remove(t, s->root,s,2);
}

void set_printPreO(Node n)
{
    if (n != NULL)
    {	printf("%d, ",*(int*)n->data);
        if(n->left) set_printPreO(n->left);
        if(n->right) set_printPreO(n->right);
    }else{
        printf("No existe arbol para imprimir");
    }
}

void set_printPosO (Node n){
    if (n != NULL)
    {
        if(n->left) set_printPosO(n->left);
        if(n->right) set_printPosO(n->right);
        printf("%d, ",*(int*)n->data);
    } else{
        printf("No existe arbol para imprimir");
    }
}

void set_printInO(Node n){
    if (n != NULL)
    {
        if(n->left) set_printInO(n->left);
        printf("%d, ",*(int*)n->data);
        if(n->right) set_printInO(n->right);
    }else{
        printf("No existe arbol para imprimir");
    }
}

void set_Destroy(Node n, Set s){
    if(n->left) set_Destroy(n->left,s);
    if(n->right)set_Destroy(n->right,s);
    free(n);
    s->size --;
    if(s->size==0){
        s->root=NULL;
        free(s);
    }
}

void set_printPreOrder( Set s){
    if(s==NULL) printf("No existe arbol a imprimir");
    set_printPreO(s->root);
}

void set_printPostOrder (Set s){
    if(s==NULL) printf("No existe arbol a imprimir");
    set_printPosO(s->root);
}

void set_printInOrder (Set s){
    if(s==NULL) printf("No existe arbol a imprimir");
    set_printInO(s->root);
}

bool set_Destroy_Bset(Set s){
    set_Destroy(s->root,s);
}

Type set_remove_max(Node n, Set s ){
    if(n->right ==NULL)
    {
        Type data;
        data = n->data;
        if(n->left)
        {
            //decirle al hijo quien es su padre antes de eliminar
            n->left->father = n->father;
            //decirle a n que hijo es de su padre
            if (s->comparefunc(n->data,n->father->data)<0)
                n->father->left = n->right;
            else n->father->right = n->right;
        }
        else{
            //ver al hijo para decirle al padre que ya no tiene hijo
            if (s->comparefunc(n->data,n->father->data)<0)
                n->father->left = NULL;
            else
                n->father->right=NULL;

        }
        free(n);
        return data;
    } else
        return set_remove_min(n->right,s);

}