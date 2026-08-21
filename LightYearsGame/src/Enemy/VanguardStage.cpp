#include "Enemy/VanguardStage.h"
#include "framework/World.h"
#include "Enemy/Vanguard.h"
#include "Enemy/TwinBlade.h"

namespace ly
{
	VanguardStage::VanguardStage(World* world)
		: GameStage{world},
		mSpawnInterval{1.5f},
		mSwitchInternal{5.f},
		mSpawnDistanceToEdge{100.f},
		mLeftSpawnLoc{0.0f, 0.0f},
		mRightSpawnLoc{0.0f, 0.0f},
		mSpawnLoc{ 0.0f, 0.0f },
		mRowsToSpawn{4},
		mRowSpawnCount{0},
		mVanguardPerRow{5},
		mCurrentRownVanguardCount{ 0 }
	{

	}

	void VanguardStage::StartStage()
	{
		auto windowSize = GetWorld()->GetWindowSize();
		mLeftSpawnLoc = sf::Vector2f{ mSpawnDistanceToEdge, -100.f };
		mRightSpawnLoc = sf::Vector2f{ windowSize.x - mSpawnDistanceToEdge, -100.f };

		SwitchRow();
	}

	void VanguardStage::SpawnVanguard()
	{
		weak<TwinBlade> newVanguard = GetWorld()->SpawnActor<TwinBlade>();
		newVanguard.lock()->SetActorLocation(mSpawnLoc);
		++mCurrentRownVanguardCount;
		if (mCurrentRownVanguardCount == mVanguardPerRow)
		{
			TimerManager::Get().ClearTimer(mSpawnTimerHandle);
			mSwitchTimerHandle = TimerManager::Get().SetTimer(GetWeakRef(), &VanguardStage::SwitchRow, mSwitchInternal, false);
			mCurrentRownVanguardCount = 0;
		}
	}

	void VanguardStage::StageFinished()
	{
		TimerManager::Get().ClearTimer(mSpawnTimerHandle);
		TimerManager::Get().ClearTimer(mSwitchTimerHandle);
	}

	void VanguardStage::SwitchRow()
	{
		if (mRowSpawnCount == mRowsToSpawn)
		{
			FinishStage();
			return;
		}

		if (mSpawnLoc == mLeftSpawnLoc)
		{
			mSpawnLoc = mRightSpawnLoc;
		}
		else
		{
			mSpawnLoc = mLeftSpawnLoc;
		}

		mSpawnTimerHandle = TimerManager::Get().SetTimer(GetWeakRef(), &VanguardStage::SpawnVanguard, mSpawnInterval, true);

		++mRowSpawnCount;
	}
}