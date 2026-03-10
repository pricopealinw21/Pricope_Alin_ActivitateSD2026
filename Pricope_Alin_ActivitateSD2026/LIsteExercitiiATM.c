/*Implementați următoarele funcționalități, pentru o listă dublu înlănțuită și circulară:

Inserarea unui element la începutul listei. insertNodeHeadOfList
Afișarea unei liste. printList
Afișarea listei în ordine inversă. printListRev
Inserarea unui element înaintea unui nod precizat printr-o cheie “key”. insertNodeBefore
Inserarea unui element după un nod precizat printr-o cheie “key”. insertNodeAfter
Inserarea unui element la sfârșitul listei. insertNodeTailOfList
Ștergerea unui element de la începutul listei. removeNodeHeadOfList
Ștergerea unui element de la finalul listei. removeNodeTailOfList
Ștergerea unui element din orice loc din listă. removeNodeFromList*/

#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {
    int valoare;
    struct Nod* urmator;
    struct Nod* anterior;
} Nod;


Nod* creeazaNod(int valoare) {

    Nod* nodNou = (Nod*)malloc(sizeof(Nod));

    nodNou->valoare = valoare;
    nodNou->urmator = nodNou;
    nodNou->anterior = nodNou;

    return nodNou;
}

Nod* inserareInceputLista(Nod* capLista, int valoare) {

    Nod* nodNou = creeazaNod(valoare);

    if (capLista == NULL) {
        return nodNou;
    }

    Nod* ultim = capLista->anterior;

    nodNou->urmator = capLista;
    nodNou->anterior = ultim;

    ultim->urmator = nodNou;
    capLista->anterior = nodNou;

    return nodNou;
}


void afisareLista(Nod* capLista) {

    if (capLista == NULL) {
        printf("Lista este goala\n");
        return;
    }

    Nod* temp = capLista;

    do {
        printf("%d ", temp->valoare);
        temp = temp->urmator;
    } while (temp != capLista);

    printf("\n");
}


void afisareListaInvers(Nod* capLista) {

    if (capLista == NULL) {
        printf("Lista este goala\n");
        return;
    }

    Nod* temp = capLista->anterior;

    do {
        printf("%d ", temp->valoare);
        temp = temp->anterior;
    } while (temp != capLista->anterior);

    printf("\n");
}

Nod* inserareDupaCheie(Nod* capLista, int cheie, int valoare) {

    if (capLista == NULL)
        return NULL;

    Nod* temp = capLista;

    do {

        if (temp->valoare == cheie) {

            Nod* nodNou = creeazaNod(valoare);

            nodNou->urmator = temp->urmator;
            nodNou->anterior = temp;

            temp->urmator->anterior = nodNou;
            temp->urmator = nodNou;

            return capLista;
        }

        temp = temp->urmator;

    } while (temp != capLista);

    printf("Cheia nu a fost gasita\n");
    return capLista;
}

Nod* inserareInainteCheie(Nod* capLista, int cheie, int valoare) {

    if (capLista == NULL)
        return NULL;

    Nod* temp = capLista;

    do {

        if (temp->valoare == cheie) {

            Nod* nodNou = creeazaNod(valoare);

            nodNou->urmator = temp;
            nodNou->anterior = temp->anterior;

            temp->anterior->urmator = nodNou;
            temp->anterior = nodNou;

            if (temp == capLista)
                capLista = nodNou;

            return capLista;
        }

        temp = temp->urmator;

    } while (temp != capLista);

    printf("Cheia nu a fost gasita\n");
    return capLista;
}

Nod* inserareSfarsitLista(Nod* capLista, int valoare) {

    Nod* nodNou = creeazaNod(valoare);

    if (capLista == NULL)
        return nodNou;

    Nod* ultim = capLista->anterior;

    nodNou->urmator = capLista;
    nodNou->anterior = ultim;

    ultim->urmator = nodNou;

    capLista->anterior = nodNou;

    return capLista;
}




Nod* stergereInceputLista(Nod* capLista) {

    if (capLista == NULL)
        return NULL;

    if (capLista->urmator == capLista) {
        free(capLista);
        return NULL;
    }

    Nod* ultim = capLista->anterior;
    Nod* temp = capLista;

    capLista = capLista->urmator;

    ultim->urmator = capLista;
    capLista->anterior = ultim;

    free(temp);

    return capLista;




}

Nod* stergereSfarsitLista(Nod* capLista) {

    if (capLista == NULL)
        return NULL;

    if (capLista->urmator == capLista) {
        free(capLista);
        return NULL;
    }

    Nod* ultim = capLista->anterior;
    Nod* nouUltim = ultim->anterior;

    nouUltim->urmator = capLista;
    capLista->anterior = nouUltim;

    free(ultim);

    return capLista;
} 
 

  
Nod* stergereDupaValoare(Nod* capLista, int cheie) {

    if (capLista == NULL)
        return NULL;

    Nod* temp = capLista;

    do {

        if (temp->valoare == cheie) {

            if (temp->urmator == temp) {
                free(temp);
                return NULL;
            }

            temp->anterior->urmator = temp->urmator;
            temp->urmator->anterior = temp->anterior;

            if (temp == capLista)
                capLista = temp->urmator;

            free(temp);
            return capLista;
        }

        temp = temp->urmator;

    } while (temp != capLista);

    printf("Elementul nu exista\n");
    return capLista;
}



int main() {

    Nod* capLista = NULL;

    capLista = inserareInceputLista(capLista, 10);
    capLista = inserareInceputLista(capLista, 5);
    capLista = inserareSfarsitLista(capLista, 20);

    afisareLista(capLista);

    capLista = inserareDupaCheie(capLista, 10, 15);
    afisareLista(capLista);

    capLista = stergereDupaValoare(capLista, 10);
    afisareLista(capLista);

    afisareListaInvers(capLista);

    return 0;
}