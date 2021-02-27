#include <AI/Inc/AI.h>
#include <XEngine.h>
#include <ImGui/Inc/imgui.h>

extern std::vector<X::Math::Vector2> minerals;


class VisualSensor : public AI::Sensor
{
public:
	void Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime) override
	{
		// 1 - check distance
		// 2 - check angle/ fov
		// 3 - raycast (TODO)

		auto viewRangeSqr = X::Math::Sqr(viewRange);
		auto cosViewAngle = cos(viewAngle * 0.5f);

		for (auto& mineral : minerals)
		{
			if (X::Math::DistanceSqr(agent.position, mineral) > X::Math::Sqr(viewRange))
			{
				continue;
			}

			if (X::Math::Dot(agent.heading, X::Math::Normalize(mineral - agent.position)))
			{
				continue;
			}

			// find in memory



		}
	}

	float viewRange = 0.0f;
	float viewAngle = 0.0f;
};

class Dude : public AI::Agent
{
public:
	Dude(AI::AIWorld& world) :
		AI::Agent(world)
	{}

	void GameInit()
	{
		maxSpeed = 200.0f;

		mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, [](AI::MemoryRecord&) {});

		mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

		auto wander = mSteeringModule->AddBehavior<AI::WanderBehaviour>("Wander");
		wander->SetParam(30.f, 60.f, 1.0f);
		//wander->SetActive(true);

		/*for (size_t i = 0; i < std::size(mTextureIds); ++i)
		{
			char name[256];
			sprintf_s(name, "scv_%02zu.png", i + 1);
			mTextureIds[i] = X::LoadTexture(name);
		}*/
		mTextureIds[0] = X::LoadTexture("scv_01.png");
	}

	void Update(float deltalTime)
	{
		mPerceptionModule->Update(deltalTime);
 		auto force = mSteeringModule->Calculate();
		auto acceleration = force / mass;

		velocity += acceleration * deltalTime;

		position += velocity * deltalTime;

		if (X::Math::MagnitudeSqr(velocity) > 1.0f)
		{
			heading = X::Math::Normalize(velocity);
		}
	}

	void Render()
	{
		X::DrawSprite(mTextureIds[0], position);
	}


	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	X::TextureId mTextureIds[1];

};
AI::AIWorld world;
Dude dude(world);
Dude dude1(world);

void GameInit()
{
	dude.GameInit();
	dude.position = { 400.f,200.f };
	dude.heading = { 200.f,200.f };

	dude1.GameInit();
	dude1.position = { 400.f,200.f };
	dude1.heading = { 300.f,300.f };
}


void GameCleanup()
{

}

bool GameLoop(float deltaTime)
{
	dude.Update(deltaTime);
	dude.Render();

	dude.destination.x = static_cast<float>(X::GetMouseScreenX());
	dude.destination.y = static_cast<float>(X::GetMouseScreenY());


	dude1.Update(deltaTime);
	dude1.Render();

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