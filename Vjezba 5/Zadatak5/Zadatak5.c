#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (50)
#define MAXLENGTH (1024)


typedef struct _Element* Position;
typedef struct _Element {
	double number;
	Position next;
} Element;

int push(Position head, Position noviEle);
int pop(Position head, double* result);

int readf(char* imeDat, char* buffer);
int ispisi(Position first);

int izrPostfix(Position head, char* imeDat, double* result);
int stringToPostfix(Position head, char* buffer, double* result);
int popIzracun(Position head, char plus, double* result);

Position newEle(double number);
int main() {
    Element head = {.number = 0, .next = NULL };
	double result = 0;

    if (izrPostfix(&head, "postfix.txt", &result) == 1) {
        printf("Result is: %0.1lf\n", result);
    }

	return 1;
}

int izrPostfix(Position head, char* imeDat, double* result) {
    char buffer[MAXLENGTH] = { 0 };
    int status = 0;

    if (readf(imeDat, buffer) != 1 || !buffer) {
        return 0;
    }

    status = stringToPostfix(head, buffer, result);
    if (status != 1) {
        return 0;
    }

    return 1;
}

int readf(char* imeDat, char* buffer) {
    FILE* dat = NULL;
    dat = fopen(imeDat, "r");
    if (dat == NULL) {
        perror("Datoteka nije otvorena\n");
        return 0;
    }

    fgets(buffer, MAXLENGTH, dat);
    printf("|%s|\n", buffer);

    fclose(dat);

    return 1;
}

int stringToPostfix(Position head, char* buffer, double* result)
{
    char* currentBuffer = buffer;
    int status = 0;
    int numBytes = 0;
    char plus = 0;
    double number = 0.0;
    Position noviEle = NULL;

    while (strlen(currentBuffer) > 0) {
        status = sscanf(currentBuffer, " %lf %n", &number, &numBytes);
        if (status != 1) {
            sscanf(currentBuffer, " %c %n", &plus, &numBytes);
            status = popIzracun(head, plus, result);

            if (status != 1) {
                return 0;
            }

            number = *result;
        }

        noviEle = newEle(number);
        if (!noviEle) {
            return 0;
        }

        currentBuffer += numBytes;
        printf("|%s|\t", currentBuffer);
        push(head, noviEle);
    }
    return 0;

}


Position newEle(double number)
{
    Position noviEle = NULL;

    noviEle = (Position)malloc(sizeof(Element));
    if (!noviEle) {
        perror("Nije alocirano\n");
        return NULL;
    }

    noviEle->number = number;
    noviEle->next = NULL;

    return noviEle;
}

int push(Position head, Position noviEle) {
    noviEle->next = head->next;
    head->next = noviEle;

    ispisi(head->next);

    return 1;
}

int ispisi(Position first) {
    Position current = first;

    while (current) {
        printf(" %lf", current->number);
        current = current->next;
    }
    printf("\n");

    return 1;
}

int pop(Position head, double* result) {
    Position del = NULL;

    del = head->next;
    if (!del) {
        printf("Prazan stog\n");
        return -1;
    }

    head->next = del->next;
    *result = del->number;
    free(del);

    return 1;
}

int popIzracun(Position head, char plus, double* result) {
    
    int status1 = 0;
    int status2 = 0;

    double op1 = 0;
    double op2 = 0;
    
    status1 = pop(head, &op1);
    if (status1 != 1) {
        return 0;
    }

    status2 = pop(head, &op2);
    if (status2 != 1) {
        return 0;
    }

    switch (plus) {
    case '+':
        *result = op2 + op1;
        break;
    case '*':
        *result = op2 * op1;
        break;
    case '-':
        *result = op2 - op1;
        break;
    case '/':
        *result = op2 / op1;
        break;
        
    return 0;
    }

    return 1;
}