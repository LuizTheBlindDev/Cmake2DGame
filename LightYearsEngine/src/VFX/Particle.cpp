#include "VFX/Particle.h"
#include "framework/MathUtility.h"

namespace ly
{
	Particle::Particle(World* owningWorld, const std::string& texturePath)
		:Actor{ owningWorld, texturePath },
		mVelocity{},
		mLifeTime{1.f},
		mTimer{}
	{

	}

	void Particle::Tick(float deltaTime)
	{
		Actor::Tick(deltaTime);

		Move(deltaTime);
		Fade(deltaTime);

		if (mTimer.getElapsedTime().asSeconds() >= mLifeTime)
		{
			Destroy();
		}
	}

	void Particle::RandomVelocity(float minSpeed, float maxSpeed)
	{
		mVelocity = RandomUnitVector() * RandomRange(minSpeed, maxSpeed);
	}

	void Particle::RandomSize(float min, float max)
	{
		float randScale = RandomRange(min, max);
		if (GetSprite().has_value())
		{
			sf::Vector2f randScaleVec{ randScale, randScale };
			GetSprite().value().setScale(randScaleVec);
		}
	}

	void Particle::RandomLifeTime(float min, float max)
	{
		mLifeTime = RandomRange(min, max);
	}

	void Particle::Move(float deltaTime)
	{
		AddActorLocationOffset(mVelocity * deltaTime);
	}

	void Particle::Fade(float deltaTime)
	{
		float elapseTime = mTimer.getElapsedTime().asSeconds();
		if (GetSprite().has_value())
		{
			GetSprite().value().setColor(LerpColor(GetSprite().value().getColor(), sf::Color(255, 255, 255, 0), elapseTime / mLifeTime));
			GetSprite().value().setScale(LerpVector(GetSprite().value().getScale(), sf::Vector2f(0,0),elapseTime/mLifeTime));
		}
	}
}