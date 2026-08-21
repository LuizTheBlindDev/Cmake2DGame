#include <box2D/types.h>
#include <box2D/base.h>
#include <box2D/box2d.h>
#include <box2D/id.h>

#include "framework/PhysicsSystem.h"
#include "framework/Actor.h"
#include "framework/MathUtility.h"

namespace ly
{
	unique<PhysicsSystem> PhysicsSystem::physicsSystem{ nullptr };

	PhysicsSystem& PhysicsSystem::Get()
	{
		if (!physicsSystem)
		{
			physicsSystem = std::move(unique<PhysicsSystem>{new PhysicsSystem});
		}
		return *physicsSystem;
	}

	void PhysicsSystem::Step(float deltaTIme)
	{
		ProcessPendingRemoveListeners();
		b2World_Step(mPhysicsWorldId, deltaTIme, mSubStepCount);
		b2ContactEvents contactEvents = b2World_GetContactEvents(mPhysicsWorldId);
		HandleContactEvent(contactEvents);
	}

	void PhysicsSystem::ProcessPendingRemoveListeners()
	{
		for (auto actorBodyId : mPendingRemoveListener)
		{
			if (B2_IS_NON_NULL(actorBodyId))
			{
				b2DestroyBody(actorBodyId);
			}
		}
		mPendingRemoveListener.clear();
	}

	void PhysicsSystem::HandleContactEvent(b2ContactEvents& contactEvents)
	{
		for (int i = 0; i < contactEvents.beginCount; ++i)
		{
			b2ContactBeginTouchEvent* beginEvent = contactEvents.beginEvents + i;
			ShapesStartTouching(beginEvent->shapeIdA, beginEvent->shapeIdB);
		}

		for (int i = 0; i < contactEvents.endCount; ++i)
		{
			b2ContactEndTouchEvent* endEvent = contactEvents.endEvents + i;

			// Use b2Shape_IsValid because a shape may have been destroyed
			if (b2Shape_IsValid(endEvent->shapeIdA) && b2Shape_IsValid(endEvent->shapeIdB))
			{
				ShapesStopTouching(endEvent->shapeIdA, endEvent->shapeIdB);
			}
		}
	}

	void PhysicsSystem::ShapesStartTouching(b2ShapeId shapeA, b2ShapeId shapeB)
	{
		Actor* ActorA = reinterpret_cast<Actor*>(b2Body_GetUserData(b2Shape_GetBody(shapeA)));
		Actor* ActorB = reinterpret_cast<Actor*>(b2Body_GetUserData(b2Shape_GetBody(shapeB)));

		if (ActorA && !ActorA->isPendingDestroy())
		{
			ActorA->OnActorBeginOverlap(ActorB);
		}

		if (ActorB && !ActorB->isPendingDestroy())
		{
			ActorB->OnActorBeginOverlap(ActorA);
		}
	}

	void PhysicsSystem::ShapesStopTouching(b2ShapeId shapeA, b2ShapeId shapeB)
	{
		Actor* ActorA = nullptr;
		Actor* ActorB = nullptr;

		ActorA = reinterpret_cast<Actor*>(b2Body_GetUserData(b2Shape_GetBody(shapeA)));
		ActorB = reinterpret_cast<Actor*>(b2Body_GetUserData(b2Shape_GetBody(shapeB)));

		if (ActorA && !ActorA->isPendingDestroy())
		{
			ActorA->OnActorEndOverlap(ActorB);
		}

		if (ActorB && !ActorB->isPendingDestroy())
		{
			ActorB->OnActorEndOverlap(ActorA);
		}
	}

	b2BodyId PhysicsSystem::AddListener(Actor* listener)
	{
		if (listener->isPendingDestroy()) return b2_nullBodyId;

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = b2_dynamicBody;

		bodyDef.userData = listener;
		bodyDef.position = {
			listener->GetActorLocation().x * GetPhysicsScale(),
			listener->GetActorLocation().y * GetPhysicsScale()
		};
		bodyDef.rotation = b2MakeRot(DegreesToRadians(listener->GetActorRotation()));

		b2BodyId bodyId = b2CreateBody(mPhysicsWorldId, &bodyDef);

		auto bound = listener->GetActorGlobalBounds();
		b2Polygon dynamicBox = b2MakeBox(bound.size.x / 2.f * GetPhysicsScale(), bound.size.y / 2.f * GetPhysicsScale());

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = 1.0f;
		shapeDef.material.friction = 0.3f;
		shapeDef.isSensor = false;
		shapeDef.enableContactEvents = true;

		b2CreatePolygonShape(bodyId, &shapeDef, &dynamicBox);

		return bodyId;
	}

	void PhysicsSystem::RemoveListener(b2BodyId& bodyToRemove)
	{
		if(B2_IS_NON_NULL(bodyToRemove))
		{
			if(VerifyRemoveListenerList(bodyToRemove))
			{
				mPendingRemoveListener.push_back(bodyToRemove);
			}
		}
	}

	bool PhysicsSystem::VerifyRemoveListenerList(b2BodyId bodyToRemove)
	{
		for (auto body: mPendingRemoveListener)
		{
			if (B2_ID_EQUALS(body, bodyToRemove))
			{
				return false;
			}
		}
		return true;
	}

	void PhysicsSystem::Cleanup()
	{
		physicsSystem = std::move(unique<PhysicsSystem>{new PhysicsSystem});
	}

	PhysicsSystem::PhysicsSystem()
		:mPhysicsWorld{ b2DefaultWorldDef() },
		mPhysicsWorldId{ b2CreateWorld(&mPhysicsWorld) },
		mPhysicsScale{0.01f},
		mSubStepCount{4},
		mPendingRemoveListener{}
	{
		mPhysicsWorld.enableSleep = false;
	}
}