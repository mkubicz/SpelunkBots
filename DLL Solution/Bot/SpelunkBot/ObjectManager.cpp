#include "stdafx.h"
#include "ObjectManager.h"

ObjectManager::ObjectManager(IBot *bot)
{
	_bot = bot;
}

std::vector<collectableObject> ObjectManager::GetVisibleCollectables()
{
	return collectablesList;
}

std::vector<enemyObject> ObjectManager::GetVisibleEnemies()
{
	return enemiesList;
}

void ObjectManager::UpdateGameObjectLists()
{
	int i = 0;
	enemiesList.clear();
	collectableObject* e = _bot->GetIthEnemy(i);

	while (e != NULL)
	{
		//top left corner of enemy is on the list, so we make it a center by adding 0.5 to x and y coordinates
		//(most enemies are roughly 1 node wide and long)
		enemyObject enemy = enemyObject{ e->type, e->id, e->x+0.5, e->y+0.5 };
		enemiesList.push_back(enemy);
		
		i++;
		e = _bot->GetIthEnemy(i);
		
	}
	
	i = 0;
	collectablesList.clear();
	collectableObject* c = _bot->GetIthCollectable(i);

	while (c != NULL)
	{
		collectableObject collectable = collectableObject{ c->type, c->id, c->x, c->y };
		collectablesList.push_back(collectable);

		i++;
		c = _bot->GetIthCollectable(i);
	}
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
