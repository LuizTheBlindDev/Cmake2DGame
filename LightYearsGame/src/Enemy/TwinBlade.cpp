#include "Enemy/TwinBlade.h"
#include "weapon/BulletShooter.h"
#include "SFML/System.hpp"

namespace ly
{
	TwinBlade::TwinBlade(World* owningWorld, const std::string& texturePath, const sf::Vector2f& velocity)
		: EnemySpaceship{owningWorld, texturePath},
		mShooterLeft{ new BulletShooter{this, 1.f} },
		mShooterRight{ new BulletShooter{this, 1.f} }
	{
		sf::Vector2f localPositionOffset = { 50.f,-20.f };
		mShooterLeft.get()->setLocalPositionOffset(localPositionOffset);
		
		localPositionOffset = { 50.f,20.f };
		mShooterRight.get()->setLocalPositionOffset(localPositionOffset);

		SetVelocity(velocity);
	}

	void TwinBlade::Tick(float deltaTime)
	{
		EnemySpaceship::Shoot();
		Shoot();
	}

	void TwinBlade::Shoot()
	{
		mShooterLeft->Shoot();
		mShooterRight->Shoot();
	}
}