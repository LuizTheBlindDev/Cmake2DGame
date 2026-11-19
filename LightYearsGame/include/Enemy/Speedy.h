#pragma once
#include "spaceship/Spaceship.h"
#include "Enemy/EnemySpaceship.h"
namespace ly
{
	class BulletShooter;
	class Speedy : public EnemySpaceship
	{
	public:
		Speedy(World* owningWorld, const std::string& texturePath = "SpaceShooterRedux/PNG/Enemies/enemyRed1.png", const sf::Vector2f& velocity = { 0.f, 340.f });
		virtual void Tick(float deltaTime) override;
	private:
		virtual void Shoot() override;
		unique<BulletShooter> mShooterLeft;
		unique<BulletShooter> mShooterRight;
	};
}
