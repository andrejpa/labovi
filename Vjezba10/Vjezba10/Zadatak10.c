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

struct _hash_table {
	DrzavaPosition buckets[11];
};
typedef struct _hash_table HashTable;

DrzavaPosition ReadFile(DrzavaPosition drzavaHead, DrzavaPosition drzavaRoot, HashTable* hashTable);

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

int findCountries(DrzavaPosition drzavaHeadList, DrzavaPosition drzavaRootTree, DrzavaPosition* countries);
DrzavaPosition searchCountryTree(DrzavaPosition drzavaRootTree, char* drzavaName);
HashTable* createHashTable() {
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	if (!hashTable) {
		printf("Can't allocate memory for hash table!\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 11; i++) {
		hashTable->buckets[i] = NULL;
	}

	return hashTable;
}

// Function to calculate the hash value for a given country name
unsigned int calculateHash(char* countryName) {
	unsigned int hashValue = 0;
	int i = 0;
	while (i < 5 && countryName[i] != '\0') {
		hashValue += (unsigned int)countryName[i];
		i++;
	}
	return hashValue % 11;
}

// Function to insert a country into the hash table
void insertCountryIntoHashTable(HashTable* hashTable, DrzavaPosition newCountry) {
	unsigned int index = calculateHash(newCountry->name);

	if (hashTable->buckets[index] == NULL) {
		hashTable->buckets[index] = newCountry;
		newCountry->next = NULL;
	}
	else {
		// Handle collision by adding the new country to the linked list at this index
		newCountry->next = hashTable->buckets[index];
		hashTable->buckets[index] = newCountry;
	}
}

// Function to search for a country in the hash table
DrzavaPosition searchCountryInHashTable(HashTable* hashTable, char* countryName) {
	unsigned int index = calculateHash(countryName);
	DrzavaPosition currentCountry = hashTable->buckets[index];

	while (currentCountry != NULL && strcmp(currentCountry->name, countryName) != 0) {
		currentCountry = currentCountry->next;
	}

	return currentCountry;
}

int printAllCountriesFromHash(HashTable* hashTable) {
	for (int index = 0; index < 11; index++) {
		DrzavaPosition currentCountry = NULL;
		currentCountry = hashTable->buckets[index];

		while (currentCountry != NULL) {
			printf("\n%s", currentCountry->name);
			printGradTree(currentCountry->gradRootTree);
			currentCountry = currentCountry->next;
		}
	}
}

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
	DrzavaPosition countries[] = { NULL, NULL, NULL};


	HashTable* hashTable = createHashTable();

	drzavaRoot = ReadFile(&drzavaHead, drzavaRoot, hashTable);
	printf("lista");
	printDrzavaList(&drzavaHead);
	printf("\nstablo");
	printDrzavaTree(drzavaRoot);
	printf("\nHASH");
	printAllCountriesFromHash(hashTable);
	
	char searchCountryName[100] = { 0 };
	printf("\nEnter country name to search in the hash table: ");
	scanf(" %s", searchCountryName);

	DrzavaPosition foundCountry = searchCountryInHashTable(hashTable, searchCountryName);

	if (foundCountry != NULL) {
		printf("\nCountry found in the hash table: %s", foundCountry->name);
	}
	else {
		printf("\nCountry not found in the hash table.");
	}

	// Free allocated memory
	// ...

	return EXIT_SUCCESS;
}

DrzavaPosition ReadFile(DrzavaPosition drzavaHead, DrzavaPosition drzavaRoot, HashTable* hashTable) {
	FILE* filePointer = NULL;
	char drzavaIme[100] = { 0 };
	char drzavaFile[100] = { 0 };
	char nullString[100] = { 0 };
	DrzavaPosition newDrzavaList = NULL;
	DrzavaPosition newDrzavaTree = NULL;
	DrzavaPosition newDrzavaHash = NULL;

	DrzavaPosition countries[] = { NULL, NULL, NULL};

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
		newDrzavaHash = countries[2];

		insertDrzavaSorted(drzavaHead, newDrzavaList);
		drzavaRoot = insertDrzavaRoot(drzavaRoot, newDrzavaTree);
				insertCountryIntoHashTable(hashTable, newDrzavaHash);

	}

	fclose(filePointer);

	return drzavaRoot;
}

int createDrzava(char* drzavaIme, char* drzavaFile, DrzavaPosition* countries) {
	char drzavaLocation[100] = { 0 };
	FILE* drzavaFilePointer = NULL;
	DrzavaPosition newDrzavaList = NULL;
	DrzavaPosition newDrzavaTree = NULL;
	DrzavaPosition newDrzavaHash  = NULL;

	GradPosition newGradList = NULL;
	GradPosition newGradTree = NULL;
	GradPosition newGradHash  = NULL;

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
	newDrzavaHash = createDrzavaEl(drzavaIme);


	while (!feof(drzavaFilePointer)) {
		fscanf(drzavaFilePointer, "%s %d", gradName, &gradPopulation);
		newGradList = createGrad(gradName, gradPopulation);
		newGradTree = createGrad(gradName, gradPopulation);
		newGradHash = createGrad(gradName, gradPopulation);

		insertGradSorted(&newDrzavaTree->gradListHead, newGradList);
		newDrzavaList->gradRootTree = insertGradRoot(newDrzavaList->gradRootTree, newGradTree);
				newDrzavaHash->gradRootTree = insertGradRoot(newDrzavaHash->gradRootTree, newGradHash);

	}
	countries[0] = newDrzavaList;
	countries[1] = newDrzavaTree;
	countries[2] = newDrzavaHash;


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

int findCountries(DrzavaPosition drzavaHeadList, DrzavaPosition drzavaRootTree, DrzavaPosition* countries){
    DrzavaPosition countryCurrentList = drzavaHeadList->next;
	DrzavaPosition countryCurrentTree = drzavaRootTree;
	char countryName[100] = { 0 };
	printf("\n\nEnter country name: ");
	scanf(" %s", countryName);

	while (countryCurrentList != NULL && strcmp(countryCurrentList->name, countryName)) {
		countryCurrentList = countryCurrentList->next;
	}

	countryCurrentTree = searchCountryTree(drzavaRootTree, countryName);

	countries[0] = countryCurrentList;
	countries[1] = countryCurrentTree;

	return EXIT_SUCCESS;
}

DrzavaPosition searchCountryTree(DrzavaPosition drzavaRootTree, char* drzavaName){
    if (drzavaRootTree == NULL || strcmp(drzavaRootTree->name, drzavaName) == 0)
		return drzavaRootTree;

	if (strcmp(drzavaRootTree->name, drzavaName) > 0)
		return searchCountryTree(drzavaRootTree->left, drzavaName);

	return searchCountryTree(drzavaRootTree->right, drzavaName);
}
