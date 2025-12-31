#define _CRT_SECURE_NO_WARNINGS
#include<curses.h>
#include<time.h>
#include<cstdlib>

#define DP 28 // DlugoscPlanszy
#define SP 40 // SzerokoscPlanszy

struct Samochod {
	int x, y;
	int predkosc;
	int rozmiar;
	int Dobry;
	bool Kierunek;
	int licznik;
	int ZmianaPredkosci;
	int STOP;
	int i;
};

struct linia {
	int Wysokosc =40;
	int alive = 0;
	Samochod* samochod;
};

struct zaba {
	int x;
	int y;
	int timer;
	int Czas;
	int liczbaRuchow;
	double liczbaPunktow;
	int liczbaKamieni;
	int Poziom;
};

struct bocian {
	int x;
	int y;
	int bTimer;
};

int GAMEOVER(WINDOW* Plansza,zaba* Zaba);
int WYGRANA(WINDOW* Plansza,zaba* Zaba);

//cala logika samochodow

void JakosSamochody(WINDOW* Plansza, linia* Linia) {
	Samochod* car = new Samochod;
	car->predkosc = rand() % 5 + 1;
	car->Kierunek = rand() % 2;
	car->Dobry = rand() % 200;
	car->licznik = 0;
	car->rozmiar = rand() % 4 + 1;
	car->STOP = rand() % 200 + 1;
	car->y = DP - Linia->Wysokosc;
	car->i = 0;
	if (car->Kierunek == 0) {
		car->x = SP - 2;
	}
	else if (car->Kierunek == 1) {
		car->x = 1;
	}
	Linia->samochod = car;
}

// Czy samochod niebieski ma cie przerzucic na najblizsze pole zielone czy ma nic nie robic

void SamochodNieibieski(WINDOW* Plansza, zaba* Zaba) {
	erase();
	refresh();
	char c;
	mvprintw(1, 1, "%s", "Czy chcesz,aby Samochod przeniosl cie na najblizsze zielone pole u gory(Kliknij spacje), czy chcesz zeby nic nie zrobil?");
	c = getch();
	if (c == 32) {
		while ((mvwinch(Plansza, Zaba->y, Zaba->x) & A_CHARTEXT) != '#') {
			Zaba->y--;
			if (Zaba->y == 1) {
				break;
			}
		}
	}
	erase();
	refresh();
}

//Od prawej do lewej
void FL(WINDOW* Plansza, linia* Linia, Samochod* car) {
	for (int i = 0; i < car->rozmiar; i++) {
		mvwprintw(Plansza, car->y - 1, car->x + i, " ");
	}
	if (car->x > 2) {
		car->x -= 1;

		if (car->Dobry > 180) {
			wattron(Plansza, COLOR_PAIR(3));
			for (int i = 0; i < car->rozmiar; i++) {
				mvwprintw(Plansza, car->y - 1, car->x + i, "o");
			}
			wattroff(Plansza, COLOR_PAIR(3));
		}
		else {
			wattron(Plansza, COLOR_PAIR(2));
			for (int i = 0; i < car->rozmiar; i++) {
				mvwaddch(Plansza, car->y - 1, car->x + i, 'W');
			}
			wattroff(Plansza, COLOR_PAIR(2));
		}
		car->licznik = 0;
		car->ZmianaPredkosci = rand() % 10000;
		if (car->ZmianaPredkosci > 9000) {
			car->predkosc = rand() % 5 + 1;
		}
	}
	else {
		delete Linia->samochod;
		Linia->alive = 0;
	}
}

void FP(WINDOW* Plansza, linia* Linia, Samochod* car) {
	for (int i = 0; i < car->rozmiar; i++) {
		mvwprintw(Plansza, car->y - 1, car->x - i, " ");
	}
	if (car->x < SP) {
		car->x += 1;

		if (car->Dobry > 180) {
			wattron(Plansza, COLOR_PAIR(3));
			for (int i = 0; i < car->rozmiar; i++) {
				mvwprintw(Plansza, car->y - 1, car->x - i, "o");
			}
			wattroff(Plansza, COLOR_PAIR(3));
		}
		else {
			wattron(Plansza, COLOR_PAIR(2));
			for (int i = 0; i < car->rozmiar; i++) {
				mvwaddch(Plansza, car->y - 1, car->x - i, 'W');
			}
			wattroff(Plansza, COLOR_PAIR(2));
		}
		car->licznik = 0;
		car->ZmianaPredkosci = rand() % 10000;
		if (car->ZmianaPredkosci > 9000) {
			car->predkosc = rand() % 5 + 1;
		}
	}
	else {
		delete Linia->samochod;
		Linia->alive = 0;
	}
}

int RuchSamochodowLewo(WINDOW* Plansza, linia* Linia, int Q, zaba* Zaba) {
	Samochod* car = Linia->samochod;

	car->licznik += 10;

		if (car->licznik >= (car->predkosc * 15)/Zaba->Poziom) { 
			
			int i = 0;
			if (car->STOP > 180 && car->Dobry <= 180 && ((Zaba->x - car->x == 1 || Zaba->x - car->x == -1 || Zaba->x - car->x == 0) && (Zaba->y - car->y == 1 || Zaba->y - car->y == -1 || Zaba->y - car->y == 0))) {
			}
			else {
				if (mvwinch(Plansza, car->y - 1, car->x - 1) == 'X' && car->Dobry <= 180) {
					Q = GAMEOVER(Plansza, Zaba);
				}
				else if (mvwinch(Plansza, car->y - 1, car->x - 1) == 'X' && car->Dobry > 180 && car->i == 0) {
					mvwaddch(Plansza, Zaba->y, Zaba->x, ' ');
						SamochodNieibieski(Plansza, Zaba);
						car->i++;
					if (Zaba->y == 1) {
						Q = WYGRANA(Plansza, Zaba);
						return Q;
					}
					mvwaddch(Plansza, Zaba->y, Zaba->x, 'X');
				}
				else {
					FL(Plansza, Linia, car);
				}
			}
		}
		wrefresh(Plansza);
		return Q;
}

	//od lewej do prawej
int RuchSamochodowPrawo(WINDOW* Plansza, linia* Linia, int Q, zaba* Zaba) {

	Samochod* car = Linia->samochod; 

	car->licznik += 10;

		if (car->licznik >= (car->predkosc * 15)/Zaba->Poziom) {
			if (car->STOP == 1 && car->Dobry <=180 && ((Zaba->x - car->x == 1 || Zaba->x - car->x == -1 || Zaba->x - car->x == 0) && (Zaba->y - car->y == 1 || Zaba->y - car->y == -1 || Zaba->y - car->y == 0))) {
			}
			else {
				if (mvwinch(Plansza, car->y - 1, car->x + 1) == 'X' && car->Dobry <= 180) {
					Q = GAMEOVER(Plansza, Zaba);
				}
				else if (mvwinch(Plansza, car->y - 1, car->x + 1) == 'X' && car->Dobry > 180 && car->i == 0) {
					mvwaddch(Plansza, Zaba->y, Zaba->x, ' ');
						SamochodNieibieski(Plansza, Zaba);
						car->i++;
					if (Zaba->y == 1) {
						Q = WYGRANA(Plansza, Zaba);
						return Q;
					}
					mvwaddch(Plansza, Zaba->y, Zaba->x, 'X');
				}
				else {
					FP(Plansza, Linia, car);
				}
			}
		}
		wrefresh(Plansza);
		return Q;
}


//Rysowanie Menu

void RysowanieMenu(WINDOW* MENU,int opcja,int wybor,const char** OpcjeMenu) {
	int x = 1, y = 1;
	werase(MENU);
	box(MENU, 0, 0);
	for (int i = 0; i < 4; i++) {
		if (i == opcja) {
			wattron(MENU, A_REVERSE);
			mvwprintw(MENU, x+i, y, "%s", OpcjeMenu[i]);
			wattroff(MENU, A_REVERSE);
		}
		else {
			mvwprintw(MENU, y + i, x, "%s", OpcjeMenu[i]);
		}
	}
	wrefresh(MENU);
}

//To zeby menu dzialo

int InterfejsMenu() {
	int opcja=0, wybor=0;
	int g;

	int wysokosc = 7, szerokosc = 15;
	int MenuW1 = LINES / 2-3, MenuS1 = COLS / 2-5;
	WINDOW* MENU = newwin(wysokosc, szerokosc, MenuW1, MenuS1);
	const char* OpcjeMenu[4] = { "Start" , "Wyniki" , "Pomoc" , "Wyjscie" };
	
	while (TRUE) {
		RysowanieMenu(MENU, opcja, wybor, OpcjeMenu);

		g = wgetch(MENU);
		if (wybor != 0) {
			break;
		}

		if (g == 'w') {
			if (opcja == 0) {
				opcja = 3;
			}
			else {
				opcja--;
			}
		}
		else if (g == 's') {
			if (opcja == 3) {
				opcja = 0;
			}
			else {
				opcja++;
			}
		}
		else if (g == 27) {
			return -1;
		}
		else {
			break;
		}
	}
	erase();
	mvprintw(LINES/2,COLS / 3, "Wybrales: %s", OpcjeMenu[opcja]);
	getch();
	delwin(MENU);
	erase();
	refresh();
	return opcja;
}


//Plansza Zabolec


int PlanszaZaba1(WINDOW* Plansza, zaba* Zaba, linia* Linia,int Q) {

	wattron(Plansza, COLOR_PAIR(1));
	for (int j = DP - 2; j > 0; j -= 5) {
		for (int i = 0; i < SP; i++) {
			if ((mvwinch(Plansza, j, i) & A_CHARTEXT) != '@'){
				mvwprintw(Plansza, j, i, "#");
			}
		}
	}
	wattroff(Plansza, COLOR_PAIR(1));

	for (int i = 0; i < DP; i++) {
		if (Linia[i].alive == 0) {
			JakosSamochody(Plansza, &Linia[i]);
			Linia[i].alive = 1;
		}
		else if(Linia[i].alive == 1) {
			if (Linia[i].samochod->Kierunek == FALSE) {
				Q = RuchSamochodowLewo(Plansza, &Linia[i], Q, Zaba);
			}
			else if (Linia[i].samochod->Kierunek == TRUE) {
				Q = RuchSamochodowPrawo(Plansza, &Linia[i], Q, Zaba);
			}
			 if (Q == 2 || Q == 1) {
				 return Q;
			 }
		}
	}

	box(Plansza, 0, 0);
	mvwaddch(Plansza, Zaba->y, Zaba->x, 'X');
	wrefresh(Plansza);
	return Q;
}



// Ruchy Zaby

int RuchZaby(WINDOW* Plansza, int k, zaba* Zaba, linia* Linia) {
	mvwaddch(Plansza, Zaba->y, Zaba->x, ' ');
	if (k == 'w') {
		if (Zaba->y > 1 && (mvwinch(Plansza, Zaba->y-1, Zaba->x) & A_CHARTEXT) != '@') {
			Zaba->y--;
		}

	}
	else if (k == 's') {
		if (Zaba->y < DP - 2 && (mvwinch(Plansza, Zaba->y+1, Zaba->x) & A_CHARTEXT) != '@') {
			Zaba->y++;
			}
	}
	else if (k == 'a') {
		if (Zaba->x > 1 && (mvwinch(Plansza, Zaba->y, Zaba->x-1) & A_CHARTEXT) != '@') {
			Zaba->x--;
		}
	}
	else if (k == 'd') {
		if (Zaba->x < SP - 2 && (mvwinch(Plansza, Zaba->y, Zaba->x+1) & A_CHARTEXT) != '@') {
			Zaba->x++;
		}
	}
	if ((mvwinch(Plansza, Zaba->y, Zaba->x) & A_CHARTEXT) == 'W' || (mvwinch(Plansza, Zaba->y, Zaba->x) & A_CHARTEXT) == 'B') {
		return GAMEOVER(Plansza, Zaba);
	}
	if ((mvwinch(Plansza, Zaba->y, Zaba->x) & A_CHARTEXT) == 'o') {
		SamochodNieibieski(Plansza, Zaba);
	}
	if (Zaba->y == 1) {
		return WYGRANA(Plansza,Zaba);
	}
	mvwaddch(Plansza, Zaba->y, Zaba->x, 'X');
	wrefresh(Plansza);
	return 0;
}

//Bocian

int Bociek(WINDOW* Plansza, zaba* Zaba, linia* Linia,bocian* Bocian) {

	mvwaddch(Plansza, Bocian->y, Bocian->x, ' ');
	if (Zaba->y - Bocian->y > 0) {
		Bocian->y += 1;
	}
	else if (Zaba->y - Bocian->y < 0) {
		Bocian->y -= 1;
	}
	if (Zaba->x - Bocian->x > 0) {
		Bocian->x += 1;
	}
	else if (Zaba->x - Bocian->x < 0) {
		Bocian->x -= 1;
	}
	if (mvwinch(Plansza, Bocian->y, Bocian->x) == 'X') {
		delete(Bocian);
		return GAMEOVER(Plansza, Zaba);
	}
	else {
		wattron(Plansza, COLOR_PAIR(4));
		mvwaddch(Plansza, Bocian->y, Bocian->x, 'B');
		wattroff(Plansza, COLOR_PAIR(4));
		return 0;
	}

}


int ZABOLEC(WINDOW* Plansza,zaba* Zaba, linia* Linia,int Q,bocian* Bocian) {
	int k,sek =0;
	Zaba->timer = 0;
	Zaba->liczbaRuchow = 0;
	Zaba->Czas = 0;
	Bocian->bTimer = 0;

	init_pair(4, COLOR_BLACK, COLOR_WHITE);

	while (Q == 0)
	{
		nodelay(Plansza, TRUE);
		k = wgetch(Plansza);
		napms(100);
		Zaba->timer++;
		Zaba->Czas++;
		Bocian->bTimer++;

		if (k != ERR && Zaba->timer >3) {
			Zaba->liczbaRuchow++;
			Zaba->timer = 0;
			Q = RuchZaby(Plansza, k, Zaba, Linia);
			if (Q != 0) {
				return Q;
			}
		}
		Q = PlanszaZaba1(Plansza, Zaba, Linia, Q);

		if (Q != 0) {
			return Q;
			}
		if (Zaba->Czas / 10 > 5) {
			wattron(Plansza, COLOR_PAIR(4));
			mvwaddch(Plansza, Bocian->y, Bocian->x, 'B');
			wattroff(Plansza, COLOR_PAIR(4));
			if (Bocian->bTimer > 20/Zaba->Poziom) {
				Bocian->bTimer = 0;
				Q = Bociek(Plansza, Zaba, Linia, Bocian);
			}
		}
	}

	return Q;
}

int GAMEOVER(WINDOW* Plansza,zaba* Zaba) {
	erase();
	refresh();
	mvprintw(LINES / 2 - 6, COLS / 3, "GAME OVER");
	mvprintw(LINES / 2 - 3, COLS / 3, "%s %d", "Liczba ruchow: ", Zaba->liczbaRuchow);
	mvprintw(LINES / 2 , COLS / 3, "%s %d %c", "Czas: ", Zaba->Czas / 10, 's');
	mvprintw(LINES / 2 + 3, COLS / 3, "%s %d", "Liczba Punktow: ", 0);
	getch();
	erase();
	refresh();
	return 2;
}

int WYGRANA(WINDOW* Plansza, zaba* Zaba) {
	delwin(Plansza);
	Zaba->Poziom++;
	erase();
	mvprintw(LINES / 2-6, COLS / 3, "Wygrana!!");
	mvprintw(LINES / 2 -3, COLS / 3, "%s %d","Liczba ruchow: ",Zaba->liczbaRuchow);
	mvprintw(LINES / 2, COLS / 3, "%s %d %c", "Czas: ", Zaba->Czas /10 , 's');
	int LiczbaPunktow = 100000 - (Zaba->Czas * 5) - Zaba->liczbaRuchow * 5;
	if (LiczbaPunktow < 0) {
		LiczbaPunktow = 0;
	}
	mvprintw(LINES / 2 + 3, COLS / 3, "%s %d", "Liczba Punktow: ", LiczbaPunktow);
	if (Zaba->Poziom < 4) {
		mvprintw(LINES / 2 + 6, COLS / 3, "%s %d", "Skonczony Poziom: ", Zaba->Poziom-1);
		getch();
		erase();
		refresh();
		return 1;
	}
	else {
		mvprintw(LINES / 2 + 6, COLS / 3, "%s", "Przeszedles Gre!");
		getch();
		erase();
		refresh();
		return -1;
	}
}

void PlanszaZaba(zaba* Zaba, FILE* file) {

	erase();

	linia* Linia = new linia[DP];

	bocian* Bocian = new bocian;

	int a,i=0;
	while (fscanf(file, "%d", &a) == 1) {
		if (i == 0) {
			Bocian->x = a;
			i++;
		}
		else if (i == 1) {
			Bocian->y = a;
			i++;
		}
		else if (i == 2) {
			Zaba->liczbaKamieni = a;
			break;
		}
	}
	int LKamL = Zaba->liczbaKamieni;

	WINDOW* Plansza = newwin(DP, SP, 0, 0);

	for (int j = DP - 2; j > 0; j -= 5) {
		for (int i = 1; i < SP-1; i++) {
			if (LKamL > 0 && rand()%6 > 4 && (j!=DP-2 && i != 1)) {
				wattron(Plansza, COLOR_PAIR(5));
				mvwprintw(Plansza, j, i, "@");
				wattroff(Plansza, COLOR_PAIR(5));
				LKamL--;
			}
			else {
				wattron(Plansza, COLOR_PAIR(1));
				mvwprintw(Plansza, j, i, "#");
				wattroff(Plansza, COLOR_PAIR(1));
			}
		}
		LKamL = Zaba->liczbaKamieni;
		Linia[j].alive = 2;
	}

	for (int i = 0; i < DP; i++) {
		Linia[i].Wysokosc = i;
		JakosSamochody(Plansza, &Linia[i]);
	}

	mvwaddch(Plansza, Zaba->y, Zaba->x, 'X');
	wrefresh(Plansza);

	ZABOLEC(Plansza, Zaba, Linia, 0, Bocian);
	
}

void Pomoc() {
	erase();
	refresh();
	mvprintw(0, COLS / 2 - 6, "%s", "GRA - ZABA");
	mvprintw(1,1,"%s", "W grze wcielamy sie w zabe, ktora musi dojsc do konca trasy.");
	mvprintw(3, 1, "%s", " Poruszamy sie za pomoca WSAD, kiedy wejdziemy w niebieski samochod mozemy klikac spacje, aby samochod przeniosl nas do nastepnej zielonej linii");
	getch();
	erase();
	refresh();
}

int WyborOpcji(int Opcja,zaba* Zaba) {
	if (Opcja == 0) {
		//gra

		start_color();
		init_pair(1, COLOR_GREEN, COLOR_GREEN);
		init_pair(2, COLOR_WHITE, COLOR_RED);
		init_pair(3, COLOR_WHITE, COLOR_BLUE);
		init_pair(4, COLOR_BLACK, COLOR_WHITE);
		init_pair(5, COLOR_CYAN, COLOR_MAGENTA);


		FILE* file = fopen("danebocian.txt", "r");
		if (file == NULL) {
			printw("%s", " Nie wykryto pliku!");
			getch();
			return -1;
		}
		Zaba->x = SP / 2;
		Zaba->y = DP - 2;
		PlanszaZaba(Zaba,file);
	}
	else if (Opcja == 1) {
		//Tabela

	}
	else if (Opcja == 2) {
		Pomoc();

	}
	else if(Opcja == 3 ){
		return -1;
	}
}


int main() {
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	srand(time(NULL));

	zaba* Zaba = new zaba;
	Zaba->Poziom = 1;
	int Opcja = InterfejsMenu();
	while (Opcja != -1) {
		Opcja = WyborOpcji(Opcja,Zaba);
		if (Opcja == -1) {
			break;
		}
		Opcja = InterfejsMenu();
	}

	endwin();
	return 0;
}
