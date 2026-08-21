#pragma Once
#include "framework/World.h"
#include "framework/TimerManager.h"

namespace ly
{
	class PlayerSpaceship;
	class GameLevelOne : public World
	{
	public:
		GameLevelOne::GameLevelOne(Application* owningApp);

	private:
		virtual void BeginPlay() override;
		weak<PlayerSpaceship> testPlayerSpaceship;
		TimerHandle mTimerHandle_test;
		virtual void InitGameStages() override;
	};
}