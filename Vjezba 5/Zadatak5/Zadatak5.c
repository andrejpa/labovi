#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct _Element* Position;
typedef struct _Element {
    double number;
    Position next;
} Element;

int push(Position head, double number);
int pop(Position head, double* result);

int evaluatePostfix(FILE* file, double* result);

int main() {
    Element head = { .number = 0, .next = NULL };
    double result = 0;

    FILE* file = fopen("dat.txt", "r");
    if (file == NULL) {
        printf("datoteka nije otvorena");
        return 1;
    }

    if (evaluatePostfix(file, &result)) {
        printf("rezultat izraza je: %.2lf\n", result);
    } else {
        printf("neispravan postfix izraz\n");
    }

    fclose(file);

    return 0;
}

int evaluatePostfix(FILE* file, double* result) {
    Element head = { .number = 0, .next = NULL };
    char buffer[MAX_SIZE];
    char* token;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        token = strtok(buffer, " \n");
        while (token != NULL) {
            if (isdigit(token[0])) {
                double number = atof(token);
                push(&head, number);
            } 
            else {
                double operand1, operand2;
                if (!pop(&head, &operand1) || !pop(&head, &operand2)) {
                    return 0;
                }
                
                switch (token[0]) {
                    case '+':
                        push(&head, operand2 + operand1);
                        break;
                    case '-':
                        push(&head, operand2 - operand1);
                        break;
                    case '*':
                        push(&head, operand2 * operand1);
                        break;
                    case '/':
                        if (operand1 == 0) {
                            return 0; 
                        }
                        push(&head, operand2 / operand1);
                        break;
                    default:
                        return 0;
                }
            }
            token = strtok(NULL, " \n");
        }
    }

    if (pop(&head, result) && head.next == NULL) {
        return 1;
    } else {
        return 0;
    }
}

int push(Position head, double number) {
    Position newElement = (Position)malloc(sizeof(Element));
    if (newElement == NULL) {
        perror("alokacija memorije nije uspjela");
        return 0;
    }
    newElement->number = number;
    newElement->next = head->next;
    head->next = newElement;
    return 1;
}

int pop(Position head, double* result) {
    if (head->next == NULL) {
        return 0;
    }
    Position temp = head->next;
    head->next = temp->next;
    *result = temp->number;
    free(temp);
    return 1;
}
