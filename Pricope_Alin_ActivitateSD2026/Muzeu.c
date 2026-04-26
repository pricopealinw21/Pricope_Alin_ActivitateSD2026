#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMuzeu {
	int id;
	int nrSali;
	float pretBilet;
	char* nume;
	char* curator;
	unsigned char sectiune;
};
typedef struct StructuraMuzeu Muzeu;

struct Heap {
	int lungime;
	Muzeu* vector;
	int nrMuzee;
};
typedef struct Heap Heap;

Muzeu citireMuzeuDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Muzeu m;
	aux = strtok(buffer, sep);
	m.id = atoi(aux);
	m.nrSali = atoi(strtok(NULL, sep));
	m.pretBilet = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m.nume = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.curator = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m.curator, strlen(aux) + 1, aux);

	m.sectiune = *strtok(NULL, sep);
	return m;
}

void afisareMuzeu(Muzeu muzeu) {
	printf("Id: %d\n", muzeu.id);
	printf("Nr. sali: %d\n", muzeu.nrSali);
	printf("Pret bilet: %.2f\n", muzeu.pretBilet);
	printf("Nume muzeu: %s\n", muzeu.nume);
	printf("Curator: %s\n", muzeu.curator);
	printf("Sectiune: %c\n\n", muzeu.sectiune);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrMuzee = 0;
	heap.vector = (Muzeu*)malloc(sizeof(Muzeu) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (pozFiuSt < heap.nrMuzee && heap.vector[pozMax].pretBilet < heap.vector[pozFiuSt].pretBilet) {
		pozMax = pozFiuSt;
	}

	if (pozFiuDr < heap.nrMuzee && heap.vector[pozMax].pretBilet < heap.vector[pozFiuDr].pretBilet) {
		pozMax = pozFiuDr;
	}

	if (pozMax != pozitieNod) {
		Muzeu aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax <= (heap.nrMuzee - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDeMuzeeDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrMuzee++] = citireMuzeuDinFisier(file);
	}

	fclose(file);

	for (int i = (heap.nrMuzee - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMuzee; i++) {
		afisareMuzeu(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMuzee; i < heap.lungime; i++) {
		afisareMuzeu(heap.vector[i]);
	}
}

Muzeu extrageMuzeu(Heap* heap) {
	if (heap->nrMuzee > 0) {
		Muzeu aux = heap->vector[0];

		heap->vector[0] = heap->vector[heap->nrMuzee - 1];
		heap->vector[heap->nrMuzee - 1] = aux;

		heap->nrMuzee--;

		for (int i = (heap->nrMuzee - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].nume);
		free(heap->vector[i].curator);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrMuzee = 0;
}

int main() {
	Heap heap = citireHeapDeMuzeeDinFisier("muzee.txt");

	afisareHeap(heap);

	printf("Muzee extrase:\n");
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));
	afisareMuzeu(extrageMuzeu(&heap));

	printf("Heap-ul ascuns:\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}