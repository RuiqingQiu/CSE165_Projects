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
#include "Vector3.h"
#include "Brick.h"
#include <vector>
#include "main.h"
using namespace Leap;

class LeapListener : public Listener {
public:
    Vector3 pos = Vector3(0,0,0);
    Vector3 color = Vector3(1,0,0);
    bool draw_mode = false;
    bool color_mode = false;
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
    vector<Brick> blist;
    vector<Vector3> sample_points;
    vector<Vector3> corresponding_colors;
    int count = 0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
private:
};

#endif
