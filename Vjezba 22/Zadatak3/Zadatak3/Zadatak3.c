#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Osoba {
	char ime[50];
	char prezime[50];
	int god;
	struct Osoba *next;
} Osoba;

//pocetak liste
Osoba *naPocetak(Osoba *lista, Osoba nova);

//ispis
void ispisi(Osoba *lista);

//na kraj liste
Osoba *naKraj(Osoba *lista, Osoba newL);

// pronalazenje
Osoba *pronadi(Osoba *lista, const char *prezime);

// brisanje
Osoba *obrisi(Osoba *lista, const char *prezime);

Osoba *novaiza(Osoba *lista, const char *prezime);

Osoba *pronp(Osoba *lista, const char *prezime);


void upisi(Osoba* lista, const char *datn);

Osoba* izdat(Osoba* lista, const char *datoteka);



int main() {
	Osoba *lista = NULL;



	while (1) {
		printf(" 1. Dodaj na pocetak \n 2. Ispis \n 3. Pronadi \n 4. Obrisi \n 5. Izlaz iz programa \n 6. Na kraj \n 7. Dinamicki iza \n 8. Dinamicki ispred \n 9. Ispis u datoteka \n 10. Upis iz datoteke\n");

		int sel;
		scanf("%d", &sel);

		if (sel == 1)
		{
			Osoba newL;
			printf("\nUnesi ime:");
			scanf("%s", newL.ime);
			printf("\nUnesi prezime:");
			scanf("%s", newL.prezime);
			printf("\nUnesi god:");
			scanf("%d", &newL.god);
			lista = naPocetak(lista, newL);
		}
		else if (sel == 2)
		{
			ispisi(lista);
		}
		else if (sel == 3)
		{
			char prez[50] = "";
			printf("Unesi prezime za pretraziti:\n");
			scanf("%s", prez);
			pronadi(lista, prez);
		}
		else if (sel == 4)
		{
			char b[50] = "";
			printf("Koje prezime zelis obrisati:\n");
			scanf("%s", b);
			lista = obrisi(lista, b);
		}
		else if (sel == 5)
		{
			return 0;
		}
		else if (sel == 6)
		{
			Osoba newL;
			printf("\nUnesi ime:");
			scanf("%s", newL.ime);
			printf("\nUnesi prezime:");
			scanf("%s", newL.prezime);
			printf("\nUnesi god:");
			scanf("%d", &newL.god);
			lista = naKraj(lista, newL);
		}
		else if (sel == 7) {
			char n[50] = "";
			printf("Iza:\n");
			scanf("%s", n);
			lista = novaiza(lista, n);
		}
		else if (sel == 8)
		{
			Osoba *prosli;
			char m[50] = "";
			printf("Ispred:\n");
			scanf("%s", m);
			lista = pronp(lista, m);
		}
		else if (sel == 9){
		    char datn[50]="";
		    
		    printf("Unesi ime datoteke:");
		    scanf("%s", datn);
		    upisi(lista, datn);
		}
		else if (sel == 10){
		    char datn[50]="";
		    
		    printf("Unesi ime datoteke:");
		    scanf("%s", datn);
		    lista = izdat(lista, datn);

		}
	}


	return 0;
}

//pocetak liste
Osoba *naPocetak(Osoba *lista, Osoba nova) {

	Osoba *nova_osoba = (Osoba*)malloc(sizeof(Osoba));
	if (nova_osoba == NULL) {
		printf("Greška pri alokaciji memorije za novu osobu.\n");
		return lista;
	}

	strncpy(nova_osoba->ime, nova.ime, sizeof(nova_osoba->ime));
	strncpy(nova_osoba->prezime, nova.prezime, sizeof(nova_osoba->prezime));
	nova_osoba->god = nova.god;
	nova_osoba->next = lista;

	return nova_osoba;
}

//ispis
void ispisi(Osoba *lista) {
	Osoba *trenutni = lista;
	while (trenutni != NULL) {
		printf("Ime: %s, Prez :%s, God %d\n\n", trenutni->ime, trenutni->prezime, trenutni->god);
		trenutni = trenutni->next;
	}
}

//na kraj liste
Osoba *naKraj(Osoba *lista, Osoba newL) {
	Osoba *nova_osoba = (Osoba*)malloc(sizeof(Osoba));
	if (nova_osoba == NULL) {
		printf("Greška pri alokaciji memorije za novu osobu.\n");
		return lista;
	}

	strncpy(nova_osoba->ime, newL.ime, sizeof(nova_osoba->ime));
	strncpy(nova_osoba->prezime, newL.prezime, sizeof(nova_osoba->prezime));
	nova_osoba->god = newL.god;
	nova_osoba->next = NULL;

	if (lista == NULL) {
		return nova_osoba;
	}

	Osoba* trenutni = lista;
	while (trenutni->next != NULL) {
		trenutni = trenutni->next;
	}
	trenutni->next = nova_osoba;

	return lista;
}

// pronalazenje
Osoba *pronadi(Osoba *lista, const char *prezime) {
	Osoba *trenutni = lista;
	while (trenutni != NULL) {
		if (strcmp(trenutni->prezime, prezime) == 0) {
			printf("Ime: %s \t Prezime:%s \t God:%d\n", trenutni->ime, trenutni->prezime, trenutni->god);
			return trenutni;
		}
		trenutni = trenutni->next;
	}

	printf("Nije pronaden");
	return NULL;
}

// brisanje
Osoba *obrisi(Osoba *lista, const char *prezime) {
	Osoba* trenutni = lista;
	Osoba* prethodni = NULL;

	while (trenutni != NULL) {
		if (strcmp(trenutni->prezime, prezime) == 0) {
			if (prethodni == NULL) {
				lista = trenutni->next;
			}
			else {
				prethodni->next = trenutni->next;
			}
			free(trenutni);
			return lista;
		}
		prethodni = trenutni;
		trenutni = trenutni->next;
	}

	return lista;
}

Osoba *novaiza(Osoba *lista, const char *prezime)
{
	Osoba *nova = (Osoba*)malloc(sizeof(Osoba));
	Osoba *trenutni = lista;

	while (trenutni != NULL) {
		if (strcmp(trenutni->prezime, prezime) == 0) {
			printf("\nUnesi ime:");
			scanf("%s", nova->ime);
			printf("\nUnesi prezime:");
			scanf("%s", nova->prezime);
			printf("\nUnesi god:");
			scanf("%d", &nova->god);

			nova->next = trenutni->next;
			trenutni->next = nova;

			return lista;

		}
		
		trenutni = trenutni->next;
	}

	printf("Nije pronaden");
	return NULL;

}

Osoba *pronp(Osoba *lista, const char *prezime)
{
	Osoba *nova = (Osoba*)malloc(sizeof(Osoba));
	Osoba *trenutni = lista;


while (trenutni != NULL) {
		if (strcmp(trenutni->next->prezime, prezime) == 0) {
		    
			printf("\nUnesi ime:");
			scanf("%s", nova->ime);
			printf("\nUnesi prezime:");
			scanf("%s", nova->prezime);
			printf("\nUnesi god:");
			scanf("%d", &nova->god);
			
			nova->next = trenutni->next;
    		trenutni -> next = nova;
    		
    		return lista;
		}
		
		trenutni = trenutni->next;
	}
	printf("Nije pronaden");
	return NULL;
}

void upisi(Osoba* lista, const char *datn){
    FILE *dat = fopen(datn, "w");
    
    if(dat == NULL)
    {
        printf("Nije otvorena datoteka");
    }
    
    Osoba* trenutni = lista;
    while(trenutni != NULL){
        fprintf(dat, "Ime: %s \t Prezime: %s \t God: %d \n", trenutni->ime, trenutni->prezime, trenutni->god);
        trenutni  = trenutni->next;
    }
    fclose(dat);
}

Osoba* izdat(Osoba* lista, const char *datoteka) {
    FILE *dat = fopen(datoteka, "r");

    if (dat == NULL) {
        printf("Nije moguće otvoriti datoteku za čitanje.\n");
        return NULL;
    }



    Osoba nova = {"", "", 0};
    
    while (fscanf(dat, "Ime: %s Prezime: %s God: %d\n", nova.ime, nova.prezime, &nova.god) == 3) {
        
        nova.next = NULL;

        lista = naKraj(lista, nova);
    }

    fclose(dat);
    return lista;
}

