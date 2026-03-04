#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraObiectMuzeu {
	int id;
	int anAchizitie;
	float valoare;
	char* denumire;
	char* numeAutor;
	char sectie;
};

typedef struct StructuraObiectMuzeu ObiectMuzeu;


void afisareObiect(ObiectMuzeu obiect) {
	printf("ID: %d\n", obiect.id);
	printf("An achizitie: %d\n", obiect.anAchizitie);
	printf("Valoare: %.2f\n", obiect.valoare);
	printf("Denumire: %s\n", obiect.denumire);
	printf("Autor: %s\n", obiect.numeAutor);
	printf("Sectie: %c\n\n", obiect.sectie);
}


void afisareVector(ObiectMuzeu* obiecte, int nrObiecte) {
	for (int i = 0; i < nrObiecte; i++) {
		afisareObiect(obiecte[i]);
	}
}


void adaugaObiectInVector(ObiectMuzeu** obiecte, int* nrObiecte, ObiectMuzeu obiectNou) {

	ObiectMuzeu* temp = (ObiectMuzeu*)realloc(*obiecte,
		sizeof(ObiectMuzeu) * ((*nrObiecte) + 1));

	if (temp != NULL) {
		*obiecte = temp;
		(*obiecte)[*nrObiecte] = obiectNou; // shallow copy
		(*nrObiecte)++;
	}
	else {
		printf("Eroare la realocare!\n");
	}
}


int citireObiectFisier(FILE* file, ObiectMuzeu* obiect) {

	char buffer[256];
	char sep[] = ",;\n";

	if (fgets(buffer, 256, file) == NULL) {
		return 0;
	}

	char* token;

	token = strtok(buffer, sep);
	obiect->id = atoi(token);

	token = strtok(NULL, sep);
	obiect->anAchizitie = atoi(token);

	token = strtok(NULL, sep);
	obiect->valoare = atof(token);

	token = strtok(NULL, sep);
	obiect->denumire = (char*)malloc(strlen(token) + 1);
	strcpy(obiect->denumire, token);

	token = strtok(NULL, sep);
	obiect->numeAutor = (char*)malloc(strlen(token) + 1);
	strcpy(obiect->numeAutor, token);

	token = strtok(NULL, sep);
	obiect->sectie = token[0];

	return 1;
}


ObiectMuzeu* citireVectorObiecte(const char* numeFisier, int* nrObiecte) {

	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Fisierul nu a putut fi deschis!\n");
		return NULL;
	}

	ObiectMuzeu* obiecte = NULL;
	*nrObiecte = 0;

	ObiectMuzeu obiect;

	while (citireObiectFisier(f, &obiect)) {
		adaugaObiectInVector(&obiecte, nrObiecte, obiect);
	}

	fclose(f);
	return obiecte;
}


void dezalocareVector(ObiectMuzeu** obiecte, int* nrObiecte) {

	for (int i = 0; i < *nrObiecte; i++) {
		free((*obiecte)[i].denumire);
		free((*obiecte)[i].numeAutor);
	}

	free(*obiecte);
	*obiecte = NULL;
	*nrObiecte = 0;
}


int main() {

	int nrObiecte = 0;

	ObiectMuzeu* obiecte =
		citireVectorObiecte("muzeu.txt", &nrObiecte);

	afisareVector(obiecte, nrObiecte);

	dezalocareVector(&obiecte, &nrObiecte);

	return 0;
}