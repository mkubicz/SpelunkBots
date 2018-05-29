#include "stdafx.h"
#include "ObjectManager.h"

#include <fstream>
//#include <ostream>

using namespace std;

ObjectManager::ObjectManager(IBot *bot, Pathfinder *pathfinder)
{
	_bot = bot;
	_pathfinder = pathfinder;
}

std::vector<Item> ObjectManager::GetItems()
{
	return _itemsList;
}

std::vector<Item> ObjectManager::GetItems(ItemKind kind)
{
	std::vector<Item> items;
	
	for (int i = 0; i < _itemsList.size(); i++)
		if (_itemsList[i].GetKind() == kind)
			items.push_back(_itemsList[i]);

	return items;
}

std::vector<Item> ObjectManager::GetItems(ItemType type)
{
	std::vector<Item> items;

	for (int i = 0; i < _itemsList.size(); i++)
		if (_itemsList[i].GetType() == type)
			items.push_back(_itemsList[i]);

	return items;
}

std::vector<Item> ObjectManager::GetItems(int ccnr)
{
	std::vector<Item> items;

	for (int i = 0; i < _itemsList.size(); i++)
		if (_pathfinder->GetCCnr(_itemsList[i].GetCoords()) == ccnr)
			items.push_back(_itemsList[i]);

	return items;
}

std::vector<Item> ObjectManager::GetItems(ItemKind kind, int ccnr)
{
	std::vector<Item> items;

	for (int i = 0; i < _itemsList.size(); i++)
		if (_pathfinder->GetCCnr(_itemsList[i].GetCoords()) == ccnr && _itemsList[i].GetKind() == kind)
			items.push_back(_itemsList[i]);

	return items;
}


std::vector<Item> ObjectManager::GetItems(ItemType type, int ccnr)
{
	std::vector<Item> items;

	for (int i = 0; i < _itemsList.size(); i++)
		if (_pathfinder->GetCCnr(_itemsList[i].GetCoords()) == ccnr && _itemsList[i].GetType() == type)
			items.push_back(_itemsList[i]);

	return items;
}


Item * ObjectManager::GetItemByID(int id)
{
	for (Item item : _itemsList)
		if (item.GetID() == id)
			return &item;

	return NULL;
}

std::vector<Item> ObjectManager::GetTreasures()
{
	std::vector<Item> treasures;

	for (Item item : _itemsList)
		if (item.GetKind() == spTreasure)
			treasures.push_back(item);

	return treasures;
}

std::vector<enemyObject> ObjectManager::GetEnemies()
{
	return _enemiesList;
}

void ObjectManager::UpdateGameObjectLists()
{
	_enemiesList.clear();
	enemyObject* e = _bot->GetNextEnemy();
	while (e != NULL)
	{
		//top left corner of enemy is on the list, so we make it a center by adding 0.5 to x and y coordinates
		//(most enemies are roughly 1 node wide and long)
		enemyObject enemy = enemyObject{ e->type, e->id, e->x + 0.5, e->y + 0.5 };
		_enemiesList.push_back(enemy);

		e = _bot->GetNextEnemy();
	}
	

	_itemsList.clear();
	collectableObject* c = _bot->GetNextCollectable();
	while (c != NULL)
	{
		//collectableObject collectable = collectableObject{ c->type, c->id, c->x, c->y };
		Item item = Item(c->x, c->y, (ItemType)c->type, c->id);
		_itemsList.push_back(item);

		c = _bot->GetNextCollectable();
	}
}

void ObjectManager::ItemsDebug()
{
	ofstream fileStream;
	fileStream.open(".\\ObjectManager\\collectables.txt");

	fileStream << "ObjectManager Items: " << endl;

	for (int i = 0; i < _itemsList.size(); i++)
	{
		fileStream << "X: " << _itemsList[i].GetX();
		fileStream << " Y: " << _itemsList[i].GetY();
		fileStream << " TYPE: " << ItemTypeStrings[_itemsList[i].GetType()];
		fileStream << " KIND: " << ItemKindStrings[_itemsList[i].GetKind()];
		fileStream << " ID: " << _itemsList[i].GetID();
		fileStream << endl;
	}

	fileStream.close();
}

void ObjectManager::EnemiesDebug()
{
	ofstream fileStream;
	fileStream.open(".\\ObjectManager\\enemies.txt");

	fileStream << "ObjectManager Enemies: " << endl;

	for (int i = 0; i < _enemiesList.size(); i++)
	{
		fileStream << "TYPE: " << _enemiesList[i].type;
		fileStream << " X: " << _enemiesList[i].x;
		fileStream << " Y: " << _enemiesList[i].y;
		fileStream << " ID: " << _enemiesList[i].id;
		fileStream << endl;
	}

	fileStream.close();
}



//void ObjectManager::ResetEnemies()
//{
//	_enemiesList.clear();
//}
//
//void ObjectManager::NodeContainsEnemy(double x, double y, double type, double id)
//{
//	enemyObject object;
//	object.x = x;
//	object.y = y;
//	object.type = type;
//	object.id = id;
//	_enemiesList.push_back(object);
//}
//
//void ObjectManager::UpdateEnemyAtNode(double x, double y, double id)
//{
//	int cSize = _enemiesList.size();
//	for (int i = 0; i < cSize; i++)
//	{
//		if (_enemiesList.at(i).id == id)
//		{
//			_enemiesList.at(i).x = x;
//			_enemiesList.at(i).y = y;
//			return;
//		}
//	}
//}
//
//void ObjectManager::RemoveEnemyWithID(double id)
//{
//	int cSize = _enemiesList.size();
//	for (int i = 0; i < cSize; i++)
//	{
//		if (_enemiesList.at(i).id == id)
//		{
//			_enemiesList.erase(_enemiesList.begin() + i);
//			return;
//		}
//	}
//}
