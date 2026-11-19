#pragma once
#include <SFML/Graphics.hpp>
#include "gameplay/GameStage.h"
#include "framework/TimerManager.h"
namespace ly
{
	class SpeedyStage : public GameStage
	{
	public:
		SpeedyStage(World* world);
		virtual void StartStage() override;
	private:
		float mSpawnInterval;
		float mSwitchInterval;
		float mSpawnDistanceToEdge;
		float mRowVerticalOffset;
		sf::Vector2f mLeftSpawnLoc;
		sf::Vector2f mCenterSpawnLoc;
		sf::Vector2f mRightSpawnLoc;
		sf::Vector2f mSpawnLoc;
		TimerHandle mSpawnTimerHandle;
		TimerHandle mSwitchTimerHandle;
		int mRowsToSpawn;
		int mRowSpawnCount;
		int mEnemiesPerRow;
		int mCurrentRowEnemyCount;
		// Spawn pattern for different rows
		int mSpawnIndex;
		virtual void StageFinished() override;
		void SpawnEnemy();
		void SwitchRow();
	};
}