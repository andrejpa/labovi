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

	dat = fopen("lista.txt", "r");

	while (!feof)
	{
		if (fgetc(dat) == '\n')
		{
			noRows++;
		}

	}
	printf("%d", noRows);
	return 0;
}