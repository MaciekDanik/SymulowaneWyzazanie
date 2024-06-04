#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <numeric>

using namespace std;

// Configuration variables
int nodeN = 40; // number of cities
vector<vector<int>> node; // instance data
vector<int> saPath; // solution
vector<double> saDataDist; // data for Dist chart
vector<pair<double, int>> saDataPraw; // data for Praw chart
vector<double> saDataTemp; // data for Temp chart
double saBestDist; // best found solution (Dist)
vector<int> saBestPath; // best found solution (path)
double saTempStart = 10000; // starting temperature
double saTempAlpha = 0.976; // temperature change coefficient
double saTemp; // current temperature

int loopN = 0; // remaining steps to execute
int loopDelay = 10; // loop delay

// Function declarations
vector<int> RandPath();
double PathDist(const vector<int>& path);
pair<int, int> RandAB();
void PathMove(vector<int>& path, pair<int, int> move);
void SaStep(vector<int>& path, double temp);
void Run();
void Loop();
void NodeRand();
void Draw(); // Placeholder for the drawing function

// Random number generator setup
random_device rd;
mt19937 gen(rd());

int main() {
    NodeRand();
    Run();
    return 0;
}

void Run() {
    vector<int> startPath = RandPath();
    double startDist = PathDist(startPath);
    saPath = startPath;
    saDataDist.push_back(startDist);
    saBestDist = startDist;
    saBestPath = startPath;

    loopN = 400;
    saTemp = saTempStart;
    saDataTemp.clear();
    saDataPraw.clear();
    Loop();
}

void Loop() {
    if (loopN == 0) return;
    loopN--;
    SaStep(saPath, saTemp);
    saTemp = saTemp * saTempAlpha;
    if (loopN == 0) {
        // In the last step, display the best solution found
        saDataDist.push_back(saBestDist);
        saPath = saBestPath;
    }
    Draw();
    this_thread::sleep_for(chrono::milliseconds(loopDelay));
    Loop();
}

void SaStep(vector<int>& path, double temp) {
    double c0 = PathDist(path);
    double praw = 0;
    int akce = 0;
    for (int i = 0; i < 100; i++) {
        pair<int, int> move = RandAB();
        PathMove(path, move);
        double c1 = PathDist(path);
        if (c1 > c0) {
            praw = exp((c0 - c1) / temp);
            akce = (gen() % 10000 < praw * 10000);
        }
        if (c1 <= c0 || akce) c0 = c1; // accept move
        else PathMove(path, move); // reject move
        if (saBestDist > c0) {
            saBestDist = c0;
            saBestPath = path;
        }
    }
    saDataDist.push_back(c0);
    saDataTemp.push_back(saTemp);
    saDataPraw.push_back({ praw, akce });
}

pair<int, int> RandAB() {
    uniform_int_distribution<int> dist(0, node.size() - 1);
    int a = dist(gen);
    int b = dist(gen);
    while (b == a) b = dist(gen);
    if (a > b) swap(a, b);
    return { a, b };
}

void PathMove(vector<int>& path, pair<int, int> move) {
    int a = move.first, b = move.second;
    while (a < b) swap(path[a++], path[b--]);
}

double PathDist(const vector<int>& path) {
    double dist = 0;
    for (int i = 0; i < path.size(); i++) {
        int n0 = path[i];
        int n1 = path[(i + 1) % path.size()];
        double x0 = node[n0][0];
        double y0 = node[n0][1];
        double x1 = node[n1][0];
        double y1 = node[n1][1];
        dist += round(sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1)));
    }
    return dist;
}

vector<int> RandPath() {
    vector<int> path(node.size());
    iota(path.begin(), path.end(), 0);
    shuffle(path.begin(), path.end(), gen);
    return path;
}

void NodeRand() {
    loopN = 0;
    node.clear();
    saPath.clear();
    saBestPath.clear();
    saDataDist.clear();
    saDataPraw.clear();
    saDataTemp.clear();
    uniform_int_distribution<int> distX(0, 400);
    uniform_int_distribution<int> distY(0, 200);
    for (int i = 0; i < nodeN; i++) {
        node.push_back({ distX(gen), distY(gen) });
    }
    Draw();
}

void Draw() {
    system("cls");
    const int width = 50;
    const int height = 25;
    const int AscVal = 254;
    vector<vector<char>> grid(height, vector<char>(width, ' '));

    for (int i = 0; i < saPath.size(); i++) {
        int n0 = saPath[i];
        int n1 = saPath[(i + 1) % saPath.size()];
        int x0 = node[n0][0] * (width - 1) / 400;
        int y0 = node[n0][1] * (height - 1) / 200;
        int x1 = node[n1][0] * (width - 1) / 400;
        int y1 = node[n1][1] * (height - 1) / 200;

        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2;

        while (true) {
            grid[y0][x0] = '*';
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    for (const auto& city : node) {
        int x = city[0] * (width - 1) / 400;
        int y = city[1] * (height - 1) / 200;
        //grid[y][x] = '#';
        grid[y][x] = char(AscVal);
    }

    for (const auto& row : grid) {
        for (const auto& cell : row) {
            
            if (cell == char(AscVal)) 
            {
                cout << "\033[32m" << cell << "\033[0m";
                //cout << cell;
            }
            else
            {
                cout << cell;
            }
        }
        cout << endl;
    }

    cout << "\n"<< "Temperatura: " << saTemp;
    cout << "\n" << "Najlepsza dlugosc: " << saBestDist;
    cout << endl;
}