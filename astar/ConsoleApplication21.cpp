#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include <math.h>
#include <ostream>
#include <time.h>


using namespace std;

const int START_X = 1;
const int START_Y = 1;
const int DOCELOWY_X = 20;
const int DOCELOWY_Y = 20;
const int WIELKOSC_ARRAY_X = 20;
const int WIELKOSC_ARRAY_Y = 20;
const int WARTOSC_SCIANY = 5;
const int WARTOSC_DLA_POPRAWNEJ_DROGI = 3;

struct Node {
	int x;
	int y;
	double f;

	//inicjalizacja struktury
	Node(int x, int y, double f)
		: x(x), y(y), f(f)
	{
	}
};

struct CompareF {
	bool operator()(Node const& p1, Node const& p2)
	{
		//komparator parametru f dla priority_queue
		return p1.f > p2.f;
	}
};

double count_h(int x, int y) {
	double h = sqrt(pow(x - DOCELOWY_X, 2) + pow(y - DOCELOWY_Y, 2));
	return h;
}

//N0E1S2W3 rodzicx rodzicy dzieckox dzieckoy
int skad_rodzic(int x, int y, int _x, int _y) {
	if (x < _x && y == _y) { return 0; } //n
	if (x > _x && y == _y) { return 2; } //s
	if (x == _x && y < _y) { return 1; } //e
	return 3;                            //w
}

vector<int> dokad_rodzic(int x, int y, int k) {
	vector<int> ret;
	if (k == 0) { ret.push_back(x - 1); ret.push_back(y); return ret; }
	if (k == 2) { ret.push_back(x + 1); ret.push_back(y); return ret; }
	if (k == 1) { ret.push_back(x); ret.push_back(y - 1); return ret; }
	ret.push_back(x); ret.push_back(y + 1); return ret;
}


int main(void) {
	string nazwap = "grid1.txt";
	vector<vector<int>> G(WIELKOSC_ARRAY_X + 1, vector<int>(WIELKOSC_ARRAY_Y + 1, 0));

	ifstream plik(nazwap.c_str());

	for (unsigned int i = 1; i < WIELKOSC_ARRAY_X + 1; i++)
	{
		for (unsigned int j = 1; j < WIELKOSC_ARRAY_Y + 1; j++)
		{
			plik >> G[i][j];
		}
	}
	plik.close();

	for (int i = 1; i < WIELKOSC_ARRAY_X + 1; i++)
	{
		for (int j = 1; j < WIELKOSC_ARRAY_Y + 1; j++)
		{
			std::cout << " " << G[i][j];
		}
		std::cout << endl;
	}
	std::cout << endl;

	//dzieki arr pozycje mamy latwy przeglad pol
	vector<pair<int, int>> pozycje;
	pozycje.push_back(make_pair(1, 0));
	pozycje.push_back(make_pair(-1, 0));
	pozycje.push_back(make_pair(0, -1));
	pozycje.push_back(make_pair(0, 1));

	//rodzice arr, dl kazdego przypisanego mamy 4 kierunki w ktorym znajduje sie ordzic
	//N0E1S2W3
	//vector<vector<int>> rodzice;
	vector<vector<int>> rodzice(WIELKOSC_ARRAY_X + 1, vector<int>(WIELKOSC_ARRAY_Y + 1, -1));
	//distance jak sama nazwa wskazuje ukazuje nam ilosc krokow od bierz. do startu
	//vector<vector<int>> distance;
	vector<vector<int>> distance(WIELKOSC_ARRAY_X + 1, vector<int>(WIELKOSC_ARRAY_Y + 1, 0));
	//lista otwarta do ktorej trafiaja wszystkie oczekujace kordy
	vector<vector<float>> open_map_list(WIELKOSC_ARRAY_X + 1, vector<float>(WIELKOSC_ARRAY_Y + 1, -1));
	//lista zamknieta do ktorej trafiaja wszystkie odiwedzone kordy
	//vector<vector<int>> close_map_list;
	vector<vector<float>> close_map_list(WIELKOSC_ARRAY_X + 1, vector<float>(WIELKOSC_ARRAY_Y + 1, -1));

	//lista odwarta zawierajaca nody do ktorych trafiaja dopiero co odiwedzane
	priority_queue<Node, vector<Node>, CompareF> open_list;
	int x;
	int y;
	//uniemozliwenie startu ze sciany
	if (G[START_X][START_Y] == WARTOSC_SCIANY)
	{
		std::cout << "Stanales na scianie" << endl;
		exit(2);
	}
	//inicjacja pierwszego noda
	open_list.push(Node(START_X, START_Y, distance[START_X][START_Y]));
	//dopoki jakis node znajduje sie na liscie
	while (!open_list.empty()) 
	{
		Node p = open_list.top();
		open_list.pop();
		//sprawdz czy znalazles docelowy -> jesli tak to daj liste DONE
		//wyszukaj po rodzicach najlepszej drogi na chate ustwiajac kordy poweotne dla WARTOSCI_DLA_LINI
		//wyswietl droge na chate uzywajac WARTOSCI_DLA_LINI = 3
		if (p.x == DOCELOWY_X && p.y == DOCELOWY_Y)//////////////////////////////////////////////////// +DONE
		{

			x = p.x;
			y = p.y;
			G[x][y] = WARTOSC_DLA_POPRAWNEJ_DROGI; //DONE
			while (START_X != x || START_Y != y)
			{
				int temp = rodzice[x][y];
				vector<int> kord = dokad_rodzic(x, y, temp);
				x = kord[0]; y = kord[1];//wyciagniecie z funkcji dokad rodzic vectora wartosci
				G[x][y] = WARTOSC_DLA_POPRAWNEJ_DROGI; //DONE
				//std::cout << " x: " << x << " - y: " << y << " - rodzic_wsk: " << temp << endl;
			}
			for (int i = 1; i < WIELKOSC_ARRAY_X + 1; i++)
			{
				for (int j = 1; j < WIELKOSC_ARRAY_Y + 1; j++)
				{
					std::cout << " " << G[i][j];
				}
				std::cout << endl;
			}
			//ustawienie dla exit()
			x = DOCELOWY_X;
			y = DOCELOWY_Y;
			break;
		}
		// dla kazdego rodzia, ustal kordy dzieci
		for (int i = 0; i < pozycje.size(); i++)
		{
			int x_rodzic = p.x;
			int y_rodzic = p.y;
			if (x_rodzic + pozycje[i].first >= WIELKOSC_ARRAY_X + 1 ||
				y_rodzic + pozycje[i].second >= WIELKOSC_ARRAY_Y + 1)
			{
				continue;
			}
			x = x_rodzic + pozycje[i].first;
			y = y_rodzic + pozycje[i].second;
			// nie wczytuj z zamknietej
			if (close_map_list[x][y] > 0)
			{
				continue;
			}
			//warunki ktore musi spelic kord
			if (G[x][y] != WARTOSC_SCIANY && x > 0 && y > 0 && x < WIELKOSC_ARRAY_X + 1 && y < WIELKOSC_ARRAY_Y + 1)
			{
				//wylicza dystans od punktu do startu
				distance[x][y] = distance[x_rodzic][y_rodzic] + 1;
				//jesli jest juz na otwartej liscie
				if (open_map_list[x][y] > 0)
				{
					//wartosc f jest wieksza, od nowej
					if ((count_h(x, y) + distance[x][y]) >= open_map_list[x][y])
					{
						continue;
					}
					else
					{
						//EP1 dla zajetego pola -> dodaje do otwartej mapy f na kordy
						//zmienia rodzica na kordy
						//dodaje do openlisty(priorityqueue) z sortowaniem na wartosc f
						open_map_list[x][y] = count_h(x, y) + distance[x][y];
						rodzice[x][y] = skad_rodzic(x_rodzic, y_rodzic, x, y);
						open_list.push(Node(x, y, count_h(x, y) + distance[x][y]));
					}
				}
				else
				{
					//EP1 dla pustego pola
					open_map_list[x][y] = count_h(x, y) + distance[x][y];
					rodzice[x][y] = skad_rodzic(x_rodzic, y_rodzic, x, y);
					open_list.push(Node(x, y, count_h(x, y) + distance[x][y]));
				}
			}
		}
		close_map_list[p.x][p.y] = 100; //zaznacza na zamknietej, ze sprawdzony
	}
	//jesli nie znalazl, zakoncz z bledem 3 ////////////todo
	if (!(x == DOCELOWY_X && y == DOCELOWY_Y)) {
		std::cout << "Przykro mi no ale nie dojdzie do niego" << endl;
		exit(3);
	}

	return 0;
}


