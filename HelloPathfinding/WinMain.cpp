#include <XEngine.h>
#include <AI/Inc/AI.h>
#include "TileMap.h"

TileMap myMap;

enum PolicyToUse
{
	BFS,
	DFS,
	Dijkstra,
	Astar
};


PolicyToUse policytouse;
std::string policyName = "None";

struct MapNode : public AI::Node
{
	X::Math::Vector2 position;	
};

AI::Graph<MapNode> graph;
int iRow = 3;
int iColunm = 3;

bool showGraphB = false;
bool showClosetPathB = false;
bool showPathB = false;
bool pathFinding = false;

bool chooseStartPoint = false;
bool chooseEndPoint = false;

AI::GraphSearch::Context context;
std::list<size_t> path;
bool pathNode = false;
int startX = 0, startY = 0;
int endX = 1, endY = 1;

bool IsWall(size_t n)
{
	/*size_t colunm = n % iColunm;
	size_t row = n / iColunm;

	return (colunm > 3 && colunm < 10 && (colunm == row || colunm + 1 == row));*/

	myMap.CheckWall(n);
	return myMap.IsWall();

}

struct CanOpenNode
{
	bool operator()(AI::GraphSearch::Context& context, size_t n)
	{
		if (context.open[n])
		{
			return false;
		}

		if (IsWall(n))
		{
			return false;
		}

		return true;
	}
};

struct CanOpenNodeD
{
	bool operator()(AI::GraphSearch::Context& context, size_t n)
	{
		
		if (!IsWall(n) && !context.closed[n])
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

		size_t fromColumn = parentIndex % iColunm;
		size_t fromRow = parentIndex / iColunm;
		size_t toColumn = nodeIndex % iColunm;
		size_t toRow = nodeIndex / iColunm;

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

		size_t x1 = nodeIndex % iColunm;
		size_t y1 = nodeIndex / iColunm;

		size_t x2 = context.end % iColunm;
		size_t y2 = context.end / iColunm;

		size_t diffX = x1 > x2 ? (x1 - x2) : (x2 - x1);
		size_t diffY = y1 > y2 ? (y1 - y2) : (y2 - y1);

		size_t valueH = diffX + diffY;

		return valueH;


		/*size_t fromColumn = parentIndex % iColunm;
		size_t fromRow = parentIndex / iColunm;
		size_t toColumn = nodeIndex % iColunm;
		size_t toRow = nodeIndex / iColunm;

		return context.g[parentIndex] + ((fromColumn != toColumn && fromRow != toRow) ? 1.414f : 1.0f) + valueH;*/

	}
};
void BuildGraph()
{
	/*
	[][][][]
	[][][][]
	[][x][][]
	[][][][]
	x = 1, y = 2
	index = x + y * column
	*/
	graph.Cleanup();
	context.Init(iRow * iColunm);

	graph.Initialized(iRow * iColunm);

	for (int y = 0; y < iRow; ++y)
	{
		for (int x = 0; x < iColunm; ++x)
		{
			int index = x + (y * iColunm);
			auto& node = graph.GetNode(index);
			node.position.x = (x + 0.5f) * 32.0f;
			node.position.y = (y + 0.5f) * 32.0f;

			int nodePos;

			// top side
			if (y == 0)
			{
				if (x == 0)
				{
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * iColunm);

					node.neighbors.push_back(nodePos);
				}
				else if (x == iColunm - 1)
				{
					// left
					nodePos = (x - 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x-1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
			}
			// bot side
			else if (y == iRow - 1)
			{
				if (x == 0)
				{
					// top
					nodePos = (x + 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				else if (x == iColunm - 1)
				{
					// left
					nodePos = (x - 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top 
					nodePos = (x - 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x - 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// top
					nodePos = x + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
			}
			else
			{
				// left side
				if (x == 0)
				{
					// top
					nodePos = x + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				// right side
				else if (x == iColunm - 1)
				{
					// top
					nodePos = x + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// left
					nodePos = (x - 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				else
				{
					// left
					nodePos = (x - 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// right
					nodePos = (x + 1) + (y * iColunm);
					node.neighbors.push_back(nodePos);
					// top
					nodePos = x + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - left
					nodePos = (x - 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// top - right
					nodePos = (x + 1) + ((y - 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down
					nodePos = x + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down - left
					nodePos = (x - 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
					// down - right
					nodePos = (x + 1) + ((y + 1) * iColunm);
					node.neighbors.push_back(nodePos);
				}
				
			}
			
		}

	}

}

void ShowGraph()
{

	for (int y = 0; y < iRow; ++y)
	{
		for (int x = 0; x < iColunm; ++x)
		{
			int index = x + (y * iColunm);
			auto& node = graph.GetNode(index);

			if (IsWall(index))
			{
				X::DrawScreenCircle(node.position, 5.0f, X::Colors::OrangeRed);
				
			}
			else
			{
				X::DrawScreenCircle(node.position, 5.0f, X::Colors::DarkSlateGray);
			}

			for (auto& neighbor : node.neighbors)
			{
				X::DrawScreenLine(node.position, graph.GetNode(neighbor).position, X::Colors::DarkSlateGray);
			}

		}

	}

}

void GameInit()
{
	myMap.LoadTextures("tileset.txt");
}

void GameCleanup()
{
	myMap.UnloadMap();
	myMap.UnloadTextures();

	path.clear();
}

void ShowUI()
{
	ImGui::Begin("Node Builder", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::Button("Edit TileMap"))
	{
		myMap.EnableEditor(!myMap.IsEditorEnable());
	}

	ImGui::SliderInt("Columns", &iColunm, 2, 30);
	ImGui::SliderInt("Rows", &iRow, 2, 20);

	ImGui::SliderInt("StartX", &startX, 0, iColunm - 1);
	ImGui::SliderInt("StartY", &startY, 0, iRow - 1);
	if (ImGui::Button("Choose Start Tile"))
	{
		chooseStartPoint = true;
		myMap.EnableEditor(false);
		myMap.EnableStartPoint(true);
	}

	ImGui::SliderInt("EndX", &endX, 0, iColunm - 1);
	ImGui::SliderInt("EndY", &endY, 0, iRow - 1);
	if (ImGui::Button("Choose End Tile"))
	{
		myMap.EnableEditor(false);
		myMap.EnableEndPoint(true);
		chooseEndPoint = true;
	}
	ImGui::Text("");
	if (ImGui::Button("Show TileMap"))
	{
		myMap.LoadMap(iRow, iColunm);
	}
	if (ImGui::Button("Show Graph"))
	{
		if (myMap.IsLoadMap())
		{
			graph.Cleanup();
			//BuildGraph();
			showGraphB = true;
		}
		
	}
	if (ImGui::Button("Finding Path"))
	{
		pathFinding = true;
	}
	if (ImGui::Button("Show Closet Path"))
	{
		showClosetPathB = true;
	}
	if (ImGui::Button("Show Path"))
	{
		showPathB = true;
	}
	if (ImGui::Button("Clean Path"))
	{
		path.clear();
		showPathB = false;
		showClosetPathB = false;
	}
	if (ImGui::Button("Clean All"))
	{
		graph.Cleanup();
		path.clear();
		showGraphB = false;
		showPathB = false;
		showClosetPathB = false;
	}

	ImGui::Text("Policy to use: %s", policyName.c_str());
	ImGui::Text("(1 - BFS, 2 - DFS, 3 - Dijkstra, 4 - Astar)");

	if (X::IsKeyPressed(X::Keys::ONE))
	{
		policytouse = PolicyToUse::BFS;
	}

	if (X::IsKeyPressed(X::Keys::TWO))
	{
		policytouse = PolicyToUse::DFS;
	}

	if (X::IsKeyPressed(X::Keys::THREE))
	{
		policytouse = PolicyToUse::Dijkstra;
	}

	if (X::IsKeyPressed(X::Keys::FOUR))
	{
		policytouse = PolicyToUse::Astar;
	}

	switch (policytouse)
	{
	case BFS:
		policyName = "BFS";
		break;
	case DFS:
		policyName = "DFS";
		break;
	case Dijkstra:
		policyName = "Dijkstra";
		break;
	case Astar:
		policyName = "Astar";
		break;
	default:
		break;
	}
	

	/*
	
	imGui::ImageButton(X::GetSprite(textureId), {100.0f, 100.0f});

	*/

	ImGui::End();

	if (chooseStartPoint)
	{
		
		if (!myMap.IsEnableStartPoint())
		{
			chooseStartPoint = false;
		}
		startX = myMap.GetStartX();
		startY = myMap.GetStartY();
	}

	if (chooseEndPoint)
	{
		
		if (!myMap.IsEnableEndPoint())
		{
			chooseEndPoint = false;
		}
		endX = myMap.GetEndX();
		endY = myMap.GetEndY();
	}

	
}

void ShowClosetPath()
{

	for (auto index : context.closedList)
	{
		auto& node = graph.GetNode(index);
		if (index == context.start)
		{
			X::DrawScreenCircle(node.position, 10.0f, X::Colors::Red);
		}
		else
		{
			size_t parentIndex = context.parent[index];
			auto& parentNode = graph.GetNode(parentIndex);
			X::DrawScreenLine(node.position, parentNode.position, X::Colors::Brown);

		}
	}
}

void ShowPath()
{

	for (auto nodeIndex : path)
	{
		auto& node = graph.GetNode(nodeIndex);
		X::DrawScreenCircle(node.position, 10.0f, X::Colors::LightGoldenrodYellow);
	}
	
}

void BFSPolicy()
{
	path.clear();
	context.Reset();
	context.start = startX + (startY * iColunm);
	context.end = endX + (endY * iColunm);

	AI::GraphSearch::BFS<AI::Graph<MapNode>, CanOpenNode> bfs(graph);
	bfs.Execute(context);
	if (context.found)
	{

		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}
	}

	pathFinding = false;
}

void DFSPolicy()
{
	path.clear();
	context.Reset();
	context.start = startX + (startY * iColunm);
	context.end = endX + (endY * iColunm);

	AI::GraphSearch::DFS<AI::Graph<MapNode>, CanOpenNode> dfs(graph);
	dfs.Execute(context);
	if (context.found)
	{

		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}
	}

	pathFinding = false;
}

void DijkstraPolicy()
{
	path.clear();
	context.Reset();
	context.start = startX + (startY * iColunm);
	context.end = endX + (endY * iColunm);

	AI::GraphSearch::Dijkstra<AI::Graph<MapNode>, CanOpenNodeD, GetGCost> dijkstra(graph);
	dijkstra.Execute(context);
	if (context.found)
	{

		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}
	}

	pathFinding = false;
}

void AstarPolicy()
{
	path.clear();
	context.Reset();
	context.start = startX + (startY * iColunm);
	context.end = endX + (endY * iColunm);

	AI::GraphSearch::AStart<AI::Graph<MapNode>, CanOpenNodeD, GetGCost, GetHCost> aStar(graph);
	aStar.Execute(context);
	if (context.found)
	{
		auto next = context.closedList.back();
		while (next != AI::InvalidNode)
		{
			path.push_front(next);
			next = context.parent[next];
		}
	}

	pathFinding = false;
}


bool GameLoop(float deltaTime)
{
	ShowUI();

	if (showGraphB)
	{
		BuildGraph();
		ShowGraph();

		if (!pathFinding)
		{
			int startIndex = startX + (startY * iColunm);
			int endIndex = endX + (endY * iColunm);
			auto& startNode = graph.GetNode(startIndex);
			auto& endNode = graph.GetNode(endIndex);

			X::DrawScreenCircle(startNode.position, 10.0f, X::Colors::Red);
			X::DrawScreenCircle(endNode.position, 10.0f, X::Colors::Red);
		}
	}
	if (showClosetPathB)
	{
		ShowClosetPath();
	}
	if (showPathB)
	{
		ShowPath();
	}


	if (pathFinding)
	{
		switch (policytouse)
		{
		case BFS:
			BFSPolicy();
			break;
		case DFS:
			DFSPolicy();
			break;
		case Dijkstra:
			DijkstraPolicy();
			break;
		case Astar:
			AstarPolicy();
			break;
		default:

			break;
		}
		
	}

	myMap.Update(deltaTime);
	myMap.Render();


	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}