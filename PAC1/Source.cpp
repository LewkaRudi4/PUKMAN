#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
char tmp_map[10][10];
short trgx_counter = 0;
char map[10][10] = {
	"######",
	"#   O#",
	"#    #",
	"#    #",
	"#    #",
	"######"
};

void ShowMap();

short yy();

short xx();

void gotoxy(short x, short y);

class entity {
public:
	entity(short x, short y) {
		this->x = x;
		this->y = y;
	}

	void move_x(short p) {
		if (map[y][x + p] == ' ') x += p;
	}

	void move_y(short p) {
		if (map[y + p][x] == ' ') y += p;
	}

	void move(short p, short q) {
		x += p;
		y += q;
	}

	short get_x() { return x; }
	short get_y() { return y; }

	void draw(char p) {
		map[x][y] = p;
		gotoxy(x, y); printf("%c", p);
	}

private:
	short x;
	short y;
};

struct walk {
	short walk_count;
	short x;
	short y;
	short back;
};

struct target {
	short x;
	short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray(short x, short y, short wc, short back);

void FindPath(short sx, short sy, short x, short y);


int main()
{
	system("cls");
	ShowMap();
	short x = 0;
	short y = 0;
	bool running = true;
	short a = 0, b = 0;

	short k = 0;
	while (k < 1)
	{
		srand((unsigned)time(NULL));
		a = rand() % 4 + 1;
		b = rand() % 4 + 1;
		if (map[a][b] != '#' && map[a][b] != 'O' && map[a][b] != '*')
		{
			k++;
		}
	}
	short ex = b;
	short ey = a;
	short frame = 0;
	gotoxy(ex, ey); 
	cout<< 'A';
	Sleep(1000);
	while (running)
	{
		while (running || trgx_counter == 3) {
			x = xx();
			y = yy();
			FindPath(ex, ey, x, y);
			while (running) {
				x = xx();
				y = yy();
				gotoxy(ex, ey);
				cout << ' ';
				map[ey][ex] = ' ';
				if (walk_queue.size() != 0) {
					ex = walk_queue.back().x;
					ey = walk_queue.back().y;
					walk_queue.pop_back();
				}
				FindPath(ex, ey, x, y);
				gotoxy(ex, ey);
				if (map[ey][ex] == '*') {
					trgx_counter++;
				}
				cout << 'A';
				if (map[4][1] != 'O')
				{
					map[4][1] = 'O';
				}
				if (trgx_counter == 3 || xx() == 0)
				{
					//cout << 'A';
					break;
				}
				frame++;
				Sleep(500);
				if (frame > 20)
					break;
			}
			Sleep(500);
			if (trgx_counter == 3 /*||xx()==0*/)
				break;
			if (frame > 20)
				break;
		}
		if (frame > 20)
			break;
		gotoxy(ex, ey);
		cout << ' ';
		FindPath(ex, ey, 4, 1);
		while (running)
		{
			if (frame > 20)
				break;
			gotoxy(ex, ey); 
			map[ey][ex] = ' ';
			cout << ' ';

			if (walk_queue.size() != 0) {
				ex = walk_queue.back().x;
				ey = walk_queue.back().y;
				walk_queue.pop_back();
			}

			gotoxy(ex, ey); 
			cout << 'A';
			Sleep(500);
			if (ex == 4 && ey == 1) {
				running = false;
			}
			frame++;/*
			if ( walk_queue.size() == 0)
				break;*/
		}
		if (frame > 20)
			break;
		/*if ( walk_queue.size() == 0)
			break;*/
	}

	system("cls");
	if (trgx_counter==3)
		printf("All goals are colected");
	else
		printf("It's impossible to achieve all the goals");
	Sleep(10000);
	return 0;
}

void ShowMap()
{
	short k = 0;
	while (k < 4)
	{
		srand((unsigned)time(NULL));
		short a = rand() % 4 + 1;
		short b = rand() % 4 + 1;
		if (map[a][b] != 'O' && map[a][b] != '#')
		{
			map[a][b] = '#';
			k++;
		}
	}
	k = 0;
	while (k < 3)
	{
		srand((unsigned)time(NULL));
		short a = rand() % 4 + 1;
		short b = rand() % 4 + 1;
		if (map[a][b] != '#' && map[a][b] != 'O' && map[a][b] != '*')
		{
			map[a][b] = '*';
			k++;
		}
	}
	for (short i = 0; i < 6; i++) {
		printf("%s\n", map[i]);
	}
}

short yy()
{
	short k = 0;
	for (short i = 0; i < 5; i++)
	{
		for (short j = 0; j < 5; j++)
		{
			if (map[i][j] == '*')
			{
				k = i;
				break;
			}
			else
				k = 0;
		}
		if (k > 0)
		{
			break;
		}
	}
	return k;
}

short xx()
{
	short k = 0;
	for (short i = 0; i < 5; i++)
	{
		for (short j = 0; j < 5; j++)
		{
			if (map[i][j] == '*')
			{
				k = j;
				break;
			}
			else
				k = 0;
		}
		if (k > 0)
		{
			break;
		}
	}
	return k;
}

void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}

void AddArray(short x, short y, short wc, short back) {
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '*' || tmp_map[y][x] == 'O')
	{
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(short sx, short sy, short x, short y) {
	memcpy(tmp_map, map, sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);

	short i = 0;
	while (i < BFSArray.size()) {
		if (BFSArray[i].x == x && BFSArray[i].y == y) {
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0) {
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);
		i++;
	}

	BFSArray.clear();
}