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


Position createNode(int value) {
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
        return createNode(value);

    if (value <= root->value)
        root->left = ins(root->left, value);
    else if (value > root->value)
        root->right = ins(root->right, value);

    return root;
}
int levelOrder(Position root) {
    if (root == NULL)
        return 0;

    Position queue[100] = { 0 };
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Position current = queue[front++];

        printf("%d ", current->value);

        if (current->left != NULL)
            queue[rear++] = current->left;

        if (current->right != NULL)
            queue[rear++] = current->right;
    }
    return 1;
}


int replace(Position root) {
    if (root == NULL) {
        return 0;
    }

    int leftValue = replace(root->left);
    int rightValue = replace(root->right);

    int originalValue = root->value;

    root->value = leftValue + rightValue;

    return originalValue + root->value;
}

int ran() {
    return (rand() % 80) + 10;
}
int main() {

    srand(time(0));
    
    Position root = NULL;
    Position rootRan = NULL;

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

    levelOrder(root);
    
    replace(root);
    printf("\n");
    levelOrder(root);


    rootRan = ins(rootRan, ran());
    for (int i = 0; i < 10; i++) {
        ins(rootRan, ran());
    }
    printf("\n");
    levelOrder(rootRan);

}