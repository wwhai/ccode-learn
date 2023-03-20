#include <stdio.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main()
{
    struct node n1 = {1, NULL, NULL};
    struct node n4 = {4, NULL, NULL};
    struct node n5 = {5, NULL, NULL};
    struct node n2 = {2, &n4, &n5};
    struct node n3 = {3, &n1, &n2};

    inorder(&n3); // 输出结果：1 3 4 2 5

    return 0;
}
