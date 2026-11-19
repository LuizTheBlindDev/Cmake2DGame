#include "Enemy/SpeedyStage.h"
#include "framework/World.h"
#include "Enemy/Speedy.h"
#include "Enemy/Bomber.h"
namespace ly
{
	SpeedyStage::SpeedyStage(World* world)
		: GameStage{ world },
		mSpawnInterval{ 1.5f },
		mSwitchInterval{ 5.f },
		mSpawnDistanceToEdge{ 100.f },
		mRowVerticalOffset{ 60.f },
		mLeftSpawnLoc{ 0.0f, 0.0f },
		mCenterSpawnLoc{ 0.0f, 0.0f },
		mRightSpawnLoc{ 0.0f, 0.0f },
		mSpawnLoc{ 0.0f, 0.0f },
		mRowsToSpawn{ 3 },
		mRowSpawnCount{ 0 },
		mEnemiesPerRow{ 2 },
		mCurrentRowEnemyCount{ 0 },
		mSpawnIndex{ 0 }
	{
	}
	void SpeedyStage::StartStage()
	{
		auto windowSize = GetWorld()->GetWindowSize();
		mLeftSpawnLoc = sf::Vector2f{ mSpawnDistanceToEdge, -100.f };
		mCenterSpawnLoc = sf::Vector2f{ windowSize.x / 2.f, -100.f };
		mRightSpawnLoc = sf::Vector2f{ windowSize.x - mSpawnDistanceToEdge, -100.f };
		SwitchRow();
	}
	void SpeedyStage::SpawnEnemy()
	{
		// Row 0 and 1: Speedys (2 per row, alternating left/right)
		// Row 2: Bombers (3 per row, left/center/right)
		if (mRowSpawnCount < 2)
		{
			// Spawn Speedy at predetermined location for this row
			weak<Speedy> newSpeedy = GetWorld()->SpawnActor<Speedy>();
			newSpeedy.lock()->SetActorLocation(mSpawnLoc);
		}
		else
		{
			// Spawn Bomber - determine location based on position in row
			sf::Vector2f bomberSpawnLoc;
			if (mCurrentRowEnemyCount == 0)
				bomberSpawnLoc = mLeftSpawnLoc;
			else if (mCurrentRowEnemyCount == 1)
				bomberSpawnLoc = mCenterSpawnLoc;
			else
				bomberSpawnLoc = mRightSpawnLoc;
			weak<Bomber> newBomber = GetWorld()->SpawnActor<Bomber>();
			newBomber.lock()->SetActorLocation(bomberSpawnLoc);
		}
		++mCurrentRowEnemyCount;
		// Check if row is complete
		int enemiesInCurrentRow = (mRowSpawnCount < 2) ? 2 : 3;
		if (mCurrentRowEnemyCount == enemiesInCurrentRow)
		{
			TimerManager::Get().ClearTimer(mSpawnTimerHandle);
			mSwitchTimerHandle = TimerManager::Get().SetTimer(GetWeakRef(), &SpeedyStage::SwitchRow, mSwitchInterval, false);
			mCurrentRowEnemyCount = 0;
		}
	}
	void SpeedyStage::StageFinished()
	{
		TimerManager::Get().ClearTimer(mSpawnTimerHandle);
		TimerManager::Get().ClearTimer(mSwitchTimerHandle);
	}
	void SpeedyStage::SwitchRow()
	{
		if (mRowSpawnCount == mRowsToSpawn)
		{
			FinishStage();
			return;
		}
		// Adjust vertical position for each row to prevent spatial overlap
		float currentRowY = -100.f - (mRowSpawnCount * mRowVerticalOffset);
		// Set spawn locations for current row
		mLeftSpawnLoc.y = currentRowY;
		mCenterSpawnLoc.y = currentRowY;
		mRightSpawnLoc.y = currentRowY;
		// Determine spawn pattern for current row
		if (mRowSpawnCount < 2)
		{
			// Rows 0-1: Speedys (2 per row, alternating left/right)
			mSpawnIndex = mRowSpawnCount % 2;
			mSpawnLoc = (mSpawnIndex == 0) ? mLeftSpawnLoc : mRightSpawnLoc;
		}
		else
		{
			// Row 2: Bombers (3 per row, left/center/right)
			mSpawnIndex = 0;
			mSpawnLoc = mLeftSpawnLoc;
		}
		mSpawnTimerHandle = TimerManager::Get().SetTimer(GetWeakRef(), &SpeedyStage::SpawnEnemy, mSpawnInterval, true);
		++mRowSpawnCount;
	}
}