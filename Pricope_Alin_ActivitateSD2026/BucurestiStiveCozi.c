#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraZona {
	int id;
	int nrBlocuri;
	float suprafata;
	char* numeZona;
	char* sector;
	unsigned char cod;
};
typedef struct StructuraZona Zona;


Zona citireZonaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";

	fgets(buffer, 100, file);

	Zona z;
	char* aux = strtok(buffer, sep);

	z.id = atoi(aux);
	z.nrBlocuri = atoi(strtok(NULL, sep));
	z.suprafata = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	z.numeZona = (char*)malloc(strlen(aux) + 1);
	strcpy(z.numeZona, aux);

	aux = strtok(NULL, sep);
	z.sector = (char*)malloc(strlen(aux) + 1);
	strcpy(z.sector, aux);

	z.cod = *strtok(NULL, sep);

	return z;
}

void afisareZona(Zona z) {
	printf("ID: %d | Blocuri: %d | Suprafata: %.2f | Zona: %s | Sector: %s | Cod: %c\n",
		z.id, z.nrBlocuri, z.suprafata, z.numeZona, z.sector, z.cod);
}


typedef struct Nod {
	Zona info;
	struct Nod* next;
} Nod;



typedef struct {
	Nod* varf;
	int nr;
} Stack;

void pushStack(Stack* s, Zona z) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = z;
	nou->next = s->varf;
	s->varf = nou;
	s->nr++;
}

Zona popStack(Stack* s) {
	if (!s->varf) {
		printf("Stiva goala!\n");
		exit(1);
	}
	Nod* temp = s->varf;
	Zona z = temp->info;
	s->varf = temp->next;
	free(temp);
	s->nr--;
	return z;
}

int emptyStack(Stack* s) {
	return s->varf == NULL;
}



typedef struct {
	Nod* prim;
	Nod* ultim;
	int nr;
} Queue;

void enqueue(Queue* q, Zona z) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = z;
	nou->next = NULL;

	if (q->ultim)
		q->ultim->next = nou;
	else
		q->prim = nou;

	q->ultim = nou;
	q->nr++;
}

Zona dequeue(Queue* q) {
	if (!q->prim) {
		printf("Coada goala!\n");
		exit(1);
	}
	Nod* temp = q->prim;
	Zona z = temp->info;

	q->prim = temp->next;
	if (!q->prim)
		q->ultim = NULL;

	free(temp);
	q->nr--;
	return z;
}


Stack citireStack(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Stack s = { NULL, 0 };

	while (!feof(f)) {
		Zona z = citireZonaDinFisier(f);
		pushStack(&s, z);
	}
	fclose(f);
	return s;
}

Queue citireQueue(const char* fisier) {
	FILE* f = fopen(fisier, "r");
	Queue q = { NULL, NULL, 0 };

	while (!feof(f)) {
		Zona z = citireZonaDinFisier(f);
		enqueue(&q, z);
	}
	fclose(f);
	return q;
}



float calculeazaSuprafataTotala(Stack* s) {
	float suma = 0;
	Nod* temp = s->varf;
	while (temp) {
		suma += temp->info.suprafata;
		temp = temp->next;
	}
	return suma;
}

Zona getZonaByID(Stack* s, int id) {
	Nod* temp = s->varf;
	while (temp) {
		if (temp->info.id == id)
			return temp->info;
		temp = temp->next;
	}
	printf("Zona nu exista!\n");
	exit(1);
}



void dezalocareStack(Stack* s) {
	while (!emptyStack(s)) {
		Zona z = popStack(s);
		free(z.numeZona);
		free(z.sector);
	}
}

void dezalocareQueue(Queue* q) {
	while (q->prim) {
		Zona z = dequeue(q);
		free(z.numeZona);
		free(z.sector);
	}
}



int main() {

	Stack s = citireStack("zone.txt");

	printf("Zone din Bucuresti:\n");
	Nod* temp = s.varf;
	while (temp) {
		afisareZona(temp->info);
		temp = temp->next;
	}

	printf("\nSuprafata totala: %.2f\n", calculeazaSuprafataTotala(&s));

	Zona z = getZonaByID(&s, 2);
	printf("\nZona cautata:\n");
	afisareZona(z);

	dezalocareStack(&s);

	return 0;
}