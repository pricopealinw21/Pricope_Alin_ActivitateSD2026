#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PartidPolitic {
    int id;
    int nrMembri;
    char* denumire;
    float procentSondaj;
    char ideologie; 
};

struct PartidPolitic initializare(int id, int nrMembri, const char* denumire,
    float procentSondaj, char ideologie) {

    struct PartidPolitic p;

    p.id = id;
    p.nrMembri = nrMembri;

    p.denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(p.denumire, denumire);

    p.procentSondaj = procentSondaj;
    p.ideologie = ideologie;

    return p;
}

struct PartidPolitic copiazaPartid(struct PartidPolitic sursa) {
    return initializare(sursa.id, sursa.nrMembri,
        sursa.denumire, sursa.procentSondaj, sursa.ideologie);
}

void afisare(struct PartidPolitic p) {
    printf("ID: %d\n", p.id);
    printf("Nr membri: %d\n", p.nrMembri);
    printf("Denumire: %s\n", p.denumire);
    printf("Procent sondaj: %.2f%%\n", p.procentSondaj);
    printf("Ideologie: %c\n\n", p.ideologie);
}

void afisareVector(struct PartidPolitic* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct PartidPolitic* copiazaPrimeleNElemente(
    struct PartidPolitic* vector,
    int nrElemente,
    int nrElementeCopiate) {

    if (nrElementeCopiate > nrElemente)
        nrElementeCopiate = nrElemente;

    struct PartidPolitic* vectorNou =
        (struct PartidPolitic*)malloc(sizeof(struct PartidPolitic) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = copiazaPartid(vector[i]);
    }

    return vectorNou;
}

void dezalocare(struct PartidPolitic** vector, int* nrElemente) {

    if (*vector != NULL) {
        for (int i = 0; i < *nrElemente; i++) {
            free((*vector)[i].denumire);
        }

        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaPartideCuProcentMare(
    struct PartidPolitic* vector,
    int nrElemente,
    float procentMin,
    struct PartidPolitic** vectorNou,
    int* dimensiune) {

    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].procentSondaj > procentMin) {
            (*dimensiune)++;
        }
    }

    *vectorNou = (struct PartidPolitic*)
        malloc(sizeof(struct PartidPolitic) * (*dimensiune));

    int contor = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].procentSondaj > procentMin) {
            (*vectorNou)[contor++] = copiazaPartid(vector[i]);
        }
    }
}

struct PartidPolitic getPrimulElementConditionat(
    struct PartidPolitic* vector,
    int nrElemente,
    const char* conditie) {

    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].denumire, conditie) == 0) {
            return copiazaPartid(vector[i]);
        }
    }

    return initializare(-1, 0, "Necunoscut", 0, '-');
}

int main() {

    int nrTotalPartide = 5;

    struct PartidPolitic* lista =
        (struct PartidPolitic*)malloc(sizeof(struct PartidPolitic) * nrTotalPartide);

    lista[0] = initializare(1, 50000, "Partid Liberal", 28.5, 'L');
    lista[1] = initializare(2, 72000, "Partid Conservator", 32.1, 'C');
    lista[2] = initializare(3, 31000, "Partid Verde", 12.4, 'V');
    lista[3] = initializare(4, 45000, "Partid Socialist", 18.9, 'S');
    lista[4] = initializare(5, 15000, "Partid National", 6.3, 'N');

    printf("VECTOR INITIAL:\n");
    afisareVector(lista, nrTotalPartide);

    int nrCopiate = 3;
    struct PartidPolitic* primele =
        copiazaPrimeleNElemente(lista, nrTotalPartide, nrCopiate);

    printf("\nPRIMELE %d PARTIDE:\n", nrCopiate);
    afisareVector(primele, nrCopiate);

    dezalocare(&primele, &nrCopiate);

    struct PartidPolitic* filtrate = NULL;
    int nrFiltrate = 0;

    copiazaPartideCuProcentMare(lista, nrTotalPartide,
        20.0, &filtrate, &nrFiltrate);

    printf("\nPARTIDE CU PROCENT > 20%%:\n");
    afisareVector(filtrate, nrFiltrate);

    dezalocare(&filtrate, &nrFiltrate);
    dezalocare(&lista, &nrTotalPartide);

    return 0;
}