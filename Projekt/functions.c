#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "Header.h"


int ilosc_klientow = 0;//zmiena przechowuj¹ca iloœæ klientów do odwiedzenia
double dlugosc = 999999999.0;//zmienna na wyjsciowa dlugosc znalezionej sciezki
double tempDlugosc = 0.0;//zmienna na tymczasowa dlugosc badanej sciezki
extern struct stos* sciezka = NULL, *sciezkaH = NULL;//zmienne na sciezke koncowa i pomocnicza
extern struct wierzcholek* pGraf = NULL;//zmienna na graf z wierzcho³kami trasy
char wierzcholki[256];//tablia zawierajaca klientów którzy musz¹ zostaæ odwiedzeni

void printHelp()
{
	printf("Blad wykorzystania przelacznikow!\n");
	printf("Instrukcja poprawnego wykorzystania przelacznikow:\n");
	printf("-i <nazwa_pliku> - plik zawierajacy informacje o polaczeniach\n");
	printf("-l <nazwa_pliku> - plik zaiwerajacy liste klientow\n");
	printf("-d <nazwa_pliku> - sciezka do pliku wyjsciowego\n");
	printf("Wprowadzenie niepoprawnej ilosci argumentow wywolania (6) spowoduje wyswietlenie bledu!\n");
}

void zapiszTrase(char* nazwa)
{
	FILE* plik = fopen(nazwa, "w");
	if (plik == NULL)
	{
		printf("Otwieranie pliku do zapisu nie powiod³o siê!\n");
		return;
	}
	struct stos* temp = sciezka;
	fprintf(plik, "Kolejnosc odwiedzania klientow:\n");
	while (temp != NULL)
	{
		fprintf(plik, "%c\n", temp->nazwa);
		temp = temp->pNext;
	}
	fprintf(plik, "Dlugosc wyznaczonej sciezki jest rowna %.2lf", dlugosc);
	fclose(plik);
}

void czytajListeKlientow(char* nazwa)
{
	FILE* plik = fopen(nazwa, "r");
	if (plik == NULL)
	{
		printf("Podano nieprawidlowa sciezke do listy klienotw!\nDzialanie zakonczy sie, plik wyjsciowy nie zostanie wygenerowany\n");
		return;
	}
	while (!feof(plik))
	{
		fscanf(plik, " %c ", &(wierzcholki[ilosc_klientow++]));
		fgetc(plik);
	}
	fclose(plik);
}

struct wierzcholek* tworzGraf(char* file_name)
{
	FILE* file = fopen(file_name, "r");
	if (file == NULL)
		return NULL;

	struct wierzcholek* pGraf = NULL;
	char A, B, h;
	char temp[3];
	double distance;
	struct wierzcholek* przyjazd = NULL, *wyjazd = NULL;
	while (fscanf(file, " ( %c %2s %c : %lf ) ", &A, temp, &B, &distance) == 4)
	{																		
		//if ()
		{
			//printf(" ODCZYT PLIKU A: %c, temp: %s, B: %c, distance: %f \n", A, temp, B, distance);
			wyjazd = znajdzWierzcholek(pGraf, A);
			if (wyjazd == NULL)
			{
				utworzWierzcholek(&pGraf, A);
				wyjazd = pGraf;
			}

			przyjazd = znajdzWierzcholek(pGraf, B);
			if (przyjazd == NULL)
			{
				utworzWierzcholek(&pGraf, B);
				przyjazd = pGraf;
			}
			if (strcmp(temp, "->") == 0)
				utworzPolaczenie(&(wyjazd->pPolaczenia), distance, przyjazd);
			else
			{
				utworzPolaczenie(&(wyjazd->pPolaczenia), distance, przyjazd);
				utworzPolaczenie(&(przyjazd->pPolaczenia), distance, wyjazd);
			}
			h = fgetc(file);
			
		}
	}
	fclose(file);
	return pGraf;
	
}

void usunGraf(struct wierzcholek** pGraf)
{
	if (*pGraf != NULL)
	{
		usunGraf(&((*pGraf)->pNext));
		usunPolaczenia(&((*pGraf)->pPolaczenia));
		free(*pGraf);
		*pGraf = NULL;
	}
}
													   

struct wierzcholek* znajdzWierzcholek(struct wierzcholek* pGraf, char nazwa)
{
	while (pGraf != NULL)
	{
		if (pGraf->nazwa == nazwa)
			return pGraf;
		pGraf = pGraf->pNext;
	}
	return NULL;
}

void utworzWierzcholek(struct wierzcholek** pGraf, char nazwa)
{
	struct wierzcholek* pNew = malloc(sizeof(struct wierzcholek));
	pNew->nazwa = nazwa;
	pNew->visited = 0;
	pNew->pNext = (*pGraf);
	pNew->pPolaczenia = NULL;
	(*pGraf) = pNew;
}


void utworzPolaczenie(struct polaczenie** pPolaczenia, double odleglosc, struct wierzcholek* miejscePrzyjazdu)
{
	struct polaczenie* pNew = malloc(sizeof(struct polaczenie));
	pNew->dlugosc = odleglosc;
	pNew->gdzie = miejscePrzyjazdu;
	pNew->pNext = (*pPolaczenia);
	(*pPolaczenia) = pNew;
	pNew = NULL;
}

void usunPolaczenia(struct polaczenie** pPolaczenie)
{
	if (*pPolaczenie != NULL)
	{
		usunPolaczenia(&((*pPolaczenie)->pNext));
		free(*pPolaczenie);
		*pPolaczenie = NULL;
	}
}


struct polaczenie* znajdzPolaczeniePowrotne(struct polaczenie* pPolaczenie, struct wierzcholek* wierzcholekPoczatkowy)
{
	while (pPolaczenie != NULL)
	{
		if (pPolaczenie->gdzie == wierzcholekPoczatkowy)
			return pPolaczenie;
		pPolaczenie = pPolaczenie->pNext;
	}
	return NULL;
}


void TSP(struct wierzcholek* aktualny, struct wierzcholek* poczatkowy)
{
	if (aktualny == NULL || poczatkowy == NULL || pGraf == NULL)//je¿eli jest w czym szukaæ 
		return;

	dodajNaStos(&sciezkaH, aktualny->nazwa);				//dodajemy wierzcholek na stos pomocniczy

	if ((sciezkaH->iloscElementow >= ilosc_klientow) && (sprawdzStos() == 1))				//gdy na stosie jest ju¿ tyle wierzcho³ków ile ma graf
	{
		struct polaczenie* polaczeniePowrotne = znajdzPolaczeniePowrotne(aktualny->pPolaczenia, poczatkowy);
		if (polaczeniePowrotne != NULL)								//je¿eli istienje po³¹czenie powrotne z wierzcho³ka aktualnego do startowego
		{
			tempDlugosc += polaczeniePowrotne->dlugosc;						//przeje¿dzamy do wierzcho³ka poczatkowego
			if (tempDlugosc < dlugosc)												//czy nowa œcie¿ka jest krótsza
			{
				dlugosc = tempDlugosc;												//kopiuj wage
				kopiujStos(&sciezka, sciezkaH);
				dodajNaStos(&sciezka, aktualny->nazwa);
			}
			tempDlugosc -= polaczeniePowrotne->dlugosc;						//wroc na wierzcholek aktualny
		}
	}
	else//jezeli nie odwiedzilismy jeszcze wszystkich wierzchlokow
	{
		aktualny->visited = 1; //ustawiamy aktualny wierzcholek na odwiedzony
		struct polaczenie* temp = aktualny->pPolaczenia;
		while (temp != NULL)//dopoki istnieje jakiœ s¹siad wierzcho³ka aktualnego
		{
			
			if (temp->gdzie->visited == 0)//jezeli ten sasiad nie byl jeszcze odwiedzony
			{
				tempDlugosc += temp->dlugosc;//jedziemy do sasiada
				TSP(temp->gdzie, poczatkowy);//wykonaj t¹ sam¹ procedure dla s¹siada
				tempDlugosc -= temp->dlugosc;//wróc od s¹siada
			}
			temp = temp->pNext;		//przejscie na kolejnego s¹siada
		}
		aktualny->visited = 0;//ustawiamy aktualny na nieodwiedzony, zeby inne trasy mogly zostac sprawdzone
	}
	zdejmijZeStosu(&sciezkaH);//sciezka z aktualnie badanym wierzch³okiem zosta³a przebyta, zdejmujemy wierzcho³ek
}


int sprawdzStos()
{
	int i = 0;

	while (i < ilosc_klientow)//dla kazdej nazwy kleinta
	{
		int znaleziona = 0;//zmienna okreslajaca czy na stosie jest dany klient
		struct stos* temp = sciezkaH;
		while (temp != NULL)//dla kazdego elementu stosu
		{
			if (temp->nazwa == wierzcholki[i])//jezeli znaleziono wierzcholek na stosie
			{
				znaleziona = 1;
				break;
			}
			temp = temp->pNext;
		}
		if (znaleziona == 0)
			return 0;
		i++;
	}
	return 1;
}

void zdejmijZeStosu(struct stos** pRoot)
{
	if (*pRoot != NULL)
	{
		if ((*pRoot)->pNext == NULL)
		{
			free(*pRoot);
			*pRoot = NULL;
			return;
		}
		struct stos* pTemp = (*pRoot)->pNext;
		free(*pRoot);
		(*pRoot) = pTemp;
		pTemp = NULL;
	}
}

void dodajNaStos(struct stos** pRoot, char nazwa)
{
	if (*pRoot == NULL)
	{
		*pRoot = malloc(sizeof(struct stos));
		(*pRoot)->pNext = NULL;
		(*pRoot)->nazwa = nazwa;
		(*pRoot)->iloscElementow = 1;
		return;
	}
	struct stos* pNew = malloc(sizeof(struct stos));
	pNew->pNext = *pRoot;
	pNew->nazwa = nazwa;
	pNew->iloscElementow = (*pRoot)->iloscElementow + 1;
	*pRoot = pNew;
	pNew = NULL;
}

void kopiujStos(struct stos** sciezka, struct stos* sciezkaH)
{
	usunStos(sciezka);
	while (sciezkaH != NULL)
	{
		dodajNaStos(sciezka, sciezkaH->nazwa);
		sciezkaH = sciezkaH->pNext;
	}
}

void usunStos(struct stos** pRoot)
{
	while (*pRoot != NULL)
	{
		zdejmijZeStosu(pRoot);
	}
}