#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraFilm {
	int id;
	int durata;
	float rating;
	char* titlu;
	char* regizor;
	unsigned char gen;
};
typedef struct StructuraFilm Film;

void afisareFilm(Film film) {
	printf("ID:%d\n", film.id);
	printf("Durata:%d\n", film.durata);
	printf("Rating:%.2f\n", film.rating);
	printf("Titlu:%s\n", film.titlu);
	printf("Regizor:%s\n", film.regizor);
	printf("Gen:%c\n\n", film.gen);
}

void afisareVectorFilme(Film* filme, int nrFilme) {
	for (int i = 0; i < nrFilme; i++) {
		afisareFilm(filme[i]);
	}
}

void adaugaFilmInVector(Film** filme, int* nrFilme, Film filmNou) {
	Film* aux = (Film*)malloc(sizeof(Film) * ((*nrFilme) + 1));
	for (int i = 0; i < *nrFilme; i++) {
		aux[i] = (*filme)[i];
	}
	aux[(*nrFilme)] = filmNou;//shallow copy
	free(*filme);
	(*filme) = aux;
	(*nrFilme)++;
}

Film citireFilmFisier(FILE* file) {
	Film f;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	f.id = atoi(strtok(buffer, sep));
	f.durata = atoi(strtok(NULL, sep));
	f.rating = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	f.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(f.titlu, aux);
	aux = strtok(NULL, sep);
	f.regizor = (char*)malloc(strlen(aux) + 1);
	strcpy(f.regizor, aux);
	f.gen = strtok(NULL, sep)[0];
	return f;
}

Film* citireVectorFilmeFisier(const char* numeFisier, int* nrFilmeCitite) {
	FILE* file = fopen(numeFisier, "r");
	Film* filme = NULL;
	(*nrFilmeCitite) = 0;
	while (!feof(file)) {
		adaugaFilmInVector(&filme, nrFilmeCitite, citireFilmFisier(file));
	}
	fclose(file);
	return filme;
}

void dezalocareVectorFilme(Film** vector, int* nrFilme) {
	for (int i = 0; i < *nrFilme; i++) {
		if ((*vector)[i].titlu != NULL) {
			free((*vector)[i].titlu);
		}
		if ((*vector)[i].regizor != NULL) {
			free((*vector)[i].regizor);
		}
	}
	free(*vector);
	(*vector) = NULL;
	(*nrFilme) = 0;
}

int main() {
	int nrFilme = 0;
	Film* filme = citireVectorFilmeFisier("filme.txt", &nrFilme);
	afisareVectorFilme(filme, nrFilme);
	dezalocareVectorFilme(&filme, &nrFilme);
	return 0;
}