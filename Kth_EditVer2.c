#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct
{
    int **ancestor ;
    int max_level;
} TreeAncestor;

TreeAncestor *treeAncestorCreate(const int n,int *parent,int parentSize)
{
    TreeAncestor *obj = malloc(sizeof(TreeAncestor));
    obj->ancestor = malloc(n * sizeof(int *));
    obj->ancestor[0] = malloc(1 * sizeof(int));
    obj->ancestor[0][0] = -1;
    for (int i = 1; i < n; i++)
    {
        int max_level = 32 - __builtin_clz(i);
        //printf("i=%d, level=%d\n",i,max_level);
        obj->ancestor[i] = malloc(max_level * sizeof(int));
        memset(obj->ancestor[i], -1, max_level*sizeof(int));
    }

    for (int i = 0; i < parentSize; i++)
        obj->ancestor[i][0] = parent[i];
/*
    for(int i=0; i<n; i++)
      {
          int max_level = 32 - __builtin_clz(i);
          for (int j = 0; j < max_level; j++)
          {
              printf("i=%d,j=%d, ancestor[i][j]=%d\n",i,j,obj->ancestor[i][j]);
          }
      }
*/

    obj->max_level=32 - __builtin_clz(n-1);
    for (int j = 1;; j++)
    {
        if(j>obj->max_level)
            break;
        for (int i = 1; i < parentSize; i++)
        {
            if(obj->ancestor[i][j + (-1)] == -1)
                break;
            if(j<(32 - __builtin_clz(i)))
            {
                obj->ancestor[i][j] = obj->ancestor[i][j -1] == -1
                                      ? -1
                                      : obj->ancestor[obj->ancestor[i][j - 1]][j - 1];
            }
            else
                continue;
        }
    }


/*
    for(int i=0; i<n; i++)
      {
          int max_level = 32 - __builtin_clz(i);
          for (int j = 0; j < max_level; j++)
          {
              printf("i=%d,j=%d, ancestor[i][j]=%d\n",i,j,obj->ancestor[i][j]);
          }
      }

*/
    return obj;


}

int treeAncestorGetKthAncestor(TreeAncestor *obj, int node, int k)
{
    int max_level = 32 - __builtin_clz(node);
    for (int i = 0; i < max_level ; ++i)
        if (k & (1<<i))
            node = obj->ancestor[node][i];
    return node;
}



void treeAncestorFree(TreeAncestor *obj)
{
    for (int i = 0; i < obj->max_level; i++)
        free(obj->ancestor[i]);
    free(obj->ancestor);
    free(obj);
}

int main()
{
    const int n=10000;
    //Creating a skew_tree :0->1->2->3->4->->5->6->7->8->9->10.
    //Skew-tree is worst case:
    int *parent;
    parent = calloc(n, sizeof(int));
    /*parent[0]=-1;
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
