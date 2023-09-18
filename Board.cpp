#include "Board.h"

using namespace std;

// ------------------------- RANDOM --------------------------- //

mt19937 Board::Random::random(time(0));
int Board::Random::Number(int min, int max)
{
	uniform_int_distribution<int> dist(min, max);
	return dist(random);
}

// -------------- CONSTRUCTION AND DESTRUCTION -------------- //

Board::Board()
{
	AddTextures();
}
Board::Board(const string& cfgName, const string& brdName)
{
	InfoFromConfig(cfgName);
	if (brdName != "none")
		TilesFromBoard(brdName);
	else
	{
		AddEmptyTiles();
		SetRandomMines();
		SetAdjacentTiles();
	}
	AddTextures();
}
Board::~Board()
{
	for (int i = 0; i < _board.size(); i++) 
		for (int j = 0; j < _board[i].size(); j++)
			delete _board[i][j];
}

// -------------------- BASIC GENERATION -------------------- //

void Board::InfoFromConfig(const string& cfgName)
{
	string temp;
	ifstream file(cfgName);

	getline(file, temp);
	_numCols = stoi(temp);
	getline(file, temp);
	_numRows = stoi(temp);
	getline(file, temp);
	_numMines = stoi(temp);
}
void Board::TilesFromBoard(const string& brdName)
{
	AddEmptyTiles();
	ifstream file(brdName);
	char mine = '0';
	for (int y = 0; y < _board.size(); y++) {
		for (int x = 0; x < _board[y].size(); x++)
		{
			_board[y][x]->Default();
			file.read(static_cast<char*>(&mine), sizeof(mine));
			if (mine == '1') {
				_board[y][x]->AddMine();
				_numMines++;
			}
		}
		file.read(static_cast<char*>(&mine), sizeof(mine));
	}
	SetAdjacentTiles();
	if (_debugBoard)
		DebugBoard();
}

// -------------------- MASS TILE GENERATION -------------------- //

void Board::Add()
{
	AddEmptyTiles();
	SetRandomMines();
	SetAdjacentTiles();
	if (_debugBoard)
		DebugBoard();
}
void Board::Reset()
{
	for (int y = 0; y < _board.size(); y++)
		_board[y].clear();
	_board.clear();
	_numMines = 0;
	_numFlags = 0;
}
void Board::AddEmptyTiles()
{
	for (int y = 0; y < _numRows; y++)
		_board.emplace_back();
	for (int y = 0; y < _board.size(); y++)
		for (int x = 0; x < _numCols; x++)
			_board[y].emplace_back(new Tile);
}
void Board::SetRandomMines()
{
	int x;
	int y;
	for (int i = 0; i < _numMines; i++) {
		x = Random::Number(0, _numCols - 1);
		y = Random::Number(0, _numRows - 1);
		if (!_board[y][x]->_mine)
			_board[y][x]->AddMine();
		else
			i--;
	}
}
void Board::SetAdjacentTiles()
{
	// -------------------- CORNERS -------------------- //

	_board[0][0]->_adjacentTiles.push_back(_board[0][1]);
	_board[0][0]->_adjacentTiles.push_back(_board[1][1]);
	_board[0][0]->_adjacentTiles.push_back(_board[1][0]);

	_board[0][_board[0].size() - 1]->_adjacentTiles.push_back(_board[0][_board[0].size() - 2]);
	_board[0][_board[0].size() - 1]->_adjacentTiles.push_back(_board[1][_board[0].size() - 2]);
	_board[0][_board[0].size() - 1]->_adjacentTiles.push_back(_board[1][_board[0].size() - 1]);

	_board[_board.size() - 1][0]->_adjacentTiles.push_back(_board[_board.size() - 2][0]);
	_board[_board.size() - 1][0]->_adjacentTiles.push_back(_board[_board.size() - 1][1]);
	_board[_board.size() - 1][0]->_adjacentTiles.push_back(_board[_board.size() - 2][1]);

	_board[_board.size() - 1][_board[0].size() - 1]->_adjacentTiles.push_back(_board[_board.size() - 2][_board[0].size() - 1]);
	_board[_board.size() - 1][_board[0].size() - 1]->_adjacentTiles.push_back(_board[_board.size() - 2][_board[0].size() - 2]);
	_board[_board.size() - 1][_board[0].size() - 1]->_adjacentTiles.push_back(_board[_board.size() - 1][_board[0].size() - 2]);

	// -------------------- EDGE LINES -------------------- //

	for (int x = 1; x < _board[0].size() - 1; x++) {

		// -------------------- TOP EDGES -------------------- //
		_board[0][x]->_adjacentTiles.push_back(_board[0][x - 1]);
		_board[0][x]->_adjacentTiles.push_back(_board[0][x + 1]);
		_board[0][x]->_adjacentTiles.push_back(_board[1][x]);
		_board[0][x]->_adjacentTiles.push_back(_board[1][x - 1]);
		_board[0][x]->_adjacentTiles.push_back(_board[1][x + 1]);

		// -------------------- BOTTOM EDGES -------------------- //

		_board[_board.size() - 1][x]->_adjacentTiles.push_back(_board[_board.size() - 1][x - 1]);
		_board[_board.size() - 1][x]->_adjacentTiles.push_back(_board[_board.size() - 1][x + 1]);
		_board[_board.size() - 1][x]->_adjacentTiles.push_back(_board[_board.size() - 2][x]);
		_board[_board.size() - 1][x]->_adjacentTiles.push_back(_board[_board.size() - 2][x - 1]);
		_board[_board.size() - 1][x]->_adjacentTiles.push_back(_board[_board.size() - 2][x + 1]);
	}

	for (int y = 1; y < _board.size() - 1; y++) {

		// -------------------- LEFT EDGES -------------------- //
		_board[y][0]->_adjacentTiles.push_back(_board[y][1]);
		_board[y][0]->_adjacentTiles.push_back(_board[y - 1][1]);
		_board[y][0]->_adjacentTiles.push_back(_board[y - 1][0]);
		_board[y][0]->_adjacentTiles.push_back(_board[y + 1][0]);
		_board[y][0]->_adjacentTiles.push_back(_board[y + 1][1]);

		// -------------------- RIGHT EDGES -------------------- //
		_board[y][_board[y].size() - 1]->_adjacentTiles.push_back(_board[y][_board[y].size() - 2]);
		_board[y][_board[y].size() - 1]->_adjacentTiles.push_back(_board[y - 1][_board[y].size() - 2]);
		_board[y][_board[y].size() - 1]->_adjacentTiles.push_back(_board[y - 1][_board[y].size() - 1]);
		_board[y][_board[y].size() - 1]->_adjacentTiles.push_back(_board[y + 1][_board[y].size() - 2]);
		_board[y][_board[y].size() - 1]->_adjacentTiles.push_back(_board[y + 1][_board[y].size() - 1]);

		for (int x = 1; x < _board[y].size() - 1; x++) {

			// -------------------- CENTER BOARD -------------------- //
			_board[y][x]->_adjacentTiles.push_back(_board[y][x + 1]);
			_board[y][x]->_adjacentTiles.push_back(_board[y][x - 1]);
			_board[y][x]->_adjacentTiles.push_back(_board[y + 1][x]);
			_board[y][x]->_adjacentTiles.push_back(_board[y - 1][x]);
			_board[y][x]->_adjacentTiles.push_back(_board[y - 1][x + 1]);
			_board[y][x]->_adjacentTiles.push_back(_board[y + 1][x - 1]);
			_board[y][x]->_adjacentTiles.push_back(_board[y + 1][x + 1]);
			_board[y][x]->_adjacentTiles.push_back(_board[y - 1][x - 1]);
		}
	}
	for (int y = 0; y < _board.size(); y++)
		for (int x = 0; x < _board[y].size(); x++)
			_board[y][x]->AddNum();
}
void Board::Flag(int& y, int& x)
{
	if (!_board[y][x]->_revealed) {
		if (_board[y][x]->_flag) {
			_board[y][x]->_flag = false;
			_numFlags--;
		}
		else {
			_board[y][x]->_flag = true;
			_numFlags++;
		}
	}
}
void Board::ShowMines()
{
	for (int y = 0; y < _numRows; y++)
		for (int x = 0; x < _numCols; x++)
			if (_board[y][x]->_mine)
				_board[y][x]->Reveal();
}
void Board::DebugBoard()
{
	for (int y = 0; y < _board.size(); y++)
		for (int x = 0; x < _board[y].size(); x++) {
			if (_board[y][x]->_mine) {
				_board[y][x]->Debug();
			}
		}
}

// ------------------------ GAME STATE ----------------------- //

bool Board::LoseCondition()
{
	for (int y = 0; y < _numRows; y++)
		for (int x = 0; x < _numCols; x++)
			if (_board[y][x]->_mine && _board[y][x]->_revealed)
				return true;
	return false;
}
bool Board::WinCondition()
{
	for (int y = 0; y < _numRows; y++)
		for (int x = 0; x < _numCols; x++) {
			if (_board[y][x]->_mine)
				continue;
			else if (!_board[y][x]->_revealed)
				return false;
		}
	return true;
}

// ------------------------ TEXTURE MAP ----------------------- //

void Board::AddTextures()
{
	textures["debug"].loadFromFile("images/debug.png");
	textures["digits"].loadFromFile("images/digits.png");
	textures["face_happy"].loadFromFile("images/face_happy.png");
	textures["face_lose"].loadFromFile("images/face_lose.png");
	textures["face_win"].loadFromFile("images/face_win.png");
	textures["flag"].loadFromFile("images/flag.png");
	textures["mine"].loadFromFile("images/mine.png");
	textures["number_1"].loadFromFile("images/number_1.png");
	textures["number_2"].loadFromFile("images/number_2.png");
	textures["number_3"].loadFromFile("images/number_3.png");
	textures["number_4"].loadFromFile("images/number_4.png");
	textures["number_5"].loadFromFile("images/number_5.png");
	textures["number_6"].loadFromFile("images/number_6.png");
	textures["number_7"].loadFromFile("images/number_7.png");
	textures["number_8"].loadFromFile("images/number_8.png");
	textures["test_1"].loadFromFile("images/test_1.png");
	textures["test_2"].loadFromFile("images/test_2.png");
	textures["test_3"].loadFromFile("images/test_3.png");
	textures["tile_hidden"].loadFromFile("images/tile_hidden.png");
	textures["tile_revealed"].loadFromFile("images/tile_revealed.png");
}

// ---------------------- BOARD WINDOW ---------------------- //

void Board::WindowFromBoard()
{
	sf::RenderWindow window(sf::VideoMode(_numCols * 32, _numRows * 32 + 100), "Minesweeper");

	while (window.isOpen())
	{
		window.setSize(sf::Vector2u(_numCols * 32, _numRows * 32 + 100));
		sf::Event event;
		sf::Vector2i mousePos;
		sf::Mouse mouse;

		window.clear(sf::Color(255, 255, 255));

		// ---------------------------------- TEXTURE SETTING ------------------------------------ //

		// ------------ MENU TEXTURES ------------ //

		sf::Sprite smiley(textures["face_happy"]);
		smiley.setPosition(((float)((_numCols / 2.0) * 32 - 32.0)), (float)(_numRows * 32));
		window.draw(smiley);

		sf::Sprite debugSprite(textures["debug"]);
		debugSprite.setPosition(((float)((_numCols / 2.0) * 32 + 96.0)), (float)(_numRows * 32));
		window.draw(debugSprite);

		sf::Sprite test1Sprite(textures["test_1"]);
		test1Sprite.setPosition(((float)((_numCols / 2.0) * 32 + 160.0)), (float)(_numRows * 32));
		window.draw(test1Sprite);

		sf::Sprite test2Sprite(textures["test_2"]);
		test2Sprite.setPosition(((float)((_numCols / 2.0) * 32 + 224.0)), (float)(_numRows * 32));
		window.draw(test2Sprite);

		sf::Sprite test3Sprite(textures["test_3"]);
		test3Sprite.setPosition(((float)((_numCols / 2.0) * 32 + 288.0)), (float)(_numRows * 32));
		window.draw(test3Sprite);

		// --------- MINE COUNT TEXTURES --------- //

		sf::Sprite _negSprite(textures["digits"]);
		_negSprite.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
		_negSprite.setPosition(0.0, (float)(_numRows * 32.0));
		if (_numMines - _numFlags < 0)
			window.draw(_negSprite);

		int index = 0;
		char char1;
		char char2;
		char char3;

		int pos1 = 0;
		int pos2 = 0;
		int pos3 = 0;

		string numString = to_string(_numMines - _numFlags);
		if (_numMines - _numFlags < 0)
			numString.erase(0, 1);
		while (numString.size() < 3)
			numString.insert(0, "0");

		char1 = numString[0];
		char2 = numString[1];
		char3 = numString[2];

		pos1 = char1 - '0';
		pos2 = char2 - '0';
		pos3 = char3 - '0';

		sf::Sprite dig1Sprite(textures["digits"]);
		dig1Sprite.setTextureRect(sf::IntRect(21 * pos1, 0, 21, 32));
		dig1Sprite.setPosition(21.0, (float)(_numRows * 32));
		window.draw(dig1Sprite);

		sf::Sprite dig2Sprite(textures["digits"]);
		dig2Sprite.setTextureRect(sf::IntRect(21 * pos2, 0, 21, 32));
		dig2Sprite.setPosition(42.0, (float)(_numRows * 32));
		window.draw(dig2Sprite);

		sf::Sprite dig3Sprite(textures["digits"]);
		dig3Sprite.setTextureRect(sf::IntRect(21 * pos3, 0, 21, 32));
		dig3Sprite.setPosition(63.0, (float)(_numRows * 32));
		window.draw(dig3Sprite);

		// ------------ TILE TEXTURES ------------- //

		for (int y = 0; y < _board.size(); y++)
			for (int x = 0; x < _board[y].size(); x++)
			{
				_board[y][x]->_tileSprite.setTexture(textures["tile_revealed"]);
				_board[y][x]->_tileSprite.setPosition((float)(x * 32), (float)(y * 32));
				window.draw(_board[y][x]->_tileSprite);

				_board[y][x]->_tileSprite.setTexture(textures[_board[y][x]->_textureName]);
				_board[y][x]->_hiddenSprite.setTexture(textures["tile_hidden"]);
				_board[y][x]->_hiddenSprite.setPosition((float)(x * 32), (float)(y * 32));


				window.draw(_board[y][x]->_tileSprite);

				if (!_board[y][x]->_revealed)
					window.draw(_board[y][x]->_hiddenSprite);

				if (_board[y][x]->_flag) {
					_board[y][x]->_flagSprite.setTexture(textures["flag"]);
					_board[y][x]->_flagSprite.setPosition((float)(x * 32), (float)(y * 32));
					window.draw(_board[y][x]->_flagSprite);
				}

				if (_board[y][x]->_debug || (LoseCondition() && _board[y][x]->_mine)) {
					_board[y][x]->_debugSprite.setTexture(textures["mine"]);
					_board[y][x]->_debugSprite.setPosition((float)(x * 32), (float)(y * 32));
					window.draw(_board[y][x]->_debugSprite);
				}

				// --------------- WIN/LOSS TEXTURES -------------- //

				if (LoseCondition()) {
					sf::Sprite lose(textures["face_lose"]);
					lose.setPosition((float)((_numCols / 2.0) * 32 - 32.0), (float)(_numRows * 32));
					window.draw(lose);
				}
				if (WinCondition()) {

					for (int y = 0; y < _numRows; y++)
						for (int x = 0; x < _numCols; x++)
							if (_board[y][x]->_mine && !_board[y][x]->_flag)
								Flag(y, x);

					sf::Sprite sunglasses(textures["face_win"]);
					sunglasses.setPosition((float)((_numCols / 2.0) * 32 - 32.0), (float)(_numRows * 32));
					window.draw(sunglasses);
				}
			}

		// -------------------------------------- EVENTS ----------------------------------------- //

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (mouse.isButtonPressed)
				{
					// ------------ BUTTON CLICKS ---------- //

					mousePos = mouse.getPosition(window);
					int yPos = mouse.getPosition(window).y / 32;
					int xPos = mouse.getPosition(window).x / 32;

					if (!LoseCondition() && !WinCondition()) {
						if (xPos < _board[0].size() && yPos < _board.size()) {

							if (mouse.isButtonPressed(sf::Mouse::Left) && mouse.isButtonPressed(sf::Mouse::Right)) {
								_board[yPos][xPos]->Chord();
								if (LoseCondition())
									ShowMines();
							}

							else if (mouse.isButtonPressed(sf::Mouse::Left)) {
								if (!_board[yPos][xPos]->_flag) {
									_board[yPos][xPos]->Reveal();
									if (LoseCondition())
										ShowMines();
								}
							}
							else if (mouse.isButtonPressed(sf::Mouse::Right)) {
								Flag(yPos, xPos);
							}
						}
					}

					// ------------- MENU BUTTONS -------------- //

					int lowerBoundx = 0;
					int upperBoundx = 0;

					if (mouse.isButtonPressed(sf::Mouse::Left))
					{
						lowerBoundx = (float)((_numCols / 2.0) * 32 - 32);
						upperBoundx = (float)((_numCols / 2.0) * 32 + 32);

						if (_numRows <= mousePos.y / 32 && mousePos.y / 32 <= _numRows + 1) {

							// --------- SMILEY FACE BUTTON --------- //

							if (lowerBoundx <= mousePos.x && mousePos.x <= upperBoundx) {
								Reset();
								InfoFromConfig("boards/config.cfg");
								Add();
							}
							// ------------ DEBUG BUTTON ------------ //

							else if (lowerBoundx + 128 <= mousePos.x && mousePos.x <= upperBoundx + 128) {
								if (!LoseCondition() && !WinCondition()) {
									DebugBoard();
									if (_debugBoard)
									_debugBoard = false;
									else
									_debugBoard = true;
								}
							}

							// ------------ TEST BUTTON 1 ------------ //

							else if (lowerBoundx + 192 <= mousePos.x && mousePos.x <= upperBoundx + 192) {
								Reset();
								TilesFromBoard("boards/testboard1.brd");
							}
							// ------------ TEST BUTTON 2 ------------ //

							else if (lowerBoundx + 256 <= mousePos.x && mousePos.x <= upperBoundx + 256) {
								Reset();
								TilesFromBoard("boards/testboard2.brd");
							}
							// ------------ TEST BUTTON 3 ------------ //

							else if (lowerBoundx + 320 <= mousePos.x && mousePos.x <= upperBoundx + 320) {
								Reset();
								TilesFromBoard("boards/testboard3.brd");
							}
						}
					}
				}
			}
		}
		window.display();
	}
}

// ---------------------- PRINT DEBUGGING --------------------- //

void Board::Display()
{
	for (int y = 0; y < _board.size(); y++) {
		for (int x = 0; x < _board[y].size(); x++)
			std::cout << _board[y][x]->_mine << " ";
		std::cout << endl;
	}
}