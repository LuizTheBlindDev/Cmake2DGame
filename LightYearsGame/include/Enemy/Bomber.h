#pragma once
#include "spaceship/Spaceship.h"
#include "Enemy/EnemySpaceship.h"
namespace ly
{
	class Explosion;
	class Bomber : public EnemySpaceship
	{
	public:
		Bomber(World* owningWorld, const std::string& texturePath = "SpaceShooterRedux/PNG/Enemies/enemyGreen4.png", const sf::Vector2f& velocity = { 0.f, 85.f });
		virtual void Tick(float deltaTime) override;
	private:
		virtual void Shoot() override;
		sf::Clock mExplosionClock;
		float mExplosionCooldown;
		unique<Explosion> mExplosion;
	};
}
