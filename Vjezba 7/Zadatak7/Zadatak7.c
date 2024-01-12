#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50


typedef struct folder* PositionFolder;
typedef struct folder {
    char name[MAX];
    PositionFolder pod;
    PositionFolder next;
} Folder;


typedef struct _Stog* PositionStog;
typedef struct _Stog {
    PositionFolder level;
    PositionStog next;
} Stog;

PositionFolder createFolder(char name[MAX]);

PositionFolder createSub(char name[MAX], PositionFolder currentDir);

PositionFolder CD(char name[MAX], PositionFolder currentDir);

int DIR(PositionFolder currentDir);


PositionFolder pop(PositionStog headStog);

void push(PositionStog headStog, PositionFolder level);


int main() {
    Folder headFolder = {
        .name = {0},
        .pod = NULL,
        .next = NULL
    };
    
    PositionFolder mainDir = createFolder("C:");
    headFolder.next = mainDir;

    PositionFolder currentDir = mainDir;

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
            printf("\tIme:\n");
            scanf("%s", dirName);
            createSub(dirName, currentDir);
        }
        else if (x == 2) {
            char dirName[MAX];
            
            printf("\tIme:\n");
            scanf("%s", dirName);
            
            push(&headStog, currentDir);
                        currentDir = CD(dirName, currentDir);

        } else if (x == 3) {
            if (currentDir != mainDir) {
                currentDir = pop(&headStog);
                printf("%s\n", currentDir->name);
            }
            else {
                printf("\tU prvom direktoriju\n");
                
            }
        } else if (x == 4) {
            DIR(currentDir);
        } else if (x == 5) {
            break;
        }
    }

    return 0;
}
PositionFolder createFolder(char name[MAX]) {
    PositionFolder newFolder = (PositionFolder)malloc(sizeof(Folder));
    if (!newFolder) {
        printf("Greska u alokaciji\n");
        return NULL;
    }
    
    strcpy(newFolder->name, name);
    newFolder->pod = NULL;
    newFolder->next = NULL;
    return newFolder;
}

PositionFolder createSub(char name[MAX], PositionFolder currentDir) {
    PositionFolder newFolder = createFolder(name);
    if (!newFolder) {
        printf("Greska\n");
        return NULL;
    }
    newFolder->next = currentDir->pod;
    currentDir->pod = newFolder;
}

PositionFolder CD(char name[MAX], PositionFolder currentDir) {
    PositionFolder insideFolder = currentDir->pod;
    
    while (insideFolder != NULL) {
        if (strcmp(insideFolder->name, name) == 0) {
            return insideFolder;
        }
        insideFolder = insideFolder->next;
    }
    
    printf("Direktorji ne postoji\n");
    return currentDir;
}

int DIR(PositionFolder currentDir) {
    printf("|->%s\n", currentDir->name);
    PositionFolder insideFolder = currentDir->pod;
    
    while (insideFolder != NULL) {
        printf("\t - %s\n", insideFolder->name);
        insideFolder = insideFolder->next;
    }
    
    if (currentDir->pod == NULL) {
        printf("\t Prazan folder\n");
    }
}


PositionFolder pop(PositionStog headStog) {
    PositionStog toDelete = headStog->next;
    if (!toDelete) {
        printf("Prazan stog\n");
        return;
    }
    
    PositionFolder level = toDelete->level;
    headStog->next = toDelete->next;
    free(toDelete);

    return level;
}

void push(PositionStog headStog, PositionFolder level) {
    PositionStog newFolder = (PositionStog)malloc(sizeof(level));
    if (!newFolder) {
        perror("Greska\n");
        return;
    }
    
    newFolder->level = level;
    newFolder->next = headStog->next;
    headStog->next = newFolder;
}
