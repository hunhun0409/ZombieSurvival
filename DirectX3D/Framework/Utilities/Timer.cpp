#include "Framework.h"

Timer::Timer()
{
    //1초동안 CPU진동수를 반환하는 함수
    QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);

    //현재 CPU진동수를 반환하는 함수
    QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

    timeScale = 1.0f / (float)periodFrequency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
    elapsedTime = (float)(curTime - lastTime) * timeScale;

    if (lockFPS != 0)
    {
        float targetFrameTime = 1.0f / (float)lockFPS;

        while (elapsedTime < targetFrameTime)
        {
            QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
            elapsedTime = (float)(curTime - lastTime) * timeScale;
        }

        elapsedTime = targetFrameTime;
    }

    lastTime = curTime;

    frameCount++;
    oneSecCount += elapsedTime;

    if (oneSecCount >= 1.0f)
    {
        frameRate = frameCount;
        frameCount = 0;
        oneSecCount = 0.0f;
    }    
}

float Timer::GetElapsedTime()
{
    if (lockFPS > 0)
        return (1.0f / lockFPS) * deltaScale;

    return min(elapsedTime, EPSILON) * deltaScale;
}