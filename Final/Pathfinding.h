#ifndef INCLUDE_PATHFINDING_H
#define INCLUDE_PATHFINDING_H

#include <XEngine.h>
#include <AI/Inc/AI.h>
#include "TileMap.h"



struct MapNode : public AI::Node
{
	X::Math::Vector2 position;
};

class PathFinding
{
public:
	

	void LoadMap();
	void MapUpdate(float deltaTime);

	void BuildGraph();
	void ShowGraph();
	void LoadStart(size_t index);
	void LoadEnd(size_t index);
	void ShowPath() const;
	void ShowClosedList() const;
	
	std::list<size_t> GetPath() { return mPath; }
	std::list<X::Math::Vector2> GetPathPosition();

	void BFSPolicy();
	void DFSPolicy();
	void DijkstraPolicy();
	void AstarPolicy();

	bool IsWall(size_t n)
	{
		mMap.CheckWall(n);
		return mMap.IsWall();
	}

	const size_t GetNodeCount() const { return mGraph.GetNodeCount(); }
	X::Math::Vector2 GetNodePos(size_t index);
	std::vector<int> GetTileIndex()
	{
		mtileIndex = mMap.GetFreeTileIndex();
		return mtileIndex;
	}

	void GetColorIndex(int index) { mColorIndex = index; }

private:
	AI::Graph<MapNode> mGraph;
	AI::GraphSearch::Context mContext;
	TileMap mMap;

	std::list<size_t> mPath;
	std::list<X::Math::Vector2> mPathPosition;
	X::Math::Vector2 mNodePosition;
	std::vector<int> mtileIndex;

	size_t mStart = 0, mEnd = 0;

	int mColorIndex = 0;

	int mRow = 20;
	int mColunm = 30;

};






#endif // !INCLUDE_PATHFINDING_H