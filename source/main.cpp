// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <map>
 
// Include the main libnx system header, for Switch development
#include <switch.h>
 
// Include headers from other parts of the program
#include "controller.hpp"
 
using namespace std;
 
// Initialize frame counter variable
int frameCount = 0;
std::vector<TasController*> controllers;
 
extern "C"
{
    // Sysmodules should not use applet*.
    u32 __nx_applet_type = AppletType_None;
 
    // Adjust size as needed.
    #define INNER_HEAP_SIZE 0x80000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char   nx_inner_heap[INNER_HEAP_SIZE];
 
    void __libnx_init_time(void);
    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);
}
 
void __libnx_initheap(void)
{
    void*  addr = nx_inner_heap;
    size_t size = nx_inner_heap_size;
 
    // Newlib
    extern char* fake_heap_start;
    extern char* fake_heap_end;
 
    fake_heap_start = (char*)addr;
    fake_heap_end   = (char*)addr + size;
}
 
// Init/exit services, update as needed.
void __attribute__((weak)) __appInit(void)
{
    Result rc;
 
    // Initialize default services.
    rc = smInitialize();
    if (R_FAILED(rc))
        fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
 
    rc = setsysInitialize();
    if (R_SUCCEEDED(rc)) {
        SetSysFirmwareVersion fw;
        rc = setsysGetFirmwareVersion(&fw);
        if (R_SUCCEEDED(rc))
            hosversionSet(MAKEHOSVERSION(fw.major, fw.minor, fw.micro));
        setsysExit();
    }
 
    // HID
    rc = hidInitialize();
    if (R_FAILED(rc))
        fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_HID));
 
    rc = fsInitialize();
    if (R_FAILED(rc))
        fatalThrow(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
 
    fsdevMountSdmc();
 
    rc = hiddbgInitialize();
    if (R_FAILED(rc))
        fatalThrow(rc);
 
    // vsync
    rc = viInitialize(ViServiceType_System);
    if(R_FAILED(rc))
        fatalThrow(rc);
}
 
void __attribute__((weak)) userAppExit(void);
 
void __attribute__((weak)) __appExit(void)
{
    // Cleanup default services.
    fsdevUnmountAll();
    fsExit();
    hidExit();
    smExit();
}
 
class file_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "File unopenable";
    }
} f_ex;
 
void waitForVsync(Event *vsync_event) {
    Result rc = eventWait(vsync_event, UINT64_MAX);
    if(R_FAILED(rc))
        fatalThrow(rc);
}
 
// Main program entrypoint
int main(int argc, char* argv[])
{
    Event vsync_event;
    // Init other services
    ViDisplay disp;
    Result rc = viOpenDefaultDisplay(&disp);
    if(R_FAILED(rc))
        fatalThrow(rc);
 
    rc = viGetDisplayVsyncEvent(&disp, &vsync_event);
    if(R_FAILED(rc))
        fatalThrow(rc);
 
    // Initialization code can go here.
    // Skyline handle
 
    auto msg = std::make_shared<controlMsg>();
 
    // Attach Work Buffer
    rc = hiddbgAttachHdlsWorkBuffer();
    if (R_FAILED(rc))
        fatalThrow(rc);
 
    // Create new thread for running frames
    // Thread countThread;
    // rc = threadCreate(&countThread, frameIncrement, NULL, NULL, 0x4000, 49, 3);
    // if(R_FAILED(rc))
    //     fatalThrow(rc);
    // rc = threadStart(&countThread);
    // if(R_FAILED(rc))
    //     fatalThrow(rc);
 
    // Your code / main loop goes here.
    while(true)
    {
        hidScanInput();
 
        if(hidKeyboardDown(KBD_1))
        {  
            controllers.push_back(new TasController(HidDeviceType_FullKey3, 0, 0, 0, 0, 0, 0));
            controllers.back()->attach();
        }
       
        if(hidKeyboardDown(KBD_MINUS))
        {
            controllers.back()->detach();
            delete controllers.back();
            controllers.pop_back();
        }
 
        if(!controllers.empty() && controllers.back()->attachFlag)
        {
            msg->keys = hidKeysHeld(CONTROLLER_P1_AUTO);
            msg->keys &= 65535;
 
            JoystickPosition pos_left;
            JoystickPosition pos_right;
            hidJoystickRead(&pos_left, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
            hidJoystickRead(&pos_right, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);
            msg->joy_l_x = pos_left.dx;
            msg->joy_l_y = pos_left.dy;
            msg->joy_r_x = pos_right.dx;
            msg->joy_r_y = pos_right.dy;
        for(long unsigned int i = 0; i < controllers.size(); i++)
            {  
                controllers[i]->runMsg(msg);
                controllers[i]->setInputNextFrame();
            }
        }
 
        waitForVsync(&vsync_event);
    }
 
    rc = hiddbgReleaseHdlsWorkBuffer();
 
    hiddbgExit();
   
    return 0;
}
