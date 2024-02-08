#include <string>
#include <iostream>
#include <stack>
using namespace std;

class Coord
{
  public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
  private:
    int m_r;
    int m_c;
};

// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	stack<Coord> coor;
	coor.push(Coord(sr, sc)); // push start coord
	maze[sr][sc] = 'o'; // breadcrumb

	while(!coor.empty()) {
		// pop current coordinate off stack
		Coord curr = coor.top();
		coor.pop();

		int r = curr.r();
		int c = curr.c();

		if (r == er && c == ec)
			return true;

		// north
		if (maze[r-1][c] == '.') {
			coor.push(Coord(r-1, c));
			maze[r-1][c] = 'o';
		}

		// west
		if (maze[r][c-1] == '.') {
			coor.push(Coord(r, c-1));
			maze[r][c-1] = 'o';
		}

		// south
		if (maze[r+1][c] == '.') {
			coor.push(Coord(r+1, c));
			maze[r+1][c] = 'o';
		}

		// east
		if (maze[r][c+1] == '.') {
			coor.push(Coord(r, c+1));
			maze[r][c+1] = 'o';
		}
	}

	return false;
}
