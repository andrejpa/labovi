#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct item;
typedef struct item* ItemPosition;
typedef struct item
{
	char name[50];
	int quantity;
	double price;
	ItemPosition next;
} Item;

struct _receipt;
typedef struct _receipt* ReceiptPosition;
typedef struct _receipt
{
	char receiptDate[11];
	ReceiptPosition next;
	Item itemHead;
} Receipt;

int readAndFillReceipts(ReceiptPosition receiptHead);
ReceiptPosition createNewRecipeFromBuffer(char* buffer);
ReceiptPosition createNewRecipe(char* date);
ItemPosition createNewItem(char* name, int quantity, double price);
int insertSortedNewItem(ItemPosition itemHead, ItemPosition newItem, int isNew);
int mergeItemAfter(ItemPosition itemCurrent, ItemPosition newItem, int isNew);
int insertItemAfter(ItemPosition itemCurrent, ItemPosition newItem);
int insertSortedNewReceipt(ReceiptPosition receiptHead, ReceiptPosition newReceipt);
int mergeReceiptAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt);
int insertReceiptAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt);
int compareDates(char* date1, char* date2);
int itemInBetweenDates(char* itemName, char* dateFrom, char* dateTo, ReceiptPosition receiptHead);

int main() {
	Receipt receiptHead = {
		.receiptDate = { 0 },
		.next = NULL,
		.itemHead = {
			.name = { 0 },
			.quantity = 0,
			.price = 0,
			.next = NULL
		}
	};

	if (readAndFillReceipts(&receiptHead) == 1) {

		ReceiptPosition currentReceipt = NULL;
		currentReceipt = receiptHead.next;
		ItemPosition currentItem = NULL;
		while (currentReceipt != NULL) {
			printf("\n%s\n", currentReceipt->receiptDate);
			currentItem = currentReceipt->itemHead.next;
			while (currentItem != NULL) {
				printf("\t%s %d %.2lf\n", currentItem->name, currentItem->quantity, currentItem->price);
				currentItem = currentItem->next;
			}
			currentReceipt = currentReceipt->next;
		}
	}

	itemInBetweenDates("Jabuka", "2023-11-10", "2023-11-17", receiptHead.next);

	return 1;
}

int readAndFillReceipts(ReceiptPosition receiptHead) {
	FILE* filePointer = NULL;
	char buffer[100] = { 0 };
	char nullString[100] = { 0 };
	ReceiptPosition newReceipt = NULL;

	filePointer = fopen("racuni.txt", "r");
	if (!filePointer) {
		printf("nije otvoren\n");
		return -1;
	}

	while (!feof(filePointer)) {
		fscanf(filePointer, " %s", buffer);
		newReceipt = createNewRecipeFromBuffer(buffer);
		insertSortedNewReceipt(receiptHead, newReceipt);
		strcpy(buffer, nullString);
	}

	fclose(filePointer);

	return 1;
}

ReceiptPosition createNewRecipeFromBuffer(char* buffer) {
	char receiptLocation[50] = { 0 };
	FILE* receiptFilePointer = NULL;
	ReceiptPosition newReceipt = NULL;
	ItemPosition newItem = NULL;
	char dateBuffer[11] = { 0 };
	char itemName[50] = { 0 };
	int itemQuantity = 0;
	double itemPrice = 0;

	strcpy(receiptLocation, buffer);

	receiptFilePointer = fopen(receiptLocation, "r");
	if (!receiptFilePointer) {
		printf("nije otvoren\n");
		return -1;
	}

	fgets(dateBuffer, 11, receiptFilePointer);
	newReceipt = createNewRecipe(dateBuffer);
	while (!feof(receiptFilePointer)) {
		fscanf(receiptFilePointer, " %s %d %lf", itemName, &itemQuantity, &itemPrice);
		newItem = createNewItem(itemName, itemQuantity, itemPrice);
		insertSortedNewItem(&newReceipt->itemHead, newItem, 1);
		newItem = NULL;
	}

	fclose(receiptFilePointer);

	return newReceipt;
}

ReceiptPosition createNewRecipe(char* date) {
	ReceiptPosition receipt = NULL;
	receipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (!receipt) {
		printf("greska u alokaciji\n");
		return NULL;
	}


	strcpy(receipt->receiptDate, date);
	receipt->next = NULL;
	receipt->itemHead.next = NULL;

	return receipt;
}

ItemPosition createNewItem(char* name, int quantity, double price) {
	ItemPosition item = NULL;
	item = (ItemPosition)malloc(sizeof(Item));
	if (!item) {
		printf("greska u alokaciji\n");
		return NULL;
	}


	strcpy(item->name, name);
	item->quantity = quantity;
	item->price = price;
	item->next = NULL;

	return item;
}

int insertSortedNewItem(ItemPosition itemHead, ItemPosition newItem, int isNew) {
	ItemPosition itemCurrent = itemHead;
	while (itemCurrent->next != NULL && strcmp(itemCurrent->next->name, newItem->name) < 0) {
		itemCurrent = itemCurrent->next;
	}

	mergeItemAfter(itemCurrent, newItem, isNew);

	return 1;
}

int mergeItemAfter(ItemPosition itemCurrent, ItemPosition newItem, int isNew) {
	int resultItemQuantity = 0;
	ItemPosition newItem2 = newItem;

	if (!isNew) {
		newItem2 = createNewItem(newItem2->name, newItem2->quantity, newItem2->price);
	}

	if (itemCurrent->next == NULL || strcmp(itemCurrent->next->name, newItem->name) != 0) {
		insertItemAfter(itemCurrent, newItem2);
	}
	else {
		resultItemQuantity = itemCurrent->next->quantity + newItem->quantity;
		itemCurrent->next->quantity = resultItemQuantity;
		if(isNew)
			free(newItem2);
	}

	return 1;
}

int insertItemAfter(ItemPosition itemCurrent, ItemPosition newItem) {
	newItem->next = itemCurrent->next;
	itemCurrent->next = newItem;

	return 1;
}

int insertSortedNewReceipt(ReceiptPosition receiptHead, ReceiptPosition newReceipt) {
	ReceiptPosition receiptCurrent = receiptHead;

	while (receiptCurrent->next != NULL && compareDates(receiptCurrent->next->receiptDate, newReceipt->receiptDate) == -1  /* < */) {
		receiptCurrent = receiptCurrent->next;
	}

	mergeReceiptAfter(receiptCurrent, newReceipt);

	return 1;
}

int mergeReceiptAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt) {
	ItemPosition itemCurrent = NULL;
	if (receiptCurrent->next == NULL || compareDates(receiptCurrent->next->receiptDate, newReceipt->receiptDate) == 1) {
		insertReceiptAfter(receiptCurrent, newReceipt);
	}
	else {
		itemCurrent = newReceipt->itemHead.next;
		while (itemCurrent != NULL) {
			insertSortedNewItem(&receiptCurrent->next->itemHead, itemCurrent, 0);
			itemCurrent = itemCurrent->next;
		}
		free(newReceipt);
	}

	return 1;
}

int insertReceiptAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt) {
	newReceipt->next = receiptCurrent->next;
	receiptCurrent->next = newReceipt;

	return 1;
}


int compareDates(char* date1, char* date2) {
	int year1, month1, day1;
	int year2, month2, day2;

	sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
	sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);

	if (year1 < year2) {
		return -1;
	}
	else if (year1 > year2) {
		return 1;
	}

	if (month1 < month2) {
		return -1;
	}
	else if (month1 > month2) {
		return 1;
	}

	if (day1 < day2) {
		return -1;
	}
	else if (day1 > day2) {
		return 1;
	}

	return 0;
}

int itemInBetweenDates(char* itemName, char* dateFrom, char* dateTo, ReceiptPosition receiptFirstElement) {
	ReceiptPosition receiptFrom = NULL;
	ReceiptPosition receiptTo = NULL;
	ReceiptPosition receiptCurrent = NULL;
	ItemPosition itemCurrent = NULL;
	int itemQuantity = 0;
	double itemPrice = 0;

	receiptCurrent = receiptFirstElement;
	while (receiptCurrent != NULL) {
		if (compareDates(dateFrom, receiptCurrent) != 1 && receiptFrom == NULL)
			receiptFrom = receiptCurrent;
		if ((compareDates(dateTo, receiptCurrent) != -1 && receiptTo == NULL) || receiptCurrent->next == NULL)
			receiptTo = receiptCurrent;
		receiptCurrent = receiptCurrent->next;
	}


	receiptCurrent = receiptFrom;
	while (receiptCurrent != NULL && compareDates(receiptCurrent->receiptDate, receiptTo->receiptDate) != 1) {
		itemCurrent = receiptCurrent->itemHead.next;
		while (itemCurrent != NULL && strcmp(itemCurrent->name, itemName) < 0) {
			itemCurrent = itemCurrent->next;
		}
		if (itemCurrent != NULL && strcmp(itemCurrent->name, itemName) == 0) {
			itemQuantity += itemCurrent->quantity;
			itemPrice += itemCurrent->price * (double)itemCurrent->quantity;
		}
		receiptCurrent = receiptCurrent->next;
	}

	printf("\n %d %.2lf", itemQuantity, itemPrice);
}