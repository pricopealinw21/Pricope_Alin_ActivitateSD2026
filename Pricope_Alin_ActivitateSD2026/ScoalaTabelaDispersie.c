#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




struct StructuraElev {
	int id;
	int varsta;
	float medie;
	char* nume;
	char* clasa;
	unsigned char initiala;
};
typedef struct StructuraElev Elev;



typedef struct Nod {
	Elev info;
	struct Nod* next;
} Nod;


typedef struct HashTable {
	int dim;
	Nod** vector;
} HashTable;



Elev citireElevDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	Elev e;
	char* aux = strtok(buffer, sep);

	e.id = atoi(aux);
	e.varsta = atoi(strtok(NULL, sep));
	e.medie = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	e.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(e.nume, aux);

	aux = strtok(NULL, sep);
	e.clasa = (char*)malloc(strlen(aux) + 1);
	strcpy(e.clasa, aux);

	e.initiala = *strtok(NULL, sep);

	return e;
}

void afisareElev(Elev e) {
	printf("ID: %d | Varsta: %d | Medie: %.2f | Nume: %s | Clasa: %s | Initiala: %c\n",
		e.id, e.varsta, e.medie, e.nume, e.clasa, e.initiala);
}


void adaugaElevInLista(Nod** cap, Elev e) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = e;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* temp = *cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void afisareListaElevi(Nod* cap) {
	while (cap) {
		afisareElev(cap->info);
		cap = cap->next;
	}
}


HashTable initializareHashTable(int dim) {
	HashTable ht;
	ht.dim = dim;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);

	for (int i = 0; i < dim; i++)
		ht.vector[i] = NULL;

	return ht;
}

int calculeazaHash(int id, int dim) {
	return id % dim;
}

void inserareElevInTabela(HashTable ht, Elev e) {
	int poz = calculeazaHash(e.id, ht.dim);
	adaugaElevInLista(&(ht.vector[poz]), e);
}



HashTable citireEleviDinFisier(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	HashTable ht = initializareHashTable(5);

	while (!feof(f)) {
		Elev e = citireElevDinFisier(f);
		inserareElevInTabela(ht, e);
	}

	fclose(f);
	return ht;
}



void afisareTabela(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d:\n", i);
		afisareListaElevi(ht.vector[i]);
	}
}



float* calculeazaMedii(HashTable ht, int* nr) {
	float* vector = (float*)malloc(sizeof(float) * ht.dim);
	*nr = 0;

	for (int i = 0; i < ht.dim; i++) {
		Nod* temp = ht.vector[i];
		float suma = 0;
		int count = 0;

		while (temp) {
			suma += temp->info.medie;
			count++;
			temp = temp->next;
		}

		if (count > 0) {
			vector[*nr] = suma / count;
			(*nr)++;
		}
	}

	return vector;
}



Elev getElevByID(HashTable ht, int id) {
	int poz = calculeazaHash(id, ht.dim);
	Nod* temp = ht.vector[poz];

	while (temp) {
		if (temp->info.id == id)
			return temp->info;
		temp = temp->next;
	}

	printf("Elevul nu exista!\n");
	exit(1);
}


void dezalocare(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* temp = ht->vector[i];
		while (temp) {
			Nod* aux = temp;
			free(aux->info.nume);
			free(aux->info.clasa);
			temp = temp->next;
			free(aux);
		}
	}
	free(ht->vector);
}



int main() {

	HashTable ht = citireEleviDinFisier("elevi.txt");

	printf("TABELA HASH:\n");
	afisareTabela(ht);

	int nr;
	float* medii = calculeazaMedii(ht, &nr);

	printf("\nMedii pe clustere:\n");
	for (int i = 0; i < nr; i++) {
		printf("%.2f ", medii[i]);
	}

	printf("\n\nElev cautat:\n");
	Elev e = getElevByID(ht, 2);
	afisareElev(e);

	free(medii);
	dezalocare(&ht);

	return 0;
}