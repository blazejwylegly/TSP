#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "header.h"

extern struct stos* sciezka, *sciezkaH;
extern struct wierzcholek* pGraf;

int main(int argc, char* argv[])
{
	char graf[256], klienci[256], wyjscie[256];
	int g = 0, k = 0, w = 0;
	if (argc == 7)
	{
		for (int i = 1; i < 6; i+=2)
		{
			if (strcmp(argv[i], "-i") == 0)
			{
				strcpy(graf, argv[i + 1]);
				g = 1;
			}
			else if (strcmp(argv[i], "-l") == 0)
			{
				strcpy(klienci, argv[i + 1]);
				k = 1;
			}
			else if (strcmp(argv[i], "-d") == 0)
			{
				strcpy(wyjscie, argv[i + 1]);
				w = 1;
			}
		}
		if ((g == 1) && (k == 1) && (w == 1))
		{
			pGraf = tworzGraf(graf);

			czytajListeKlientow(klienci);
			struct wierzcholek* temp = pGraf;
			while (temp != NULL)
			{
				TSP(temp, temp);
				temp = temp->pNext;
			}
			if (sciezka == NULL)
			{
				printf("W zadanym grafie nie udalo sie znalezc odpowiedniej sciezki!\n");
			}
			else
			{
				printf("Sciezka zostala wyznaczona i zapisana do pliku.\nDzialanie programu zakonczylo sie powodzeniem.\n");
				zapiszTrase(wyjscie);
			} 
			usunGraf(&pGraf);
			usunStos(&sciezka);
			usunStos(&sciezkaH);
		}
		else
		{
			printHelp();
		}
	}
	else
	{
		printHelp();
	}

	getchar();
	return 0;
}	   
