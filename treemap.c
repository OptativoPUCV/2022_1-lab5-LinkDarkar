#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};


int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}

TreeNode * createTreeNode(void* key, void * value)
{
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2))
{
    TreeMap* tree = (TreeMap *)malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    //new->lower_than = lower_than;
    return tree;
}

void insertTreeMap(TreeMap * tree, void* key, void * value)
{
    if(searchTreeMap(tree, key) != NULL)
    {
        return;
    }

    TreeNode* new = createTreeNode(key , value);
    int menor = 0;
    tree->current = tree->root;
    while (tree->current != NULL)
    {
        if (tree->lower_than(key , tree->current->pair->key) == 1)
        {
           if (tree->current->left == NULL)
           {
               menor = 1;
               break;
           }
           tree->current = tree->current->left;
           menor = 1;
        }
        else
        {
           if (tree->current->right == NULL)
           {
               menor = 0;
           }
           tree->current = tree->current->right;
           menor = 0;
        }
    }

    if (menor == 1)
    {
        new->parent = tree->current;
        tree->current->left = new; 
    }
    else
    {
        new->parent = tree->current;
        tree->current->right = new;
    }
    
    tree->current = new;
}

TreeNode * minimum(TreeNode * x)
{
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node)
{
    /*opcion nodo sin hijos*/
    if (node->left == NULL && node->right == NULL)
    {
        if(tree->lower_than(node->parent->pair->key , node->pair->key) == 1)
        {
            node->parent->right = NULL;
        }
        else
        {
            node->parent->left = NULL;
        }
        return;
    }

    /*opcion nodo con un hijo*/
    if (node->left == NULL && node->right != NULL)
    {
        if(tree->lower_than(node->parent->pair->key , node->pair->key) == 1)
        {
            node->parent->right = node->right;
            node->right->parent = node->parent;
        }
        else
        {
            node->parent->left = node->right;
            node->right->parent = node->parent;
        }
        return;
    }
    else if (node->left != NULL && node->right == NULL)
    {
        if(tree->lower_than(node->parent->pair->key , node->pair->key) == 1)
        {
            node->parent->right = node->left;
            node->left->parent = node->parent;
        }
        else
        {
            node->parent->left = node->left;
            node->left->parent = node->parent;
        }
        return;
    }

    /*opcion nodo con dos hijos*/
    if(node->left != NULL && node->right != NULL)
    {
        TreeNode* aux = minimum(node->right);
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        removeNode(tree , aux);
    }
}

void eraseTreeMap(TreeMap * tree, void* key)
{
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}

Pair * searchTreeMap(TreeMap * tree, void* key)
{
    //TreeNode* nodo = createTreeNode(tree->root->pair->key , tree->root->pair->value);
    tree->current = tree->root;
    while (tree->current != NULL)
    {
        if(is_equal(tree , tree->current->pair->key , key) == 1)
        {
            return tree->current->pair;
        }
        else if (tree->lower_than(tree->current->pair->key , key) == 0)
        {
            tree->current = tree->current->left;
            continue;
        }
        else
        {
            tree->current = tree->current->right;
            continue;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key)
{
    tree->current = tree->root;
    TreeNode* ub_node = tree->current;
    while (tree->current->pair->key != key)
    {
        if (tree->lower_than(tree->current->pair->key , key) == 1)
        {
            if (tree->current->right == NULL)
            {
                break;
            }
            tree->current = tree->current->right;
        }
        
        if (tree->lower_than(tree->current->pair->key , key) == 0)
        {
            /*
            if (tree->current->left == NULL)
            {
                break;
            }
            
            if (tree->lower_than(tree->current->pair->key , key) == 0 && tree->lower_than(tree->current->pair->key , ub_node->pair->key) == 1)
            {
                ub_node = tree->current;
            }*/

            tree->current = tree->current->left;
        }
    }

    if (tree->current->pair->key != key)
    {
        return ub_node->pair;
    }
    
    return tree->current->pair;
}

Pair * firstTreeMap(TreeMap * tree)
{
    return minimum(tree->root)->pair;
}

Pair * nextTreeMap(TreeMap * tree)
{
    if (tree->current->right != NULL)
    {
        tree->current = minimum(tree->current->right);
        return tree->current->pair;
    }
    
    TreeNode* aux = tree->current->parent;
    while (aux != NULL)
    {
        if (tree->lower_than(tree->current->pair->key , aux->pair->key) == 1)
        {
            tree->current = aux;
            return tree->current->pair;
        }
        else
        {
            aux = aux->parent;
        }
    }
    return NULL;
}
