#include <SFML/Graphics.hpp>
#include <box2D/base.h>

#include "framework/Actor.h"
#include "framework/Core.h"
#include "framework/AssetManager.h"
#include "framework/MathUtility.h"
#include "framework/World.h"
#include "framework/PhysicsSystem.h"

namespace ly
{
	Actor::Actor(World* mOwningWorld, const std::string& texturePath)
		: mOwningWorld{ mOwningWorld },
		mHasBeganPlay{ false },
		mTexture{ std::make_shared<sf::Texture>() },
		mSprite{ std::nullopt },
		mPhysicBodyId{ b2_nullBodyId },
		mPhysicsEnabled{ false },
		mTeamID{ GetNeutralTeamID() }
	{
		SetTexture(texturePath);
	}

	Actor::~Actor()
	{
		//LOG("Actor destroyed");
	}

	void Actor::BeginPlayInternal()
	{
		if (!mHasBeganPlay)
		{
			mHasBeganPlay = true;
			BeginPlay();
		}
	}

	void Actor::TickInternal(float deltaTime)
	{
		if (!isPendingDestroy())
		{
			Tick(deltaTime);
		}
	}

	void Actor::BeginPlay()
	{
	}

	void Actor::Tick(float deltaTime)
	{
	}

	void Actor::SetTexture(const std::string& texturePath)
	{

		mTexture = AssetManager::Get().LoadTexture(texturePath);
		if (!mTexture) return;
		if (mTexture != nullptr)
		{
			mSprite.emplace(*mTexture);

			int textureWidth = mTexture->getSize().x;
			int textureHeight = mTexture->getSize().y;
			mSprite->setTextureRect(sf::IntRect{ sf::Vector2i{}, sf::Vector2i{textureWidth, textureHeight} });
			CenterPivot();
		}
	}

	void Actor::Render(sf::RenderWindow& window)
	{
		if (isPendingDestroy() || !mTexture)
			return;
		
		window.draw(*mSprite);
	}

	void Actor::SetActorLocation(const sf::Vector2f& newLoc)
	{
		mSprite->setPosition(newLoc);
		UpdatePhysicsBodyTransform();
	}

	void Actor::SetActorRotation(float newRot)
	{
		sf::Angle rotation = sf::degrees(newRot);
		mSprite->setRotation(rotation);
		UpdatePhysicsBodyTransform();
	}

	void Actor::AddActorLocationOffset(const sf::Vector2f& offsetAmt)
	{
		SetActorLocation(GetActorLocation() + offsetAmt);
	}

	void Actor::AddActorRotationOffset(float offsetAmt)
	{
		SetActorRotation(GetActorRotation() + offsetAmt);
	}

	sf::Vector2f Actor::GetActorLocation() const
	{
		return mSprite->getPosition();
	}

	float Actor::GetActorRotation() const
	{
		return mSprite->getRotation().asDegrees();
	}

	sf::Vector2f Actor::GetActorFowardDirection() const
	{
		return RotationToVector(GetActorRotation());
	}

	sf::Vector2f Actor::GetActorRightDirection() const
	{
		return RotationToVector(GetActorRotation() + 90.f);
	}

	void Actor::InitializePhysics()
	{
		if (B2_IS_NULL(mPhysicBodyId))
		{
			mPhysicBodyId = PhysicsSystem::Get().AddListener(this);
		}
	}

	void Actor::UnInitializePhysics()
	{
		if (B2_IS_NON_NULL(mPhysicBodyId))
		{
			PhysicsSystem::Get().RemoveListener(mPhysicBodyId);
			mPhysicBodyId = b2_nullBodyId;
		}
	}

	void Actor::UpdatePhysicsBodyTransform()
	{
		if (B2_IS_NON_NULL(mPhysicBodyId))
		{
			float physicsScale = PhysicsSystem::Get().GetPhysicsScale();
			b2Vec2 pos{ GetActorLocation().x * physicsScale, GetActorLocation().y * physicsScale };
			float rotation = DegreesToRadians(GetActorRotation());

			b2Body_SetTransform(mPhysicBodyId,pos, b2MakeRot(rotation));
		}
	}

	void Actor::CenterPivot()
	{
		sf::FloatRect bound = mSprite->getGlobalBounds();
		mSprite->setOrigin(bound.getCenter());
	}

	bool Actor::IsActorOutOfWindowBounds(float allowance) const
	{
		float windowWidth = GetWorld()->GetWindowSize().x;
		float windowHeight = GetWorld()->GetWindowSize().y;

		float width = GetActorGlobalBounds().size.x;
		float height = GetActorGlobalBounds().size.y;

		sf::Vector2f actorPos = GetActorLocation();

		if (actorPos.x < -width -allowance || actorPos.x > windowWidth + width + allowance)
		{
			return true;
		}

		if (actorPos.y < -height -allowance || actorPos.y > windowHeight + height + allowance)
		{
			return true;
		}

		return false;
	}

	void Actor::SetEnablePhysics(bool enable)
	{
		mPhysicsEnabled = enable;
		if (mPhysicsEnabled)
		{
			InitializePhysics();
		}
		else
		{
			UnInitializePhysics();
		}
	}

	void Actor::OnActorBeginOverlap(Actor* other)
	{
		//LOG("OVERLAP");
	}

	void Actor::OnActorEndOverlap(Actor* other)
	{
		//LOG("END OVERLAP");
	}

	void Actor::Destroy()
	{
		UnInitializePhysics();
		Object::Destroy();
	}

	bool Actor::IsOtherHostile(Actor* other) const
	{
		if (other == nullptr) return false;

		if (GetTeamID() == GetNeutralTeamID() || other->GetTeamID() == GetNeutralTeamID())
		{
			return false;
		}
		return GetTeamID() != other->GetTeamID();
	}

	sf::FloatRect Actor::GetActorGlobalBounds() const
	{
		if (mSprite.has_value())
		{
			return mSprite->getGlobalBounds();
		}

		sf::FloatRect emptyReturn({ 0.0, 0.0 }, { 0.0, 0.0 });
		return emptyReturn;
	}

	sf::Vector2u Actor::GetWindowSize() const
	{
		return mOwningWorld->GetWindowSize();
	}

	void Actor::ApplyDamage(float amt)
	{
	
	}
}