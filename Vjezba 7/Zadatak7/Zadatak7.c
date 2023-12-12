#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50


typedef struct _directory* PositionDirectory;
typedef struct _directory {
    char name[MAX];
    PositionDirectory pod;
    PositionDirectory next;
} Directory;


typedef struct _Stog* PositionStog;
typedef struct _Stog {
    PositionDirectory level;
    PositionStog next;
} Stog;

PositionDirectory createDir(char name[MAX]) {
    PositionDirectory novi = NULL;
    novi = (PositionDirectory)malloc(sizeof(Directory));
    if (!novi) {
        printf("Greska u alokaciji\n");
        return NULL;
    }
    strcpy(novi->name, name);
    novi->pod = NULL;
    novi->next = NULL;
    return novi;
}

PositionDirectory createSub(char name[MAX], PositionDirectory currentDir) {
    PositionDirectory novi = NULL;
    novi = createDir(name);
    if (!novi) {
        printf("Greska\n");
        return NULL;
    }
    novi->next = currentDir->pod;
    currentDir->pod = novi;
    return novi;
}

PositionDirectory cDir(char name[MAX], PositionDirectory currentDir) {
    PositionDirectory poddir = currentDir->pod;
    while (poddir != NULL) {
        if (strcmp(poddir->name, name) == 0) {
            return poddir;
        }
        poddir = poddir->next;
    }
    printf("Direktorji ne postoji\n", name);
    return currentDir;
}

int ispis(PositionDirectory currentDir) {
    printf("--%s--\n", currentDir->name);
    PositionDirectory poddir = currentDir->pod;
    while (poddir != NULL) {
        printf("\t - %s\n", poddir->name);
        poddir = poddir->next;
    }
    if (currentDir->pod == NULL) {
        printf("\t empty\n");
    }
    return 0;
}


PositionDirectory pop(PositionStog headStog) {
    PositionStog toDelete = NULL;
    PositionDirectory level = NULL;

    toDelete = headStog->next;
    if (!toDelete) {
        printf("Prazan stog\n");
        return NULL;
    }

    headStog->next = toDelete->next;
    level = toDelete->level;
    free(toDelete);

    return level;
}
PositionStog noviStogEl(PositionDirectory level) {
    PositionStog novi = NULL;

    novi = (PositionStog)malloc(sizeof(Stog));
    if (!novi) {
        perror("Greska u alokaciji\n");
        return NULL;
    }

    novi->level = level;
    novi->next = NULL;

    return novi;
}
int push(PositionStog headStog, PositionDirectory level) {
    PositionStog novi = NULL;

    novi = noviStogEl(level);
    if (!novi) {
        perror("Greska\n");
        return NULL;
    }

    novi->next = headStog->next;
    headStog->next = novi;
}


int main() {
    Directory headDirectory = {
        .name = {0},
        .pod = NULL,
        .next = NULL
    };
    PositionDirectory mainDir = createDir("C:");
    headDirectory.next = mainDir;

    PositionDirectory currentDir = mainDir;

    Stog headStog = {
        .level = NULL,
        .next = NULL
    };
    push(&headStog, currentDir);

    while (1) {
        printf("\n1. MD\n");
        printf("2. CD DIR\n");
        printf("3. CD..\n");
        printf("4. DIR\n");
        printf("5. IZLAZ\n");

        int x;
        scanf("%d", &x);

        if (x == 1) {
            char dirName[MAX];
            printf("Ime:\n");
            scanf("%s", dirName);
            createSub(dirName, currentDir);
        }
        else if (x == 2) {
            char dirName[MAX];
            printf("Ime:\n");
            scanf("%s", dirName);
            currentDir = cDir(dirName, currentDir);
            push(&headStog, currentDir);
        } else if (x == 3) {
            if (currentDir != mainDir) {
                currentDir = pop(&headStog);
                printf("%s\n", currentDir->name);
            }
            else {
                printf("U prvom direktoriju\n");
                
            }
        } else if (x == 4) {
            ispis(currentDir);
        } else if (x == 5) {
            break;
        }
    }

    return 0;
}

