/*Implementați funcția care creează un arbore Shannon–Fano.
Dintr-un fișer se citesc caracterele.
Pentru fiecare caracter se numără câte apariții are în fișer (vector structură numă apariții și caracter).
Se sortează după numărul de apariții.
Se crează arborele Shannon–Fano împărțind vector în două bucăți în mod recursiv. Inițial rădăcina va avea tot vectorul, apoi cei 2 copii vor avea fiecare câte jumate, și tot așa până ce fiecare nod va avea maxim un caracter. Împărțirea se va face nu după numărul de elemente ci după suma probabilităților. (Hint reprezentare: căutare binară)
Implementați funcția care să afișeze un arbore Shannon–Fano.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARACTERS 256
#define MAX_FILENAME 100


typedef struct {
    char character;
    int frequency;
} CharFreq;


typedef struct Node {
    char character;
    int frequency;
    struct Node* left;
    struct Node* right;
} Node;


int compareFreq(const void* a, const void* b) {
    CharFreq* fa = (CharFreq*)a;
    CharFreq* fb = (CharFreq*)b;
    return fb->frequency - fa->frequency;
}


int calculateSum(CharFreq* arr, int start, int end) {
    int sum = 0;
    for (int i = start; i <= end; i++) {
        sum += arr[i].frequency;
    }
    return sum;
}


int findSplitPoint(CharFreq* arr, int start, int end, int totalSum) {
    int leftSum = 0;
    int bestDiff = totalSum;
    int bestIndex = start;

    for (int i = start; i < end; i++) {
        leftSum += arr[i].frequency;
        int rightSum = totalSum - leftSum;
        int diff = abs(leftSum - rightSum);

        if (diff < bestDiff) {
            bestDiff = diff;
            bestIndex = i;
        }
    }

    return bestIndex;
}

Node* buildShannonFanoTree(CharFreq* arr, int start, int end) {

    if (start == end) {
        Node* leaf = (Node*)malloc(sizeof(Node));
        leaf->character = arr[start].character;
        leaf->frequency = arr[start].frequency;
        leaf->left = NULL;
        leaf->right = NULL;
        return leaf;
    }

    int totalSum = calculateSum(arr, start, end);
    int splitIndex = findSplitPoint(arr, start, end, totalSum);

    Node* internalNode = (Node*)malloc(sizeof(Node));
    internalNode->character = '\0'; 
    internalNode->frequency = totalSum;


    internalNode->left = buildShannonFanoTree(arr, start, splitIndex);
    internalNode->right = buildShannonFanoTree(arr, splitIndex + 1, end);

    return internalNode;
}


void printTree(Node* root, int level) {
    if (root == NULL) return;

   
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

  
    if (root->character != '\0') {
      
        if (root->character == '\n')
            printf("Caracter: '\\n' (newline), Frecvență: %d\n", root->frequency);
        else if (root->character == ' ')
            printf("Caracter: ' ' (spațiu), Frecvență: %d\n", root->frequency);
        else
            printf("Caracter: '%c', Frecvență: %d\n", root->character, root->frequency);
    }
    else {
        
        printf("Nod intern, Sumă frecvențe: %d\n", root->frequency);
    }

    
    printTree(root->left, level + 1);
    printTree(root->right, level + 1);
}


void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}


int countFrequencies(const char* filename, CharFreq* freqs, int* size) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Eroare: Nu s-a putut deschide fișierul %s\n", filename);
        return 0;
    }

    int freqArray[256] = { 0 };
    char ch;

   
    while ((ch = fgetc(file)) != EOF) {
        unsigned char uch = (unsigned char)ch;
        freqArray[uch]++;
    }

    fclose(file);

  
    *size = 0;
    for (int i = 0; i < 256; i++) {
        if (freqArray[i] > 0) {
            freqs[*size].character = (char)i;
            freqs[*size].frequency = freqArray[i];
            (*size)++;
        }
    }

    return 1;
}

int main() {
    char filename[MAX_FILENAME];
    CharFreq freqs[MAX_CHARACTERS];
    int size = 0;

    printf("Introduceți numele fișierului: ");
    scanf("%s", filename);

   
    if (!countFrequencies(filename, freqs, &size)) {
        return 1;
    }

    if (size == 0) {
        printf("Fișierul este gol!\n");
        return 1;
    }


    qsort(freqs, size, sizeof(CharFreq), compareFreq);

  
    printf("\nCaractere și frecvențe (sortate descrescător):\n");
    for (int i = 0; i < size; i++) {
        if (freqs[i].character == '\n')
            printf("'\\n' (newline): %d\n", freqs[i].frequency);
        else if (freqs[i].character == ' ')
            printf("' ' (spațiu): %d\n", freqs[i].frequency);
        else
            printf("'%c': %d\n", freqs[i].character, freqs[i].frequency);
    }

    Node* root = buildShannonFanoTree(freqs, 0, size - 1);

    // Afișarea arborelui
    printf("\nArborele Shannon-Fano:\n");
    printf("========================\n");
    printTree(root, 0);

    // Eliberarea memoriei
    freeTree(root);

    return 0;
}