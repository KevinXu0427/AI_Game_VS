#include <AI/Inc/AI.h>
#include <xEngine.h>

AI::AIWorld world;

class SeekBehavior : public AI::SteeringBehavior
{
public:
	X::Math::Vector2 Calculate(AI::Agent& agent) override
	{
		auto desiredVelocity = X::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed;
		return desiredVelocity - agent.velocity;
	}
};

float mag;
struct Dude : AI::Agent
{
	Dude(AI::AIWorld world) :
		AI::Agent(world)
	{}

	void Initialize()
	{
		maxSpeed = 300.f;
		steeringModule 	= std::make_unique<AI::SteeringModule>(*this);
		steeringModule->AddBehavior<SeekBehavior>("seek")->SetActive(true);

		textureIds[0] = X::LoadTexture("carrier_01.png");
	}

	void Update(float deltaTime)
	{
		auto force = steeringModule->Calculate();
		auto acceleration = force / mass;

		velocity += acceleration * deltaTime;

		// TODO Add truncate method here
		// if magnitude > maxSpeed
		// normalize then times maxspeed.
		/*mag = X::Math::Magnitude(velocity);
		if (mag > maxSpeed)
		{
			velocity = X::Math::Normalize(velocity) * maxSpeed;
		}*/

		position += velocity * deltaTime;
		
		if (X::Math::MagnitudeSqr(velocity) > 1.0f)
		{
			heading = X::Math::Normalize(velocity);
		}
	}

	void Render()
	{
		//float angle = atan2(heading.x, heading.y) + X::Math::KPi;
		//int frame
		X::DrawSprite(textureIds[0], position);

	}

	std::unique_ptr<AI::SteeringModule> steeringModule;
	X::TextureId textureIds[1];
};

Dude dudes(world);

void GameInit()
{
	//dudes.resize();
	dudes.Initialize();
}

void GameCleanup()
{

}

bool GameLoop(float deltaTime)
{
	//ImGui::Text("mag: %f", mag);


	dudes.destination.x = static_cast<float>(X::GetMouseScreenX());
	dudes.destination.y = static_cast<float>(X::GetMouseScreenY());

	dudes.Update(deltaTime);
	dudes.Render();
	
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