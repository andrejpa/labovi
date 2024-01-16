#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _node* Position;
typedef struct _node {
    int value;
    Position left;
    Position right;
} Node;

Position initNode(int);
Position ins(Position, int);
Position removeNode(Position, int);
int inorder(Position);
int preorder(Position);
int postorder(Position);
Position searchFor(Position, int);


int main() {
    Position root = NULL;

    int rootvalue = 0;
    printf("Pocetna:");
    scanf("%d", &rootvalue);
    root = ins(root, rootvalue);

    while (1) {
        printf("1 Insert\n");
        printf("2 searchFor\n");
        printf("3 delete\n");
        printf("4 preorder\n");
        printf("5 inorder\n");
        printf("6 postorder\n");
        printf("7 exit\n");

        int input = 0;
        printf("Izbor : ");
        scanf("%d", &input);

        if (input == 1) {
            int value = 0;
            
            printf("Vrijednost :");
            scanf("%d", &value);
            
            ins(root, value);
        }
        else if (input == 2) {
            int value = 0;
            Position result = NULL;
            
            printf("Vrijednost :");
            scanf("%d", &value);
            
            result = searchFor(root, value);
            
            if (result != NULL)
                printf("Ne postoji");
        }
        else if (input == 3) {
            int value = 0;
            
            printf("Vrijednost :");
            scanf("%d", &value);
            
            removeNode(root, value);
        }
        else if (input == 4) {
            preorder(root);
            printf("\n");
        }
        else if (input == 5) {
            inorder(root);
            printf("\n");
        }
        else if (input == 6) {
            postorder(root);
            printf("\n");
        }
        else if (input == 7) {
            break;
        }
    }
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
Position removeNode(Position root, int value) {
    if (root == NULL)
        return root;

    if (root->value > value){
        root->left = removeNode(root->left, value);
        return root;
    }  
    else if (root->value < value){
        root->right = removeNode(root->right, value);
        return root;
    }
    
    
    if (root->left == NULL) {
        Position temp = root->right;
        free(root);
        return temp;
    }
    else if (root->right == NULL) {
        Position temp = root->left;
        free(root);
        return temp;
    }
    
    else {
        Position temp = root;
 
        Position tempChild = root->right;
        
        while (tempChild->left != NULL) {
            temp = tempChild;
            tempChild = tempChild->left;
        }
 

        if (temp != root)
            temp->left = tempChild->right;
        else
            temp->right = tempChild->right;
 
        root->value = tempChild->value;
 
        free(tempChild);
        return root;
    }
}
int inorder(Position root) {
    if (root == NULL)
        return;
    
    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);
    
}
int preorder(Position root) {
    if (root == NULL)
        return;
        
    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);
    
}
int postorder(Position root) {
    if (root == NULL)
        return;
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);

}
Position searchFor(Position root, int value) {
    if (root == NULL || root->value == value)
        return root;

    if (value < root->value)
        return searchFor(root->left, value);
    else
        return searchFor(root->right, value);
}
