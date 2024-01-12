#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct _node* Position;
typedef struct _node {
    int value;
    Position left;
    Position right;
} Node;

Position initNode(int);
Position ins(Position, int);

int treeHeight(Position);
void CurrentLevel(Position, int);
void levelOrder(Position);

int replace(Position);
int RandomNumberGen();

Position insertNumbers(Position);


int main() {
    srand(time(0));
    
    Position root = NULL;
    Position rootRandomized = NULL;

    root = insertNumbers(root);
    levelOrder(root);
    
    replace(root);
    printf("\n");
    levelOrder(root);

    rootRandomized = ins(rootRandomized, RandomNumberGen());
    for (int i = 0; i < 10; i++) {
        ins(rootRandomized, RandomNumberGen());
    }
    printf("\n");
    levelOrder(rootRandomized);

}
Position initNode(int value) {
    Position newNode = NULL;
    newNode = (Position)malloc(sizeof(Node));
    if (!newNode) {
        printf("error\n");
        return NULL;
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
Position ins(Position root, int value) {
    if (root == NULL)
        return initNode(value);

    if (value < root->value)
        root->left = ins(root->left, value);
    else if (value >= root->value)
        root->right = ins(root->right, value);

    return root;
}
int treeHeight(Position root) {
    if (root == NULL)
        return 0;
    else {
        int leftHeight = treeHeight(root -> left);
        int rightHeight = treeHeight(root -> right);
        
        if (leftHeight > rightHeight)
            return (leftHeight + 1);
        else 
            return (rightHeight + 1);
    }
}
void CurrentLevel(Position root, int height) {
    if (root == NULL)
        return;
        
    if (height == 1)
        printf("%d ", root -> value);
    else if (height > 1) {
        CurrentLevel(root -> right, height - 1);
        CurrentLevel(root -> left, height - 1);

    }
}
void levelOrder(Position root) {
    for (int i = 1; i <= treeHeight(root); i++)
        CurrentLevel(root, i);
}
int replace(Position root) {
    if (root == NULL) {
        return 0;
    }

    int leftValue = replace(root->left);
    int rightValue = replace(root->right);

    int childValue = root->value;
    root->value = leftValue + rightValue;

    return childValue + root->value;
}
int RandomNumberGen() {
    return (rand() % 80) + 10;
}
Position insertNumbers(Position root){
    root = ins(root, 2);
    ins(root, 5);
    ins(root, 7);
    ins(root, 8);
    ins(root, 11);
    ins(root, 1);
    ins(root, 4);
    ins(root, 2);
    ins(root, 3);
    ins(root, 7);
    return root;
}
