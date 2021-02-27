#include <XEngine.h>
#include <AI/Inc/AI.h>
#include "TileMap.h"
#include "Pathfinding.h"

std::string textureName;
std::string textureName2;
std::string textureName3;
std::string textureName4;

std::string textureName_mr;
std::vector<int> freeTile;
int count = 0;

int screenWidth = 0;
int screenHeight = 0;

enum class GameState
{
	StartScreen,
	Gameplay,
	GameOver
};
GameState gameState;

struct character
{
	
	void Initialize()
	{
		stateMachine = std::make_unique<AI::StateMachine<character>>(*this);
	}
	std::unique_ptr<AI::StateMachine<character>> stateMachine;

	X::Math::Vector2 position;
	int policy = 0;

	X::Math::Vector2 MushroomPos;
	size_t MushroomIndex = 0;
	int mushroomCount = 0;

	X::TextureId textureId = 0;
	X::TextureId textureId_mr = 0;

	PathFinding myPF;
	std::list<X::Math::Vector2> myPath;
};

struct FindState : public AI::State<character>
{
	static std::string GetName() { return "Find"; }

	void PutMushroom(character& agent)
	{
		freeTile = agent.myPF.GetTileIndex();
		int Random = std::rand() % freeTile.size();
		agent.MushroomIndex = freeTile[Random];
		agent.MushroomPos = agent.myPF.GetNodePos(agent.MushroomIndex);

	}

	void Enter(character& agent) override
	{
		position = agent.position;
	}
	void Update(character& agent, float deltaTime) override
	{
		
		if (agent.myPath.empty())
		{
			agent.myPath.clear();
			agent.myPF.LoadStart(agent.MushroomIndex);

			
			int Random = std::rand() % freeTile.size();
			agent.MushroomIndex = freeTile[Random];
			agent.MushroomPos = agent.myPF.GetNodePos(agent.MushroomIndex);

			agent.myPF.LoadEnd(agent.MushroomIndex);
			

			switch (agent.policy)
			{
			case 0:
				agent.myPF.BFSPolicy();
				break;
			case 1:
				agent.myPF.DFSPolicy();
				break;
			case 2:
				agent.myPF.DijkstraPolicy();
				break;
			case 3:
				agent.myPF.AstarPolicy();
				break;
			default:
				break;
			}

			agent.myPath = agent.myPF.GetPathPosition();
			agent.stateMachine->ChangeState("Run");
		}
	}
	void Exit(character& agent) override
	{
		position = agent.position;
	}

	X::Math::Vector2 position;
};

struct RunState : public AI::State<character>
{
	static std::string GetName() { return "Run"; }

	void Enter(character& agent) override
	{
		destination = agent.myPath.front();
	}
	void Update(character& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			if (!agent.myPath.empty())
			{
				destination = agent.myPath.front();
				agent.myPath.pop_front();
			}
			else
			{
				agent.mushroomCount++;
				agent.stateMachine->ChangeState("Find");
			}
			
		}
		else
		{			
			agent.position += X::Math::Normalize(destination - agent.position) *220.0f * deltaTime;

			X::DrawScreenLine(agent.position, destination, X::Colors::Red);
			X::DrawScreenCircle(destination, 10.0f, X::Colors::Green);
		}
	}
	void Exit(character& agent) override
	{
	}

	X::Math::Vector2 destination;
};

character myChar;
character myChar2;
character myChar3;
character myChar4;



bool showGraph = false;

bool showPathBFS = false;
bool showPathDFS = false;
bool showPathDijkstra = false;
bool showPathAstar = false;

bool showClosetPathBFS = false;
bool showClosetPathDFS = false;
bool showClosetPathDijkstra = false;
bool showClosetPathAstar = false;

void ShowUI()
{
	ImGui::Begin("Mushroom Collector");

	ImGui::Text(" -- Gray Knight -> BFS -- ");
	ImGui::Text("GK get Mushroom: %i", myChar.mushroomCount);
	ImGui::Text("");
	ImGui::Text(" -- Orange Knight -> DFS -- ");
	ImGui::Text("OK Mushroom: %i", myChar2.mushroomCount);

	ImGui::Text("");
	ImGui::Text(" -- Purple Knight -> Dijkstra -- ");
	ImGui::Text("PK Mushroom: %i", myChar3.mushroomCount);

	ImGui::Text("");
	ImGui::Text(" -- Light Green Knight -> A* -- ");
	ImGui::Text("LGK Mushroom: %i", myChar4.mushroomCount);

	ImGui::Text("");
	if (ImGui::Button("Show Graph"))
	{
		showGraph = !showGraph;
	}
	if (ImGui::Button("Clean all path"))
	{
		showPathBFS = false;
		showPathDFS = false;
		showPathDijkstra = false;
		showPathAstar = false;
	}
	if (ImGui::Button("Clean all closet path"))
	{
		showClosetPathBFS = false;
		showClosetPathDFS = false;
		showClosetPathDijkstra = false;
		showClosetPathAstar = false;
	}

	if (ImGui::CollapsingHeader("BFS"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Button("BFS Path"))
		{
			showPathBFS = !showPathBFS;
		}
		if (ImGui::Button("BFS Closet Path"))
		{
			showClosetPathBFS = !showClosetPathBFS;
		}
	}
	if (ImGui::CollapsingHeader("DFS"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Button("DFS Path"))
		{
			showPathDFS = !showPathDFS;
		}
		if (ImGui::Button("DFS Closet Path"))
		{
			showClosetPathDFS = !showClosetPathDFS;
		}
	}
	if (ImGui::CollapsingHeader("Dijkstra"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Button("Dijkstra Path"))
		{
			showPathDijkstra = !showPathDijkstra;
		}
		if (ImGui::Button("Dijkstra Closet Path"))
		{
			showClosetPathDijkstra = !showClosetPathDijkstra;
		}
	}
	if (ImGui::CollapsingHeader("A*"), ImGuiTreeNodeFlags_DefaultOpen)
	{
		if (ImGui::Button("A* Path"))
		{
			showPathAstar = !showPathAstar;
		}
		if (ImGui::Button("A* Closet Path"))
		{
			showClosetPathAstar = !showClosetPathAstar;
		}
	}
	ImGui::End();
}

void GameInit()
{
	screenWidth = X::GetScreenWidth();
	screenHeight = X::GetScreenHeight();

	textureName = "isd1_fr1.png"; 
	textureName2 = "avt1_fr1.png";
	textureName3 = "amg1_fr1.png";
	textureName4 = "trk1_fr1.png";

	textureName_mr = "mushroom1.png";
	myChar.textureId = X::LoadTexture(textureName.c_str());
	myChar.textureId_mr = X::LoadTexture(textureName_mr.c_str());

	myChar2.textureId = X::LoadTexture(textureName2.c_str());
	myChar2.textureId_mr = X::LoadTexture(textureName_mr.c_str());

	myChar3.textureId = X::LoadTexture(textureName3.c_str());
	myChar3.textureId_mr = X::LoadTexture(textureName_mr.c_str());

	myChar4.textureId = X::LoadTexture(textureName4.c_str());
	myChar4.textureId_mr = X::LoadTexture(textureName_mr.c_str());

	// 1
	myChar.Initialize();
	myChar.policy = 0;
	myChar.position = { 16.f,16.f };
	myChar.stateMachine->AddState<FindState>();
	myChar.stateMachine->AddState<RunState>();
	myChar.myPF.LoadStart(0);
	myChar.myPF.LoadMap();
	myChar.myPF.BuildGraph();
	myChar.myPF.GetColorIndex(0);

	// 2
	myChar2.Initialize();
	myChar2.policy = 1;
	myChar2.position = { 16.f,16.f };
	myChar2.stateMachine->AddState<FindState>();
	myChar2.stateMachine->AddState<RunState>();
	myChar2.myPF.LoadStart(0);
	myChar2.myPF.LoadMap();
	myChar2.myPF.BuildGraph();
	myChar2.myPF.GetColorIndex(1);

	// 3
	myChar3.Initialize();
	myChar3.policy = 2;
	myChar3.position = { 16.f,16.f };
	myChar3.stateMachine->AddState<FindState>();
	myChar3.stateMachine->AddState<RunState>();
	myChar3.myPF.LoadStart(0);
	myChar3.myPF.LoadMap();
	myChar3.myPF.BuildGraph();
	myChar3.myPF.GetColorIndex(2);

	// 4
	myChar4.Initialize();
	myChar4.policy = 3;
	myChar4.position = { 16.f,16.f };
	myChar4.stateMachine->AddState<FindState>();
	myChar4.stateMachine->AddState<RunState>();
	myChar4.myPF.LoadStart(0);
	myChar4.myPF.LoadMap();
	myChar4.myPF.BuildGraph();
	myChar4.myPF.GetColorIndex(3);

	freeTile = myChar2.myPF.GetTileIndex();

}

void GameCleanup()
{

}

void GameLoopStart(float deltaTime)
{
	float xPos = screenWidth *0.3f;
	float yPos = screenHeight*0.4f;


	// print the header
	X::DrawScreenText("Mushroom Collector", xPos, yPos, 50.0f, X::Math::Vector4(1.0f, 2.0f, 0.0f, 1.0f));
	X::DrawScreenText("AI Pathfinding Simulation", xPos-100.0f, yPos+60.0f, 50.0f, X::Math::Vector4(0.50f, 1.0f, 0.0f, 1.0f));
	



	if (X::IsKeyPressed(X::Keys::Y))
	{
		gameState = GameState::Gameplay;
		return;
	}

}



bool GameLoop(float deltaTime)
{
	ShowUI();
	
	if (showGraph)
	{
		myChar.myPF.ShowGraph();
	}

	if (showPathBFS)
	{
		myChar.myPF.ShowPath();
		

	}
	if (showPathDFS)
	{
		myChar2.myPF.ShowPath();
		
	}
	if (showPathDijkstra)
	{
		myChar3.myPF.ShowPath();
		
	}
	if (showPathAstar)
	{
		myChar4.myPF.ShowPath();
	}

	if (showClosetPathBFS)
	{
		myChar.myPF.ShowClosetPath();
	}
	if (showClosetPathDFS)
	{
		myChar2.myPF.ShowClosetPath();
	}
	if (showClosetPathDijkstra)
	{
		myChar3.myPF.ShowClosetPath();
	}
	if (showClosetPathAstar)
	{
		myChar4.myPF.ShowClosetPath();
	}


	myChar.myPF.MapUpdate(deltaTime);
	myChar2.myPF.MapUpdate(deltaTime);
	myChar3.myPF.MapUpdate(deltaTime);
	myChar4.myPF.MapUpdate(deltaTime);


	myChar.stateMachine->Update(deltaTime);
	myChar2.stateMachine->Update(deltaTime);
	myChar3.stateMachine->Update(deltaTime);
	myChar4.stateMachine->Update(deltaTime);

	X::DrawSprite(myChar.textureId, myChar.position);
	X::DrawSprite(myChar.textureId_mr, myChar.MushroomPos);


	X::DrawSprite(myChar2.textureId, myChar2.position);
	X::DrawSprite(myChar2.textureId_mr, myChar2.MushroomPos);

	X::DrawSprite(myChar3.textureId, myChar3.position);
	X::DrawSprite(myChar3.textureId_mr, myChar3.MushroomPos);

	X::DrawSprite(myChar4.textureId, myChar4.position);
	X::DrawSprite(myChar4.textureId_mr, myChar4.MushroomPos);


	return X::IsKeyPressed(X::Keys::ESCAPE);
}

bool MainLoop(float deltaTime)
{

	switch (gameState)
	{
	case GameState::StartScreen:
		GameLoopStart(deltaTime);
		break;
	case GameState::Gameplay:
		GameLoop(deltaTime);
		break;
	case GameState::GameOver:
		break;
	}

	return X::IsKeyPressed(X::Keys::ESCAPE);
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start();
	GameInit();

	X::Run(MainLoop);

	GameCleanup();
	X::Stop();
	return 0;
}