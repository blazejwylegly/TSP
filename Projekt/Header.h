#ifndef HEADER_H
#define HEADER_H

//struktura opisuj¹ca wierzcho³ki grafu
struct wierzcholek
{
	char nazwa;						//nazwa wierzcho³ka
	int visited;					//zmienna okreœlaj¹ca czy wierzcho³ek zosta³ ju¿ odwiedzony
	struct wierzcholek* pNext;		//nastepny wierzcholek grafu
	struct polaczenie* pPolaczenia; //lista celow do ktorych mozna dojechac z danego wierzcholka
};

//struktura opisuj¹ca po³¹czenie miêdzy dwoma wierzcho³kami grafu
struct polaczenie
{
	double dlugosc;					//waga krawedzi
	struct polaczenie * pNext;		//wskaznik na kolejne polaczenie
	struct wierzcholek* gdzie;		//miejsce docelowe
};

//struktura reprezentuj¹ca stos wyznaczaj¹cy kolejnoœæ odwiedzania klientów
struct stos
{
	char nazwa;						//nazwa klienta
	int iloscElementow;				//który to element na stosie
	struct stos *pNext;				//nastêpny element stosu
};

//funkcja wyswietla komunikat o b³êdzie oraz instrukcjê korzystania z programu
void printHelp();

//funckcja zapisuje do pliku o podanej nazwie wyznaczon¹ trasê i jej d³ugoœæ
void zapiszTrase(char* nazwa);

//funkcja odczytuje listê klientów których musi odwiedziæ kurier
void czytajListeKlientow(char* nazwa);

//funckja tworzy graf z podanego pliku, zwraca wskaŸnik na jego pocz¹tek
struct wierzcholek* tworzGraf(char* file_name);

//funkcja zwalnia pamiêæ zaalokowan¹ na graf
void usunGraf(struct wierzcholek** pGraf);

//funkcja wyszukuje w grafie wierzcho³ek o zadanej nazwie i zwraca jego adres
struct wierzcholek* znajdzWierzcholek(struct wierzcholek* pGraf, char nazwa);

//funckja dodaje do grafu wierzcholek o podanej nazwie
void utworzWierzcholek(struct wierzcholek** pGraf, char nazwa);

//funkcja dodaje do listy polaczen polaczenie o zadanej dlugosci i destynacji
void utworzPolaczenie(struct polaczenie** pPolaczenia, double odleglosc, struct wierzcholek* miejscePrzyjazdu);

//funkcja zwalnia pamiêæ zaalokowan¹ na listê po³¹czeñ
void usunPolaczenia(struct polaczenie** pPolaczenie);

//funkcja znajduje w liœcie po³¹czeñ po³¹czenie do wskazanego wierzcho³ka i zwraca jego adres
struct polaczenie* znajdzPolaczeniePowrotne(struct polaczenie* listaPolaczen, struct wierzcholek* miejsceDocelowe);

//funkcja odpowiedzialna za wyznaczenie œcie¿ki i jej d³ugoœci
void TSP(struct wierzcholek* aktualny, struct wierzcholek* poczatkowy);
												 
//funckja sprawdzaj¹ca, czy wszyscy klienci podani w liœcie zostali ju¿ odwiedzeni
int sprawdzStos();

//funkcja zdejmuje ze wskazanego stosu górny element
void zdejmijZeStosu(struct stos** pRoot);

//funkcja dodaje na górê stosu element o zadanej nazwie
void dodajNaStos(struct stos** pRoot, char nazwa);

//funkcja przekopiowuje zawwartosc jednego stosu do drugiego
void kopiujStos(struct stos** sciezka, struct stos* sciezkaH);
		
//funkcja zwalnia pamiêæ zaalokowan¹ na stos
void usunStos(struct stos** pRoot);

#endif