#include <iostream>
#include <iomanip>
#include <queue>
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
	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	
	while (!coordQueue.empty())
	{
		Coord cur	= coordQueue.front();	//uses Copy Contructor of Coord class
		coordQueue.pop();
		
		/*
		 dist [cur.r()-1][cur.c()]			//above (NORTH) the current location
		 dist [cur.r()+1][cur.c()]			//below (SOUTH) the current location
		 dist [cur.r()][cur.c()-1]			//left (WEST) of current location
		 dist [cur.r()][cur.c()+1]			//right (EAST) of current location
		 */
		
		int v	=	dist [cur.r()][cur.c()];
		int m	=	dist [cur.r()-1][cur.c()];
		if (dist [cur.r()+1][cur.c()] < m)
			m	= dist [cur.r()+1][cur.c()];
		if (dist [cur.r()][cur.c()-1] < m)
			m	= dist [cur.r()][cur.c()-1];
		if (dist [cur.r()][cur.c()+1] < m)
			m	= dist [cur.r()][cur.c()+1];
		
		if (dist [cur.r()][cur.c()] > m+1)	//prevent erasing previous path which is shorter
			v	= m+1;
		
		//Move North?
		if (maze [cur.r()-1][cur.c()] != 'X' && dist [cur.r()-1][cur.c()] > v+1)
		{	
			dist [cur.r()-1][cur.c()]	= v+1;
			coordQueue.push(Coord(cur.r()-1, cur.c()));
		}
		//Move East?
		if (maze [cur.r()][cur.c()+1] != 'X' && dist [cur.r()][cur.c()+1] > v+1)
		{
			dist [cur.r()][cur.c()+1]	= v+1;
			coordQueue.push(Coord(cur.r(), cur.c()+1));
		}
		//Move South?
		if (maze [cur.r()+1][cur.c()] != 'X' && dist [cur.r()+1][cur.c()] > v+1)
		{
			dist [cur.r()+1][cur.c()]	= v+1;
			coordQueue.push(Coord(cur.r()+1, cur.c()));
		}
		
		//Move West?
		if (maze [cur.r()][cur.c()-1] != 'X' && dist [cur.r()][cur.c()-1] > v+1)
		{
			dist [cur.r()][cur.c()-1]	= v+1;
			coordQueue.push(Coord(cur.r(), cur.c()-1));
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
	
	/*
	 #####################
	 #*.................*#
	 #.##.#.#######.#.##.#
	 #....#.........#....#
	 ####.#.###%###.#.####
	 #......#  $  #......#
	 ####.#.#######.#.####
	 #  #.#....@....#.#  #
	 ####.#.#######.#.####
	 #.........#.........#
	 #.##.####.#.####.##.#
	 #*.#.............#.*#
	 ##.#.#.#######.#.#.##
	 #...................#
	 #####################
	 */
	 int distances[10][10];
	determineDistances(myMaze, 6, 3, distances);
	for (int r = 0; r < 10; r++)
	{
		for (int c = 0; c < 10; c++)
			cout << ' ' << setw(2) << distances[r][c];
		cout << endl;
	}
	
	// Output should be
	//  99 99 99 99 99 99 99 99 99 99
	//  99  7  6  7  8  9 99  9 10 99
	//  99 99  5 99 99  8  7  8  9 99
	//  99  5  4 99  8  7  6 99 10 99
	//  99  4  3  2 99 99  5 99 99 99
	//  99 99 99  1  2  3  4 99 99 99
	//  99 99  1  0  1 99  5 99 99 99
	//  99 99  2 99  2 99  6 99 99 99
	//  99 99  3  4  3 99  7  8  9 99
	//  99 99 99 99 99 99 99 99 99 99
}
