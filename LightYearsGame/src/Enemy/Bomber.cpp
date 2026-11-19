#include "Enemy/Bomber.h"
#include "VFX/Explosion.h"
#include "framework/World.h"
namespace ly
{
	Bomber::Bomber(World* owningWorld, const std::string& texturePath, const sf::Vector2f& velocity)
		: EnemySpaceship{ owningWorld, texturePath },
		mExplosionCooldown{ 1.f },
		mExplosion{ new Explosion{
			15,											// particleAmt
			0.5f,										// lifeTimeMin
			1.f,										// lifeTimeMax
			1.f,										// sizeMin
			2.f,										// sizeMax
			150.f,										// speedMin
			300.f,										// speedMax
			sf::Color{ 0, 255, 0, 255 },				// particleColor (green)
			List<std::string>{
				"SpaceShooterRedux/PNG/Effects/star1.png",
				"SpaceShooterRedux/PNG/Effects/star2.png",
				"SpaceShooterRedux/PNG/Effects/star3.png"
			}
		} }
	{
		SetVelocity(velocity);
	}
	void Bomber::Tick(float deltaTime)
	{
		EnemySpaceship::Tick(deltaTime);
		Shoot();
	}
	void Bomber::Shoot()
	{
		if (mExplosionClock.getElapsedTime().asSeconds() > mExplosionCooldown)
		{
			mExplosionClock.restart();
			mExplosion->SpawnExplosion(GetWorld(), GetActorLocation());
		}
	}
}