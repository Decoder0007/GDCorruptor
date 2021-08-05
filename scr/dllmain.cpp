// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cocos2d.h>
#include "MinHook.h"
#include "gd.h"
#include <thread>

bool timerMode = 0;

void Random() {
    auto director = cocos2d::CCDirector::sharedDirector();
    auto Layer = (cocos2d::CCLayer*)director->getRunningScene()->getChildren()->objectAtIndex(0);
    auto randomObj = (cocos2d::CCNode*)(Layer->getChildren()->randomObject());
    auto action = (rand() % 8) + 1;
    auto winSize = director->getWinSize();

    if (action == 1) {
        randomObj->setPositionX((rand() % 1) + (winSize.width - randomObj->getScaledContentSize().width));
    }
    if (action == 2) {
        randomObj->setPositionY((rand() % 1) + (winSize.height - randomObj->getScaledContentSize().height));
    }
    if (action == 3) {
        randomObj->setScaleX((rand() % 1) + 5);
    }
    if (action == 4) {
        randomObj->setScaleY((rand() % 1) + 5);
    }
    if (action == 5) {
        randomObj->setRotationX((rand() % 360) + 1);
    }
    if (action == 6) {
        randomObj->setRotationY((rand() % 360) + 1);
    }
    if (action == 7) {
        randomObj->setSkewX((rand() % 180) + 1);
    }
    if (action == 8) {
        randomObj->setSkewY((rand() % 180) + 1);
    }
}

void Timer_Key_Thread() {
    while (1) {
        if (GetAsyncKeyState(84)) {
            if (timerMode == 0) timerMode = 1;
            else timerMode = 0;
            while (GetAsyncKeyState(84)) Sleep(100);
        }
        Sleep(100);
    }
}

void Timer_Thread() {
    while (1) {
        Sleep((rand() % 1000) + 4000);
        if (timerMode) {
            Random();
        }
    }
}

void Main_Shduff() {
    std::thread key(Timer_Key_Thread);
    key.detach();

    std::thread timer(Timer_Thread);
    timer.detach();

    while (1) {
        if (GetAsyncKeyState(82)) {

            Random();

            while (GetAsyncKeyState(82)) {
                Sleep(100);
            }
        }
        Sleep(100);
    }
}

DWORD WINAPI StartShit(void* hModule) {
    MH_Initialize();
    std::thread t(Main_Shduff);
    t.detach();
    MH_EnableHook(MH_ALL_HOOKS);
    
    return 0;

    //This line will dettach your DLL when executed. Remove if needed
    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(hModule), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0x1000, StartShit, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

