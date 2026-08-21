#include "framework/World.h"
#include "framework/Actor.h"
#include "framework/AssetManager.h"
#include "framework/TimerManager.h"
#include "Enemy/Vanguard.h"
#include "Enemy/VanguardStage.h"
#include "spaceship/Spaceship.h"
#include "player/PlayerSpaceship.h"
#include "Level/GameLevelOne.h"
#include "gameplay/GameStage.h"


namespace ly
{
	GameLevelOne::GameLevelOne(Application* owningApp)
		:World(owningApp)
	{
		testPlayerSpaceship = SpawnActor<PlayerSpaceship>();
		testPlayerSpaceship.lock()->SetActorLocation(sf::Vector2f(300.f, 490.f));

	}

	void GameLevelOne::BeginPlay()
	{

	}

	void GameLevelOne::InitGameStages()
	{
		AddStage(shared<VanguardStage>{new VanguardStage{this}});
	}
}