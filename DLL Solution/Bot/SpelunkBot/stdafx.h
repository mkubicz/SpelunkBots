// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS 

#include <windows.h>
#include <iostream>
#include <queue>
//#include <deque>
#include <string>
#include "utilities.h"
//#include "Node.h"
#include <time.h>

#pragma region Constants and structs

const int NODE_COORDS = 0;
const int PIXEL_COORDS = 1;

const int NORUN = 0;
const int RUN = 1;

const int Y_NODES = 34;
const int X_NODES = 42;

const int PIXELS_IN_NODE = 16;

struct collectableObject {
	int type;
	int id;
	double x;
	double y;
};

struct enemyObject {
	int type;
	int id;
	double x;
	double y;
	double directionFacing;
	double status;
};

struct moveTarget {
	int x;
	int y;
	bool exploration = false;
	moveTarget(int x, int y, bool exploration)
		: x(x), y(y), exploration(exploration) {};
	moveTarget(int x, int y)
		: moveTarget(x, y, false) {};
	moveTarget()
		: moveTarget(0, 0) {};
	moveTarget(collectableObject c)
		: moveTarget(c.x, c.y) {};
};

#pragma endregion

#pragma region Enums

enum DIRECTIONX {
	xLEFT = -1,
	xNONE = 0,
	xRIGHT = 1
};

enum DIRECTIONY {
	yUP = -1,
	yNONE = 0,
	yDOWN = 1
};

enum ACTION_TARGET {
	GROUND,
	LADDER,
	LADDERTOP,
	LEDGE
};

enum MOVEMENTACTION
{
	IDLE,
	CENTRALIZE,
	WALK,
	HANG,
	DROP,
	HANGDROP,
	CLIMBFROMHANG,
	CLIMB,
	JUMPABOVERIGHT, //jumpabove is split to left/right because its target is always right above spelunkers head, so we cant infer the direction from it
	JUMPABOVELEFT,
	JUMP,
	JUMPFROMLADDER,
	WALKOFFLEDGE,
};

enum SpState
{
	spSTANDING = 10,
	spRUNNING = 11,
	spDUCKING = 12,
	spLOOKING_UP = 13,
	spCLIMBING = 14,
	spJUMPING = 15,
	spFALLING = 16,
	spDYING = 17,
	spLEFT = 18,
	spRIGHT = 19,
	spON_GROUND = 20,
	spIN_AIR = 21,
	spON_LADDER = 22,
	spHANGING = 23,
	spDUCKTOHANG = 24
};

enum MVSTATE
{
	mvSTANDING,
	mvCLIMBING,
	mvHANGING,
	mvCLIMBING_WITH_MOMENTUM
};

enum TerrainType
{
	spEmptyNode = 0,
	spStandardTerrain,
	spLadder,
	spLadderTop,
	spExit,
	spEntrance,
	spArrowTrapRight,
	spArrowTrapLeft,
	spIsInShop,
	spIce,
	spSpike,
	spSpearTrap,
	spGameEntrance,
	spTree,
	spTreeBranchLeaf,
	spSpringTrap,
	spSacAltar
};

enum ItemKind
{
	spJunkLight,
	spJunkHeavy,
	spTreasure,
	spWeapon,
	spEquipable,
	spOther,
	spNotInGame
};



enum ItemType
{
	spGoldBar = 1,
	spGoldBars,
	spEmerald,
	spEmeraldBig,
	spSapphire,
	spSapphireBig,
	spRuby,
	spRubyBig,
	spDiamond,
	spGoldNugget,
	spGoldChunk,

	spChest,
	spLockedChest,
	spKey,
	spCrate,
	spFlareCrate,
	spBombBag,
	spBombBox,
	spPaste,
	spRopePile,
	spParachute,
	spCompass,
	spSpringShoes,
	spSpikeShoes,
	spJordans,
	spSpecs,
	spUdjat,
	spCrown,

	spKapala,
	spAnkh,
	spGloves,
	spMitt,
	spJetpack,
	spCape,
	spRopeBag,

	// TODO IMPLEMENT THESE
	spRope,
	spBomb,
	spFireFrogBomb,
	spRock,
	spDice,

	spMattockHead,
	spBasketball,
	spShotgun,
	spPistol,
	spWebCannon,
	spTeleporter,
	spBow,
	spMachete,
	spSceptre,
	spMattock,
	spJar,
	spSkull,
	spArrow,
	spFishBone,
	spFlare,
	spGoldIdol,
	spCrystalSkull,
	spLampItem,

	spDamsel,
	spBlock,
	spPushBlock,
	spDarkFall
};

enum Threats
{
	spGhost = 1,
	spBat,
	spScarab,
	spSpider,
	spGiantSpiderHang,
	spGiantSpider,
	spFrog,
	spFireFrog,
	spZombie,
	spVampire,
	spPiranha,
	spJaws,
	spDeadFish,
	spManTrap,
	spMonkey,
	spYeti,
	spYetiKing,
	spUFO,
	spUFOCrash,
	spAlienEject,
	spAlien,
	spAlienBoss,
	spBarrierEmitter,
	spBarrier,
	spCaveman,
	spHawkman,
	spMagma,
	spMagmaTrail,
	spMagmaMan,
	spTombLord,
	spOlmec,
	spCavemanWorship,
	spHawkmanWorship,
	spOlmecDebris,
	spSnake,
	spSpiderHang,
	//spMagmaMan,
	spShopKeeper = 38,

	spBones = 60,
	spSmashTrap,
	spCeilingTrap,
	spBoulder
	//spSpringTrap = 99
};

#pragma endregion

#pragma region String maps

const std::string ItemTypeStrings[] =
{
	"nothing",
	"GoldBar",
	"GoldBars",
	"Emerald",
	"EmeraldBig",
	"Sapphire",
	"SapphireBig",
	"Ruby",
	"RubyBig",
	"Diamond",
	"GoldNugget",
	"GoldChunk",
	"Chest",
	"LockedChest",
	"Key",
	"Crate",
	"FlareCrate",
	"BombBag",
	"BombBox",
	"Paste",
	"RopePile",
	"Parachute",
	"Compass",
	"SpringShoes",
	"SpikeShoes",
	"Jordans",
	"Specs",
	"Udjat",
	"Crown",
	"Kapala",
	"Ankh",
	"Gloves",
	"Mitt",
	"Jetpack",
	"Cape",
	"RopeBag",
	"Rope",
	"Bomb",
	"FireFrogBomb",
	"Rock",
	"Dice",
	"MattockHead",
	"Basketball",
	"Shotgun",
	"Pistol",
	"WebCannon",
	"Teleporter",
	"Bow",
	"Machete",
	"Sceptre",
	"Mattock",
	"Jar",
	"Skull",
	"Arrow",
	"FishBone",
	"Flare",
	"GoldIdol",
	"CrystalSkull",
	"LampItem",
	"Damsel",
	"Block",
	"PushBlock",
	"DarkFall"
};

const std::string ItemKindStrings[] =
{
	"JunkLight",
	"JunkHeavy",
	"Treasure",
	"Weapon",
	"Equipable",
	"Other",
	"NotInGame"
};

const std::string ActionTargetStrings[] =
{
	"GROUND",
	"LADDER",
	"LADDERTOP",
	"LEDGE"
};


const std::string MVactionStrings[] =
{
	"IDLE",
	"CENTRALIZE",
	"WALK",
	"HANG",
	"DROP",
	"HANGDROP",
	"CLIMBFROMHANG",
	"CLIMB",
	"JUMPABOVERIGHT",
	"JUMPABOVELEFT",
	"JUMP",
	"JUMPFROMLADDER",
	"WALKOFFLEDGE"
};

const std::string BotLogicStrings[] =
{
	"IDLE",
	"EXIT",
	"WAITING",
	"GATHER_FROM_CC",
	"EXPLORE_CC",
	"PICK_TARGET_IN_NEXT_CC",
	"MOVE_TO_NEXT_CC",
	"SEARCH_FOR_EXIT",
	"GO_TO_EXIT",
	"EXIT_REACHED",
	"UNREACHABLE_EXIT",
	"NO_EXIT_EXPLORE",
	"NO_EXIT_ERROR"

};

#pragma endregion