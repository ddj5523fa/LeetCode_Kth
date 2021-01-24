#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct
{
    int **parent;
    int max_level;
} TreeAncestor;

TreeAncestor *treeAncestorCreate(int n, int *parent, int parentSize)
{
    TreeAncestor *obj = malloc(sizeof(TreeAncestor));
    obj->parent = malloc(n * sizeof(int *));

    int max_level = 32 - __builtin_clz(n);  //

    for (int i = 0; i < n; i++)
    {
        obj->parent[i] = malloc(max_level * sizeof(int));  //
        memset(obj->parent[i], -1, max_level * sizeof(int));
    }
    for (int i = 0; i < parentSize; i++)
        obj->parent[i][0] = parent[i];


    for (int i = 0; i < parentSize; i++)
    {
        for (int j = 1; j<max_level; j++)
        {
            if(obj->parent[i][j + (-1)] == -1)
                break;
            else
                obj->parent[i][j] = obj->parent[obj->parent[i][j - 1]][j - 1];

        }

    }
    obj->max_level = max_level; //
    return obj;
}

void treeAncestorFree(TreeAncestor *obj)
{
    for (int i = 0; i < obj->max_level; i++)
        free(obj->parent[i]);
    free(obj->parent);
    free(obj);
}


int treeAncestorGetKthAncestor(TreeAncestor *obj, int node, int k)
{
    int max_level = obj->max_level;
    for (int i = 0; i < max_level && node != -1; ++i)
        if (k & (1<<i))
            node = obj->parent[node][i];
    return node;
}

int main()
{
    const int n=10000;
    //Creating a skew_tree :0->1->2->3->4->->5->6->7->8->9->10.
    //Skew-tree is worst case:
    int *parent;
    parent = calloc(n, sizeof(int));
   /*
    parent[0]=-1;
    parent[1]=0;
    parent[2]=0;
    parent[3]=1;
    parent[4]=1;
    parent[5]=2;
    parent[6]=2;
    */
    for(int i=0; i<n; i++)
    {
        parent[i]=i-1;
        //printf("%d\n",parent[i]);
    }

    TreeAncestor *obj=treeAncestorCreate(n,parent,10000);
	/*
    int anc=treeAncestorGetKthAncestor(obj, 3, 1);
    printf("A(x,k)=%d\n",anc);
    anc=treeAncestorGetKthAncestor(obj, 5, 2);
    printf("A(x,k)=%d\n",anc);
    anc=treeAncestorGetKthAncestor(obj, 6, 3);
    printf("A(x,k)=%d\n",anc);
    */
    for(int i=0;i<1000;i++)
    {
	    int k=treeAncestorGetKthAncestor(obj, 1000, i);
	    printf("A(1000,%d)=%d\n",i,k);
    }
    treeAncestorFree(obj);
    free(parent);
    return 0;
}

