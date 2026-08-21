#pragma once
#include <box2d/box2d.h>
#include <box2d/types.h>
#include "framework/Core.h"

namespace ly
{
	class Actor;

	class PhysicsSystem
	{
	public:
		static PhysicsSystem& Get();
		void Step(float deltaTIme);
		b2BodyId AddListener(Actor* listener);
		void RemoveListener(b2BodyId& bodyToRemove);
		bool VerifyRemoveListenerList(b2BodyId bodyToRemove);
		float GetPhysicsScale() const { return mPhysicsScale; }
		static void Cleanup();
	protected:
		PhysicsSystem();
	private:
		void ProcessPendingRemoveListeners();
		void HandleContactEvent(b2ContactEvents& contactEvents);
		void ShapesStartTouching(b2ShapeId shapeA, b2ShapeId shapeB);
		void ShapesStopTouching(b2ShapeId shapeA, b2ShapeId shapeB);
		static unique<PhysicsSystem> physicsSystem;
		b2WorldDef mPhysicsWorld;
		b2WorldId mPhysicsWorldId;
		int mSubStepCount;
		float mPhysicsScale;
		List<b2BodyId> mPendingRemoveListener;

	};
}