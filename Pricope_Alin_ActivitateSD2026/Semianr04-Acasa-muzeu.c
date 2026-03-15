#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMuzeu {
	int id;
	int nrExponate;
	float pretBilet;
	char* numeMuzeu;
	char* oras;
	unsigned char cod;
};
typedef struct StructuraMuzeu Muzeu;

struct Nod {
	Muzeu info;
	struct Nod* next;
};
typedef struct Nod Nod;



Muzeu citireMuzeuDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Muzeu m = { 0 };
		return m;
	}
	char* aux;
	Muzeu m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrExponate = atoi(strtok(NULL, sep));
	m1.pretBilet = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.numeMuzeu = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.numeMuzeu, aux);

	aux = strtok(NULL, sep);
	m1.oras = (char*)malloc(strlen(aux) + 1);
	strcpy(m1.oras, aux);

	m1.cod = *strtok(NULL, sep);
	return m1;
}

void afisareMuzeu(Muzeu muzeu) {
	printf("Id: %d, Exponate: %d, Pret Bilet: %.2f, Nume: %s, Oras: %s, Cod: %c\n",
		muzeu.id, muzeu.nrExponate, muzeu.pretBilet, muzeu.numeMuzeu, muzeu.oras, muzeu.cod);
}

// --- GESTIONARE LISTA ---

void afisareListaMuzee(Nod* cap) {
	while (cap) {
		afisareMuzeu(cap->info);
		cap = cap->next;
	}
}

void adaugaMuzeuInLista(Nod** cap, Muzeu muzeuNou) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = muzeuNou;
	temp->next = NULL;

	if (*cap == NULL) {
		*cap = temp;
	}
	else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = temp;
	}
}

Nod* citireListaMuzeeDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) return NULL;

	Nod* cap = NULL;
	while (!feof(file)) {
		Muzeu m = citireMuzeuDinFisier(file);
		if (m.numeMuzeu != NULL) { 
			adaugaMuzeuInLista(&cap, m);
		}
	}
	fclose(file);
	return cap;
}

void dezalocareListaMuzee(Nod** cap) {
	while (*cap) {
		free((*cap)->info.numeMuzeu);
		free((*cap)->info.oras);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	*cap = NULL;
}



float calculeazaPretMediuBilet(Nod* cap) {
	if (!cap) return 0;
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.pretBilet;
		contor++;
		cap = cap->next;
	}
	return (contor > 0) ? (suma / contor) : 0;
}

void stergeMuzeeCuCodul(Nod** cap, char codCautat) {
	while (*cap && (*cap)->info.cod == codCautat) {
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.numeMuzeu);
		free(temp->info.oras);
		free(temp);
	}

	if (*cap == NULL) return;

	Nod* p = *cap;
	while (p->next) {
		if (p->next->info.cod == codCautat) {
			Nod* temp = p->next;
			p->next = temp->next;
			free(temp->info.numeMuzeu);
			free(temp->info.oras);
			free(temp);
		}
		else {
			p = p->next;
		}
	}
}

float calculeazaPretBileteMuzeeDinOras(Nod* cap, const char* oras) {
	if (!cap || !oras) return 0;
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.oras, oras) == 0) {
			suma += cap->info.pretBilet;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* cap = citireListaMuzeeDinFisier("muzee.txt");
	if (cap) {
		printf("=== Lista initiala ===\n");
		afisareListaMuzee(cap);

		printf("\nPret mediu bilet: %.2f\n", calculeazaPretMediuBilet(cap));

		printf("\nStergem muzeele cu codul 'A'...\n");
		stergeMuzeeCuCodul(&cap, 'A');

		printf("\n=== Lista dupa stergere ===\n");
		afisareListaMuzee(cap);

		dezalocareListaMuzee(&cap);
		printf("\nMemorie eliberata. Program terminat.\n");
	}
	else {
		printf("Fisierul nu a putut fi deschis sau este gol!\n");
	}
	return 0;
}