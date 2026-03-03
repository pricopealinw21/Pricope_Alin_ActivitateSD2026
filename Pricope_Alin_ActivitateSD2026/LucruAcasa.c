#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct SectiePolitie {
    int id;
    int nrOfiteri;
    char* denumire;
    float buget;
    char sector;
};

struct SectiePolitie initializare(int id, int nrOfiteri, char* denumire, float buget, char sector) {
    struct SectiePolitie s;
    s.id = id;
    s.nrOfiteri = nrOfiteri;
    s.denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(s.denumire, denumire);
    s.buget = buget;
    s.sector = sector;
    return s;
}

struct SectiePolitie copiazaSectie(struct SectiePolitie sursa) {
    return initializare(sursa.id, sursa.nrOfiteri, sursa.denumire, sursa.buget, sursa.sector);
}

void afisare(struct SectiePolitie s) {
    printf("ID: %d\n", s.id);
    printf("Nr ofiteri: %d\n", s.nrOfiteri);
    printf("Denumire: %s\n", s.denumire);
    printf("Buget: %.2f\n", s.buget);
    printf("Sector: %d\n\n", s.sector);
}

void afisareVector(struct SectiePolitie* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct SectiePolitie* copiazaPrimeleNElemente(struct SectiePolitie* vector,
    int nrElemente,
    int nrElementeCopiate) {

    if (nrElementeCopiate > nrElemente) {
        nrElementeCopiate = nrElemente;
    }

    struct SectiePolitie* vectorNou =
        (struct SectiePolitie*)malloc(sizeof(struct SectiePolitie) * nrElementeCopiate);

    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = copiazaSectie(vector[i]);
    }

    return vectorNou;
}

void dezalocare(struct SectiePolitie** vector, int* nrElemente) {

    if (*vector != NULL) {
        for (int i = 0; i < *nrElemente; i++) {
            free((*vector)[i].denumire);
        }

        free(*vector);
        *vector = NULL;
        *nrElemente = 0;
    }
}

void copiazaSectiiCuBugetMare(struct SectiePolitie* vector,
    int nrElemente,
    float bugetMin,
    struct SectiePolitie** vectorNou,
    int* dimensiune) {

    *dimensiune = 0;

    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].buget > bugetMin) {
            (*dimensiune)++;
        }
    }

    *vectorNou = (struct SectiePolitie*)malloc(sizeof(struct SectiePolitie) * (*dimensiune));

    int contor = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].buget > bugetMin) {
            (*vectorNou)[contor++] = copiazaSectie(vector[i]);
        }
    }
}

struct SectiePolitie getPrimulElementConditionat(struct SectiePolitie* vector,
    int nrElemente,
    const char* conditie) {

    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].denumire, conditie) == 0) {
            return copiazaSectie(vector[i]);
        }
    }

    return initializare(-1, 0, "Necunoscut", 0, 0);
}

//int main() {
//
//    int nrTotalSectii = 5;
//
//    struct SectiePolitie* listaSectii =
//        (struct SectiePolitie*)malloc(sizeof(struct SectiePolitie) * nrTotalSectii);
//
//    listaSectii[0] = initializare(1, 120, "Sectia Centrala", 500000.5, 1);
//    listaSectii[1] = initializare(2, 85, "Sectia Nord", 320000.0, 2);
//    listaSectii[2] = initializare(3, 60, "Sectia Sud", 210000.3, 3);
//    listaSectii[3] = initializare(4, 150, "Sectia Vest", 800000.7, 4);
//    listaSectii[4] = initializare(5, 95, "Sectia Est", 450000.2, 5);
//
//    printf("VECTOR INITIAL:\n");
//    afisareVector(listaSectii, nrTotalSectii);
//
//    int nrElementeDeCopiat = 3;
//    struct SectiePolitie* primeleElemente =
//        copiazaPrimeleNElemente(listaSectii,
//            nrTotalSectii,
//            nrElementeDeCopiat);
//
//    printf("\nPRIMELE %d ELEMENTE COPIATE:\n", nrElementeDeCopiat);
//    afisareVector(primeleElemente, nrElementeDeCopiat);
//
//    dezalocare(&primeleElemente, &nrElementeDeCopiat);
//
//    struct SectiePolitie* sectiiFiltrate = NULL;
//    int nrSectiiFiltrate = 0;
//
//    copiazaSectiiCuBugetMare(listaSectii,
//        nrTotalSectii,
//        300000,
//        &sectiiFiltrate,
//        &nrSectiiFiltrate);
//
//    printf("\nSECTII CU BUGET > 300000:\n");
//    afisareVector(sectiiFiltrate, nrSectiiFiltrate);
//
//    dezalocare(&sectiiFiltrate, &nrSectiiFiltrate);
//
//    dezalocare(&listaSectii, &nrTotalSectii);
//
//    return 0;
//}