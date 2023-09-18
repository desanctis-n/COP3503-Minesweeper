#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

struct Tile
{
	bool _mine = false;
	bool _flag = false;

	bool _revealed = false;
	bool _clicked = false;

	bool _safe = false;
	bool _debug = false;

	vector<Tile*> _adjacentTiles;

	sf::Sprite _tileSprite;
	sf::Sprite _blankSprite;
	sf::Sprite _hiddenSprite;
	sf::Sprite _flagSprite;
	sf::Sprite _debugSprite;

	string _textureName = "tile_revealed";

	Tile();
	~Tile();

	void Default();
	void AddNum();
	void AddMine();
	void Debug();
	void Reveal();

	void Chord();
};

