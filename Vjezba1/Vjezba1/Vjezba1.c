#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 1024

typedef struct{
    char ime[MAX];
    char prezime[MAX];
    int bod;
}studenti;

int main() {
    FILE *file = NULL;
    int cline = 0;
    char *buffer = NULL;
    int i = 0;
    studenti *stud;
    double *rel = 0;
    int max = 0;
    
    
    file = fopen("studenti.txt", "r");
    if (file == NULL) {
        printf("Nemoguće otvoriti datoteku.\n");
        return -1;
    }
    
    buffer = (char*)malloc(MAX * sizeof(char));
        if (buffer == NULL) {
            printf("Neuspjela alokacija memorije za buffer.\n");
            return -1;
        }
    while (getline(&buffer, (size_t*)buffer, file) != -1) {
        cline++;
    }
    
    printf("Broj studenata u datoteci: %d\n", cline);
    
    stud = (studenti*)malloc(sizeof(studenti) * cline);
    rel = (double*)malloc(sizeof(double) * cline);
    rewind(file);
    
    for(i=0; i<cline ; i++)
    {
        fscanf(file, "%s %s %d", stud[i].ime, stud[i].prezime, &stud[i].bod);
         if(stud[i].bod > max)
            max = stud[i].bod;
        
    }
    
    for(i = 0; i<cline;i++)
    {
        rel[i] = ((double)(stud[i].bod)/(double)max) *100;
    }
    for(i=0; i<cline ; i++)
    {
        printf("%s \t %s \t %d \t %lf \n", stud[i].ime, stud[i].prezime, stud[i].bod, rel[i]);
    }
    
    fclose(file);
    free(buffer);
    free(stud);

    return 0;
}
