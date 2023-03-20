#include <stdio.h>
struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
};

struct TreeNode *invertTree(struct TreeNode *root)
{
    if (root == NULL)
    { // 如果根节点为空，直接返回
        return NULL;
    }
    struct TreeNode *left = invertTree(root->left);   // 递归翻转左子树
    struct TreeNode *right = invertTree(root->right); // 递归翻转右子树
    root->left = right;                               // 将右子树作为根节点的左子树
    root->right = left;                               // 将左子树作为根节点的右子树
    return root;                                      // 返回翻转后的根节点
}