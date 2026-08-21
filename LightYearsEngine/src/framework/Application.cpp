#include <iostream>
#include "framework/Core.h"
#include "framework/Application.h"
#include "framework/World.h"
#include "framework/AssetManager.h"
#include "framework/PhysicsSystem.h"
#include "framework/TimerManager.h"

namespace ly
{

    Application::Application(unsigned int windowWidth, unsigned int windowHeight, const std::string& title, std::uint32_t style)
        : mWindow{ sf::VideoMode({windowWidth, windowHeight}), title , style },
        mTargetFrameRate{ 60.f },
        mTickClock{},
        currentWorld{nullptr},
        mCleanCycleClock{},
        mCleanCycleInterval{2.f}
    {

    }

    void Application::Run()
	{
        mTickClock.restart();
        float accumulatedTime = 0.f;
        float targetDeltaTime = 1.f / mTargetFrameRate;

        while (mWindow.isOpen())
        {
            while (const std::optional event = mWindow.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    mWindow.close();
                }
                else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        mWindow.close();
                }
            }

            float frameDeltaTime = mTickClock.restart().asSeconds();
            accumulatedTime += frameDeltaTime;
            while (accumulatedTime > targetDeltaTime)
            {
                accumulatedTime -= targetDeltaTime;
                TickInternal(targetDeltaTime);
                RenderInternal();
            }
        }
	}

    sf::Vector2u Application::GetWindowSize() const
    {
        return mWindow.getSize();
    }

    void Application::TickInternal(float deltaTime)
    {
        Tick(deltaTime);

        if (currentWorld)
        {
            currentWorld->TickInternal(deltaTime);
        }

        TimerManager::Get().UpdateTimer(deltaTime);

        PhysicsSystem::Get().Step(deltaTime);

        if (mCleanCycleClock.getElapsedTime().asSeconds() >= mCleanCycleInterval)
        {
            mCleanCycleClock.restart();
            AssetManager::Get().CleanCycle();
            if (currentWorld)
            {
                currentWorld->CleanCycle();
            }
        }
    }

    void Application::RenderInternal()
    {
        mWindow.clear();

        Render();

        mWindow.display();
    }

    void Application::Render()
    {
        if (currentWorld)
        {
            currentWorld->Render(mWindow);
        }
    }

    void Application::Tick(float deltaTime)
    {


    }
}