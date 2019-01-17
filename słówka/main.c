#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Slowa //ta struktura jest do przechowywania kazdej formy slowa w poszczegolnym wierszu
{
	char simple[20];
	char past[20];
	char participle[20];
	char tlumaczenie[20];
}typedef Slowa_p;

struct Dane //struktura pomocnicza, w ktorej jest zawarta ilosc wierszy ze slowami w pliku, dlugosci i numery linii
{
	int ilosc_wierszy;
	int *nr_wiersza;
	int *dlugosc_wiersza;
}typedef Dane_p;

void dodaj_slowko(Slowa_p slowa); //dodaje slowo lub kilka slowek do pliku
void podaj_slowko(Slowa_p * slowa); //zbiera podane przez uzytkownika slowa do wyslania do pliku
void toString(Slowa_p slowa,char * tab); //laczy 4 formy slowa w jeden wiersz gotwy do wrzucenia do pliku
char* pobierz_slowko(Dane_p dane, int x); //pobiera losowy wiersz z pliku
void policz_plik(Dane_p * dane); //aktualizuje dane struktury Dane_p
void sprawdz_sie(Slowa_p dane); //odpala uczenie sie slowek
void usun_slowka(Dane_p *dane); //usuwa jeden wybrany wiersz z pliku
void modyfikuj_slowka(Dane_p *dane); //pozwala edytowac wybranu wiersz w pliku
//(edytuje ten wiersz prawidlowo, ale dodaje jakims cudem na poczatku spacje, i pozniej program zle czyta inne wiersze)
void wyswietl(Dane_p * dane); //wyswietla baze slowek z pliku
void menu(char *c) // menu glowne programu
{
	printf("-----Program do uczenia sie slowek w formie simple, past i participle-----\n\n");
	printf("wprowadz numer wyswietlony przy opcji, zeby ja wlaczyc\n\n");
	printf("1. Zacznij sie uczyc\n");
	printf("2. Edytuj baze slow\n");
	printf("3. Wyjdz z programu\n");

	do
	{
		*c = getch();
	} while (*c != '1' && *c != '2' && *c != '3');
	system("cls");
} //menu glowne programu
int suma_char(char * c)
{
	int x = strlen(c);
	int suma = 0;
	int i;
	for (i = 0; i < x; i++)
	{
		suma += c[i];
	}
	return suma;
}
char * obetnij(char * c,char * d)
{
	char wynik[20];
	size_t t = strlen(d);
	strncpy(wynik, c, t);
	char wynik1[20];
	sprintf(wynik1, "%.*s", t,wynik);
	return wynik1;
}
int czy_jedna(char * a, char * b) // sprawdza czy slowa roznia sie tylko jedna litera
{
	int i;
	int jedna = 0;
	if (strlen(a) != strlen(b)) return 0;
	int x = strlen(a);
	for (i = 0; i < x; i++)
	{
		if (a[i] != b[i])jedna++;
	}
	if (jedna == 1) return 1;
	return 0;
}

int main()
{
	FILE * f = fopen("file.txt", "r"); //tworzy plik jesli go nie ma
	if (f == NULL) f = fopen("file.txt", "w");
	fclose(f);
	Dane_p linie;
	int i;
	policz_plik(&linie); 
	Slowa_p words;
	srand(time(NULL));
	char d;
	do //ta petla sie wykonuje dopoki w menu glownym nie wyjdzie sie z programu
	{
		menu(&d);

		switch (d)
		{
			char c;
		case '1': //odpala uczenie sie slowek
		{
			//sprawdza czy sa slowa w pliku, jesli nie ma to ta petla sie konczy i czeka na inny wybor
			if (linie.ilosc_wierszy == 0) { printf("brak slow w bazie\n"); system("pause"); system("cls"); break; }
			Slowa_p tezt;
			do
			{

				int x = rand() % linie.ilosc_wierszy; //losowanie numeru wiersza do uczenia

				char * test = pobierz_slowko(linie, x); // pobranie wiersza do tablicy znakow zaleznie od numeru wiersza


				char * schowek = strtok(test, " "); // ta funkcja dzieli tablice znakow na oddzielne wyrazy

				strcpy(tezt.simple, schowek); //cale
				schowek = strtok(NULL, " "); //jej
				strcpy(tezt.past, schowek); //dzialanie
				schowek = strtok(NULL, " "); //tak 
				strcpy(tezt.participle, schowek); //wyglada
				schowek = strtok(NULL, " "); //troche
				strcpy(tezt.tlumaczenie, schowek); //dziwne

				sprawdz_sie(tezt); //pytanie o wprowadzenie form slowa i sprawdzenie czy sa dobrze

				printf("chcesz dalej cwiczyc?      N/n - nie   dowolny klawisz - tak");
				//c = getch();
				c = getch(); // getch() pobiera z klawiatury jeden klawisz do zmiennej c, jesli c == n/N to funkcja dalej nie pyta o slowka
				system("cls");
				getchar();

			} while (c != 'n' && c != 'N'); //slowka ciagle sa rzucane dopoki nie kliknie sie n/N
			break;
		}
		case '2': //otwiera edycje pliku(bazy slowek)
		{
			//int indeks_slow = 0;
			char e = ' ';
			do
			{
				wyswietl(&linie); //po prostu wyswietla wszystkie slowka i aktualizuje sie od razu po edycji pliku
				do {
					e = getch();
				} while (e != 'n' && e != 'N' && e != '1' && e != '2' && e != '3'); //pyta o podanie klawisza tak dlugo az kliknie sie n/N lub 1 lub 2 lub 3
				if (e == '1')
				{
					do
					{
						podaj_slowko(&words);
						printf("Dodac inne slowka?     n/N - nie    dowolny klawisz - tak\n");
						//c = getch(); <--- te getch() w trybie debug sa potrzebne, a wtrybie release nie
						dodaj_slowko(words/*, indeks_slow*/);
						c = getch();
						//indeks_slow++;

					} while (c != 'n' && c != 'N'); //pyta tak dlugo o podanie nowych slowek, az kliknie sie n/N

					 //dodaje wszystkie podane slowka do pliku (dodaje z powrotem te co zostaly usuniete, ale to sie naprawi)
				}
				if (e == '2')
				{
					usun_slowka(&linie); //usuwa wybrany wiersz, i wtedy petla sie powtarza od nowa
				}
				if (e == '3')
					modyfikuj_slowka(&linie); //modyfikuje wybrany wiersz i wtedy petla sie powtarza od nowa
				policz_plik(&linie); //liczy za kazdym razem ilosc wierszy i dlugosci tych wierszy, zeby byc na biezaco
				//c = getch(); <--- te tez
				system("cls");
			} while (e != 'n' && e != 'N'); //wychodzi z edycji slowek kiedy sie nacisnie n/N


			break;

		}
		case '3':
			exit(0); break;
		}
	} while (1);

	getchar();
	//free(words); // funkcja free zwalnia pamiec w tablicach, wazne dla optymalizacji
	//fclose(f);
	return 0;
}

void dodaj_slowko(Slowa_p slowa)
{
	FILE * file = fopen("file.txt", "a");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); } //sprawdza czy wpisalo sie dobra nazwe pliku, jesli nie, to program sie wylacza
	char * wynik;
	int i;
		wynik = malloc(40 * sizeof(char)); //alokowanie pamieci..

		toString(slowa, wynik); //do tej tablicy wrzucane sa wierze sklejone z nowo podanych form slowek

			fwrite(wynik, sizeof(char), strlen(wynik), file); // w takiej formie sa wrzucane do pliku
		fclose(file); //trzeba zawsze pamietac o zamknieciu pliku

	char* ptr = wynik;
	free(ptr); //az dotad jest zwalniana pamiec
}

void toString(Slowa_p slowa, char * string)
{
	sprintf(string, "%s %s %s %s\n\0", slowa.simple, slowa.past, slowa.participle, slowa.tlumaczenie);
}

void podaj_slowko(Slowa_p * slowa)
{
	//zwykle pobranie z konsoli slowek i wrzucenie ich do struktury slowa
	char slowka[4][20];
	puts("Podaj slowko ");
	gets(&slowka[0]);
	puts("Podaj forme past ");
	gets(&slowka[1]);
	puts("Podaj forme past participle ");
	gets(&slowka[2]);
	puts("Podaj tlumaczenie ");
	gets(&slowka[3]);
	strcpy(slowa->simple, &slowka[0]);
	strcpy(slowa->past, &slowka[1]);
	strcpy(slowa->participle, &slowka[2]);
	strcpy(slowa->tlumaczenie, &slowka[3]);
}

char* pobierz_slowko(Dane_p dane, int x)
{
	FILE * file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	fseek(file, 0, 2);//sprawdzenie czy plik jest pusty..
	if (ftell(file) == 0)
	{
		puts("plik jest pusty"); getchar(); exit(-1); 
	}
	fseek(file, 0, 0); //..dotad
	//fseek ustawia wskaznik na konkretnym miejscu w pliku
	size_t bufor;
	char * slowo;
	slowo = malloc((dane.dlugosc_wiersza[x] + 1) * sizeof(char)); //alokowanie tablicy znakow do pobrania wiersza z pliku
	char * temp;
	temp = malloc((dane.dlugosc_wiersza[x] + 1) * sizeof(char)); //alokowanie kopii, do ktorej trzeba wrzucic oryginal razem
	//z rozmiarem bufora, co zapobiega powstaniu dziwnych znaczkow
	fseek(file, dane.nr_wiersza[x], 0); //ustawianie wskaznika na poczatek losowego wiersza w pliku
	bufor = fread(temp, sizeof(char), dane.dlugosc_wiersza[x], file); //pobranie tego wiersza z pliku
	sprintf(slowo,"%.*s", bufor,temp);// razem z buforem wstawiamy tablice znakow do kopii, teraz jest bez dziwnych znaczkow
	fclose(file);
	return slowo; // zwracanie kopii do tablicy w mainie
	//free(temp);
}

void policz_plik(Dane_p *dane)
{
	FILE * file;
	dane->ilosc_wierszy = 0;
	file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik");  getchar(); exit(-1); }
	int znaki = 0;
	int numery = 0;
	int i = 0;
	while (!feof(file)) //dopoki wskaznik nie bedzie na koncu pliku petle bedzie sie wykonywac
	{
		if (getc(file) == '\n') //liczenie wierszy
			dane->ilosc_wierszy++;
	}
	fseek(file, 0, 0);
	dane->nr_wiersza = malloc(dane->ilosc_wierszy * sizeof(int)); //nadanie tablicy z numerami wierszy rozmiaru ilosci wierszy
	dane->dlugosc_wiersza = malloc(dane->ilosc_wierszy * sizeof(int));//nadanie tablicy z dlugosciami wierszy rozmiaru ilosci wierszy

	while (!feof(file))
	{
			znaki++; //liczy ilosc znakow w jednym wierszu
			if (getc(file) == '\n')
			{
				dane->dlugosc_wiersza[i] = znaki - 1;// przypisuje do miejsca w tablicy dlugosc tego wiersza
				dane->nr_wiersza[i] = numery; //nadaje numer wiersza(ktory char z calego pliku jest poczatkiem tego wiersza)
				numery += znaki + 1;
				znaki = 0;
				i++;
			}
	}
	fclose(file);
}

void sprawdz_sie(Slowa_p dane)
{
	printf("slowo %s\n", dane.simple);
	int good = 0;
	char odpowiedzi[3][20];

	printf("podaj forme past ");
	scanf("%s",&odpowiedzi[0]);
	printf("podaj forme participle ");
	scanf("%s", &odpowiedzi[1]);
	printf("podaj tlumaczenie ");
	scanf("%s", &odpowiedzi[2]);

	if (strcmp(dane.past, &odpowiedzi[0]) == 0 ) good++; //funckja strcmp porownuje dwa stringi i zwraca 0 jesli sa identyczne
	else if (suma_char(dane.past) == suma_char(odpowiedzi[0])) good++;
	else if (czy_jedna(dane.past, odpowiedzi[0])) good++;
	else if (strcmp(dane.past, obetnij(&odpowiedzi[0], dane.past)) == 0) good++;
	else 
	{
		printf("napisales '%s', a powinno byc '%s'\n", odpowiedzi[0], dane.past);
	}
	if (strcmp(dane.participle, &odpowiedzi[1]) == 0) good++;
	else if (suma_char(dane.participle) == suma_char(odpowiedzi[1])) good++;
	else if (czy_jedna(dane.participle, odpowiedzi[1])) good++;
	else if (strcmp(dane.participle, obetnij(&odpowiedzi[1], dane.participle)) == 0) good++;
	else {
		printf("napisales '%s', a powinno byc '%s'\n", odpowiedzi[1], dane.participle);
	}
	if (strcmp(dane.tlumaczenie, &odpowiedzi[2]) == 0) good++;
	else if (suma_char(dane.tlumaczenie) == suma_char(odpowiedzi[2])) good++;
	else if (czy_jedna(dane.tlumaczenie, odpowiedzi[2])) good++;
	else if (strcmp(dane.tlumaczenie, obetnij(&odpowiedzi[2], dane.tlumaczenie)) == 0) good++;
	else {
		printf("napisales '%s', a powinno byc '%s'\n", odpowiedzi[2], dane.tlumaczenie);
	}
	
	printf("\nPodales %i na 3 dobre odpowiedzi\n", good);


}

void usun_slowka(Dane_p *dane)
{
	FILE * file;
	file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	fseek(file, 0, 2);
	if (ftell(file) == 0)
	{
		puts("brak slow do usuniecia\n"); system("pause"); return;
	}
	fseek(file, 0, 0);//jesli plik jest pusty, funckja nic nie zrobi
	int x;
	int i,j;
	puts("podaj nr wiersza do usuniecia ");
	scanf("%i", &x);// wiersz o numerze x bedzie usuniety
	getchar();
	char ** buffer;
	size_t * buffer_size;
	buffer = malloc(dane->ilosc_wierszy * sizeof(char*));
	buffer_size = malloc(dane->ilosc_wierszy * sizeof(size_t));
	for (i = 0; i < dane->ilosc_wierszy; i++) 
	{
		buffer[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
		buffer_size[i] = fread(buffer[i], sizeof(char), dane->dlugosc_wiersza[i], file); //tak jak funkcji pobierz_slowko sa pobierane
		//wiersze z pliku
		fseek(file, 2, 1);
	}

	fclose(file);

	file = fopen("file.txt", "w");

	char ** back;
	back = malloc((dane->ilosc_wierszy - 1) * sizeof(char*));
	for (i = 0,j = 0; i < dane->ilosc_wierszy; i++) //potem do pliku wszystkie slowa oprocz tego do usuniecia sa wrzucane jeszcze raz
	{
		if (i + 1 != x) //sprawdzenie czy dany wiersz jest tym do usuniecia, jesli tak, to ten if sie nie wykona
		{
			back[j] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
			sprintf(back[j], "%.*s\n", buffer_size[i], buffer[i]);
			fwrite(back[j], sizeof(char), dane->dlugosc_wiersza[i]+1, file); 
			j++;
		}
	}
	fclose(file);
}

void modyfikuj_slowka(Dane_p *dane)
{
	FILE * file;
	file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	fseek(file, 0, 2);
	if (ftell(file) == 0)
	{
		puts("brak slow do modyfikacji\n"); system("pause"); return;
	}
	fseek(file, 0, 0); //funkcja nic nie zrobi jak plik jest pusty
	int x;
	int i, j;
	puts("podaj nr wiersza do edycji ");
	scanf("%i", &x); //wiersz nr x bedzie edytowany
	char ** buffer;
	size_t * buffer_size;
	buffer = malloc(dane->ilosc_wierszy * sizeof(char*));
	buffer_size = malloc(dane->ilosc_wierszy * sizeof(size_t));
	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
		buffer[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
		buffer_size[i] = fread(buffer[i], sizeof(char), dane->dlugosc_wiersza[i], file); //pobranie wszystkich wierszy
		fseek(file, 2, 1);
	}
	getchar();
	fclose(file);
	char nowe[4][20];    //pytanie
	puts("podaj nowa forme simple "); //o
	gets(nowe[0]);
	puts("podaj nowa forme past "); // nowe 
	gets(nowe[1]);
	puts("podaj nowa forme participle ");// formy
	gets(nowe[2]);
	puts("podaj nowe tlumaczenie ");// slowka
	gets(nowe[3]);

	char ** back;
	back = malloc((dane->ilosc_wierszy) * sizeof(char*));
	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
		back[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char)); //tworzona jest kopia tak jak przy funkcji pobierz_slowka
		sprintf(back[i], "%.*s\n", buffer_size[i], buffer[i]); // do kopii wrzucamy wiersze bez dziwnych znaczkow
	}
	int length = strlen(nowe[0]) + strlen(nowe[1]) + strlen(nowe[2]) + strlen(nowe[3]) + 1;
	back[x - 1] = malloc((length + 1) * sizeof(char));
	sprintf(back[x - 1], "%s %s %s %s\n", nowe[0], nowe[1], nowe[2], nowe[3]); // teraz zmieniamy wybrany wiersz na nowe slowka
	dane->dlugosc_wiersza[x - 1] = strlen(back[x - 1]) - 1; //trzeba tez zmienic dlugosc tego wiersza
	file = fopen("file.txt", "w");

	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
			fwrite(back[i], sizeof(char), dane->dlugosc_wiersza[i] + 1, file); //wrzucamy gotowe wiersze z powrotem do pliku
	}
	fclose(file);
}
void wyswietl(Dane_p * dane)
{
	FILE * file;
	int i;
	file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	fseek(file, 0, 2);
	if (ftell(file) == 0)
	{
		puts("plik jest pusty"); 
		printf("1 - dodaj slowo/slowa     2 - usun slowo    3 - modyfikuj slowo\n");
		return;
	} 
	fseek(file, 0, 0); //jesli plik jest pusty, funkcja wyswietli tylko dostepne opcje edycji

	char * buffer;
	size_t buffer_size;
	buffer = malloc(dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1] * sizeof(char)); //alokowanie tablicy o rozmiarze odpowiadajecmu ilosci znakow w pliku
	//numer ostatniego wiersza plus jego dlugosc to ilosc wszystkich znakow
	buffer_size = fread(buffer, sizeof(char), dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1], file); // pobranie wszystkich znakow do tej tablicy
	char * tak; tak = malloc(dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1] * sizeof(char));
	sprintf(tak, "%.*s", buffer_size, buffer); //wrzucenie originalu tablicy wraz z wielkoscia buforu ,zeby zapobiec brzdkim znakom
	fclose(file);
	printf("Simple       Past       Participle      Tlumaczenie\n\n");
	//int najdluzszy wyraz = dane->dlugosc_wiersza[0];
	/*for (i = 1; i < dane->ilosc_wierszy; i++)
		if (c < dane->dlugosc_wiersza[i]) c = dane->dlugosc_wiersza[i];*/
	printf("1. ");
	int g = 2;
	for (i = 0; i < strlen(tak); i++)
	{

		printf("%c", tak[i]);
		if (tak[i] == ' ') printf("%6c",tak[i]);
		if (tak[i] == '\n' && g - 1 <= dane->ilosc_wierszy) { printf("%i. ", g); g++; } //wydrukowanie slowek na ekranie
	}
	printf("\n1 - dodaj slowo/slowa     2 - usun slowo    3 - modyfikuj slowo\n");

}