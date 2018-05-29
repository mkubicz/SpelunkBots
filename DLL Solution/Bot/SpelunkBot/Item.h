#pragma once
#include "Coords.h"

class Item
{

private:
	Coords _coords;
	int _id;
	ItemType _type;
	ItemKind _kind;

	void SetKind(ItemType type);

public:
	Item(double x, double y, ItemType type, int id);
	Item(double x, double y, ItemType type);

	double GetX();
	double GetY();
	Coords GetCoords();
	int GetID();
	ItemType GetType();
	ItemKind GetKind();

};
