#include "Tile.h"
// ------------------------- TILE --------------------------- //

Tile::Tile() = default;
Tile::~Tile() = default;

void Tile::Default()
{
	bool _mine = false;
	bool _flag = false;
	bool _revealed = false;
	bool _safe = false;
	_adjacentTiles.clear();
}
void Tile::AddNum()
{
	if (!_mine)
	{
		int adjMines = 0;
		for (int i = 0; i < _adjacentTiles.size(); i++)
			if (_adjacentTiles[i]->_mine)
				adjMines++;
		if (adjMines > 0) {
			_textureName = "number_";
			_textureName.append(to_string(adjMines));
		}
		else
			_safe = true;
	}
}
void Tile::AddMine()
{
	_mine = true;
	_textureName = "mine";
}
void Tile::Debug()
{
	if (_debug)
		_debug = false;
	else 
		_debug = true;
}
void Tile::Reveal()
{
	_revealed = true;
	if (_safe) {
		for (int i = 0; i < _adjacentTiles.size(); i++)
			if ((!_adjacentTiles[i]->_revealed) && (!_adjacentTiles[i]->_flag))
				_adjacentTiles[i]->Reveal();
	}
}
void Tile::Chord()
{
	int adjFlags = 0;
	int adjMines = 0;
	if (_revealed) {
		for (int i = 0; i < _adjacentTiles.size(); i++)
		{
			if (_adjacentTiles[i]->_mine)
				adjMines++;
			if (_adjacentTiles[i]->_flag)
				adjFlags++;
		}
		if (adjMines == adjFlags && adjFlags != 0)
			for (int i = 0; i < _adjacentTiles.size(); i++)
				if (!_adjacentTiles[i]->_flag)
					_adjacentTiles[i]->Reveal();
	}
}