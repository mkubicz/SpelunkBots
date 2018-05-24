#pragma once

class Item
{

private:
	double _x;
	double _y;
	int _id;
	ItemType _type;
	ItemKind _kind;

	void SetKind(ItemType type);

public:
	Item(double x, double y, ItemType type, int id);
	Item(double x, double y, ItemType type);

	double GetX();
	double GetY();
	int GetID();
	ItemType GetType();
	ItemKind GetKind();

};
