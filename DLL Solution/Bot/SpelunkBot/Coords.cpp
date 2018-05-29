#include "stdafx.h"
#include "Coords.h"

Coords::Coords()
{
	_xNode = 0;
	_yNode = 0;
	_xPixel = 0;
	_yPixel = 0;
}

Coords::Coords(int xNode, int yNode)
{
	_xNode = xNode;
	_yNode = yNode;
	_xPixel = ConvertToPixel(xNode);
	_yPixel = ConvertToPixel(yNode);
}

Coords::Coords(double x, double y, COORD_TYPE ctype)
{
	switch (ctype)
	{
	case NODE:
		_xNode = x;
		_yNode = y;
		_xPixel = ConvertToPixel(x);
		_yPixel = ConvertToPixel(y);
		break;
	case NODE_ROUNDDOWN:
		_xNode = x;
		_yNode = y;
		_xPixel = ConvertToPixel((int)x);
		_yPixel = ConvertToPixel((int)y);
		break;
	case PIXEL:
		_xNode = ConvertToNode(x);
		_yNode = ConvertToNode(y);
		_xPixel = (int)x;
		_yPixel = (int)y;
		break;
	default:
		_xNode = 0;
		_yNode = 0;
		_xPixel = 0;
		_yPixel = 0;
		break;
	}
}

int Coords::GetX()
{
	return (int)ConvertToNode(_xPixel);
}

double Coords::GetXnode()
{
	return ConvertToNode(_xPixel);
}

int Coords::GetXpixel()
{
	return _xPixel;
}

int Coords::GetY()
{
	return (int)ConvertToNode(_yPixel);
}

double Coords::GetYnode()
{
	return ConvertToNode(_yPixel);
}

int Coords::GetYpixel()
{
	return _yPixel;
}

int Coords::GetMidXpixel()
{
	return ConvertToPixel(GetX()) + (PIXELS_IN_NODE / 2);
}

double Coords::GetMidXnode()
{
	return GetX() + 0.5;
}

int Coords::GetMidYpixel()
{
	return ConvertToPixel(GetY()) + (PIXELS_IN_NODE / 2);
}

double Coords::GetMidYnode()
{
	return GetX() + 0.5;
}

Coords Coords::Offset(int xDistNode, int yDistNode)
{
	return Coords(this->GetXnode() + xDistNode, this->GetYnode() + yDistNode, NODE);
}

Coords Coords::OffsetX(int xDistNode)
{
	return Coords(this->GetXnode() + xDistNode, this->GetYnode(), NODE);
}

Coords Coords::OffsetY(int yDistNode)
{
	return Coords(this->GetXnode(), this->GetYnode() + yDistNode, NODE);
}

int Coords::ConvertToPixel(double nodeCoord)
{
	return (int)(nodeCoord * 16);
}

double Coords::ConvertToNode(int pixelCoord)
{
	return (double)(pixelCoord / 16);
}