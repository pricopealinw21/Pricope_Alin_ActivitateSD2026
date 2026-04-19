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



Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	Masina m;
	char* aux = strtok(buffer, sep);

	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy(m.numeSofer, aux);

	m.serie = *strtok(NULL, sep);

	return m;
}

void afisareMasina(Masina m) {
	printf("ID: %d | Usi: %d | Pret: %.2f | Model: %s | Sofer: %s | Serie: %c\n",
		m.id, m.nrUsi, m.pret, m.model, m.numeSofer, m.serie);
}



typedef struct Nod {
	Masina info;
	struct Nod* next;
} Nod;



typedef struct {
	Nod* varf;
	int nr;
} Stack;

void pushStack(Stack* s, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = s->varf;
	s->varf = nou;
	s->nr++;
}

Masina popStack(Stack* s) {
	if (!s->varf) {
		printf("Stiva goala!\n");
		exit(1);
	}
	Nod* temp = s->varf;
	Masina m = temp->info;
	s->varf = temp->next;
	free(temp);
	s->nr--;
	return m;
}

int emptyStack(Stack* s) {
	return s->varf == NULL;
}

int sizeStack(Stack* s) {
	return s->nr;
}



typedef struct {
	Nod* prim;
	Nod* ultim;
	int nr;
} Queue;

void enqueue(Queue* q, Masina m) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;

	if (q->ultim)
		q->ultim->next = nou;
	else
		q->prim = nou;

	q->ultim = nou;
	q->nr++;
}

Masina dequeue(Queue* q) {
	if (!q->prim) {
		printf("Coada goala!\n");
		exit(1);
	}
	Nod* temp = q->prim;
	Masina m = temp->info;

	q->prim = temp->next;
	if (!q->prim)
		q->ultim = NULL;

	free(temp);
	q->nr--;
	return m;
}



Stack citireStack(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Stack s = { NULL, 0 };

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&s, m);
	}
	fclose(f);
	return s;
}

Queue citireQueue(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Queue q = { NULL, NULL, 0 };

	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&q, m);
	}
	fclose(f);
	return q;
}



float calculeazaPretTotalStack(Stack* s) {
	float suma = 0;
	Nod* temp = s->varf;
	while (temp) {
		suma += temp->info.pret;
		temp = temp->next;
	}
	return suma;
}

Masina getMasinaByIDStack(Stack* s, int id) {
	Nod* temp = s->varf;
	while (temp) {
		if (temp->info.id == id)
			return temp->info;
		temp = temp->next;
	}
	printf("Nu exista!\n");
	exit(1);
}



void dezalocareStack(Stack* s) {
	while (!emptyStack(s)) {
		Masina m = popStack(s);
		free(m.model);
		free(m.numeSofer);
	}
}

void dezalocareQueue(Queue* q) {
	while (q->prim) {
		Masina m = dequeue(q);
		free(m.model);
		free(m.numeSofer);
	}
}



int main() {

	Stack s = citireStack("masini.txt");

	printf("Masini in stack:\n");
	Nod* temp = s.varf;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
	}

	printf("\nPret total: %.2f\n", calculeazaPretTotalStack(&s));

	Masina m = getMasinaByIDStack(&s, 2);
	printf("\nMasina cautata:\n");
	afisareMasina(m);

	dezalocareStack(&s);

	return 0;
}