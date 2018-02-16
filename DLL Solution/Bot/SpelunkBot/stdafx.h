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
#include <string>
#include "utilities.h"
#include <time.h>

const int NODE_COORDS = 0;
const int PIXEL_COORDS = 1;

const int NONE = -1;

const int LEFT = 0;
const int RIGHT = 1;

const int DOWN = 0;
const int UP = 1;

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

enum JUMP_TARGET {
	GROUND,
	LADDER,
	LEDGE
};

enum MOVEMENTACTION
{
	IDLE,
	CENTRALIZE,
	WALKRIGHT,
	WALKLEFT,
	RUNRIGHT,
	RUNLEFT,
	HANGRIGHT,
	HANGLEFT,
	DROP,
	HANGDROPRIGHT,
	HANGDROPLEFT,
	JUMPABOVERIGHT,
	JUMPABOVELEFT,
	JUMPUPRIGHT,
	JUMPUPLEFT,
	JUMPUPRIGHT_LEDGE,
	JUMPUPLEFT_LEDGE,
	JUMPDOWNRIGHT,
	JUMPDOWNLEFT,
	WALKOFFLEDGERIGHT,
	WALKOFFLEDGELEFT,
	WALKUPRIGHT,
	WALKUPLEFT,
	CLIMBFROMHANG_RIGHT,
	CLIMBFROMHANG_LEFT,
	CLIMBUP,
	CLIMBDOWN,
	JUMPUP_LADDER,
	JUMPUPRIGHT_LADDER,
	JUMPUPLEFT_LADDER,
	JUMPDOWNRIGHT_LADDER,
	JUMPDOWNLEFT_LADDER,
	WALKOFFLEDGERIGHT_LADDER,
	WALKOFFLEDGELEFT_LADDER
};

const std::string MVactionStrings[] =
{ "IDLE",
"CENTRALIZE",
"WALKRIGHT",
"WALKLEFT",
"RUNRIGHT",
"RUNLEFT",
"HANGRIGHT",
"HANGLEFT",
"DROP",
"HANGDROPRIGHT",
"HANGDROPLEFT",
"JUMPABOVERIGHT",
"JUMPABOVELEFT",
"JUMPUPRIGHT",
"JUMPUPLEFT",
"JUMPUPRIGHT_LEDGE",
"JUMPUPLEFT_LEDGE",
"JUMPDOWNRIGHT",
"JUMPDOWNLEFT",
"WALKOFFLEDGERIGHT",
"WALKOFFLEDGELEFT",
"WALKUPRIGHT",
"WALKUPLEFT",
"CLIMBFROMHANG_RIGHT",
"CLIMBFROMHANG_LEFT",
"CLIMBUP",
"CLIMBDOWN",
"JUMPUP_LADDER",
"JUMPUPRIGHT_LADDER",
"JUMPUPLEFT_LADDER",
"JUMPDOWNRIGHT_LADDER",
"JUMPDOWNLEFT_LADDER",
"WALKOFFLEDGERIGHT_LADDER",
"WALKOFFLEDGELEFT_LADDER" 
};


enum VariableType
{
	TYPE_BOOLEAN = 0, TYPE_STRING, TYPE_DOUBLE
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
	spDUCKTOHANG = 24,
};

enum TerrainType
{
	spEmptyNode = 0,
	spStandardTerrain,
	spLadder,
	spExit,
	spEntrance,
	spSacAltar,
	spArrowTrapRight,
	spArrowTrapLeft,
	spIsInShop,
	spIce,
	spSpike,
	spSpearTrap,
	spGameEntrance
};

enum Collectable
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
	//spRope = 36;
	spBomb = 37,
	spFireFrogBomb,
	spRock,
	spDice,

	spMattockHead,

	// spBasketball = 42;

	spShotgun = 43,

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

	spDamsel = 64,

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
	spBoulder,
	spSpringTrap = 99
};