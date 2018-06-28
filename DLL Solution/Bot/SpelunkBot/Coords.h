#pragma once

///<summary> Class representing coordinates. Can be used as a member class of other object or standalone.
/// Internally holds coords as pixel coordinates. </summary>
class Coords
{
private: 
	int ConvertToPixel(double nodeCoord);
	double ConvertToNode(int pixelCoord);

protected:

	double _xNode;
	double _yNode;
	int _xPixel;
	int _yPixel;

public:
	///<summary> Creates (0,0) Coords.</summary>
	Coords();

	///<summary> Creates Coords representing top left corner of a node.</summary>
	///<param name="x"> Node coordinate x.</param>
	///<param name="y"> Node coordinate y.</param>
	Coords(int x, int y);

	///<summary> Creates Coords.</summary>
	///<param name="x"> Coordinate x.</param>
	///<param name="y"> Coordinate y.</param>
	///<param name="ctype"> Type of x, y coordinates: (NODE, PIXEL, NODE_ROUNDED).</param>
	Coords(double x, double y, COORD_TYPE ctype);

	///<summary>Returns node coordinate x, rounded down.</summary>
	int GetX();
	///<summary>Returns node coordinate x.</summary>
	double GetXnode();
	///<summary>Returns pixel coordinate x.</summary>
	int GetXpixel();

	///<summary>Returns node coordinate y, rounded down.</summary>
	int GetY();
	///<summary>Returns node coordinate y.</summary>
	double GetYnode();
	///<summary>Returns pixel coordinate y.</summary>
	int GetYpixel();

	///<summary>Returns middle x pixel coordinate of current node.</summary>
	int GetMidXpixel();
	///<summary>Returns middle x node coordinate of current node.</summary>
	double GetMidXnode();
	///<summary>Returns middle y pixel coordinate of current node.</summary>
	int GetMidYpixel();
	///<summary>Returns middle y node coordinate of current node.</summary>
	double GetMidYnode();

	///<summary>Returns new Coords object with changed coordinates.</summary>
	///<param name="xDistNode">Distance in nodes to add to X coordinate.</param>
	///<param name="yDistNode">Distance in nodes to add to Y coordinate.</param>
	Coords Offset(int xDistNode, int yDistNode);
	///<summary>Returns new Coords object with changed coordinates.</summary>
	///<param name="xDistNode">Distance in nodes to add to X coordinate.</param>
	Coords OffsetX(int xDistNode);
	///<summary>Returns new Coords object with changed coordinates.</summary>
	///<param name="yDistNode">Distance in nodes to add to Y coordinate.</param>
	Coords OffsetY(int yDistNode);

	///<summary>Returns true if node c is equal in terms of rounded node coordinates.</summary>
	///<param name="c">Coords to test.</param>
	bool SameNode(Coords c);

	inline bool operator==(const Coords& c)
	{
		return this->_xPixel == c._xPixel && this->_yPixel == c._yPixel;

	};
};