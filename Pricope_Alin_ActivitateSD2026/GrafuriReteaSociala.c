

/*
Se dă o rețea socială, unde pentru fiecare persoană se reține lista de prieteni.
Construiți graful social, citind datele din fișier.
Să se afișeze numele persoanei cu cei mai mulți prieteni.
Să se calculeze numărul de grupuri.
Se dă un nume, pentru care se cere afișarea tuturor prietenilor de grad 2 (adică prietenii prietenilor)

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERSOANE 100
#define MAX_NUME 50
#define MAX_LINIE 1000

typedef struct Nod {
    char nume[MAX_NUME];
    struct Nod* urmator;
} Nod;

typedef struct {
    char nume[MAX_NUME];
    Nod* prieteni;
    int nr_prieteni;
} Persoana;

Persoana persoane[MAX_PERSOANE];
int nr_persoane = 0;
int vizitat[MAX_PERSOANE];

int cauta_persoana(char* nume) {
    for (int i = 0; i < nr_persoane; i++) {
        if (strcmp(persoane[i].nume, nume) == 0) {
            return i;
        }
    }
    return -1;
}

int adauga_persoana(char* nume) {
    int idx = cauta_persoana(nume);
    if (idx != -1) return idx;

    strcpy(persoane[nr_persoane].nume, nume);
    persoane[nr_persoane].prieteni = NULL;
    persoane[nr_persoane].nr_prieteni = 0;
    return nr_persoane++;
}


void adauga_prietenie(char* nume1, char* nume2) {
    int idx1 = adauga_persoana(nume1);
    int idx2 = adauga_persoana(nume2);


    Nod* nou = (Nod*)malloc(sizeof(Nod));
    strcpy(nou->nume, nume2);
    nou->urmator = persoane[idx1].prieteni;
    persoane[idx1].prieteni = nou;
    persoane[idx1].nr_prieteni++;

    nou = (Nod*)malloc(sizeof(Nod));
    strcpy(nou->nume, nume1);
    nou->urmator = persoane[idx2].prieteni;
    persoane[idx2].prieteni = nou;
    persoane[idx2].nr_prieteni++;
}


void citeste_fisier(char* nume_fisier) {
    FILE* f = fopen(nume_fisier, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului!\n");
        exit(1);
    }

    char linie[MAX_LINIE];
    while (fgets(linie, sizeof(linie), f)) {
      
        linie[strcspn(linie, "\n")] = 0;
        if (strlen(linie) == 0) continue;

        char* colon = strchr(linie, ':');
        if (!colon) continue;

        *colon = '\0';
        char nume[MAX_NUME];
        strcpy(nume, linie);

 
        char* p = nume + strlen(nume) - 1;
        while (p >= nume && *p == ' ') p--;
        *(p + 1) = '\0';

        adauga_persoana(nume);

  
        char* prieteni_str = colon + 1;
        char* token = strtok(prieteni_str, ",");
        while (token) {
          
            while (*token == ' ') token++;
            char* end = token + strlen(token) - 1;
            while (end > token && *end == ' ') end--;
            *(end + 1) = '\0';

            if (strlen(token) > 0) {
                adauga_prietenie(nume, token);
            }

            token = strtok(NULL, ",");
        }
    }

    fclose(f);
}


void afiseaza_cel_mai_popular() {
    int max_prieteni = -1;
    int idx_max = -1;

    for (int i = 0; i < nr_persoane; i++) {
        if (persoane[i].nr_prieteni > max_prieteni) {
            max_prieteni = persoane[i].nr_prieteni;
            idx_max = i;
        }
    }

    if (idx_max != -1) {
        printf("\nPersoana cu cei mai multi prieteni: %s (%d prieteni)\n",
            persoane[idx_max].nume, max_prieteni);
    }
}

void dfs(int idx) {
    vizitat[idx] = 1;

    Nod* current = persoane[idx].prieteni;
    while (current) {
        int idx_prieten = cauta_persoana(current->nume);
        if (idx_prieten != -1 && !vizitat[idx_prieten]) {
            dfs(idx_prieten);
        }
        current = current->urmator;
    }
}


int numar_grupuri() {
    for (int i = 0; i < nr_persoane; i++) {
        vizitat[i] = 0;
    }

    int grupuri = 0;
    for (int i = 0; i < nr_persoane; i++) {
        if (!vizitat[i]) {
            grupuri++;
            dfs(i);
        }
    }

    return grupuri;
}


void afiseaza_prieteni_grad2(char* nume) {
    int idx = cauta_persoana(nume);
    if (idx == -1) {
        printf("Persoana %s nu exista in retea!\n", nume);
        return;
    }
    int grad1[MAX_PERSOANE] = { 0 };
    int grad2[MAX_PERSOANE] = { 0 };

    Nod* current = persoane[idx].prieteni;
    while (current) {
        int idx_prieten = cauta_persoana(current->nume);
        if (idx_prieten != -1) {
            grad1[idx_prieten] = 1;
        }
        current = current->urmator;
    }
    grad1[idx] = 1; 

    
    current = persoane[idx].prieteni;
    while (current) {
        int idx_prieten = cauta_persoana(current->nume);
        if (idx_prieten != -1) {
            Nod* prieten2 = persoane[idx_prieten].prieteni;
            while (prieten2) {
                int idx_p2 = cauta_persoana(prieten2->nume);
                if (idx_p2 != -1 && !grad1[idx_p2]) {
                    grad2[idx_p2] = 1;
                }
                prieten2 = prieten2->urmator;
            }
        }
        current = current->urmator;
    }


    printf("\nPrietenii de grad 2 pentru %s:\n", nume);
    int exista = 0;
    for (int i = 0; i < nr_persoane; i++) {
        if (grad2[i]) {
            printf("  - %s\n", persoane[i].nume);
            exista = 1;
        }
    }

    if (!exista) {
        printf("  (niciun prieten de grad 2)\n");
    }
}


void afiseaza_retea() {
    printf("\nReteaua sociala:\n");
    for (int i = 0; i < nr_persoane; i++) {
        printf("%s: ", persoane[i].nume);
        Nod* current = persoane[i].prieteni;
        while (current) {
            printf("%s ", current->nume);
            current = current->urmator;
        }
        printf("(%d prieteni)\n", persoane[i].nr_prieteni);
    }
}


void elibereaza_memorie() {
    for (int i = 0; i < nr_persoane; i++) {
        Nod* current = persoane[i].prieteni;
        while (current) {
            Nod* temp = current;
            current = current->urmator;
            free(temp);
        }
    }
}

int main() {
    
    citeste_fisier("retea.txt");

   
    afiseaza_retea();


    afiseaza_cel_mai_popular();


    int grupuri = numar_grupuri();
    printf("\nNumarul de grupuri (componente conexe): %d\n", grupuri);

    char nume_cautat[MAX_NUME];
    printf("\nIntroduceti numele persoanei pentru prietenii de grad 2: ");
    fgets(nume_cautat, sizeof(nume_cautat), stdin);
    nume_cautat[strcspn(nume_cautat, "\n")] = 0;

    afiseaza_prieteni_grad2(nume_cautat);

  
    elibereaza_memorie();

    return 0;
}