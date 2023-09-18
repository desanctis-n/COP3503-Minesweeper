#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Tile.h"

using namespace std;

class Board
{
	// ------------------------- RANDOM --------------------------- //

	class Random
	{
		static mt19937 random;
	public:
		static int Number(int min, int max);
	};

	vector<vector<Tile*>> _board;
	map<string, sf::Texture> textures;

	bool _debugBoard = false;
	int _numCols = 0;
	int _numRows = 0;
	int _numMines = 0;
	int _numFlags = 0;

public:

	// -------------- CONSTRUCTION AND DESTRUCTION -------------- //

	Board();
	Board(const string& cfgName, const string& brdName="none");
	~Board();

	// -------------------- BASIC GENERATION -------------------- //

	void InfoFromConfig(const string& cfgName);
	void TilesFromBoard(const string& brdName);
	
	// ------------------ TILE GENERATION ------------------ //

	void Add();
	void Reset();

	void AddEmptyTiles();
	void SetRandomMines();
	void SetAdjacentTiles();

	void Flag(int& y, int& x);
	void ShowMines();
	void DebugBoard();

	// ------------------------ GAME STATE ----------------------- //

	bool LoseCondition();
	bool WinCondition();

	// ------------------------ TEXTURE MAP ---------------------- //

	void AddTextures();

	// ----------------------- BOARD WINDOW ---------------------- //

	void WindowFromBoard();

	// ---------------------- PRINT DEBUGGING -------------------- //

	void Display();

};

