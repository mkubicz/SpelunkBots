#pragma once
#include "stdafx.h"
#include "IBot.h"
#include "Item.h"

class ObjectManager
{
public:
	ObjectManager(IBot *bot);

	std::vector<Item> GetItems();
	Item * GetItemByID(int id);
	std::vector<Item> GetTreasures();

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
	IBot* _bot;
	//std::vector<collectableObject> _collectablesList;
	std::vector<Item> _itemsList;
	std::vector<enemyObject> _enemiesList;


};