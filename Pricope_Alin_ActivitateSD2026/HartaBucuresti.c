#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraZonaBucuresti {
	int id;
	int nrStrazi;
	float suprafata;
	char* denumireZona;
	char* sector;
	unsigned char codHarta;
};
typedef struct StructuraZonaBucuresti ZonaBucuresti;

struct Heap {
	int lungime;
	ZonaBucuresti* vector;
	int nrZone;
};
typedef struct Heap Heap;

ZonaBucuresti citireZonaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;

	ZonaBucuresti zona;

	aux = strtok(buffer, sep);
	zona.id = atoi(aux);

	zona.nrStrazi = atoi(strtok(NULL, sep));

	zona.suprafata = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	zona.denumireZona = (char*)malloc(strlen(aux) + 1);
	strcpy_s(zona.denumireZona, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	zona.sector = (char*)malloc(strlen(aux) + 1);
	strcpy_s(zona.sector, strlen(aux) + 1, aux);

	zona.codHarta = *strtok(NULL, sep);

	return zona;
}

void afisareZona(ZonaBucuresti zona) {
	printf("Id: %d\n", zona.id);
	printf("Numar strazi: %d\n", zona.nrStrazi);
	printf("Suprafata: %.2f\n", zona.suprafata);
	printf("Denumire zona: %s\n", zona.denumireZona);
	printf("Sector: %s\n", zona.sector);
	printf("Cod harta: %c\n\n", zona.codHarta);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrZone = 0;
	heap.vector = (ZonaBucuresti*)malloc(sizeof(ZonaBucuresti) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;

	if (pozFiuSt < heap.nrZone &&
		heap.vector[pozMax].suprafata < heap.vector[pozFiuSt].suprafata) {
		pozMax = pozFiuSt;
	}

	if (pozFiuDr < heap.nrZone &&
		heap.vector[pozMax].suprafata < heap.vector[pozFiuDr].suprafata) {
		pozMax = pozFiuDr;
	}

	if (pozMax != pozitieNod) {
		ZonaBucuresti aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax <= (heap.nrZone - 2) / 2) {
			filtreazaHeap(heap, pozMax);
		}
	}
}

Heap citireHeapZoneDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);

	while (!feof(file)) {
		heap.vector[heap.nrZone++] = citireZonaDinFisier(file);
	}

	fclose(file);

	for (int i = (heap.nrZone - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrZone; i++) {
		afisareZona(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrZone; i < heap.lungime; i++) {
		afisareZona(heap.vector[i]);
	}
}

ZonaBucuresti extrageZona(Heap* heap) {
	if (heap->nrZone > 0) {
		ZonaBucuresti aux = heap->vector[0];

		heap->vector[0] = heap->vector[heap->nrZone - 1];
		heap->vector[heap->nrZone - 1] = aux;

		heap->nrZone--;

		for (int i = (heap->nrZone - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].denumireZona);
		free(heap->vector[i].sector);
	}

	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrZone = 0;
}

int main() {
	Heap heap = citireHeapZoneDinFisier("harta_bucuresti.txt");

	afisareHeap(heap);

	printf("Zone extrase din harta Bucuresti:\n");
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));
	afisareZona(extrageZona(&heap));

	printf("Heap-ul ascuns:\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}