#include "stdafx.h"
#include "ObjectManager.h"

#include <fstream>
//#include <ostream>

using namespace std;

ObjectManager::ObjectManager(IBot *bot)
{
	_bot = bot;
}

std::vector<collectableObject> ObjectManager::GetCollectables()
{
	return collectablesList;
}

std::vector<enemyObject> ObjectManager::GetEnemies()
{
	return enemiesList;
}

void ObjectManager::UpdateGameObjectLists()
{
	enemiesList.clear();
	enemyObject* e = _bot->GetNextEnemy();
	while (e != NULL)
	{
		//top left corner of enemy is on the list, so we make it a center by adding 0.5 to x and y coordinates
		//(most enemies are roughly 1 node wide and long)
		enemyObject enemy = enemyObject{ e->type, e->id, e->x + 0.5, e->y + 0.5 };
		enemiesList.push_back(enemy);

		e = _bot->GetNextEnemy();
	}
	

	collectablesList.clear();
	collectableObject* c = _bot->GetNextCollectable();
	while (c != NULL)
	{
		collectableObject collectable = collectableObject{ c->type, c->id, c->x, c->y };
		collectablesList.push_back(collectable);

		c = _bot->GetNextCollectable();
	}
}

void ObjectManager::CollectablesDebug()
{
	ofstream fileStream;
	fileStream.open(".\\ObjectManager\\collectables.txt");

	fileStream << "ObjectManager Collectables: " << endl;

	for (int i = 0; i < collectablesList.size(); i++)
	{
		fileStream << "TYPE: " << collectablesList[i].type;
		fileStream << " X: " << collectablesList[i].x;
		fileStream << " Y: " << collectablesList[i].y;
		fileStream << " ID: " << collectablesList[i].id;
		fileStream << endl;
	}

	fileStream.close();
}

void ObjectManager::EnemiesDebug()
{
	ofstream fileStream;
	fileStream.open(".\\ObjectManager\\enemies.txt");

	fileStream << "ObjectManager Enemies: " << endl;

	for (int i = 0; i < enemiesList.size(); i++)
	{
		fileStream << "TYPE: " << enemiesList[i].type;
		fileStream << " X: " << enemiesList[i].x;
		fileStream << " Y: " << enemiesList[i].y;
		fileStream << " ID: " << enemiesList[i].id;
		fileStream << endl;
	}

	fileStream.close();
}

//void ObjectManager::ResetEnemies()
//{
//	enemiesList.clear();
//}
//
//void ObjectManager::NodeContainsEnemy(double x, double y, double type, double id)
//{
//	enemyObject object;
//	object.x = x;
//	object.y = y;
//	object.type = type;
//	object.id = id;
//	enemiesList.push_back(object);
//}
//
//void ObjectManager::UpdateEnemyAtNode(double x, double y, double id)
//{
//	int cSize = enemiesList.size();
//	for (int i = 0; i < cSize; i++)
//	{
//		if (enemiesList.at(i).id == id)
//		{
//			enemiesList.at(i).x = x;
//			enemiesList.at(i).y = y;
//			return;
//		}
//	}
//}
//
//void ObjectManager::RemoveEnemyWithID(double id)
//{
//	int cSize = enemiesList.size();
//	for (int i = 0; i < cSize; i++)
//	{
//		if (enemiesList.at(i).id == id)
//		{
//			enemiesList.erase(enemiesList.begin() + i);
//			return;
//		}
//	}
//}
