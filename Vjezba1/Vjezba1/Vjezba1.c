#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX 50

typedef struct {
	char ime[MAX];
	char prezime[MAX];
	int bodovi;
}student;

int main()
{
	student stud;
	FILE *dat;
	int noRows;
	char c;

	dat = fopen("lista.txt", "r");

	for (c = getc(dat); c != EOF; c = getc(dat))
		if (c == '\n')
			noRows = noRows + 1;
	printf("%d", noRows);
	return 0;
}