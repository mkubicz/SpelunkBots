#pragma once
#include "stdafx.h"
#include "IBot.h"

class ObjectManager
{
public:
	ObjectManager(IBot *bot);

	std::vector<collectableObject> GetCollectables();
	std::vector<enemyObject> GetEnemies();
	void UpdateGameObjectLists();
	
	void CollectablesDebug();
	void EnemiesDebug();

	/*
	void ResetEnemies();
	void NodeContainsEnemy(double x, double y, double type, double id);
	void UpdateEnemyAtNode(double x, double y, double id);
	void RemoveEnemyWithID(double id);
	*/
private:
	IBot* _bot;
	std::vector<collectableObject> collectablesList;
	std::vector<enemyObject> enemiesList;




};