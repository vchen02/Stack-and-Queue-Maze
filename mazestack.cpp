#include <iostream>
#include <iomanip>
#include <stack>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

void determineDistances(const char maze[][10], int sr, int sc, int dist[][10])
{
	//Set every element of the distance array to 99.
	for (int row =0; row < 10; row ++)
	{
		for (int col=0; col <10; col ++)
			dist[row][col]	= 99;
	}
	
	//Set position (sr,sc) of the distance array to 0.
	dist [sr][sc]	= 0;
	
	//Push the starting coordinate (sr,sc) onto the coordinate stack.
	stack<Coord> coordStack;
	coordStack.push(Coord(sr, sc));
	
	while (!coordStack.empty())
	{
		Coord cur	= coordStack.top();		//uses Copy Contructor of Coord class
		coordStack.pop();
		
		/*
		dist [cur.r()-1][cur.c()]			//above (NORTH) the current location
		dist [cur.r()+1][cur.c()]			//below (SOUTH) the current location
		dist [cur.r()][cur.c()-1]			//left (WEST) of current location
		dist [cur.r()][cur.c()+1]			//right (EAST) of current location
		*/
		
		int v	=	dist [cur.r()][cur.c()];	//holds current value on distance array
		
		int m	=	dist [cur.r()-1][cur.c()]; //finds the smallest value between the 4
		if (dist [cur.r()+1][cur.c()] < m)	   //directions centered at dist [cur.r()][cur.c()]
			m	= dist [cur.r()+1][cur.c()];
		if (dist [cur.r()][cur.c()-1] < m)
			m	= dist [cur.r()][cur.c()-1];
		if (dist [cur.r()][cur.c()+1] < m)
			m	= dist [cur.r()][cur.c()+1];
	
		if (dist [cur.r()][cur.c()] > m+1)		//reached a number on distance array thats smaller than current path
			v	= m+1;							//prevent overwriting previous path on distance array
	
		//Move North?
		if (maze [cur.r()-1][cur.c()] != 'X' && dist [cur.r()-1][cur.c()] > v+1)
		{	
			dist [cur.r()-1][cur.c()]	= v+1;
			coordStack.push(Coord(cur.r()-1, cur.c()));
		}
		//Move East?
		if (maze [cur.r()][cur.c()+1] != 'X' && dist [cur.r()][cur.c()+1] > v+1)
		{
			dist [cur.r()][cur.c()+1]	= v+1;
			coordStack.push(Coord(cur.r(), cur.c()+1));
		}
		//Move South?
		if (maze [cur.r()+1][cur.c()] != 'X' && dist [cur.r()+1][cur.c()] > v+1)
		{
			dist [cur.r()+1][cur.c()]	= v+1;
			coordStack.push(Coord(cur.r()+1, cur.c()));
		}
		//Move West?
		if (maze [cur.r()][cur.c()-1] != 'X' && dist [cur.r()][cur.c()-1] > v+1)
		{
			dist [cur.r()][cur.c()-1]	= v+1;
			coordStack.push(Coord(cur.r(), cur.c()-1));
		}
	}
}

int main()
{
	char myMaze[10][10] = {
		{ 'X','X','X','X','X','X','X','X','X','X'},
		{ 'X','.','.','.','.','.','X','.','.','X'},
		{ 'X','X','.','X','X','.','.','.','.','X'},
		{ 'X','.','.','X','.','.','.','X','.','X'},
		{ 'X','.','.','.','X','X','.','X','X','X'},
		{ 'X','X','X','.','.','.','.','X','.','X'},
		{ 'X','X','.','.','.','X','.','X','.','X'},
		{ 'X','X','.','X','.','X','.','X','X','X'},
		{ 'X','X','.','.','.','X','.','.','.','X'},
		{ 'X','X','X','X','X','X','X','X','X','X'}
	};
	
	int distances[10][10];
	determineDistances(myMaze, 6, 3, distances);
	for (int r = 0; r < 10; r++)
	{
		for (int c = 0; c < 10; c++)
			cout << ' ' << setw(2) << distances[r][c];
		cout << endl;
	}
}
