#pragma once
#include "Bot.h"

#pragma region Typedefs

typedef bool(__cdecl *IsClearPathToExitPROC)();
typedef double(__cdecl *UpdatePlayerVariablesPROC)(char *name, char *value, double type);
typedef double(__cdecl *DisplayPlayerVariablesPROC)();
typedef double(__cdecl *SetScreenXYWHPROC)(double x, double y, double w, double h);
typedef double(__cdecl *SetCoolGlassesPROC)(double d);
typedef double(__cdecl *SetUdjatEyePROC)(double d);
typedef double(__cdecl *ResetFogForNewLevelPROC)();
typedef double(__cdecl *TerrainWasDestroyedPROC)(double x, double y);
typedef double(__cdecl *SpiderWebCreatedPROC)(double x, double y);
typedef double(__cdecl *SpiderWebWasRemovedPROC)(double x, double y);
typedef double(__cdecl *DestroyFogPROC)(double x, double y);
typedef double(__cdecl *TerrainIsLadderPROC)(double x, double y);
typedef double(__cdecl *TerrainIsGoalPROC)(double x, double y);
typedef double(__cdecl *TerrainIsStartPROC)(double x, double y);
typedef double(__cdecl *TerrainIsAltarPROC)(double x, double y);
typedef double(__cdecl *TerrainIsArrowTrapRightPROC)(double x, double y);
typedef double(__cdecl *TerrainIsArrowTrapLeftPROC)(double x, double y);
typedef double(__cdecl *TerrainIsShopKeeperAreaPROC)(double x, double y);
typedef double(__cdecl *TerrainIsIcePROC)(double x, double y);
typedef double(__cdecl *TerrainIsSpikePROC)(double x, double y);
typedef double(__cdecl *TerrainIsSpearTrapPROC)(double x, double y);
typedef double(__cdecl *TerrainIsSwimWaterPROC)(double x, double y);
typedef double(__cdecl *TerrainIsLavaPROC)(double x, double y);
typedef double(__cdecl *TerrainIsGameEntrancePROC)(double x, double y);
typedef double(__cdecl *TerrainIsTreePROC)(double x, double y);
typedef double(__cdecl *TerrainIsTreeBranchLeafPROC)(double x, double y);
typedef double(__cdecl *TerrainIsEmptyPROC)(double x, double y);
typedef double(__cdecl *GetLiquidValuePROC)(double x, double y);
typedef double(__cdecl *SpringPadAtPositionPROC)(double x, double y);
typedef double(__cdecl *FillShopkeeperAreaPROC)(double x, double y);
typedef double(__cdecl *SetMapCoordPROC)(double x, double y, double state);
typedef double(__cdecl *ClearFogFromSquarePROC)(double x, double y);
typedef double(__cdecl *GetNodeStatePROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *GetFogStatePROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *ClearDynamicObjectsPROC)();
typedef double(__cdecl *NodeContainsPushBlockPROC)(double x, double y);
typedef double(__cdecl *GetNodeContainsPushBlockPROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *NodeContainsBatPROC)(double x, double y);
typedef double(__cdecl *GetNodeContainsBatPROC)(double x, double y, double usingPixelCOords);
typedef double(__cdecl *ResetCollectablesPROC)();
typedef double(__cdecl *NodeContainsCollectablePROC)(double x, double y, double type, double id);
typedef double(__cdecl *UpdateCollectableAtNodePROC)(double x, double y, double id);
typedef double(__cdecl *RemoveCollectableWithIDPROC)(double id);
typedef double(__cdecl *NumberOfCollectableTypeInNodePROC)(double type, double x, double y, double usingPixelCoords);
typedef double(__cdecl *ResetEnemiesPROC)();
typedef double(__cdecl *NodeContainsEnemyPROC)(double x, double y, double type, double id);
typedef double(__cdecl *UpdateEnemyAtNodePROC)(double x, double y, double id);
typedef double(__cdecl *RemoveEnemyWithIDPROC)(double id);
typedef double(__cdecl *NumberOfWebsInNodePROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *NumberOfEnemyTypeInNodePROC)(double type, double x, double y, double usingPixelCoords);
typedef double(__cdecl *GetIDOfEnemyInNodePROC)(double type, double x, double y, double usingPixelCoords);
typedef double(__cdecl *GetIDOfCollectableInNodePROC)(double type, double x, double y, double usingPixelCoords);
typedef double(__cdecl *SaveDynamicObjectFilesDebugPROC)();
typedef double(__cdecl *SaveSpiderwebsToFilePROC)();
typedef double(__cdecl *SaveLevelLayoutToFilePROC)();
typedef double(__cdecl *CalculatePathFromXYtoXYPROC)(double x1, double y1, double x2, double y2, double usingPixelCoords);
typedef double(__cdecl *GetNextPathXPosPROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *GetNextPathYPosPROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *IsEnemyInNodePROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *IsCollectableInNodePROC)(double x, double y, double usingPixelCoords);
typedef double(__cdecl *IsNodePassablePROC)(double x, double y, double usingPixelCoords);
typedef bool(__cdecl *IsArrowTrapDisarmedPROC)(int x, int y);
typedef int(__cdecl *GetHeldItemIDPROC)();
typedef bool(__cdecl *IsHoldingItemPROC)();
typedef int(__cdecl *GetSpearTrapStatePROC)(int x, int y);

typedef double(__cdecl *DisplayLevelLayoutPROC)();
typedef bool(__cdecl *IsFacingLeftPROC)();
typedef bool(__cdecl *IsFacingRightPROC)();
typedef int(__cdecl *GetSpelunkerStatePROC)();
typedef int(__cdecl *GetHitPointsPROC)();
typedef int(__cdecl *GetTimePROC)();
typedef int(__cdecl *GetMoneyPROC)();
typedef int(__cdecl *GetRopesPROC)();
typedef int(__cdecl *GetBombsPROC)();

typedef collectableObject*(__cdecl *GetNextCollectablePROC)();
typedef enemyObject*(__cdecl *GetNextEnemyPROC)();
//typedef collectableObject(__cdecl *GetVisibleEnemiesPROC)();

#pragma endregion

class IBot
{
private:
	HMODULE spelunkbots_hModule;

	void InitialiseDLLFunctions(void);
	GetSpelunkerStatePROC GetSpelunkerStateDLL;


protected:
	std::shared_ptr<IBot> _bot;

	double _pathCount;
	double _tempID;
	double _waitTimer;
	double _targetX;
	double _targetY;
	double _playerPositionX;
	double _playerPositionY;
	double _playerPositionXNode;
	double _playerPositionYNode;
	bool _hasGoal;
	bool _spIsInAir;
	bool _spIsJetpacking;
	bool _isHanging;
	bool _itemGoal;
	bool _fogGoal;
	bool _endGoal;
	bool _headingRight;
	bool _headingLeft;
	bool _goRight;
	bool _goLeft;
	bool _jump;
	bool _attack;
	bool _lookUp;
	bool _ropep;
	bool _bombp;
	bool _runp;
	bool _payp;
	bool _itemp;
	bool _duck;
	bool _holdingItem;
	bool _hanging;

	bool _rightPressed;
	bool _rightReleased;
	bool _leftPressed;
	bool _leftReleased;

public:
	IBot();
	~IBot();

	UpdatePlayerVariablesPROC UpdatePlayerVariables;
	DisplayPlayerVariablesPROC DisplayPlayerVariables;
	SetScreenXYWHPROC SetScreenXYWH;
	SetCoolGlassesPROC SetCoolGlasses;
	SetUdjatEyePROC SetUdjatEye;
	ResetFogForNewLevelPROC ResetFogForNewLevel;
	TerrainWasDestroyedPROC TerrainWasDestroyed;
	SpiderWebCreatedPROC SpiderWebCreated;
	SpiderWebWasRemovedPROC SpiderWebWasRemoved;
	DestroyFogPROC DestroyFog;
	TerrainIsLadderPROC TerrainIsLadder;
	TerrainIsGoalPROC TerrainIsGoal;
	TerrainIsStartPROC TerrainIsStart;
	TerrainIsAltarPROC TerrainIsAltar;
	TerrainIsArrowTrapRightPROC TerrainIsArrowTrapRight;
	TerrainIsArrowTrapLeftPROC TerrainIsArrowTrapLeft;
	TerrainIsShopKeeperAreaPROC TerrainIsShopKeeperArea;
	TerrainIsIcePROC TerrainIsIce;
	TerrainIsSpikePROC TerrainIsSpike;
	TerrainIsSpearTrapPROC TerrainIsSpearTrap;
	TerrainIsSwimWaterPROC TerrainIsSwimWater;
	TerrainIsLavaPROC TerrainIsLava;
	TerrainIsGameEntrancePROC TerrainIsGameEntrance;
	TerrainIsTreePROC TerrainIsTree;
	TerrainIsTreeBranchLeafPROC TerrainIsTreeBranchLeaf;
	TerrainIsEmptyPROC TerrainIsEmpty;
	GetLiquidValuePROC GetLiquidValue;
	SpringPadAtPositionPROC SpringPadAtPosition;
	FillShopkeeperAreaPROC FillShopkeeperArea;
	SetMapCoordPROC SetMapCoord;
	ClearFogFromSquarePROC ClearFogFromSquare;
	GetNodeStatePROC GetNodeState;
	GetFogStatePROC GetFogState;
	IsClearPathToExitPROC IsClearPathToExit;
	ClearDynamicObjectsPROC ClearDynamicObjects;
	NodeContainsPushBlockPROC NodeContainsPushBlock;
	GetNodeContainsPushBlockPROC GetNodeContainsPushBlock;
	NodeContainsBatPROC NodeContainsBat;
	GetNodeContainsBatPROC GetNodeContainsBat;
	ResetCollectablesPROC ResetCollectables;
	NodeContainsCollectablePROC NodeContainsCollectable;
	UpdateCollectableAtNodePROC UpdateCollectableAtNode;
	RemoveCollectableWithIDPROC RemoveCollectableWithID;
	NumberOfCollectableTypeInNodePROC NumberOfCollectableTypeInNode;
	ResetEnemiesPROC ResetEnemies;
	NodeContainsEnemyPROC NodeContainsEnemy;
	UpdateEnemyAtNodePROC UpdateEnemyAtNode;
	RemoveEnemyWithIDPROC RemoveEnemyWithID;
	NumberOfWebsInNodePROC NumberOfWebsInNode;
	NumberOfEnemyTypeInNodePROC NumberOfEnemyTypeInNode;
	GetIDOfEnemyInNodePROC GetIDOfEnemyInNode;
	GetIDOfCollectableInNodePROC GetIDOfCollectableInNode;
	SaveDynamicObjectFilesDebugPROC SaveDynamicObjectFilesDebug;
	SaveSpiderwebsToFilePROC SaveSpiderwebsToFile;
	SaveLevelLayoutToFilePROC SaveLevelLayoutToFile;
	CalculatePathFromXYtoXYPROC CalculatePathFromXYtoXY;
	GetNextPathXPosPROC GetNextPathXPos;
	GetNextPathYPosPROC GetNextPathYPos;
	IsEnemyInNodePROC IsEnemyInNode;
	IsCollectableInNodePROC IsCollectableInNode;
	IsNodePassablePROC IsNodePassable;
	IsArrowTrapDisarmedPROC IsArrowTrapDisarmed;
	GetHeldItemIDPROC GetHeldItemID;
	IsHoldingItemPROC IsHoldingItem;
	GetSpearTrapStatePROC GetSpearTrapState;

	DisplayLevelLayoutPROC DisplayLevelLayout;
	IsFacingLeftPROC IsFacingLeft;
	IsFacingRightPROC IsFacingRight;
	GetHitPointsPROC GetHitPoints;
	GetTimePROC GetTime;
	GetMoneyPROC GetMoney;
	GetBombsPROC GetBombs;
	GetRopesPROC GetRopes;

	GetNextCollectablePROC GetNextCollectable;
	GetNextEnemyPROC GetNextEnemy;
	//GetVisibleEnemiesPROC GetVisibleEnemies;

	inline double GetHasGoal(void)
	{
		return ConvertBoolToDouble(_hasGoal);
	}
	inline double GetHoldingItem(void)
	{
		return ConvertBoolToDouble(_holdingItem);
	}
	inline double GetIsInAir(void)
	{
		return ConvertBoolToDouble(_spIsInAir);
	}
	inline double GetIsJetpacking(void)
	{
		return ConvertBoolToDouble(_spIsJetpacking);
	}
	inline double GetIsHanging(void)
	{
		return ConvertBoolToDouble(_isHanging);
	}
	inline double GetItemGoal(void)
	{
		return ConvertBoolToDouble(_itemGoal);
	}
	inline double GetFogGoal(void)
	{
		return ConvertBoolToDouble(_fogGoal);
	}
	inline double GetEndGoal(void)
	{
		return ConvertBoolToDouble(_endGoal);
	}
	inline double GetHeadingRight(void)
	{
		return ConvertBoolToDouble(_headingRight);
	}
	inline double GetHeadingLeft(void)
	{
		return ConvertBoolToDouble(_headingLeft);
	}
	inline double GetGoRight(void)
	{
		return ConvertBoolToDouble(_goRight);
	}
	inline double GetGoLeft(void)
	{
		return ConvertBoolToDouble(_goLeft);
	}
	inline double GetJump(void)
	{
		return ConvertBoolToDouble(_jump);
	}
	inline double GetAttack(void)
	{
		return ConvertBoolToDouble(_attack);
	}
	inline double GetLookUp(void)
	{
		return ConvertBoolToDouble(_lookUp);
	}
	inline double GetDuck(void)
	{
		return ConvertBoolToDouble(_duck);
	}
	inline double GetHanging(void)
	{
		return ConvertBoolToDouble(_hanging);
	}
	inline double GetRopep(void)
	{
		return ConvertBoolToDouble(_ropep);
	}
	inline double GetBombp(void)
	{
		return ConvertBoolToDouble(_bombp);
	}
	inline double GetRunp(void)
	{
		return ConvertBoolToDouble(_runp);
	}
	inline double GetPayp(void)
	{
		return ConvertBoolToDouble(_payp);
	}
	inline double GetItemp(void)
	{
		return ConvertBoolToDouble(_itemp);
	}
	inline double GetTargetX(void)
	{
		return _targetX;
	}
	inline double GetTargetY(void)
	{
		return _targetY;
	}
	inline double GetPathCount(void)
	{
		return _pathCount;
	}
	inline double GetTempID(void)
	{
		return _tempID;
	}
	inline double GetWaitTimer(void)
	{
		return _waitTimer;
	}
	inline double GetPlayerPositionX(void)
	{
		return _playerPositionX;
	}
	inline double GetPlayerPositionY(void)
	{
		return _playerPositionY;
	}
	inline double GetPlayerPositionXNode(void)
	{
		return _playerPositionXNode;
	}
	inline double GetPlayerPositionYNode(void)
	{
		return _playerPositionYNode;
	}

	inline double GetRightPressed(void)
	{
		return ConvertBoolToDouble(_rightPressed);
	}	
	inline double GetRightReleased(void)
	{
		return ConvertBoolToDouble(_rightReleased);
	}	
	inline double GetLeftPressed(void)
	{
		return ConvertBoolToDouble(_leftPressed);
	}	
	inline double GetLeftReleased(void)
	{
		return ConvertBoolToDouble(_leftReleased);
	}

	void SetOwnPointer(std::shared_ptr<IBot> ptr);
	virtual void CreateHelpers(){};

	virtual void Update() = 0;
	virtual void Reset();
	virtual void NewLevel(){};

	void UpdateBotPosition(double nodeX, double nodeY);

	// Used on initialising a bot and re-initialising upon entering a room
	void InitialiseVariables(void);

	SpState GetSpelunkerState();
};