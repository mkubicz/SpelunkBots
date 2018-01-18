
class MapSearchNode
{

friend class Pathfinder;

private:
	int _x;
	int _y;
	int _gScore;
	int _hScore;
	int _fScore;
	MapSearchNode* _parent;
	bool _opened;
	bool _closed;
	bool _visited = false;

public:
	int GetX();
	int GetY();
	double GetGScore(MapSearchNode *p);
	double GetHScore(MapSearchNode *p);
	double GetFScore();

	void ComputeScores(MapSearchNode * end);
}; 
