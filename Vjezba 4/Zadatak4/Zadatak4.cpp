#include <stdio.h>
#include <stdlib.h>

typedef struct Clan {
    int koef;
    int eks;
    struct Clan* next;
} Clan;

Clan* ucitajPolinom(const char* dat) {
    FILE* datoteka = fopen(dat, "r");
    if (datoteka == NULL) {
        printf("Nije otvorena\n");
        return 0;
    }
    Clan* glava = NULL;
    int koef, eks;

    while (fscanf(datoteka, "%d %d", &koef, &eks) == 2) {
        Clan* noviClan = (Clan*)malloc(sizeof(Clan));
        noviClan->koef = koef;
        noviClan->eks = eks;
        noviClan->next = glava;
        glava = noviClan;
    }

    return glava;
}

Clan* swap(Clan* pol1, Clan* pol2){
    Clan* temp = pol2->next;
    pol2->next = pol1;
    pol1->next = temp;
    return pol2;
}
Clan* sort(Clan* polinom){
    
    int swapped;
    do{
        swapped = 0;
        Clan* current = polinom;
        Clan* before = NULL;
        
        while(current -> next != NULL){
            if(current->eks < current->next->eks)
            {
                current=swap(current, current->next);
                swapped = 1;
                if(before != NULL){
                    before->next = current;
                } else{
                    polinom = current;
                }
            }
            before = current;
            current = current->next;
        }
    }while(swapped);
    
    return polinom;
}

void ispisiPolinom(Clan* pol) {
    Clan* temp1=pol;
        while (temp1 != NULL) {
    		printf("%dx^%d", temp1->koef, temp1->eks);
    		if(temp1->next != NULL && (temp1->next->koef)>0){
    		    printf(" + ");
    		}else if(temp1->next != NULL && (temp1->next->koef)<0){
    		    printf(" - ");
    		}
    		temp1 = temp1->next;
	}
	
    printf("\n");
}

Clan* zbroji(Clan* pol1, Clan* pol2) {
    Clan* rezultat = NULL;
    Clan* trenutniRezultat = NULL;

    while (pol1 != NULL || pol2 != NULL) {
        Clan* noviClan = (Clan*)malloc(sizeof(Clan));
        
        if (noviClan == NULL) {
            printf("Greška pri alokaciji memorije\n");
            return rezultat;
        }

        noviClan->next = NULL;

        if (pol1 != NULL && pol2 != NULL) {
            if (pol1->eks < pol2->eks) {
                noviClan->eks = pol2->eks;
                noviClan->koef = pol2->koef;
                pol2 = pol2->next;
            } else if (pol1->eks > pol2->eks) {
                noviClan->eks = pol1->eks;
                noviClan->koef = pol1->koef;
                pol1 = pol1->next;
            } else {
                noviClan->eks = pol1->eks;
                noviClan->koef = pol1->koef + pol2->koef;
                pol1 = pol1->next;
                pol2 = pol2->next;
            }
        }

        if (rezultat == NULL) {
            rezultat = noviClan;
            trenutniRezultat = noviClan;
        } else {
            trenutniRezultat->next = noviClan;
            trenutniRezultat = noviClan;
        }
    }

    return rezultat;
}


Clan* pomnozi(Clan* pol1, Clan* pol2) {
    Clan* rezultat = NULL;

    while (pol1 != NULL) {
        Clan* trenutniPol2 = pol2;
        while (trenutniPol2 != NULL) {
            Clan* noviClan = (Clan*)malloc(sizeof(Clan));

            if (noviClan == NULL) {
                printf("Greška pri alokaciji memorije\n");
                return rezultat;
            }

            noviClan->eks = pol1->eks + trenutniPol2->eks;
            noviClan->koef = pol1->koef * trenutniPol2->koef;
            noviClan->next = NULL;


            if (rezultat == NULL) {
                rezultat = noviClan;
            } else {
                Clan* trenutniRezultat = rezultat;
                Clan* prethodniRezultat = NULL;
                while (trenutniRezultat != NULL && trenutniRezultat->eks > noviClan->eks) {
                    prethodniRezultat = trenutniRezultat;
                    trenutniRezultat = trenutniRezultat->next;
                }
                if (prethodniRezultat != NULL) {
                    prethodniRezultat->next = noviClan;
                    noviClan->next = trenutniRezultat;
                } else {
                    noviClan->next = rezultat;
                    rezultat = noviClan;
                }
            }

            trenutniPol2 = trenutniPol2->next;
        }

        pol1 = pol1->next;
    }

    return rezultat;
}

int main() {
    Clan* pol1 = NULL;
    Clan* pol2 = NULL;
    Clan* zb = NULL;
    Clan* mn = NULL;
    int i=0;
    char datoteka[50]="";
    char datoteka2[50]="";
    
    printf("Unesi ime prve datoteke: ");
    scanf("%s", datoteka);
    printf("Unesi ime druge datoteke: ");
    scanf("%s", datoteka2);
    pol1 = ucitajPolinom(datoteka);
    pol2 = ucitajPolinom(datoteka2);
    pol1 = sort(pol1);
    pol2 = sort(pol2);
    
    ispisiPolinom(pol1);
    ispisiPolinom(pol2);
    
    zb=zbroji(pol1, pol2);
    mn=pomnozi(pol1, pol2);
        ispisiPolinom(zb);
        ispisiPolinom(mn);

 

}
