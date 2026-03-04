/*Să se scrie programul pentru concatenarea a două sau mai multe fişiere ce conţin numere reale. Se va afişa pe ecran informaţia din fişierul astfel rezultat.
*/


#include <stdio.h>

int main() {
    FILE* f1, * f2, * frez; //am trei fisiere, 2 din care citesc date, si unul in care pun date
    float x;

    // Deschidere fisiere 
    f1 = fopen("f1.txt", "r");
    f2 = fopen("f2.txt", "r");

    if (f1 == NULL || f2 == NULL) {  //testez
        printf("Eroare la deschiderea fisierelor sursa!");
        return 1;
    }


    frez = fopen("rezultat.txt", "w");

    // Copiere din primul 
    while (fscanf(f1, "%f", &x) == 1) {
        fprintf(frez, "%f ", x);
    }

    // Copiere din al doilea fisier
    while (fscanf(f2, "%f", &x) == 1) {
        fprintf(frez, "%f ", x);
    }

    fclose(f1);
    fclose(f2);
    fclose(frez);

    // Afisare rezultat
    frez = fopen("rezultat.txt", "r");

    printf("Continutul fisierului rezultat:\n");
    while (fscanf(frez, "%f", &x) == 1) {  //folosim referina deoarce in C , parmaetrii sunt pasati prin valaore,adica se face copie, pentru a lucra cu originialul ne trebuie adresa ei

        printf("%f ", x);
    }

    fclose(frez);

    return 0;
}
