#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraBloc {
	int id;
	int nrEtaje;
	float suprafata;
	char* adresa;
	char* numeAdministrator;
	unsigned char scara;
};
typedef struct StructuraBloc Bloc;

struct Nod {
	Bloc info;
	struct Nod* next;
};
typedef struct Nod Nod;



Bloc citireBlocDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Bloc b = { 0 };
		return b;
	}
	char* aux;
	Bloc b1;
	aux = strtok(buffer, sep);
	b1.id = atoi(aux);
	b1.nrEtaje = atoi(strtok(NULL, sep));
	b1.suprafata = (float)atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	b1.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy(b1.adresa, aux);

	aux = strtok(NULL, sep);
	b1.numeAdministrator = (char*)malloc(strlen(aux) + 1);
	strcpy(b1.numeAdministrator, aux);

	b1.scara = *strtok(NULL, sep);
	return b1;
}

void afisareBloc(Bloc bloc) {
	printf("Id: %d, Etaje: %d, Suprafata: %.2f, Adresa: %s, Administrator: %s, Scara: %c\n",
		bloc.id, bloc.nrEtaje, bloc.suprafata, bloc.adresa, bloc.numeAdministrator, bloc.scara);
}



void afisareListaBlocuri(Nod* cap) {
	while (cap) {
		afisareBloc(cap->info);
		cap = cap->next;
	}
}

void adaugaBlocInLista(Nod** cap, Bloc blocNou) {
	Nod* temp = (Nod*)malloc(sizeof(Nod));
	temp->info = blocNou;
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

Nod* citireListaBlocuriDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) return NULL;

	Nod* cap = NULL;
	while (!feof(file)) {
		Bloc b = citireBlocDinFisier(file);
		if (b.adresa != NULL) { 
			adaugaBlocInLista(&cap, b);
		}
	}
	fclose(file);
	return cap;
}

void dezalocareListaBlocuri(Nod** cap) {
	while (*cap) {
		free((*cap)->info.adresa);
		free((*cap)->info.numeAdministrator);
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
	}
	*cap = NULL;
}



float calculeazaSuprafataMedie(Nod* cap) {
	if (!cap) return 0;
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.suprafata;
		contor++;
		cap = cap->next;
	}
	return (contor > 0) ? (suma / contor) : 0;
}

void stergeBlocuriDePeScara(Nod** cap, char scaraCautata) {
	while (*cap && (*cap)->info.scara == scaraCautata) {
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.adresa);
		free(temp->info.numeAdministrator);
		free(temp);
	}

	if (*cap == NULL) return;

	Nod* p = *cap;
	while (p->next) {
		if (p->next->info.scara == scaraCautata) {
			Nod* temp = p->next;
			p->next = temp->next;
			free(temp->info.adresa);
			free(temp->info.numeAdministrator);
			free(temp);
		}
		else {
			p = p->next;
		}
	}
}

float calculeazaSuprafataBlocurilorAdministrateDe(Nod* cap, const char* numeAdministrator) {
	if (!cap || !numeAdministrator) return 0;
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeAdministrator, numeAdministrator) == 0) {
			suma += cap->info.suprafata;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* cap = citireListaBlocuriDinFisier("blocuri.txt");
	if (cap) {
		printf("=== Lista initiala ===\n");
		afisareListaBlocuri(cap);

		printf("\nSuprafata medie: %.2f\n", calculeazaSuprafataMedie(cap));

		printf("\nStergem blocurile de pe scara 'A'...\n");
		stergeBlocuriDePeScara(&cap, 'A');

		printf("\n=== Lista dupa stergere ===\n");
		afisareListaBlocuri(cap);

		dezalocareListaBlocuri(&cap);
		printf("\nMemorie eliberata. Program terminat.\n");
	}
	else {
		printf("Fisierul nu a putut fi deschis sau este gol!\n");
	}
	return 0;
}