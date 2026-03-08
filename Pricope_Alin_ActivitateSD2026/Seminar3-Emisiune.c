#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraEmisiune {
	int id;
	int durata;
	float rating;
	char* titlu;
	char* prezentator;
	unsigned char categorie;
};
typedef struct StructuraEmisiune Emisiune;

void afisareEmisiune(Emisiune emisiune) {
	printf("ID:%d\n", emisiune.id);
	printf("Durata:%d\n", emisiune.durata);
	printf("Rating:%.2f\n", emisiune.rating);
	printf("Titlu:%s\n", emisiune.titlu);
	printf("Prezentator:%s\n", emisiune.prezentator);
	printf("Categorie:%c\n\n", emisiune.categorie);
}

void afisareVectorEmisiuni(Emisiune* emisiuni, int nrEmisiuni) {
	for (int i = 0; i < nrEmisiuni; i++) {
		afisareEmisiune(emisiuni[i]);
	}
}

void adaugaEmisiuneInVector(Emisiune** emisiuni, int* nrEmisiuni, Emisiune emisiuneNoua) {
	Emisiune* aux = (Emisiune*)malloc(sizeof(Emisiune) * ((*nrEmisiuni) + 1));
	for (int i = 0; i < *nrEmisiuni; i++) {
		aux[i] = (*emisiuni)[i];
	}
	aux[(*nrEmisiuni)] = emisiuneNoua;//shallow copy
	free(*emisiuni);
	(*emisiuni) = aux;
	(*nrEmisiuni)++;
}

Emisiune citireEmisiuneFisier(FILE* file) {
	Emisiune e;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	e.id = atoi(strtok(buffer, sep));
	e.durata = atoi(strtok(NULL, sep));
	e.rating = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	e.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(e.titlu, aux);
	aux = strtok(NULL, sep);
	e.prezentator = (char*)malloc(strlen(aux) + 1);
	strcpy(e.prezentator, aux);
	e.categorie = strtok(NULL, sep)[0];
	return e;
}

Emisiune* citireVectorEmisiuniFisier(const char* numeFisier, int* nrEmisiuniCitite) {
	FILE* file = fopen(numeFisier, "r");
	Emisiune* emisiuni = NULL;
	(*nrEmisiuniCitite) = 0;
	while (!feof(file)) {
		adaugaEmisiuneInVector(&emisiuni, nrEmisiuniCitite, citireEmisiuneFisier(file));
	}
	fclose(file);
	return emisiuni;
}

void dezalocareVectorEmisiuni(Emisiune** vector, int* nrEmisiuni) {
	for (int i = 0; i < *nrEmisiuni; i++) {
		if ((*vector)[i].titlu != NULL) {
			free((*vector)[i].titlu);
		}
		if ((*vector)[i].prezentator != NULL) {
			free((*vector)[i].prezentator);
		}
	}
	free(*vector);
	(*vector) = NULL;
	(*nrEmisiuni) = 0;
}

int main() {
	int nrEmisiuni = 0;
	Emisiune* emisiuni = citireVectorEmisiuniFisier("emisiuni.txt", &nrEmisiuni);
	afisareVectorEmisiuni(emisiuni, nrEmisiuni);
	dezalocareVectorEmisiuni(&emisiuni, &nrEmisiuni);
	return 0;
}