#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct Poly;
typedef struct Poly* Position;
typedef struct Poly
{
	int koef;
	int eks;
	Position next;
} Poly;


int ReadFile(Position head1, Position head2);
int StringToPol(Position head, char* buffer);
int Remove(Position previous);
int insertAfter(Position position, Position newPoly);
int Insert(Position head, Position newPoly);
int Merge(Position position, Position newPoly);
Position CreatePoly(int koef, int eks);
int PrintPoly(char* name, Position first);
int CreateAndinsertAfter(int koef, int eks, Position position);
int AddPoly1(Position resultHead, Position head1, Position head2);
int AddPoly2(Position resultHead, Position head1, Position head2);
int MultiplyPoly(Position resultHead, Position head1, Position head2);
int FreeMemory(Position head);

int main()
{
	Poly head1 = { .koef = 0, .eks = 0, .next = NULL };
	Poly head2 = { .koef = 0, .eks = 0, .next = NULL };
	Poly headAdd = { .koef = 0, .eks = 0, .next = NULL };
	Poly headMul = { .koef = 0, .eks = 0, .next = NULL };
	char fileName[50] = { 0 };

	if (ReadFile(&head1, &head2) == 1)
	{
		AddPoly1(&headAdd, &head1, &head2);
		MultiplyPoly(&headMul, &head1, &head2);

		PrintPoly("zbroj", headAdd.next);
		PrintPoly("umnozak", headMul.next);

		FreeMemory(&head1);
		FreeMemory(&head2);
		FreeMemory(&headAdd);
		FreeMemory(&headMul);
	}

	return 1;
}

int ReadFile(Position head1, Position head2){
	FILE* file = NULL;
	char buffer[1024] = { 0 };
	int status = 1;

	file = fopen("polinomi.txt", "r");
	if (!file)
	{
		printf("greska\n");
		return -1;
	}

	fgets(buffer, 1024, file);
	if (!StringToPol(head1, buffer))
	{
		return 0;
	}

	fgets(buffer, 1024, file);
	if (!StringToPol(head2, buffer))
	{
		return 0;
	}

	fclose(file);

	return 1;
}

int StringToPol(Position head, char* buffer){
	char* newBuffer = buffer;
	int koef = 0;
	int eks = 0;
	int numBytes = 0;
	int status = 0;
	Position newPoly = NULL;

	while (strlen(newBuffer) > 0)
	{
		if (sscanf(newBuffer, " %d %d %n", &koef, &eks, &numBytes) != 2)
		{
			printf("format error\n");
			return 0;
		}

		newPoly = CreatePoly(koef, eks);
		if (!newPoly)
		{
			return 0;
		}

		Insert(head, newPoly);


		newBuffer += numBytes;
	}

	return 1;
}

int Remove(Position previous)
{
	Position toDelete = NULL;

	toDelete = previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return 1;
}

int insertAfter(Position position, Position newPoly)
{
	newPoly->next = position->next;
	position->next = newPoly;

	return 1;
}

int Merge(Position position, Position newPoly)
{
	if (position->next == NULL || position->next->eks != newPoly->eks)
	{
		insertAfter(position, newPoly);
	}
	else 
	{
		int resultkoef = position->next->koef + newPoly->koef;
		if (resultkoef == 0) 
		{
			Remove(position);
		}
		else
		{
			position->next->koef = resultkoef;
		}
		free(newPoly);
	}

	return 1;
}

int Insert(Position head, Position newPoly)
{
	Position temp = head;

	while (temp->next != NULL && temp->next->eks < newPoly->eks) 
	{
		temp = temp->next;
	}

	Merge(temp, newPoly);

	return 1;
}

Position CreatePoly(int koef, int eks)
{
	Position Poly = NULL;

	Poly = (Position)malloc(sizeof(Poly));
	if (!Poly)
	{
		printf("greska u alokaciji\n");
		return Poly; 
	}

	Poly->koef = koef;
	Poly->eks = eks;
	Poly->next = NULL;

	return Poly;
}

int PrintPoly(char* name, Position first) {
	printf(" %s = ", name);
	if (first) {
		if (first->eks < 0) {
			if (first->koef == 1)
			{
				printf("x^(%d)", first->eks);
			}
			else
			{
				printf("%dx^(%d)", first->koef, first->eks);
			}
		}
		else {
			if (first->koef == 1)
			{
				printf("x^%d", first->eks);
			}
			else
			{
				printf("%dx^%d", first->koef, first->eks);
			}
		}

		first = first->next;
	}

	for (; first != NULL; first = first->next) {
		if (first->koef < 0) {
			if (first->eks < 0) {
				printf(" - %dx^(%d)", abs(first->koef), first->eks);
			}
			else {
				printf(" - %dx^%d", abs(first->koef), first->eks);
			}
		}
		else {
			if (first->eks < 0) {
				if (first->koef == 1)
				{
					printf(" + x^(%d)", first->eks);
				}
				else
				{
					printf(" + %dx^(%d)", first->koef, first->eks);
				}
			}
			else
			{
				if (first->koef == 1)
				{
					printf(" + x^%d", first->eks);
				}
				else
				{
					printf(" + %dx^%d", first->koef, first->eks);
				}
			}
		}
	}

	printf("\n");
	return 1;
}

int CreateAndinsertAfter(int koef, int eks, Position position)
{
	Position newPoly = CreatePoly(koef, eks);
	if (!newPoly)
	{
		return 0;
	}

	insertAfter(position, newPoly);
	position = position->next;

	return 1;

}

int AddPoly1(Position resultHead, Position head1, Position head2)
{
	Position i = head1->next;
	Position j = head2->next;
	Position k = resultHead;
	Position temp = NULL;

	while (i != NULL && j != NULL) 
	{
		if (i->eks == j->eks)
		{
			CreateAndinsertAfter(i->koef + j->koef, i->eks, k);
			i = i->next;
			j = j->next;
		}
		else if (i->eks < j->eks)
		{
			CreateAndinsertAfter(i->koef, i->eks, k);
			i = i->next;
		}
		else 
		{
			CreateAndinsertAfter(j->koef, j->eks, k);
			j = j->next;
		}

	}
	if (i == NULL)
	{
		temp = j;
	}
	else 
	{
		temp = i;
	}
	while (temp != NULL)  
	{
		CreateAndinsertAfter(temp->koef, temp->eks, k);
		temp = temp->next;
	}
	return 1;
}

int AddPoly2(Position resultHead, Position head1, Position head2)
{
	Position i = NULL;
	Position j = NULL;

	for (i = head1->next; i != NULL; i = i->next)
	{
		Position newPoly = CreatePoly(i->koef, i->eks);
		if (!newPoly)
		{
			return 0;
		}

		Insert(resultHead, newPoly);
	}
	for (j = head2->next; j != NULL; j = j->next)
	{
		Position newPoly = CreatePoly(j->koef, j->eks);
		if (!newPoly)
		{
			return 0;
		}

		Insert(resultHead, newPoly);
	}
	return 1;
}

int MultiplyPoly(Position resultHead, Position head1, Position head2)
{
	Position i = NULL;
	Position j = NULL;

	for (i = head1->next; i != NULL; i = i->next)
	{
		for (j = head2->next; j != NULL; j = j->next)
		{
			Position newPoly = CreatePoly(i->koef * j->koef, i->eks + j->eks);
			if (!newPoly)
			{
				return 0;
			}

			Insert(resultHead, newPoly);
		}
	}
	return 1;
}
int FreeMemory(Position head)
{
	Position temp = head;

	while (temp->next)
	{
		Remove(temp);
	}

	return 1;
}
