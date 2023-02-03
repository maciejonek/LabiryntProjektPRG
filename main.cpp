#include <iostream>
#include <ncurses.h>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <ctime>
using namespace std;

void ReadMaze(int **maze);//Wczytywanie z pliku
void PrintMaze(int **maze,int x, int y);//Wydruk w oknie
int CursLimit(int);//Blokowanie przed wyjsciem poza tablice maze
void Moving(int **maze, int &x, int &y, int &tempX, int &tempY, int maxx, int maxy);//Poruszanie sie po polach 1 i blokowanie przez sciany
void WinCheck(int **maze, int, int, bool &exit, int);//Sprawdzanie czy pole ma wartosc 3 czyli mety oraz zapis do pliku
void FindStart(int **maze, int &x, int &y);//Znajdowanie startu w zaleznosci od pliku
void DrawFrame();//Bardzo estetyczna ramka labiryntu
void Record(int);//Stoper (mega zaawansowany)
int main(){
	int **maze;
	int maxx,maxy,x=0,y=0,tempY=0,tempX=4,switchInput;
	int sec=0;
	bool exit=0;
	int T = std::chrono::seconds(std::time(NULL)).count();
	int a=0;
	maze=new int*[19];
	for(int i=0;i<19;i++)
		maze[i]=new int[19];
	ReadMaze(maze);
	initscr();
	keypad(stdscr,true);
	timeout(-1);
	FindStart(maze,x,y);
		while(!exit){
		sec=(std::chrono::seconds(std::time(NULL)).count())-T;
		Record(sec);
		maxx=(getmaxx(stdscr)/2)-10;
		maxy=(getmaxy(stdscr)/2)-10;
		PrintMaze(maze,maxx,maxy);
		DrawFrame();
		Moving(maze,x,y,tempX,tempY,maxx,maxy);
		switchInput=getch();
		switch(switchInput){
			case KEY_UP: y--;
			break;
			case KEY_DOWN: y++;
			break;
			case KEY_LEFT: x--;
			break;
			case KEY_RIGHT: x++;
			break;
			case 'q': exit=1;
			break;
		}
		x=CursLimit(x);
		y=CursLimit(y);
		WinCheck(maze,x,y,exit,sec);
		clear();
	}
	refresh();
	endwin();
}

void ReadMaze(int **maze){
	string lines[19];
	char singl[19][19];
	int i=0;
	ifstream file;
	file.open("maze.txt");
	for(int i=0;i<19;i++){
		getline(file, lines[i]);
		strcpy(singl[i], lines[i].c_str());
	}
	for(int i=0;i<19;i++){
		for(int j=0;j<19;j++){
			maze[i][j]=(int)singl[i][j]-48;
		}
	}
	file.close();
}
void PrintMaze(int **maze,int x,int y){
	start_color();
	init_pair(2,COLOR_BLACK,COLOR_BLACK);
	init_pair(3,COLOR_BLACK,COLOR_RED);
	init_pair(4,COLOR_BLACK,COLOR_GREEN);
	for(int i=0;i<19;i++){
		for(int j=0;j<19;j++){
			if(maze[i][j]==0){
				attron(COLOR_PAIR(2));
				mvprintw(y+i,x+j," ");
				attroff(COLOR_PAIR(2));
				}
			else if(maze[i][j]==3){
				attron(COLOR_PAIR(3));
				mvprintw(y+i,x+j," ");
				attroff(COLOR_PAIR(3));
			}
			else{
				attron(COLOR_PAIR(4));
				mvprintw(y+i,x+j," ");
				attroff(COLOR_PAIR(4));
			}
	}

}
}
void Moving(int **maze, int &x, int &y, int &tempX, int &tempY, int maxx, int maxy){
		if(maze[y][x]!=0){
			move(y+maxy,x+maxx);
			tempX=x;
			tempY=y;
		}
		else{
			x=tempX;
			y=tempY;
			move(y+maxy,x+maxx);
		}
}

int CursLimit(int pos){
	if(pos<0) pos=0;
	if(pos>19) pos=19;
	return pos;
}
void WinCheck(int **maze, int x, int y, bool &exit, int sec){
		int exitInput;
		ofstream file;
		file.open("bestscore.txt");
		file<<"Wynik: "<<sec;
		file.close();
		if(maze[y][x]==3){
			mvprintw(20,20,"wygrana, nacisnij q");
			while(!exit){
				exitInput=getch();
				if(exitInput=='q')
					exit=1;
			}	
		}
}
void FindStart(int **maze, int &x, int &y){
	int i,j;
	for(i=0;i<19;i++){
		for(j=0;j<19;j++){
			if(maze[i][j]==2)
				goto startxy;
		}
	}
	startxy:
		x=j;
		y=i;
}
void DrawFrame(){
	int maxx=getmaxx(stdscr)/2, maxy=getmaxy(stdscr)/2;
	start_color();
	init_pair(1,COLOR_CYAN, COLOR_YELLOW);
	attron(COLOR_PAIR(1));
	for(int i=maxy-11;i<maxy+10;i++)
		for(int j=maxx-11;j<maxx+10;j++)
			if(j==maxx-11 || j==maxx+9)
				mvprintw(i,j," ");
			else if(i==maxy-11 || i==maxy+9)
				mvprintw(i,j," ");
	attroff(COLOR_PAIR(1));
}
void Record(int T){
		mvprintw(1,1,"%d",T);
}
