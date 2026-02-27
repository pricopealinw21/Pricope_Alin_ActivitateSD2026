#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Biblioteca {
    int id;
    int nrCarti;
    char* nume;
    float suprafata;
    char sector;
};

struct Biblioteca initializare(int id, int nrCarti, char* nume, float suprafata, char sector) {
    struct Biblioteca s;
    s.id = id;
    s.nrCarti = nrCarti;
    s.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(s.nume, nume);
    s.suprafata = suprafata;
    s.sector = sector;
    return s;
}

struct Biblioteca copiazaBiblioteca(struct Biblioteca sursa) {
    return initializare(sursa.id, sursa.nrCarti, sursa.nume, sursa.suprafata, sursa.sector);
}

void afisare(struct Biblioteca s) {
    printf("ID: %d\n", s.id);
    printf("Nr carti: %d\n", s.nrCarti);
    printf("Denumire: %s\n", s.nume);
    printf("Suprafata: %.2f\n", s.suprafata);
    printf("Sector: %d\n\n", s.sector);
}

void afisareVector(struct Biblioteca* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Biblioteca* copiazaPrimeleNElemente(struct Biblioteca* vector,
    int nrElemente,
    int nrElementeCopiate) {

    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente;
    }

    struct Biblioteca* vectorNou =
        (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = copiazaBiblioteca(vector[i]);
    }

    return vectorNou;
}

void dezalocare(struct Biblioteca** vector, int* nrElemente) {

    if (*vector != NULL) {
        for (int i = 0; i < *nrElemente; i++) {
            free((*vector)[i].nume);
        }

        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaBibcuSupMare(struct Biblioteca* vector,
    int nrElemente,
    float supMin,
    struct Biblioteca** vectorNou,
    int* dimensiune) {

    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].suprafata > supMin) {
            (*dimensiune)++;
        }
    }

    *vectorNou = (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * (*dimensiune));

    int contor = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].suprafata > supMin) {
            (*vectorNou)[contor++] = copiazaBiblioteca(vector[i]);
        }
    }
}

struct Biblioteca getPrimulElementConditionat(struct Biblioteca* vector,
    int nrElemente,
    const char* conditie) {

    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].nume, conditie) == 0) {
            return copiazaBiblioteca(vector[i]);
        }
    }

    // daca nu gaseste, returnam unul gol
    return initializare(-1, 0, "Necunoscut", 0, 0);
}

int main() {

    int nrTotalBiblioteci = 5;

    struct Biblioteca* listaBiblioteci =
        (struct Biblioteca*)malloc(sizeof(struct Biblioteca) * nrTotalBiblioteci);

    listaBiblioteci[0] = initializare(1, 425, "Camil Petrecu", 482.95, 5);
    listaBiblioteci[1] = initializare(2, 644, "Preda", 450.4, 1);
    listaBiblioteci[2] = initializare(3, 133, "Creanga", 300.2, 2);
    listaBiblioteci[3] = initializare(4, 951, "Slavici", 78.7, 5);
    listaBiblioteci[4] = initializare(5, 7355, "Eminescu", 102.2, 2);

    printf("VECTOR INITIAL:\n");
    afisareVector(listaBiblioteci, nrTotalBiblioteci);

    // Copiere primele N elemente
    int nrElementeDeCopiat = 3;
    struct Biblioteca* primeleElemente =
        copiazaPrimeleNElemente(listaBiblioteci,
            nrTotalBiblioteci,
            nrElementeDeCopiat);

    printf("\nPRIMELE %d ELEMENTE COPIATE:\n", nrElementeDeCopiat);
    afisareVector(primeleElemente, nrElementeDeCopiat);

    dezalocare(&primeleElemente, &nrElementeDeCopiat);

    // Filtrare dupa suprafata
    struct Biblioteca* biblioteciFiltrate = NULL;
    int nrBiblioteciFiltrate = 0;

    copiazaBibcuSupMare(listaBiblioteci,
        nrTotalBiblioteci,
        100,
        &biblioteciFiltrate,
        &nrBiblioteciFiltrate);

    printf("\nBIBLIOTECI CU SUPRAFATA > 100:\n");
    afisareVector(biblioteciFiltrate, nrBiblioteciFiltrate);

    dezalocare(&biblioteciFiltrate, &nrBiblioteciFiltrate);

    dezalocare(&listaBiblioteci, &nrTotalBiblioteci);

    return 0;
}