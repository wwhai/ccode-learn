#include <stdio.h>

struct node
{
    int data;
    struct node *left;
    struct node *right;
};

void postorder(struct node *root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main()
{
    struct node n1 = {1, NULL, NULL};
    struct node n4 = {4, NULL, NULL};
    struct node n5 = {5, NULL, NULL};
    struct node n2 = {2, &n4, &n5};
    struct node n3 = {3, &n1, &n2};

    postorder(&n3); // 输出结果：1 4 5 2 3

    return 0;
}
