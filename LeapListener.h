//
//  LeapListener.h
//  CSE165 HW1
//
//  Created by Ruiqing Qiu on 1/20/15.
//  Copyright (c) 2015 Ruiqing Qiu. All rights reserved.
//

#ifndef CSE165_HW1_LeapListener_h
#define CSE165_HW1_LeapListener_h

#include <iostream>
#include <string.h>
#include "Leap.h"

using namespace Leap;

class LeapListener : public Listener {
public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
    
private:
};

#endif
