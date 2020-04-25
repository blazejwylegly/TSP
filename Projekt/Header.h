#ifndef HEADER_H
#define HEADER_H

//struktura opisuj�ca wierzcho�ki grafu
struct wierzcholek
{
	char nazwa;						//nazwa wierzcho�ka
	int visited;					//zmienna okre�laj�ca czy wierzcho�ek zosta� ju� odwiedzony
	struct wierzcholek* pNext;		//nastepny wierzcholek grafu
	struct polaczenie* pPolaczenia; //lista celow do ktorych mozna dojechac z danego wierzcholka
};

//struktura opisuj�ca po��czenie mi�dzy dwoma wierzcho�kami grafu
struct polaczenie
{
	double dlugosc;					//waga krawedzi
	struct polaczenie * pNext;		//wskaznik na kolejne polaczenie
	struct wierzcholek* gdzie;		//miejsce docelowe
};

//struktura reprezentuj�ca stos wyznaczaj�cy kolejno�� odwiedzania klient�w
struct stos
{
	char nazwa;						//nazwa klienta
	int iloscElementow;				//kt�ry to element na stosie
	struct stos *pNext;				//nast�pny element stosu
};

//funkcja wyswietla komunikat o b��dzie oraz instrukcj� korzystania z programu
void printHelp();

//funckcja zapisuje do pliku o podanej nazwie wyznaczon� tras� i jej d�ugo��
void zapiszTrase(char* nazwa);

//funkcja odczytuje list� klient�w kt�rych musi odwiedzi� kurier
void czytajListeKlientow(char* nazwa);

//funckja tworzy graf z podanego pliku, zwraca wska�nik na jego pocz�tek
struct wierzcholek* tworzGraf(char* file_name);

//funkcja zwalnia pami�� zaalokowan� na graf
void usunGraf(struct wierzcholek** pGraf);

//funkcja wyszukuje w grafie wierzcho�ek o zadanej nazwie i zwraca jego adres
struct wierzcholek* znajdzWierzcholek(struct wierzcholek* pGraf, char nazwa);

//funckja dodaje do grafu wierzcholek o podanej nazwie
void utworzWierzcholek(struct wierzcholek** pGraf, char nazwa);

//funkcja dodaje do listy polaczen polaczenie o zadanej dlugosci i destynacji
void utworzPolaczenie(struct polaczenie** pPolaczenia, double odleglosc, struct wierzcholek* miejscePrzyjazdu);

//funkcja zwalnia pami�� zaalokowan� na list� po��cze�
void usunPolaczenia(struct polaczenie** pPolaczenie);

//funkcja znajduje w li�cie po��cze� po��czenie do wskazanego wierzcho�ka i zwraca jego adres
struct polaczenie* znajdzPolaczeniePowrotne(struct polaczenie* listaPolaczen, struct wierzcholek* miejsceDocelowe);

//funkcja odpowiedzialna za wyznaczenie �cie�ki i jej d�ugo�ci
void TSP(struct wierzcholek* aktualny, struct wierzcholek* poczatkowy);
												 
//funckja sprawdzaj�ca, czy wszyscy klienci podani w li�cie zostali ju� odwiedzeni
int sprawdzStos();

//funkcja zdejmuje ze wskazanego stosu g�rny element
void zdejmijZeStosu(struct stos** pRoot);

//funkcja dodaje na g�r� stosu element o zadanej nazwie
void dodajNaStos(struct stos** pRoot, char nazwa);

//funkcja przekopiowuje zawwartosc jednego stosu do drugiego
void kopiujStos(struct stos** sciezka, struct stos* sciezkaH);
		
//funkcja zwalnia pami�� zaalokowan� na stos
void usunStos(struct stos** pRoot);

#endif