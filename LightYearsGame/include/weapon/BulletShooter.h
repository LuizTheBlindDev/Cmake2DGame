#pragma once
#include "SFML/System.hpp"
#include "weapon/Shooter.h"

namespace ly
{
	class BulletShooter : public Shooter
	{
	public:
		BulletShooter(Actor* owner, float cooldownTime = 2.f, const sf::Vector2f& localPositionOffset = { 0.f,0.f }, float localRotationOffset = 0.f);
		virtual bool IsOnCooldown() const override;
		virtual void setLocalPositionOffset(sf::Vector2f& localPositionOffset) { mLocalPositionOffset = localPositionOffset; }
		virtual void setLocalRotationOffset(float localRotationOffset) { mLocalRotationOffset = localRotationOffset; }
	private:
		virtual void ShootImpl() override;
		sf::Clock mCooldownClock;
		float mCooldownTime;
		sf::Vector2f mLocalPositionOffset;
		float mLocalRotationOffset;
	};
}