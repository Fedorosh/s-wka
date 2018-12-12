#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <random>

struct Slowa
{
	char simple[20];
	char past[20];
	char participle[20];
	char tlumaczenie[20];
}typedef Slowa_p;

struct Dane
{
	int ilosc_wierszy;
	int *nr_wiersza;
	int *dlugosc_wiersza;
	char ** juz_sa;
}typedef Dane_p;

void dodaj_slowko(Slowa_p * slowa,int ilosc_slow);
void podaj_slowko(Slowa_p * slowa);
void toString(Slowa_p * slowa,int rozmiar,char ** tab);
char* pobierz_slowko(Dane_p dane, int x);
void policz_plik(Dane_p * dane);
void sprawdz_sie(Slowa_p dane);
void usun_slowka(Dane_p *dane);
void modyfikuj_slowka(Dane_p *dane);
void wyswietl(Dane_p * dane);
void menu(char *c)
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
}

int main()
{
	FILE * f = fopen("file.txt", "w");
	fclose(f);
	Dane_p linie;
	int i;
	policz_plik(&linie);
	Slowa_p * words;
	words = malloc(20 * sizeof(Slowa_p));
	srand(time(NULL));
	char c;
	do
	{
		menu(&c);

		switch (c)
		{
		case '1':
		{
			if (linie.ilosc_wierszy == 0) { printf("brak slow w bazie\n"); system("pause"); system("cls"); break; }
			Slowa_p tezt;
			do
			{

				int x = rand() % linie.ilosc_wierszy;

				char * test = pobierz_slowko(linie, x);


				char * schowek = strtok(test, " ");

				strcpy(tezt.simple, schowek);
				schowek = strtok(NULL, " ");
				strcpy(tezt.past, schowek);
				schowek = strtok(NULL, " ");
				strcpy(tezt.participle, schowek);
				schowek = strtok(NULL, " ");
				strcpy(tezt.tlumaczenie, schowek);

				sprawdz_sie(tezt);

				printf("chcesz dalej cwiczyc?      N/n - nie   dowolny klawisz - tak");
				//c = getch();
				c = getch();
				system("cls");

			} while (c != 'n' && c != 'N');
			break;
		}
		case '2':
		{
			int indeks_slow = 0;
			char e = ' ';
			do
			{
				wyswietl(&linie);
				do {
					e = getch();
				} while (e != 'n' && e != 'N' && e != '1' && e != '2' && e != '3');
				if (e == '1')
				{
					do
					{
						podaj_slowko(&words[indeks_slow]);
						printf("Dodac inne slowka?     n/N - nie    dowolny klawisz - tak\n");
						//c = getch();
						c = getch();
						indeks_slow++;

					} while (c != 'n' && c != 'N');

					dodaj_slowko(words, indeks_slow);
				}
				if (e == '2')
					usun_slowka(&linie);
				if (e == '3')
					modyfikuj_slowka(&linie);
				policz_plik(&linie);
				//c = getch();
				system("cls");
			} while (e != 'n' && e != 'N');


			break;

		}
		case '3':
			exit(0); break;
		}
	} while (1);
	return 0;

	getchar();
	free(words);
	return 0;
}

void dodaj_slowko(Slowa_p * slowa,int ilosc_slow)
{
	FILE * file = fopen("file.txt", "a");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	char ** wynik;
	int i;
		wynik = malloc(ilosc_slow * sizeof(char*));
		for (i = 0; i < ilosc_slow; i++)
		{
			wynik[i] = malloc(40 * sizeof(char));
		}

		toString(slowa, ilosc_slow, wynik);

		for (i = 0; i < ilosc_slow; i++)
			fwrite(wynik[i], sizeof(char), strlen(wynik[i]), file);
		fclose(file);

	for (i = ilosc_slow - 1; i >= 0; i--)
	{
		char* ptr = wynik[i];
		free(ptr);
	}
	char* ptr = wynik;
	free(ptr);
}

void toString(Slowa_p * slowa, int rozmiar, char ** string)
{
	int i;
		for(i = 0; i < rozmiar; i++)
		{
			sprintf(*string, "%s %s %s %s\n\0", slowa->simple, slowa->past, slowa->participle, slowa->tlumaczenie);
			slowa++;
			string++;
		}
}

void podaj_slowko(Slowa_p * slowa)
{
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
	fseek(file, 0, 2);
	if (ftell(file) == 0)
	{
		puts("plik jest pusty"); getchar(); exit(-1);
	}
	fseek(file, 0, 0);
	size_t bufor;
	char * slowo;
	slowo = malloc((dane.dlugosc_wiersza[x] + 1) * sizeof(char));
	char * temp;
	temp = malloc((dane.dlugosc_wiersza[x] + 1) * sizeof(char));
	fseek(file, dane.nr_wiersza[x], 0);
	bufor = fread(temp, sizeof(char), dane.dlugosc_wiersza[x], file);
	sprintf(slowo,"%.*s", bufor,temp);
	fclose(file);
	return slowo;
	//free(temp);
}

void policz_plik(Dane_p *dane)
{
	FILE * file;
	dane->ilosc_wierszy = 0;
	file = fopen("file.txt", "r");
	if (file == NULL) { puts("nieprawidlowy plik"); getchar(); exit(-1); }
	int znaki = 0;
	int numery = 0;
	int i = 0;
	while (!feof(file))
	{
		if (getc(file) == '\n')
			dane->ilosc_wierszy++;
	}
	fseek(file, 0, 0);
	dane->nr_wiersza = malloc(dane->ilosc_wierszy * sizeof(int));
	dane->dlugosc_wiersza = malloc(dane->ilosc_wierszy * sizeof(int));

	while (!feof(file))
	{
			znaki++;
			if (getc(file) == '\n')
			{
				dane->dlugosc_wiersza[i] = znaki - 1;
				dane->nr_wiersza[i] = numery;
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

	if (strcmp(dane.past, &odpowiedzi[0]) == 0) good++;
	else {
		printf("napisales '%s', a powinno byc '%s'\n", odpowiedzi[0], dane.past);
	}
	if (strcmp(dane.participle, &odpowiedzi[1]) == 0) good++;
	else {
		printf("napisales '%s', a powinno byc '%s'\n", odpowiedzi[1], dane.participle);
	}
	if (strcmp(dane.tlumaczenie, &odpowiedzi[2]) == 0) good++;
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
	fseek(file, 0, 0);
	int x;
	int i,j;
	puts("podaj nr wiersza do usuniecia ");
	scanf("%i", &x);
	char ** buffer;
	size_t * buffer_size;
	buffer = malloc(dane->ilosc_wierszy * sizeof(char*));
	buffer_size = malloc(dane->ilosc_wierszy * sizeof(size_t));
	for (i = 0; i < dane->ilosc_wierszy; i++) 
	{
		buffer[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
		buffer_size[i] = fread(buffer[i], sizeof(char), dane->dlugosc_wiersza[i], file);
		fseek(file, 2, 1);
	}

	fclose(file);

	file = fopen("file.txt", "w");

	char ** back;
	back = malloc((dane->ilosc_wierszy - 1) * sizeof(char*));
	for (i = 0,j = 0; i < dane->ilosc_wierszy; i++)
	{
		if (i + 1 != x)
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
	fseek(file, 0, 0);
	int x;
	int i, j;
	puts("podaj nr wiersza do edycji ");
	scanf("%i", &x);
	char ** buffer;
	size_t * buffer_size;
	buffer = malloc(dane->ilosc_wierszy * sizeof(char*));
	buffer_size = malloc(dane->ilosc_wierszy * sizeof(size_t));
	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
		buffer[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
		buffer_size[i] = fread(buffer[i], sizeof(char), dane->dlugosc_wiersza[i], file);
		fseek(file, 2, 1);
	}
	getchar();
	fclose(file);
	char nowe[4][20];
	puts("podaj nowa forme simple ");
	gets(nowe[0]);
	puts("podaj nowa forme past ");
	gets(nowe[1]);
	puts("podaj nowa forme participle ");
	gets(nowe[2]);
	puts("podaj nowe tlumaczenie ");
	gets(nowe[3]);

	char ** back;
	back = malloc((dane->ilosc_wierszy) * sizeof(char*));
	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
		back[i] = malloc((dane->dlugosc_wiersza[i] + 1) * sizeof(char));
		sprintf(back[i], "%.*s\n", buffer_size[i], buffer[i]);
	}

	sprintf(back[x - 1], "%s %s %s %s\n", nowe[0], nowe[1], nowe[2], nowe[3]);

	dane->dlugosc_wiersza[x - 1] = strlen(back[x - 1]);

	file = fopen("file.txt", "w");

	for (i = 0; i < dane->ilosc_wierszy; i++)
	{
			fwrite(back[i], sizeof(char), dane->dlugosc_wiersza[i] + 1, file);
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
	fseek(file, 0, 0);

	char * buffer;
	size_t buffer_size;
	buffer = malloc(dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1] * sizeof(char));
	buffer_size = fread(buffer, sizeof(char), dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1], file);
	char * tak; tak = malloc(dane->nr_wiersza[dane->ilosc_wierszy - 1] + dane->dlugosc_wiersza[dane->ilosc_wierszy - 1] * sizeof(char));
	sprintf(tak, "%.*s", buffer_size, buffer);
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
		if (tak[i] == '\n' && g - 1 <= dane->ilosc_wierszy) { printf("%i. ", g); g++; }
	}
	printf("\n1 - dodaj slowo/slowa     2 - usun slowo    3 - modyfikuj slowo\n");

}