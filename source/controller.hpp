#pragma once
#include <switch.h>
#include <cstring>
#include <string>
#include <memory>

extern Event vsync_event;

struct controlMsg
{
    // Joys between -32768 to 32767
    int frame;
    u64 keys;
    s32 joy_l_x;
    s32 joy_l_y;
    s32 joy_r_x;
    s32 joy_r_y;
};

class TasController
{
    private:

    u64 HdlsHandle = 0;
    HiddbgHdlsDeviceInfo device = {0};
    HiddbgHdlsState state = {0};
    static char controllerCount;

    public:
    bool attachFlag = false;

    TasController(uint8_t deviceType, uint8_t bodyR, uint8_t bodyG, uint8_t bodyB, uint8_t buttonR, uint8_t buttonG, uint8_t buttonB);
    TasController();
    ~TasController();
    void pressA();
    void pressLR();
    void attach();
    void detach();

    void waitForVsync();
    void setInputNextFrame();
    void setMotion();

    template<class T, class... Args> void runScript(Args&&... args)
    {
        auto provider = std::make_shared<T>(std::forward<Args>(args)...);
        if(!provider->isGood()) return;

        provider->populateQueue();
        std::shared_ptr<struct controlMsg> nextLine = provider->nextLine();
        provider->populateQueue();

        int currentFrame = 0;
        bool pause = false;

        while(provider->hasNextLine() || nextLine->frame >= currentFrame)
        {
            if(hidKeyboardDown(KBD_PAUSE)) pause = !pause;
            if(hidKeyboardDown(KBD_SCROLLLOCK)) break;
            if(pause) continue;
            if(nextLine->frame == currentFrame)
            {
                runMsg(nextLine);
                if(provider->hasNextLine())
                {
                    nextLine.reset();
                    nextLine = provider->nextLine();
                }
            }
            else
            {
                emptyMsg();
            }

            if(currentFrame % 8 == 0) pushProvider(provider);

            setInputNextFrame();

            currentFrame++;

        }
        emptyMsg();
        setInputNextFrame();

        nextLine.reset();

        hidScanInput();

        waitForVsync();
    }

    void runMsg(std::shared_ptr<struct controlMsg> msg);
    void emptyMsg();
};
