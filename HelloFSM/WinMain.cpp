#include <AI/Inc/AI.h>
#include <XEngine.h>


//
//struct Dude
//{
//	/*Dude() : stateMachine(*this) {}
//
//	AI::StateMachine<Dude> stateMachine;
//	X::Math::Vector2 position;
//*/
//
//	void Initialize()
//	{
//		stateMachine = std::make_unique<AI::StateMachine<Dude>>(*this);
//	}
//
//	std::unique_ptr<AI::StateMachine<Dude>> stateMachine;
//	X::Math::Vector2 position;
//
//};
//
//struct DanceState : public AI::State<Dude>
//{
//	static std::string GetName() { return "Dance"; }
//
//	void Enter(Dude& agent) override
//	{
//		position = agent.position;
//		time = 0.0f;
//	}
//	void Update(Dude& agent, float deltaTime) override
//	{
//		time += deltaTime * 10.0f;
//		if (time > X::Math::kTwoPi)
//			agent.stateMachine->ChangeState("Run");
//		else
//		{
//			agent.position.x = position.x + sin(time * 2.0f) * 30.0f;
//			agent.position.y = position.y + cos(time * 2.0f) * 10.0f;
//		}
//	}
//	void Exit(Dude& agent) override
//	{
//		agent.position = position;
//	}
//
//	X::Math::Vector2 position;
//	float time;
//};
//
//struct RunState : public AI::State<Dude>
//{
//	static std::string GetName() { return "Run"; }
//
//	void Enter(Dude& agent) override
//	{
//		destination = X::RandomUnitCircle() * 250.0f + X::Math::Vector2{ 600.0f, 350.0f };
//	}
//	void Update(Dude& agent, float deltaTime) override
//	{
//		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
//			agent.stateMachine->ChangeState("Dance");
//		else
//		{
//			agent.position += X::Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
//			X::DrawScreenLine(agent.position, destination, X::Colors::Red);
//			X::DrawScreenCircle(destination, 10.0f, X::Colors::Green);
//		}
//	}
//	void Exit(Dude& agent) override	{}
//
//	X::Math::Vector2 destination;
//};
//
//Dude dude;
//X::TextureId textureId = 0;

std::unique_ptr<X::TextureId[]> mTextureIds;
std::string textures[11] = { "build.png", "crystal.png", "transf.png","scv_05.png","scv_11.png","refinery_01.png","refinery_02.png" ,"refinery_03.png" ,"refinery_04.png" ,"refinery_05.png","scv_03.png" };

X::Math::Vector2 crystalPosition = { 1000.f, 300.f };
X::Math::Vector2 refineryPosition = { 500.f, 300.f };
X::Math::Vector2 buildrefineryPosition = {200.f, 500.f };

X::TextureId worker1TextureId = 0;
X::TextureId refineryTextureId = 0;

int crystalCount = 0;
int buildCount = 0;

struct Worker1
{

	void Initialize()
	{
		stateMachine = std::make_unique<AI::StateMachine<Worker1>>(*this);
	}

	std::unique_ptr<AI::StateMachine<Worker1>> stateMachine;
	X::Math::Vector2 position;

};

struct walkState : public AI::State<Worker1>
{
	static std::string GetName() { return "Walk"; }

	void Enter(Worker1& agent) override
	{
		destination = crystalPosition;
	}
	void Update(Worker1& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			
			worker1TextureId = mTextureIds[2];
			agent.stateMachine->ChangeState("WalkBack");
		}
		else
		{
			worker1TextureId = mTextureIds[3];
			agent.position += X::Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
		}
	}
	void Exit(Worker1& agent) override	{}

	X::Math::Vector2 destination;
};

struct walkBackState : public AI::State<Worker1>
{
	static std::string GetName() { return "WalkBack"; }

	void Enter(Worker1& agent) override
	{
		destination = refineryPosition;
	}
	void Update(Worker1& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			crystalCount += 100;
			worker1TextureId = mTextureIds[3];
			agent.stateMachine->ChangeState("Walk");

			if (crystalCount == 600)
			{
				agent.stateMachine->ChangeState("WalkBuild");
			}
		}
		else
		{
			worker1TextureId = mTextureIds[2];
			agent.position += X::Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
		}
	}
	void Exit(Worker1& agent) override {}

	X::Math::Vector2 destination = refineryPosition;
};

struct walkBuildBackState : public AI::State<Worker1>
{
	static std::string GetName() { return "WalkBuild"; }

	void Enter(Worker1& agent) override
	{
		destination = buildrefineryPosition;
	}
	void Update(Worker1& agent, float deltaTime) override
	{
		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			crystalCount -= 600;
			agent.stateMachine->ChangeState("Building");

		}
		else
		{
			worker1TextureId = mTextureIds[4];
			agent.position += X::Math::Normalize(destination - agent.position) * 300.0f * deltaTime;
		}
	}
	void Exit(Worker1& agent) override {}

	X::Math::Vector2 destination = buildrefineryPosition;
};

struct BuildState : public AI::State<Worker1>
{
	static std::string GetName() { return "Build"; }

	void Enter(Worker1& agent) override
	{
		destination = buildrefineryPosition;
	}
	void Update(Worker1& agent, float deltaTime) override
	{

		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			buildCount += 5;
			agent.stateMachine->ChangeState("Building");
		}
		else
		{
			worker1TextureId = mTextureIds[4];
			agent.position += X::Math::Normalize(destination - agent.position) * 100.0f * deltaTime;
		}
		
	}
	void Exit(Worker1& agent) override {}

	X::Math::Vector2 destination = buildrefineryPosition;

};
struct BuildingState : public AI::State<Worker1>
{
	static std::string GetName() { return "Building"; }

	void Enter(Worker1& agent) override
	{
		destination = buildrefineryPosition -25.f;
	}
	void Update(Worker1& agent, float deltaTime) override
	{

		if (X::Math::DistanceSqr(agent.position, destination) < 10.0f)
		{
			agent.stateMachine->ChangeState("Build");

		}
		else
		{
			worker1TextureId = mTextureIds[0];
			agent.position += X::Math::Normalize(destination - agent.position) * 100.0f * deltaTime;
		}


	}
	void Exit(Worker1& agent) override {}

	X::Math::Vector2 destination = buildrefineryPosition - 25.f;

};

struct DoneState : public AI::State<Worker1>
{
	static std::string GetName() { return "Done"; }

	void Enter(Worker1& agent) override
	{
		destination = refineryPosition;
	}
	void Update(Worker1& agent, float deltaTime) override
	{

		worker1TextureId = mTextureIds[10];
		agent.position += X::Math::Normalize(destination - agent.position) * 150.0f * deltaTime;

	}
	void Exit(Worker1& agent) override {}

	X::Math::Vector2 destination = refineryPosition;

};


Worker1 worker1;

void GameInit()
{
	mTextureIds = std::make_unique<X::TextureId[]>(textures->size());
	mTextureIds[0] = X::LoadTexture(textures[0].c_str());
	mTextureIds[1] = X::LoadTexture(textures[1].c_str());
	mTextureIds[2] = X::LoadTexture(textures[2].c_str());
	mTextureIds[3] = X::LoadTexture(textures[3].c_str());
	mTextureIds[4] = X::LoadTexture(textures[4].c_str());
	mTextureIds[5] = X::LoadTexture(textures[5].c_str());
	mTextureIds[6] = X::LoadTexture(textures[6].c_str());
	mTextureIds[7] = X::LoadTexture(textures[7].c_str());
	mTextureIds[8] = X::LoadTexture(textures[8].c_str());
	mTextureIds[9] = X::LoadTexture(textures[9].c_str());
	mTextureIds[10] = X::LoadTexture(textures[10].c_str());



	worker1.Initialize();
	worker1.position = {550.f,300.f};
	worker1.stateMachine->AddState<walkState>();
	worker1.stateMachine->AddState<walkBackState>();
	worker1.stateMachine->AddState<walkBuildBackState>();
	worker1.stateMachine->AddState<BuildState>();
	worker1.stateMachine->AddState<BuildingState>();
	worker1.stateMachine->AddState<DoneState>();



}

void GameCleanup()
{
}

bool GameLoop(float deltaTime)
{
	//dude.stateMachine->Update(deltaTime);
	//X::DrawSprite(textureId, dude.position);

	// crystal info
	float xPos = X::GetScreenWidth() * 0.2f;
	float yPos = X::GetScreenHeight() * 0.2f;
	std::string crystal;
	std::stringstream ss;
	ss << crystalCount;
	crystal = ss.str();
	X::DrawScreenText("Crystal: ", xPos, yPos, 24.0f, X::Math::Vector4(1.0f, 2.0f, 0.0f, 1.0f));
	X::DrawScreenText(crystal.c_str(), xPos+130.f, yPos, 24.0f, X::Math::Vector4(1.0f, 2.0f, 0.0f, 1.0f));
	
	// build refinery info
	std::string building;
	std::stringstream sr;
	sr << buildCount;
	building = sr.str();
	if (buildCount > 0)
	{
		X::DrawScreenText(building.c_str(), buildrefineryPosition.x, buildrefineryPosition.y -80.f, 24.0f, X::Math::Vector4(1.0f, 2.0f, 0.0f, 1.0f));
		X::DrawScreenText("%", buildrefineryPosition.x+40.f, buildrefineryPosition.y-80.f, 24.0f, X::Math::Vector4(1.0f, 2.0f, 0.0f, 1.0f));
		if (buildCount < 30)
		{
			X::DrawSprite(mTextureIds[5], buildrefineryPosition);
		}
		if (buildCount >= 30 && buildCount < 50)
		{
			X::DrawSprite(mTextureIds[6], buildrefineryPosition);
		}
		if (buildCount >= 50 && buildCount < 70)
		{
			X::DrawSprite(mTextureIds[7], buildrefineryPosition);
		}
		if (buildCount >= 70 && buildCount < 99)
		{
			X::DrawSprite(mTextureIds[8], buildrefineryPosition);
		}
		if (buildCount == 100)
		{
			X::DrawSprite(mTextureIds[9], buildrefineryPosition);
			worker1.stateMachine->ChangeState("Done");
		}
	}
	


	// crystal
	X::DrawSprite(mTextureIds[1], crystalPosition);
	// refinery
	X::DrawSprite(mTextureIds[9], refineryPosition);

	// worker1
	worker1.stateMachine->Update(deltaTime);
	X::DrawSprite(worker1TextureId, worker1.position);



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