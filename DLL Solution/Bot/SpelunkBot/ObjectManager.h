#pragma once
#include "stdafx.h"
#include "IBot.h"
#include "Item.h"
#include "Pathfinder.h"

class ObjectManager
{
public:
	ObjectManager(std::shared_ptr<IBot> bot, std::shared_ptr<Pathfinder> pathfinder);

	void NewLevel();

	Item * GetItemByID(int id);
	std::vector<Item> GetTreasures();
	std::vector<Item> GetItems(ItemKind kind, int ccnr);
	std::vector<Item> GetItems(ItemType type, int ccnr);
	std::vector<Item> GetItems(ItemKind kind);
	std::vector<Item> GetItems(ItemType type);
	std::vector<Item> GetItems(int ccnr);
	std::vector<Item> GetItems();

	std::vector<enemyObject> GetEnemies();


	void UpdateGameObjectLists();
	
	void ItemsDebug();
	void EnemiesDebug();

	/*
	void ResetEnemies();
	void NodeContainsEnemy(double x, double y, double type, double id);
	void UpdateEnemyAtNode(double x, double y, double id);
	void RemoveEnemyWithID(double id);
	*/
private:
	std::shared_ptr<IBot> _bot;
	std::shared_ptr<Pathfinder> _pathfinder;

	//std::vector<collectableObject> _collectablesList;
	std::vector<Item> _itemsList;
	std::vector<enemyObject> _enemiesList;



};