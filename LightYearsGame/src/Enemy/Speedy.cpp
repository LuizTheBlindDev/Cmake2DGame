#include "Enemy/Speedy.h"
#include "weapon/BulletShooter.h"
namespace ly
{
	Speedy::Speedy(World* owningWorld, const std::string& texturePath, const sf::Vector2f& velocity)
		: EnemySpaceship{ owningWorld, texturePath },
		mShooterLeft{ new BulletShooter{this, 1.f, 180.f} },
		mShooterRight{ new BulletShooter{this, 1.f, 180.f} }
	{
		sf::Vector2f localPositionOffset = { 50.f, -20.f };
		mShooterLeft->setLocalPositionOffset(localPositionOffset);
		localPositionOffset = { 50.f, 20.f };
		mShooterRight->setLocalPositionOffset(localPositionOffset);
		SetVelocity(velocity);
	}
	void Speedy::Tick(float deltaTime)
	{
		EnemySpaceship::Tick(deltaTime);
		Shoot();
	}
	void Speedy::Shoot()
	{
		mShooterLeft->Shoot();
		mShooterRight->Shoot();
	}
}