#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct NodArbore NodArbore;
struct NodArbore {
	Masina info;
	NodArbore* right;
	NodArbore* left;
};

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(NodArbore* radacina) {
	if (radacina == NULL)return 0;
	int dr = calculeazaInaltimeArbore(radacina->right);
	int st = calculeazaInaltimeArbore(radacina->left);

	return 1+max(dr,st);
}

void rotireStanga(NodArbore** radacina) {
	NodArbore* aux = (*radacina)->right;
	(*radacina)->right = aux->left;
	aux->left = (*radacina);
	(*radacina) = aux;

}

void rotireDreapta(NodArbore** radacina) {
	NodArbore* aux = (*radacina)->left;
	(*radacina)->left = aux->right;
	aux->right = (*radacina);
	(*radacina) = aux;
}

int gradEchilibru(NodArbore* radacina) {
	int grad = calculeazaInaltimeArbore(radacina->left) - calculeazaInaltimeArbore(radacina->right);
	return grad;
}

void adaugaMasinaInArboreEchilibrat(NodArbore** radacina,  Masina masinaNoua) {
	if (*radacina!=NULL) {
		if (masinaNoua.id > (*radacina)->info.id)
			adaugaMasinaInArboreEchilibrat(&(*radacina)->right, masinaNoua);
		else
			adaugaMasinaInArboreEchilibrat(&(*radacina)->left, masinaNoua);

		//incepe alg de verificare a echilibrarii
		int factorEchilibru = gradEchilibru((*radacina));
		if (factorEchilibru == -2) { 
			//dezechilibru la dreapta
			if(gradEchilibru((*radacina)->right)==-1)
			{
				rotireStanga(radacina);
			}
			else {
				rotireDreapta((*radacina)->right);
				rotireStanga(radacina);
			}
		}
		else if (factorEchilibru == 2) { 
			//dezechilibru stanga
			if(gradEchilibru((*radacina)->left)==-1)
				rotireStanga((*radacina)->left);
			rotireDreapta(radacina);
		}
		
	}
	else {
		NodArbore* nou = malloc(sizeof(NodArbore));
		nou->info = masinaNoua;
		nou->right = NULL;
		nou->left = NULL;
		(*radacina) = nou;
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;

	while (!feof(f)) {
		adaugaMasinaInArboreEchilibrat(&radacina, citireMasinaDinFisier(f));
	}
	fclose(f);
	return radacina;

}

void afisareMasiniDinArbore(NodArbore* radacina) {
	if (radacina) {
		afisareMasina(radacina->info);
		afisareMasiniDinArbore(radacina->left);
		afisareMasiniDinArbore(radacina->right);

	}
}

void dezalocareArboreDeMasini(NodArbore** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*radacina) {
		dezalocareArboreDeMasini(&(*radacina)->left);
		dezalocareArboreDeMasini(&(*radacina)->right);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);

		free(*radacina);
		*radacina = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/arborele de masini/int id);

int determinaNumarNoduri(/arborele de masini/);

float calculeazaPretTotal(/arbore de masini/);

float calculeazaPretulMasinilorUnuiSofer(/arbore de masini/ const char* numeSofer);

int main() {
	NodArbore* radacina = NULL;
	radacina = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArbore(radacina);

	dezalocareArboreDeMasini(&radacina);
	afisareMasiniDinArbore(radacina);
	return 0;
}