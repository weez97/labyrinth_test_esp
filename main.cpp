#include <algorithm>
#include <random>
#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>

using namespace std;

// this struct will help us remember both longest path
// and whether the cell has been already visited or not
struct Cell {
    char c;
    int path_length;
    bool is_visited = false;
};

const int start_pos = 1; // we're starting on top left corner, int will suffice
int n_size;              // maze width and height
pair<int, int> end_pos = { 0, 0 };
int max_path = 0;

vector<string> directions = { "up", "down", "left", "right" };
vector<vector<Cell>> maze;

pair<int, int> convertDirection(string dir, int x, int y)
{
    if (dir == "up")
        return { x, y + 2 };
    else if (dir == "down")
        return { x, y - 2 };
    else if (dir == "left")
        return { x - 2, y };
    else
        return { x + 2, y };
}

bool withinValidBounds(pair<int, int> pos)
{
    return (pos.first > 0 && pos.first < n_size&& pos.second > 0 && pos.second < n_size);
}

bool isVisited(pair<int, int> pos)
{
    return maze[pos.first][pos.second].is_visited;
}

vector<int> getRandomOrder() {
    vector<int> v = { 0, 1, 2, 3 };

    //this changes the seed each time the function is called, based on current time
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    shuffle(v.begin(), v.end(), default_random_engine(seed));
    return v;
}

void visitPath(string dir, int x, int y) {
    if (dir == "up")
        maze[x][y + 1].c = ' ';
    else if (dir == "down")
        maze[x][y - 1].c = ' ';
    else if (dir == "left")
        maze[x - 1][y].c = ' ';
    else
        maze[x + 1][y].c = ' ';
}

//this is our recursive backtracking algorithm
void moveToAdjCell(int x, int y, int n_path_len) {

    maze[x][y].is_visited = true;
    maze[x][y].c = ' ';
    maze[x][y].path_length = n_path_len;

    //this keep track of which path is the longest
    if (n_path_len > max_path)
    {
        end_pos = { x, y };  //store path
        max_path = n_path_len; //update max path length
    }

    //we'll set a random order for the directions to follow
    auto v = getRandomOrder();

    for (int i = 0; i < v.size(); i++)
    {
        pair<int, int> nxt_n = convertDirection(directions[v[i]], x, y);
        if (withinValidBounds(nxt_n) && !isVisited(nxt_n)) {
            visitPath(directions[v[i]], x, y); // this will connect the paths
            moveToAdjCell(nxt_n.first, nxt_n.second, n_path_len + 1);
        }
    }
}

void carvePath(int x, int y) {
    moveToAdjCell(x, y, 1);
}

vector<vector<Cell>> getMatrix(int n) {
    vector<vector<Cell>> matrix(n, vector<Cell>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j].c = '*';
            matrix[i][j].path_length = 0;
        }
    }
    return matrix;
}

void printMaze(int n) {
    //we'll modify the maze in the entrance and exit cells for visualization purposes 
    maze[start_pos][start_pos].c = 'A';
    maze[end_pos.first][end_pos.second].c = 'B';

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << maze[i][j].c;
        }
        cout << endl;
    }
}

void createMaze2D(int n) {
    maze = getMatrix(n);
    carvePath(start_pos, start_pos);
    printMaze(n);
}

int main() {
    n_size = 51;
    createMaze2D(n_size);
    getch();
    return 0;
}