#pragma once

#include <vector>

using namespace std;

class Algorytm
{
public:
	const int nodeN = 40; // liczba miast
	vector<vector<int>> node; // dane instancji
	vector<int> saPath; // rozwi¹zanie
	vector<int> saDataDist; // dane wykresu Dist
	vector<int> saDataPraw; // dane wykresu Praw
	vector<int> saDataTemp; // dane wykresu Temp
	int saBestDist; // najlepsze znalezione rozwi¹zanie (Dist)
	vector<int> saBestPath; // najlepsze znalezione rozwi¹zanie (path)
	const double saTempStart = 10000; // temperatura Startowa
	const double saTempAlpha = 0.976; // wspó³czynnik zmiany temperatury
	double saTemp; // bie¿¹ca temperatura

	int loopN = 0; // ile pozostalo kroków do wykonania
	const int loopDelay = 10; // opóŸnienie pêtl

	//funkcje
	vector<int> RandPath();
	double PathDist(const vector<int>& path);
	pair<int, int> RandAB();
	void PathMove(vector<int>& path, pair<int, int> move);
	void SaStep(vector<int>& path, double temp);
	void Run();
	void Loop();
	void NodeRand();
	void Draw(); // Placeholder for the drawing function

};

