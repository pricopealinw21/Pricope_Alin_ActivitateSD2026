//Probleme clasice Liste

//Eliminarea tutuor valorilor pare 

#include<stdlib.h>
#include<stdio.h>

typedef struct Nod
{
	int val;
	struct Nod* next;


}Nod; 


//adaugam la inceput
 
void adaugaInceput(Nod** cap, int valoare)  //avem nevoie de ** pentru nu a fi copie
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->val = valoare;
	nou->next = *cap;  //mereu ia adresa de la inceput
	*cap = nou; //si reia,se muta, in fata era acea valoare, acum se afla acolo
}

//eliminam valorile pare

void eliminarePare(Nod** cap)

{
	//stergem elementele pare de la inceput 
	while (*cap != NULL && (*cap)->val % 2 == 0)
	{
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp);
		//deci am gasit nod cu valoare para,adica informatia utila para, facem o copie pentru a sterge pe rand valorile, iar cu originalul mergem mai departe
	}



	//acum daca nu dam de un nod cu valoare para  

	Nod* curent = *cap;
   
	while (curent != NULL && curent->next != NULL)
	{
		if (curent->next->val % 2 == 0)
		{
			Nod* temp = curent->next;
			curent->next=temp->next; 
			free(temp);

		}
		else
		{
			curent = curent->next;
		}
	 }
}

//functie pentru afisare lista
 
void afisareLista(Nod* cap)
{
	while (cap != NULL)
	{
	printf("%d ", cap->val);
	cap = cap -> next;
	}

	printf("\n"); 

 }

int main()
{
	Nod* cap = NULL;
	adaugaInceput(&cap, 8);
	adaugaInceput(&cap, 3);
	adaugaInceput(&cap, 2);
	adaugaInceput(&cap, 6);
	adaugaInceput(&cap, 12);
	adaugaInceput(&cap, 10);
	adaugaInceput(&cap, 5); 
	adaugaInceput(&cap, 3);
	adaugaInceput(&cap, 7);

	printf("lista initiala : ");
	afisareLista(cap); 
	 
	eliminarePare(&cap);
	 
	 
	printf("lista dupa eliminarea celor pare : "); 
	afisareLista(cap); 

	return 0; 
}