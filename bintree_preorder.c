#include <stdio.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

void preorder(struct node *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

int main()
{
    struct node n1 = {1, NULL, NULL};
    struct node n4 = {4, NULL, NULL};
    struct node n5 = {5, NULL, NULL};
    struct node n2 = {2, &n4, &n5};
    struct node n3 = {3, &n1, &n2};

    preorder(&n3); // 输出结果：3 1 2 4 5

    return 0;
}
