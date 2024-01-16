#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _grad* GradPosition;
typedef struct _grad
{
	char name[100];
	int broj;
	GradPosition left;
	GradPosition right;
	GradPosition next;
}Grad;

typedef struct _drzava* DrzavaPosition;
typedef struct _drzava
{
	char name[100];
	DrzavaPosition left;
	DrzavaPosition right;
	DrzavaPosition next;
	GradPosition gradRootTree;
	Grad gradListHead;
} Drzava;

DrzavaPosition ReadFile(DrzavaPosition drzavaHead, DrzavaPosition drzavaRoot);

int createDrzava(char* drzavaIme, char* drzavaFile, DrzavaPosition* countries);
DrzavaPosition createDrzavaEl(char* drzavaIme);

GradPosition createGrad(char* gradName, int gradPopulation);

int insertGradSorted(GradPosition gradHeadList, GradPosition newGrad);
int insertGrad(GradPosition gradCurrent, GradPosition newGrad);
GradPosition insertGradRoot(GradPosition gradRootTree, GradPosition newGrad);

int insertDrzavaSorted(DrzavaPosition gradHeadList, DrzavaPosition newDrzava);
int insertDrzava(DrzavaPosition drzavaCurrent, DrzavaPosition newDrzava);
DrzavaPosition insertDrzavaRoot(DrzavaPosition drzavaRoot, DrzavaPosition newDrzava);


int printDrzavaList(DrzavaPosition drzavaHead);
int printDrzavaTree(DrzavaPosition drzavaRoot);
int printGradList(GradPosition gradHeadList);
int printGradTree(GradPosition gradRootTree);

int main() {
	Drzava drzavaHead = {
		.name = {0},
		.next = NULL,
		.left = NULL,
		.right = NULL,
		.gradRootTree = NULL,
		.gradListHead = {.name = {0}, .broj = 0, .next = NULL, .left = NULL,.right = NULL}
	};

	DrzavaPosition drzavaRoot = NULL;

	DrzavaPosition drzavaCurrentList = NULL;
	DrzavaPosition drzavaCurrentTree = NULL;
	DrzavaPosition countries[] = { NULL, NULL };


	drzavaRoot = ReadFile(&drzavaHead, drzavaRoot);
	printf("lista");
	printDrzavaList(&drzavaHead);
	printf("\nstablo");
	printDrzavaTree(drzavaRoot);
	
	return 1;
}

DrzavaPosition ReadFile(DrzavaPosition drzavaHead, DrzavaPosition drzavaRoot) {
	FILE* filePointer = NULL;
	char drzavaIme[100] = { 0 };
	char drzavaFile[100] = { 0 };
	char nullString[100] = { 0 };
	DrzavaPosition newDrzavaList = NULL;
	DrzavaPosition newDrzavaTree = NULL;
	DrzavaPosition countries[] = { NULL, NULL};

	filePointer = fopen("drzave.txt", "r");
	if (!filePointer) {
		printf("drzave.txt ne postoji\n");
		return 0;
	}

	while (!feof(filePointer)) {
		fscanf(filePointer, " %s %s", drzavaIme, drzavaFile);
		createDrzava(drzavaIme, drzavaFile, countries);
		newDrzavaList = countries[0];
		newDrzavaTree = countries[1];
		insertDrzavaSorted(drzavaHead, newDrzavaList);
		drzavaRoot = insertDrzavaRoot(drzavaRoot, newDrzavaTree);
	}

	fclose(filePointer);

	return drzavaRoot;
}

int createDrzava(char* drzavaIme, char* drzavaFile, DrzavaPosition* countries) {
	char drzavaLocation[100] = { 0 };
	FILE* drzavaFilePointer = NULL;
	DrzavaPosition newDrzavaList = NULL;
	DrzavaPosition newDrzavaTree = NULL;
	GradPosition newGradList = NULL;
	GradPosition newGradTree = NULL;
	char gradName[100] = { 0 };
	int gradPopulation = 0;

	strcpy(drzavaLocation, drzavaFile);

	drzavaFilePointer = fopen(drzavaLocation, "r");
	if (!drzavaFilePointer) {
		printf("greska u otvaranju\n");
		return 0;
	}

	newDrzavaList = createDrzavaEl(drzavaIme);
	newDrzavaTree = createDrzavaEl(drzavaIme);

	while (!feof(drzavaFilePointer)) {
		fscanf(drzavaFilePointer, "%s %d", gradName, &gradPopulation);
		newGradList = createGrad(gradName, gradPopulation);
		newGradTree = createGrad(gradName, gradPopulation);
		insertGradSorted(&newDrzavaTree->gradListHead, newGradList);
		newDrzavaList->gradRootTree = insertGradRoot(newDrzavaList->gradRootTree, newGradTree);
	}
	countries[0] = newDrzavaList;
	countries[1] = newDrzavaTree;

	fclose(drzavaFilePointer);

	return 1;
}

DrzavaPosition createDrzavaEl(char* drzavaIme) {
	DrzavaPosition drzava = NULL;
	drzava = (DrzavaPosition)malloc(sizeof(Drzava));
	if (!drzava) {
		printf("greska u alokaciji memorije\n");
		return NULL;
	}


	strcpy(drzava->name, drzavaIme);
	drzava->next = NULL;
	drzava->left = NULL;
	drzava->right = NULL;
	drzava->gradRootTree = NULL;
	
	drzava->gradListHead.broj = 0;
	drzava->gradListHead.next = NULL;
	drzava->gradListHead.left = NULL;
	drzava->gradListHead.right = NULL;

	return drzava;
}

GradPosition createGrad(char* gradName, int gradPopulation) {
	GradPosition grad = NULL;
	grad = (GradPosition)malloc(sizeof(Grad));
	if (!grad) {
		printf("greska u alokaciji memorije\n");
		return NULL;
	}

	strcpy(grad->name, gradName);
	grad->broj = gradPopulation;
	grad->next = NULL;
	grad->left = NULL;
	grad->right = NULL;

	return grad;
}

int insertGradSorted(GradPosition gradHeadList, GradPosition newGrad) {
	GradPosition gradCurrent = gradHeadList;
	while (gradCurrent->next != NULL && gradCurrent->next->broj > newGrad->broj) {
		gradCurrent = gradCurrent->next;
	}

	insertGrad(gradCurrent, newGrad);

	return 1;
}

int insertGrad(GradPosition gradCurrent, GradPosition newGrad) {
	newGrad->next = gradCurrent->next;
	gradCurrent->next = newGrad;

	return 1;
}

GradPosition insertGradRoot(GradPosition gradRootTree, GradPosition newGrad) {
	if (gradRootTree == NULL)
		return newGrad;

	if (newGrad->broj > gradRootTree->broj)
		gradRootTree->left = insertGradRoot(gradRootTree->left, newGrad);
	else if (newGrad->broj <= gradRootTree->broj)
		gradRootTree->right = insertGradRoot(gradRootTree->right, newGrad);

	return gradRootTree;
}

int insertDrzavaSorted(DrzavaPosition gradHeadList, DrzavaPosition newDrzava) {
	DrzavaPosition drzavaCurrent = gradHeadList;
	while (drzavaCurrent->next != NULL && strcmp(drzavaCurrent->next->name, newDrzava->name) < 0) {
		drzavaCurrent = drzavaCurrent->next;
	}

	insertDrzava(drzavaCurrent, newDrzava);

	return 1;
}

int insertDrzava(DrzavaPosition drzavaCurrent, DrzavaPosition newDrzava) {
	newDrzava->next = drzavaCurrent->next;
	drzavaCurrent->next = newDrzava;

	return 1;
}

DrzavaPosition insertDrzavaRoot(DrzavaPosition drzavaRoot, DrzavaPosition newDrzava) {
	if (drzavaRoot == NULL)
		return newDrzava;

	if (strcmp(drzavaRoot->name, newDrzava->name) > 0)
		drzavaRoot->left = insertDrzavaRoot(drzavaRoot->left, newDrzava);
	else if (strcmp(drzavaRoot->name, newDrzava->name) <= 0)
		drzavaRoot->right = insertDrzavaRoot(drzavaRoot->right, newDrzava);

	return drzavaRoot;
}

int printDrzavaList(DrzavaPosition drzavaHead) {
	DrzavaPosition currentDrzava = drzavaHead->next;
	while (currentDrzava != NULL) {
		printf("\n%s", currentDrzava->name);
		printGradTree(currentDrzava->gradRootTree);
		currentDrzava = currentDrzava->next;
	}
}

int printDrzavaTree(DrzavaPosition drzavaRoot) {
	if (drzavaRoot) {
		printDrzavaTree(drzavaRoot->left);
		printf("\n%s", drzavaRoot->name);
		printGradList(&drzavaRoot->gradListHead);
		printDrzavaTree(drzavaRoot->right);
	}
	return 1;
}

int printGradList(GradPosition gradHeadList) {
	GradPosition currentGrad = gradHeadList->next;
	while (currentGrad != NULL) {
		printf("\n\t%s %d", currentGrad->name, currentGrad->broj);
		currentGrad = currentGrad->next;
	}
}

int printGradTree(GradPosition gradRootTree) {
	if (gradRootTree) {
		printGradTree(gradRootTree->left);
		printf("\n\t%s %d", gradRootTree->name, gradRootTree->broj);
		printGradTree(gradRootTree->right);
	}
	return 1;
}

