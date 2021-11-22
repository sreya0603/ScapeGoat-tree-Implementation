#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

int n=0,q=0;

static int const log32 (int n)
{
    int const log23 = 2.4663034623764317;
    return (int) ceil(log23 * log (n));
}

struct SG
{
    float value;
    struct SG *right,*left,*parent;
};
struct SG*root;

int storeInArray (struct SG * ptr,struct SG * arr[], int i)
{
    if (ptr == NULL)
        return i;

    i = storeInArray (ptr->left, arr, i);
    arr[i++] = ptr;
    return storeInArray (ptr->right, arr, i);
}

void preorder(struct SG*node)
{
    if (node != NULL)
    {
        printf("%f ",node->value);
        preorder(node -> left);
        preorder(node -> right);
    }
}

int size (struct SG * node)
{
    if (node == NULL)
        return 0;
    return 1 + size (node->left) + size (node->right);
}

struct SG * Balancedtree(struct SG **a,int i, int n)
{
    if (n== 0)
        return NULL;
    int m = n / 2;
  
    a[i+m]->left = Balancedtree(a, i, m);
  
    if (a[i+m]->left != NULL)
        a[i+m]->left->parent = a[i+m];
  
    a[i+m]->right =
         Balancedtree(a, i+m+1, n-m-1);
    if (a[i+m]->right != NULL)
        a[i+m]->right->parent = a[i+m];
  
    return a[i+m];
}

void rebuild(struct SG *u)
{
    int n = size(u);
    struct SG *p;
    p = u->parent;
    //printf("\nhi\n");
    struct SG **a;
    a= (struct SG**)malloc(n*sizeof (struct SG*));
    storeInArray(u, a, 0);
    if (p == NULL)
    {
        u = Balancedtree(a, 0, n);
        u->parent = NULL;
    }
    else if (p->right == u)
    {
        p->right = Balancedtree(a, 0, n);
        p->right->parent = p;
    }
    else
    {
        p->left = Balancedtree(a, 0, n);
        p->left->parent = p;
    }
}

int BSTInsert(struct SG *u)
{
    struct SG *w = root;
    if (w == NULL)
    {
        root = u;
        n++;
        q++;
        return 0;
    }
  
    bool done = false;
    int d = 0;
    do
    {
        if (u->value < w->value)
        {
            if (w->left == NULL)
            {
                w->left = u;
                u->parent = w;
                done = true;
            }
            else
                w = w->left;
        }
        else if (u->value > w->value)
        {
            if (w->right == NULL)
            {
                w->right = u;
                u->parent = w;
                done = true;
            }
            else
                w = w->right;
        }
        else
            return -1;
        d++;
    }
    while (!done);
  
    n++;
    q++;
    return d;
}

bool insert(float x)
{
    struct SG*node=(struct SG*)malloc(sizeof(struct SG));
    node->value=x;
    node->left=NULL;
    node->right=NULL;
    node->parent=NULL;
    int h=BSTInsert(node);
    //printf("\n%lf,%lf\n",h,log32(q));
    
    if (h > log32(q))
    {
        struct SG *p = node->parent;
        while (3*size(p) <= 2*size(p->parent))
            p = p->parent;
        rebuild(p->parent);
    }

    return h >= 0;
}

int search (float y)
{
    struct SG *w = root;
    while (1)
    {
        if (w == NULL)
	    {
	        printf ("not found\n");
	        return 0;
	    }
        if (w->value == y)
	    {
	        printf ("found\n");
	        return 1;
	    }
        if (w->value < y)
	    w = w->right;
        if (w->value > y)
	    w = w->left;
    } 
}

int BSTdelet(float z)
{
    struct SG* curr = root;
    struct SG* prev = NULL;
    while (curr != NULL && curr->value != z) {
        prev = curr;
        if (z < curr->value)
            curr = curr->left;
        else
            curr = curr->right;
    }
 
    if (curr == NULL) {
        printf("key %f not found in the provided tree\n",z);
        return 0;
    }
 
    if (curr->left == NULL || curr->right == NULL) {
 
        struct SG* newCurr;
 
        if (curr->left == NULL)
            newCurr = curr->right;
        else
            newCurr = curr->left;
 
        if (prev == NULL){
            n=n-1;
            return 1;}
 
        if (curr == prev->left)
            prev->left = newCurr;
        else
            prev->right = newCurr;
        free(curr);
    }
    else {
        struct SG* p = NULL;
        struct SG* temp;
 
        temp = curr->right;
        while (temp->left != NULL) {
            p = temp;
            temp = temp->left;
        }
 
        if (p != NULL)
            p->left = temp->right;
 
        else
            curr->right = temp->right;
 
        curr->value = temp->value;
        free(temp);
    }
    n=n-1;
    return 1;
}

struct SG* delet(float z)
{
    int h=BSTdelet(z);
    if(h)
    {printf("%d,%d",q,n);  
        if (q>2*n)
        {
           rebuild(root);
           q=n;
        }
    }
    return root;
}

int main ()
{
    float d;
    int s;
    char ch;
    while (1)
    {
        printf ("i) Insert :\ns) search :\nd) delete:\np) print preorder\nq) Quit:\nEnter char:");
        scanf ("%s", &ch);
        switch (ch)
	    {
	        case 'i':
                printf("Enter a data to insert:");
                scanf("%f",&d);
                insert(d);
                break;
            case 's':
                printf("Enter data you want to search:");
                scanf("%f",&d);
                s=search(d);
                break;
            case 'd':
                printf("Enter data you want to delete:");
                scanf("%f",&d);
                delet(d);
                break;
            case 'p':
                preorder(root);
                break;
            case 'r':
                rebuild(root->right);
                break;
            case'q':
                exit(0);
	    }
    }
}
