#include "Pathfinding.h"


struct CanOpenNode
{
	PathFinding pf;
	bool operator()(AI::GraphSearch::Context& context, size_t n)
	{
		if (context.open[n])
		{
			return false;
		}

		if (pf.IsWall(n))
		{
			return false;
		}

		return true;
	}
};


struct CanOpenNodeD
{
	PathFinding pf;

	bool operator()(AI::GraphSearch::Context& context, size_t n)
	{	
		if (!pf.IsWall(n) && !context.closed[n])
		{
			return true;
		}

		return false;
	}
};

struct GetGCost
{
	float operator()(AI::GraphSearch::Context& context, size_t parentIndex, size_t nodeIndex)
	{
		if (parentIndex == AI::InvalidNode)
		{
			return 0.0f;
		}

		size_t fromColumn = parentIndex % 30;
		size_t fromRow = parentIndex / 30;
		size_t toColumn = nodeIndex % 30;
		size_t toRow = nodeIndex / 30;

		return context.g[parentIndex] + ((fromColumn != toColumn && fromRow != toRow) ? 1.414f : 1.0f);

	}
};

struct GetHCost
{
	float operator()(AI::GraphSearch::Context& context, size_t parentIndex, size_t nodeIndex)
	{

		if (parentIndex == AI::InvalidNode)
		{
			return 0.0f;
		}

		size_t x1 = nodeIndex % 30;
		size_t y1 = nodeIndex / 30;

		size_t x2 = context.end % 30;
		size_t y2 = context.end / 30;

		size_t diffX = x1 > x2 ? (x1 - x2) : (x2 - x1);
		size_t diffY = y1 > y2 ? (y1 - y2) : (y2 - y1);

		size_t valueH = diffX + diffY;

		return static_cast<float>(valueH);
	}
};



void PathFinding::LoadMap()
{
	mMap.LoadTextures("tileset.txt");
	mMap.LoadMap("level01.txt");
}

void PathFinding::MapUpdate(float deltaTime)
{
	mMap.Update(deltaTime);
	mMap.Render();
}

void PathFinding::BuildGraph()
{
	mGraph.Cleanup();
	mContext.Init(mRow * mColunm);

	mGraph.Initialized(mRow * mColunm);

	for (int y = 0; y < mRow; ++y)
	{
		for (int x = 0; x < mColunm; ++x)
		{
			int index = x + (y * mColunm);
			auto& node = mGraph.GetNode(index);
			node.position.x = (x + 0.5f) * 32.0f;
			node.position.y = (y + 0.5f) * 32.0f;

			int nodePos;

			// top side
			if (y == 0)
			{
				if (x == 0)
				{
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * mColunm);

					node.neighbors.push_back(nodePos);
				}
				else if (x == mColunm - 1)
				{
					// left
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
			}
			// bot side
			else if (y == mRow - 1)
			{
				if (x == 0)
				{
					// top
					nodePos = (x + 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
				else if (x == mColunm - 1)
				{
					// left
					nodePos = (x - 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top 
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// top
					nodePos = x + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
			}
			else
			{
				// left side
				if (x == 0)
				{
					// top
					nodePos = x + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
				// right side
				else if (x == mColunm - 1)
				{
					// top
					nodePos = x + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// left
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x - 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * mColunm);
					node.neighbors.push_back(nodePos);
					// top
					nodePos = x + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * mColunm);
					node.neighbors.push_back(nodePos);
				}

			}

		}

	}

}

void PathFinding::ShowGraph()
{
	for (int y = 0; y < mRow; ++y)
	{
		for (int x = 0; x < mColunm; ++x)
		{
			int index = x + (y * mColunm);
			auto& node = mGraph.GetNode(index);

			
			X::DrawScreenCircle(node.position, 5.0f, X::Colors::DarkGreen);
			

			for (auto& neighbor : node.neighbors)
			{
				X::DrawScreenLine(node.position, mGraph.GetNode(neighbor).position, X::Colors::DarkGreen);
			}

		}

	}
}

void PathFinding::LoadStart(size_t index)
{
	mStart = index;
}

void PathFinding::LoadEnd(size_t index)
{
	mEnd = index;

}

void PathFinding::ShowPath() const
{
	for (auto iter = mPath.begin(); iter != mPath.end(); ++iter)
	{
		size_t nodeIndex = *iter;
		auto& node = mGraph.GetNode(nodeIndex);

		const X::Color colors[] = {
			X::Colors::LightBlue,
			X::Colors::Orange,
			X::Colors::Cyan,
			X::Colors::OrangeRed
		};

		X::DrawScreenCircle(node.position, 5.0f, colors[mColorIndex]);

		auto next = iter;
		next++;
		if (next != mPath.end())
		{
			size_t nextNodeIndex = *next;
			auto& nextNode = mGraph.GetNode(nextNodeIndex);

			const X::Math::Vector2 offsets[] =
			{
				{  0.0f,  0.0f },
				{  1.0f,  0.0f },
				{  0.0f,  1.0f },
				{  1.0f,  1.0f },
			};
			for (auto& offset : offsets)
				X::DrawScreenLine(node.position + offset, nextNode.position + offset, colors[mColorIndex]);
		}
	}
}

void PathFinding::ShowClosedList() const
{
	for (auto index : mContext.closedList)
	{
		auto& node = mGraph.GetNode(index);

		switch (mColorIndex)
		{
		case 0:
			//X::DrawScreenCircle(node.position, 5.0f, X::Colors::DarkGray);
			if (index == mContext.start)
			{
				X::DrawScreenCircle(node.position, 10.0f, X::Colors::Red);
			}
			else
			{
				size_t parentIndex = mContext.parent[index];
				auto& parentNode = mGraph.GetNode(parentIndex);
				X::DrawScreenLine(node.position, parentNode.position, X::Colors::LightBlue);

			}
			break;
		case 1:
			//X::DrawScreenCircle(node.position, 5.0f, X::Colors::Orange);
			if (index == mContext.start)
			{
				X::DrawScreenCircle(node.position, 10.0f, X::Colors::Red);
			}
			else
			{
				size_t parentIndex = mContext.parent[index];
				auto& parentNode = mGraph.GetNode(parentIndex);
				X::DrawScreenLine(node.position, parentNode.position, X::Colors::Orange);

			}
			break;
		case 2:
			//X::DrawScreenCircle(node.position, 5.0f, X::Colors::Purple);
			if (index == mContext.start)
			{
				X::DrawScreenCircle(node.position, 10.0f, X::Colors::Red);
			}
			else
			{
				size_t parentIndex = mContext.parent[index];
				auto& parentNode = mGraph.GetNode(parentIndex);
				X::DrawScreenLine(node.position, parentNode.position, X::Colors::Cyan);

			}
			break;
		case 3:
			//X::DrawScreenCircle(node.position, 5.0f, X::Colors::LightGreen);
			if (index == mContext.start)
			{
				X::DrawScreenCircle(node.position, 10.0f, X::Colors::Red);
			}
			else
			{
				size_t parentIndex = mContext.parent[index];
				auto& parentNode = mGraph.GetNode(parentIndex);
				X::DrawScreenLine(node.position, parentNode.position, X::Colors::OrangeRed);

			}
			break;
		default:
			break;
		}
	}
}

std::list<X::Math::Vector2> PathFinding::GetPathPosition()
{
	mPathPosition.clear();
	for (auto nodeIndex : mPath)
	{
		auto& node = mGraph.GetNode(nodeIndex);
		mPathPosition.push_back(node.position);
	}

	return mPathPosition;
}

void PathFinding::AstarPolicy()
{
	mPath.clear();
	mContext.Reset();
	mContext.start = mStart;
	mContext.end = mEnd;

	AI::GraphSearch::AStart<AI::Graph<MapNode>, CanOpenNodeD, GetGCost, GetHCost> aStar(mGraph);
	aStar.IsValid.pf.LoadMap();
	aStar.IsValid.pf.BuildGraph();

	aStar.Execute(mContext);
	if (mContext.found)
	{
		auto next = mContext.closedList.back();
		while (next != AI::InvalidNode)
		{
			mPath.push_front(next);
			next = mContext.parent[next];
		}
	}

}

void PathFinding::DijkstraPolicy()
{
	mPath.clear();
	mContext.Reset();
	mContext.start = mStart;
	mContext.end = mEnd;

	AI::GraphSearch::Dijkstra<AI::Graph<MapNode>, CanOpenNodeD, GetGCost> dijkstra(mGraph);
	dijkstra.IsValid.pf.LoadMap();
	dijkstra.IsValid.pf.BuildGraph();
	dijkstra.Execute(mContext);
	if (mContext.found)
	{

		auto next = mContext.closedList.back();
		while (next != AI::InvalidNode)
		{
			mPath.push_front(next);
			next = mContext.parent[next];
		}
	}

}

void PathFinding::BFSPolicy()
{
	mPath.clear();
	mContext.Reset();
	mContext.start = mStart;
	mContext.end = mEnd;

	AI::GraphSearch::BFS<AI::Graph<MapNode>, CanOpenNode> bfs(mGraph);
	bfs.IsValid.pf.LoadMap();
	bfs.IsValid.pf.BuildGraph();
	bfs.Execute(mContext);
	if (mContext.found)
	{

		auto next = mContext.closedList.back();
		while (next != AI::InvalidNode)
		{
			mPath.push_front(next);
			next = mContext.parent[next];
		}
	}
}

void PathFinding::DFSPolicy()
{
	mPath.clear();
	mContext.Reset();
	mContext.start = mStart;
	mContext.end = mEnd;

	AI::GraphSearch::DFS<AI::Graph<MapNode>, CanOpenNode> dfs(mGraph);
	dfs.IsValid.pf.LoadMap();
	dfs.IsValid.pf.BuildGraph();
	dfs.Execute(mContext);
	if (mContext.found)
	{

		auto next = mContext.closedList.back();
		while (next != AI::InvalidNode)
		{
			mPath.push_front(next);
			next = mContext.parent[next];
		}
	}
}




X::Math::Vector2 PathFinding::GetNodePos(size_t index)
{
	auto& node = mGraph.GetNode(index);
	mNodePosition = node.position;
	return mNodePosition;
}
