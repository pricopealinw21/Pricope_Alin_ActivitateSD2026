/*. Creaţi o listă simplu înlănţuită care să permită evidenţa unor produse caracterizate prin următoarele informaţii: int id_produs; int cant_produs; float preţ_produs. Scrieţi un program C în care să realizezi următoarele operaţii:
a) Populaţi lista simplu înlănţuită cu informaţii despre câteva produse distincte, simulând astfel gestionarea evidenţei produselor dintr-un magazin;
b) Implementaţi un coş de cumpărături pe baza informaţiilor din listă. Pentru implementarea coşului se va folosi tot o (altă) listă simplu înlănţuită;
c) Permiteţi utilizatorului să adauge, să elimine produse în/din coş, dar şi să modifice cantitatea precizată pentru anumite produse deja incluse în acesta;
d) Determinaţi costul total final al coşului de cumpărături.
Atenţie: corelaţi informaţiile din cele două liste! De exemplu, preţul unui produs din coşul de cumpărături va fi obţinut din lista de produse. De asemenea, dacă utilizatorul doreşte să cumpere 5 produse de acelaşi fel însă pe stoc există doar 3, situaţia trebuie tratată corespunzător (se vor achiziţiona doar 3 produse; stocul nu poate avea valoare negativă).*/

#include<stdio.h>
#include<stdlib.h>

typedef struct Produs
{
	int id_produs; 
	int cant_produs;
	float pret_produs; 
	struct Produs* next; 

 }Produs;

//facem 2 lsite 

typedef struct Cos
{
	int id_produs;
	int cantitate;
	struct Cos* next;
}Cos;


Produs* adaugaProdus(Produs* head, int id, int cant, float pret)
{
	Produs* nou = (Produs*)malloc(sizeof(Produs)); 
	nou->id_produs = id;
	nou->cant_produs = cant;
	nou->pret_produs = pret; 
	nou->next = head;
	return nou; 
}

Produs* cautaProdus(Produs* head, int id)
{
	while (head != NULL)
	{
		if (head->id_produs == id)
		{
			return head; 
			head = head->next;
		}


	}
	return NULL;
}

void afisareProduse(Produs* head)
{
	printf("\nLista Produse:\n");
	while (head != NULL)
	{
		printf("ID: %d | Stoc : %d | Pret : %.2f\n", head->id_produs, head->cant_produs, head->pret_produs);
		head = head->next;
	}
}
 
///functii cos 

Cos* adaugaInCos(Cos* cos, Produs* listaProduse, int id, int cantitate)
{
	Produs* p = cautaProdus(listaProduse, id);

	if (p == NULL)
	{
		printf("produs inexistent!!");
		return cos;
	}
	if (p->cant_produs < cantitate)
	{
		printf("stoc insuficient! se adauga doar %d bucati.\n", p->cant_produs);
		cantitate = p->cant_produs;

	}

	if (cantitate <= 0)
	{
		printf("produs indisponibil\n");
		return cos;

	}
}


	Cos* stergeDinCos(Cos * cos, Produs * listaProduse, int id)
	{ 
		 Cos* curent = cos, * anterior = NULL;
		while (curent != NULL && curent->id_produs != id)
		{
			anterior = curent;
			curent = curent->next;
		}
		if (curent == NULL)
		{
			printf("produsl nu a fost gasit!");
			return cos; 

		}

		Produs* p = cautaProdus(listaProduse, id);
		if (p != NULL)
		{
			p->cant_produs += curent->cantitate;
			 
		}

		if (anterior = NULL)
			cos = curent->next;
		else
			anterior->next = curent->next; 


		free(curent);
		return cos;

	  }

 
	float calculeazaTotal(Cos* cos, Produs* listaProduse)
	{
		float total = 0;
		while (cos != NULL)
		{
			Produs* p = cautaProdus(listaProduse, cos->id_produs);
			if (p != NULL)
			{
				total += cos->cantitate * p->pret_produs; 
				cos = cos->next; 

			}
		}
		return total; 

	}

	void afisareCos(Cos* cos)
	{
		printf("\nCosul de cumparaturi :\n");
		while (cos != NULL)
		{
			printf("ID PRODUS : %d | Cantitate :%d\n", cos->id_produs, cos->cantitate);
			cos = cos->next; 

		}
	}

	int main()
	{

		Produs* listaProduse = NULL;
		Cos* cos = NULL;;

		//POPULAREA LISTEI 

		listaProduse = adaugaProdus(listaProduse, 1, 10, 3.5);
		listaProduse = adaugaProdus(listaProduse, 4, 10, 5.3);
		listaProduse = adaugaProdus(listaProduse, 5, 2, 5.9); 
		listaProduse = adaugaProdus(listaProduse, 8, 10, 2.5);
		listaProduse = adaugaProdus(listaProduse, 4, 13, 4.5); 
		listaProduse = adaugaProdus(listaProduse, 3, 3, 9.5); 
		listaProduse = adaugaProdus(listaProduse, 10, 23, 7.5);
		afisareProduse(listaProduse);
		cos = adaugaInCos(cos, listaProduse, 1, 3);
		cos = adaugaInCos(cos, listaProduse, 3, 7); 

		afisareCos(cos); 
		afisareProduse(listaProduse); 

		cos = stergeDinCos( cos, listaProduse, 1);
		afisareCos(cos);
		afisareProduse(listaProduse);

		float total = calculeazaTotal(cos, listaProduse);
		printf("\nTotal de plata : %.2f\n", total);

			return 0; 
	}
