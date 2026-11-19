#pragma once
#include <SFML/Graphics.hpp>
#include <box2D/base.h>
#include <box2d/types.h>

#include "framework/Object.h"
#include "framework/Core.h"

namespace ly
{
	class World;
	class Actor : public Object
	{
	public:
		Actor(World* mOwningWorld, const std::string& texturePath = "");
		virtual ~Actor();
		void BeginPlayInternal();
		void TickInternal(float deltaTime);
		virtual void BeginPlay();
		virtual void Tick(float deltaTime);
		void SetTexture(const std::string& texturePath);
		void Render(sf::RenderWindow& window);

		void SetActorLocation(const sf::Vector2f& newLoc);
		void SetActorRotation(float newRot);
		void AddActorLocationOffset(const sf::Vector2f& offsetAmt);
		void AddActorRotationOffset(float offsetAmt);
		float GetActorRotation() const;
		sf::Vector2f GetActorLocation() const;
		sf::Vector2f GetActorFowardDirection() const;
		sf::Vector2f GetActorRightDirection() const;

		sf::Vector2u GetWindowSize() const;

		sf::FloatRect GetActorGlobalBounds() const;

		const World* GetWorld() const { return mOwningWorld; }
		World* GetWorld() { return mOwningWorld; }

		bool IsActorOutOfWindowBounds(float allowance = 10.f) const;

		void SetEnablePhysics(bool enable);
		virtual void OnActorBeginOverlap(Actor* other);
		virtual void OnActorEndOverlap(Actor* other);
		virtual void Destroy() override;
		static uint8 GetNeutralTeamID() { return neutralTeamID; }

		void SetTeamID(uint8 teamID) { mTeamID = teamID; }
		uint8 GetTeamID() const { return mTeamID; }
		bool IsOtherHostile(Actor* other) const;

		virtual void ApplyDamage(float amt);

		b2BodyId GetBodyID() { return mPhysicBodyId; }
		void SetBodyID(b2BodyId bodyId) { mPhysicBodyId = bodyId; }

		std::optional<sf::Sprite>& GetSprite(){ return mSprite;  }
		const std::optional<sf::Sprite>& GetSprite() const { return mSprite; }

	private:
		void OnActorOutOfBounds();
		void InitializePhysics();
		void UnInitializePhysics();
		void UpdatePhysicsBodyTransform();
		void CenterPivot();
		World* mOwningWorld;
		bool mHasBeganPlay;

		shared<sf::Texture> mTexture;
		std::optional<sf::Sprite> mSprite; //find out if its needed to create a clean cycle for the sprites
		
		b2BodyId mPhysicBodyId;
		bool mPhysicsEnabled;

		uint8 mTeamID;

		const static uint8 neutralTeamID = 255;
	};
}