#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMBERS 30
#define LENGHT 100

typedef struct Book* PositionBook;
typedef struct Member* PositionMember;
typedef struct Book {
    char title[LENGHT];
    char author[LENGHT];
    int quantity;
    int borrowed_qty;
    int publication_year;
    PositionBook next;
    PositionMember borrowed_by[5];
}Book;


typedef struct Member {
    char name[LENGHT];
    int num_books_borrowed;
    PositionBook borrowed_books[5];
    PositionMember next;
}Member;


PositionBook createBook();
PositionMember createMember();
void borrowBook(PositionBook books, PositionMember members);
void returnBook(PositionBook books, PositionMember members);
void printBooks(PositionBook books);
void printMembers(PositionMember members);
void saveData(PositionBook books, PositionMember members);
void loadData(PositionBook books, PositionMember members);
void searchBooksByYear(PositionBook, int year);
void searchBooksByAuthor(PositionBook, const char* author);
void insertBookSorted(PositionBook head, PositionBook new_book);
void insertMemberSorted(PositionMember head, PositionMember newMember);
void bookPrint(PositionBook book);


int main() {
    Member members = {
        .name = {0},
        .num_books_borrowed = 0,

        .next = NULL,
        .borrowed_books = NULL
    };
    Book books = {
        .title = {0},
        .author = {0},
        .quantity = 0,
        .borrowed_qty = 0,
        .publication_year = 0,
        .next = NULL,
        .borrowed_by = NULL
    };

    loadData(&books, &members);

    int choice;
    do {
        printf("----------------------------------------");
        printf("\nBiblioteka - Izaberite opciju:\n");
        printf("1. Dodaj knjigu\n");
        printf("2. Dodaj clana\n");
        printf("3. Posudi knjigu\n");
        printf("4. Vrati knjigu\n");
        printf("5. Ispisi sve knjige i koje clanove drze posudjene\n");
        printf("6. Ispisi sve clanove i koje knjige imaju posudjene\n");
        printf("7. Spremi podatke u datoteku\n");
        printf("8. Pretraga po godini\n");
        printf("9. Pretraga po autoru\n");
        printf("10. Izlaz\n");
        printf("----------------------------------------\n");

        printf("Izaberite opciju: ");
        scanf("%d", &choice);
        printf("----------------------------------------\n");


        switch (choice) {
        case 1:
            PositionBook newBook = NULL;
            newBook = createBook();
            insertBookSorted(&books, newBook);
            break;
        case 2:
            PositionMember newMember = NULL;
            newMember = createMember();
            insertMemberSorted(&members, newMember);
            break;
        case 3:
            borrowBook(&books, &members);
            break;
        case 4:
            returnBook(&books, &members);
            break;
        case 5:
            printf("\nSve knjige u sistemu:\n\n");
            printBooks(&books);
            break;
        case 6:
            printf("\nSvi clanovi u sistemu:\n\n");
            printMembers(&members);
            break;
        case 7:
            saveData(&books, &members);
            break;
        case 8:
            int year;
            printf("Unesite godinu izdanja za pretragu: ");
            scanf("%d", &year);
            searchBooksByYear(&books, year);
            break;
        case 9:
            char author[LENGHT];
            printf("Unesite autora za pretragu: ");
            scanf(" %[^\n]s", author);
            searchBooksByAuthor(&books, author);
            break;
        case 10:
            break;
        default:
            printf("Nepostojeca opcija\n");
        }
    } while (choice != 10);

    return 0;
}


PositionBook createBook() {
    PositionBook q = (PositionBook)malloc(sizeof(Book));
    if (q == NULL) {
        printf("Greška prilikom alokacije memorije za knjigu.\n");
        return NULL;
    }

    printf("Unesite naslov knjige: ");
    scanf(" %[^\n]s", q->title);
    printf("Unesite autora knjige: ");
    scanf(" %[^\n]s", q->author);

    printf("Unesite kolicinu knjiga: ");
    scanf(" %d", &q->quantity);

    printf("Unesite godinu izdanja knjige: ");
    scanf("%d", &q->publication_year);

    q->borrowed_qty = 0;

    for (int i = 0; i < 5; i++) {
        q->borrowed_by[i] = NULL;
    }
    printf("\nKnjiga je uspješno dodana!\n");
    return q;

}
void insertBookSorted(PositionBook head, PositionBook new_book) {
    PositionBook current = head->next;
    if (head->next == NULL || strcmp(head->next->title, new_book->title) > 0) {
        new_book->next = head->next;
        head->next = new_book;
        return;
    }

    while (current->next != NULL && strcmp(current->next->title, new_book->title) <= 0) {
        current = current->next;
    }
    new_book->next = current->next;
    current->next = new_book;
}


PositionMember createMember() {
    PositionMember q = NULL;
    q = (PositionMember)malloc(sizeof(Member));

    printf("Unesite ime clana: ");
    scanf(" %[^\n]s", q->name);

    q->num_books_borrowed = 0;

    for (int i = 0; i < 5; i++) {
        q->borrowed_books[i] = NULL;
    }
    printf("Clan je uspjesno dodan!\n");
    return q;

}
void insertMemberSorted(PositionMember head, PositionMember newMember) {
    PositionMember current = head;

    //prazna lista ili prije prvog el
    if (head == NULL || strcmp(head->name, newMember->name) >= 0) {
        newMember->next = head;
        head = newMember;
    }
    else {
        while (current->next != NULL && strcmp(current->next->name, newMember->name) < 0) {
            current = current->next;
        }
        newMember->next = current->next;
        current->next = newMember;
    }
}

void borrowBook(PositionBook books, PositionMember members) {
    char book_title[LENGHT];
    char member_name[LENGHT];
    int num_copies_requested;

    printf("Unesite naslov knjige koju zelite posuditi: ");
    scanf(" %[^\n]s", book_title);

    printf("Unesite ime clana koji zeli posuditi knjigu: ");
    scanf(" %[^\n]s", member_name);

    printf("Koliko primjeraka zelite posuditi: ");
    scanf("%d", &num_copies_requested);

    //trazenje knjige
    PositionBook book = books;
    while (book != NULL && strcmp(book->title, book_title) != 0) {
        book = book->next;
    }

    //trazemke membera
    PositionMember member = members;
    while (member != NULL && strcmp(member->name, member_name) != 0) {
        member = member->next;
    }
    //nije pronadeno
    if (book == NULL || member == NULL) {
        printf("Knjiga ili clan nisu pronadjeni\n");
        return;
    }

    //quanitity test
    if (book->quantity < num_copies_requested) {
        printf("Knjiznica nema dovoljno primjeraka knjige\n");
        printf("Trenutno dostupno: %d primjeraka\n", book->quantity);
        printf("Zelite li posuditi koliko ima (da/ne)? ");
        char choice[5];
        scanf(" %s", choice); //da/ne

        if (strcmp(choice, "da") == 0) {
            num_copies_requested = book->quantity;
        }
        else {
            return;
        }
    }

    //book count test
    if (member->num_books_borrowed + num_copies_requested > 5) {
        printf("Clan vec ima maksimalan broj posudjenih knjiga ili prelazi limit od 5 knjiga\n");
        return;
    }

    int i = 0;
    while (i < 5 && member->borrowed_books[i] != NULL) {
        i++;
    }

    for (int j = 0; j < num_copies_requested; j++) {
        if (book->borrowed_qty >= book->quantity) {
            printf("Sve kopije knjige su vec posudjene\n");
            break;
        }
        book->quantity--;
        book->borrowed_by[book->borrowed_qty] = member;
        book->borrowed_qty++;
        member->borrowed_books[member->num_books_borrowed] = book;

        member->num_books_borrowed++;
    }

    printf("Knjige su uspjesno posudjene clanu\n");
}
void searchBooksByYear(PositionBook books, int year) {
    printf("\nKnjige izdate %d. godine:\n\n", year);

    PositionBook book = books->next;
    while (book != NULL) {
        if (book->publication_year == year) {
            bookPrint(book);
        }
        book = book->next;
    }
}
void searchBooksByAuthor(PositionBook books, const char* author) {
    printf("\nKnjige autora %s:\n", author);

    PositionBook book = books->next;
    while (book != NULL) {
        if (strcmp(book->author, author) == 0) {
            bookPrint(book);
        }
        book = book->next;
    }
}

void returnBook(PositionBook books, PositionMember members) {
    char book_title[LENGHT];
    char member_name[LENGHT];

    printf("Unesite naslov knjige koju zelite vratiti: ");
    scanf(" %[^\n]s", book_title);

    printf("Unesite ime clana koji zeli vratiti knjigu: ");
    scanf(" %[^\n]s", member_name);

    //trazenje knjige
    PositionBook book = books;
    while (book != NULL && strcmp(book->title, book_title) != 0) {
        book = book->next;
    }

    //trazenje membera
    PositionMember member = members;
    while (member != NULL && strcmp(member->name, member_name) != 0) {
        member = member->next;
    }

    //nisu prondajeni
    if (book == NULL || member == NULL) {
        printf("Knjiga ili clan nisu pronadjeni.\n");
        return;
    }

    //trazi mjesto
    int i = 0;
    while (i < 5 && member->borrowed_books[i] != book) {
        i++;
    }

    if (i >= 5) {
        printf("Ovaj clan nema posudjenu ovu knjigu.\n");
        return;
    }

    // test za iste primjerke u knjigama
    for (int j = i; j < 4; j++) {
        member->borrowed_books[j] = member->borrowed_books[j + 1];
    }
    member->borrowed_books[4] = NULL;
    member->num_books_borrowed--;

    i = 0;
    while (i < 5 && book->borrowed_by[i] != member) {
        i++;
    }

    if (i < 5) {
        // test za iste primjerke u memberima
        for (int j = i; j < 4; j++) {
            book->borrowed_by[j] = book->borrowed_by[j + 1];
        }
        book->borrowed_by[4] = NULL;
        book->borrowed_qty--;
        
    }
    book->quantity++;
    printf("Knjiga je uspjesno vracena.\n");
}

void printBooks(PositionBook books) {
    PositionBook book = books->next;
    while (book != NULL) {
        bookPrint(book);

        book = book->next;
    }
}
void bookPrint(PositionBook book) {
    printf("\nNaslov: %s, Autor: %s, Kolicina: %d, Posudjeno: %d, Godina izdanja: %d\n", book->title, book->author, book->quantity, book->borrowed_qty, book->publication_year);
    if (book->borrowed_qty > 0) {
        printf("  Posudjena od strane:");
        for (int i = 0; i < book->borrowed_qty; i++) {
            if (book->borrowed_by[i] != NULL) {
                printf(", %s", book->borrowed_by[i]->name);
            }
        }
        printf("\n");
    }
}

void printMembers(PositionMember members) {
    PositionMember member = members->next;
    while (member != NULL) {
        printf("\nIme: %s, Broj posudjenih knjiga: %d\n", member->name, member->num_books_borrowed);
        if (member->num_books_borrowed > 0) {
            printf("  Posudjene knjige:");
            for (int i = 0; i < 5; i++) {
                if (member->borrowed_books[i] != NULL) {
                    printf(", %s", member->borrowed_books[i]->title);
                }
            }
            printf("\n");
        }

        member = member->next;
    }
}

void saveData(PositionBook books, PositionMember members) {
    FILE* file = fopen("library_data.txt", "w");
    if (file == NULL) {
        printf("Nemoguce otvoriti datoteku za pisanje\n");
        return;
    }

    //zapis knjiga
    PositionBook book = books->next;
    while (book != NULL) {
        fprintf(file, "Book,%s,%s,%d,%d,%d\n", book->title, book->author, book->quantity, book->borrowed_qty, book->publication_year);
        book = book->next;
    }

    //zapis membera
    PositionMember member = members->next;
    while (member != NULL) {
        fprintf(file, "Member,%s,%d,", member->name, member->num_books_borrowed);

        for (int i = 0; i < 5; i++) {
            if (member->borrowed_books[i] != NULL) {
                fprintf(file, "%s", member->borrowed_books[i]->title);
                if (i < 4 && member->borrowed_books[i + 1] != NULL) {
                    fprintf(file, ",");
                }
            }
        }

        fprintf(file, "\n");
        member = member->next;
    }

    fclose(file);

    printf("Podaci su uspjesno sacuvani u datoteku\n");
}
void loadData(PositionBook books, PositionMember members) {
    FILE* file = fopen("library_data.txt", "r");
    if (file == NULL) {
        printf("Nije moguce otvoriti datoteku za citanje.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, ",");
        if (token != NULL && strcmp(token, "Book") == 0) {
            PositionBook new_book = (PositionBook)malloc(sizeof(struct Book));
            strcpy(new_book->title, strtok(NULL, ","));
            strcpy(new_book->author, strtok(NULL, ","));
            new_book->quantity = atoi(strtok(NULL, ","));
            new_book->borrowed_qty = atoi(strtok(NULL, ","));
            new_book->publication_year = atoi(strtok(NULL, "\n"));

            new_book->next = NULL;
            for (int i = 0; i < 5; i++) {
                new_book->borrowed_by[i] = NULL;
            }
            insertBookSorted(books, new_book);
        }

        if (token != NULL && strcmp(token, "Member") == 0) {
            PositionMember new_member = (PositionMember)malloc(sizeof(struct Member));
            strcpy(new_member->name, strtok(NULL, ","));
            new_member->num_books_borrowed = atoi(strtok(NULL, ","));
            for (int i = 0; i < 5; i++) {
                new_member->borrowed_books[i] = NULL;
            }
            new_member->next = NULL;

            for (int i = 0; i < new_member->num_books_borrowed; i++) {
                token = strtok(NULL, i < new_member->num_books_borrowed - 1 ? "," : "\n");
                if (token != NULL) {
                    PositionBook book = books;
                    while (book != NULL && strcmp(book->title, token) != 0) {
                        book = book->next;
                    }
                    if (book != NULL) {
                        new_member->borrowed_books[i] = book;
                        for (int j = 0; j < 5; j++) {
                            if (book->borrowed_by[j] == NULL) {
                                book->borrowed_by[j] = new_member;
                                break;
                            }
                        }
                    }
                }
            }
            insertMemberSorted(members, new_member);
        }
    }
    fclose(file);
    printf("Podaci su uspjesno ucitani iz datoteke.\n");
}