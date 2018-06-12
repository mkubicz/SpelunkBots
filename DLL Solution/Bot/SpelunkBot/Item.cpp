#include "stdafx.h"
#include "Item.h"

Item::Item(double x, double y, ItemType type, int id)
	: _coords(Coords(x, y, NODE)), _id(id), _type(type)
{
	SetKind(type);
}

Item::Item(double x, double y, ItemType type)
	: Item(x, y, type, 0)
{
}

double Item::GetX()
{
	return _coords.GetXnode();
}

double Item::GetY()
{
	return _coords.GetYnode();
}

Coords Item::GetCoords()
{
	return _coords;
}

int Item::GetID()
{
	return _id;
}

ItemType Item::GetType()
{
	return _type;
}

ItemKind Item::GetKind()
{
	return _kind;
}

void Item::SetKind(ItemType type)
{
	switch (type)
	{
	case spGoldBar:
	case spGoldBars:
	case spEmerald:
	case spEmeraldBig:
	case spSapphire:
	case spSapphireBig:
	case spRuby:
	case spRubyBig:
	case spDiamond:
	case spGoldNugget:
	case spGoldChunk:
	case spBombBox:
	case spRopePile:
	case spBombBag:
		_kind = kndTreasure;
		break;
	case spChest:
	case spLockedChest:
	case spFlareCrate:
	case spDamsel:
	case spGoldIdol:
	case spCrate:
		_kind = kndJunkHeavy;
		break;
	case spKey:
	case spRock:
	case spMattockHead:
	case spJar:
	case spSkull:
	case spArrow:
	case spFishBone:
	case spFlare:
		_kind = kndJunkLight;
		break;
		break;
		break;
	case spPaste:
	case spParachute:
	case spCompass:
	case spSpringShoes:
	case spSpikeShoes:
	case spSpecs:
	case spUdjat:
	case spCrown:
	case spKapala:
	case spAnkh:
	case spGloves:
	case spMitt:
	case spJetpack:
	case spCape:
		_kind = kndEquipable;
		break;
	case spShotgun:
	case spPistol:
	case spWebCannon:
	case spTeleporter:
	case spBow:
	case spMachete:
	case spSceptre:
	case spMattock:
		_kind = kndWeapon;
		break;
	case spRope:
	case spBomb:
	case spFireFrogBomb:
	case spDice:
	case spBasketball:
	case spCrystalSkull:
	case spLampItem:
	case spBlock:
	case spPushBlock:
	case spDarkFall:
		_kind = kndOther;
		break;
	case spJordans:
	case spRopeBag:
		_kind = kndNotInGame;
		break;
	default:
		_kind = kndOther;
		break;
	}
}