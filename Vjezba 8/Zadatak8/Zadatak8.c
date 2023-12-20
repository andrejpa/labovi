#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _node* Position;
typedef struct _node {
    int x;
    Position L;
    Position R;
} Node;

Position create(int x) {
    Position newNode = NULL;
    newNode = (Position)malloc(sizeof(Node));
    if (!newNode) {
        printf("Greska\n");
        return NULL;
    }
    newNode->x = x;
    newNode->L = NULL;
    newNode->R = NULL;
    return newNode;
}
Position insert(Position root, int x) {
    if (root == NULL)
        return create(x);

    if (x < root->x)
        root->L = insert(root->L, x);
    else if (x >= root->x)
        root->R = insert(root->R, x);

    return root;
}
Position brisi(Position root, int x) {
    if (root == NULL)
        return root;

    if (x < root->x)
        root->L = brisi(root->L, x);
    else if (x > root->x)
        root->R = brisi(root->R, x);
    else {

        if (root->L == NULL) {
            Position temp = root->R;
            free(root);
            return temp;
        }
        else if (root->R == NULL) {
            Position temp = root->L;
            free(root);
            return temp;
        }

        Position temp = root->R;
        while (temp->L != NULL)
            temp = temp->L;

        root->x = temp->x;

        root->R = brisi(root->R, temp->x);
    }

    return root;
}
int inorder(Position root) {
    if (root != NULL) {
        inorder(root->L);
        printf("%d ", root->x);
        inorder(root->R);
    }
    return 1;
}
int preorder(Position root) {
    if (root != NULL) {
        printf("%d ", root->x);
        preorder(root->L);
        preorder(root->R);
    }
    return 1;
}

int postorder(Position root) {
    if (root != NULL) {
        postorder(root->L);
        postorder(root->R);
        printf("%d ", root->x);
    }
    return 1;
}

Position trazi(Position root, int x) {
    if (root == NULL || root->x == x)
        return root;

    if (x < root->x)
        return trazi(root->L, x);

    return trazi(root->R, x);
}


int main() {
    Position root = NULL;

    int rootX = 0;
    printf("Pocetna:");
    scanf("%d", &rootX);
    root = insert(root, rootX);

    while (1) {
        printf("1 insert\n");
        printf("2 trazi\n");
        printf("3 delete\n");
        printf("4 preorder\n");
        printf("5 inorder\n");
        printf("6 postorder\n");
        printf("7 levelorder\n");
        printf("8 exit\n");

        int x = 0;
        printf("Enter your x: ");
        scanf("%d", &x);

        if (x == 1) {
            int x = 0;
            printf("Vrijednost:");
            scanf("%d", &x);
            insert(root, x);
        }
        else if (x == 2) {
            int x = 0;
            Position result = NULL;
            printf("Vrijednost:");
            scanf("%d", &x);
            result = trazi(root, x);
            if (result != NULL)
                printf("Ne postoji");
        }
        else if (x == 3) {
            int x = 0;
            printf("Vrijednost:");
            scanf("%d", &x);
            brisi(root, x);
        }
        else if (x == 4) {
            preorder(root);
            printf("\n");
        }
        else if (x == 5) {
            inorder(root);
            printf("\n");
        }
        else if (x == 6) {
            postorder(root);
            printf("\n");
        }
        else if (x == 7) {
            levelorder(root);
            printf("\n");
        }
        else if (x == 8) {
            break;
        }
    }
}
