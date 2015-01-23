/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include "LeapListener.h"

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void LeapListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void LeapListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void LeapListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void LeapListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void LeapListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
//  std::cout << "Frame id: " << frame.id()
//            << ", timestamp: " << frame.timestamp()
//            << ", hands: " << frame.hands().count()
//            << ", extended fingers: " << frame.fingers().extended().count()
//            << ", tools: " << frame.tools().count()
//            << ", gestures: " << frame.gestures().count() << std::endl;

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
  {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
    //std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
      //        << ", palm position: " << hand.palmPosition() << std::endl;
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

//    // Calculate the hand's pitch, roll, and yaw angles
//    std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
//              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
//              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
//
//    // Get the Arm bone
//    Arm arm = hand.arm();
//    std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
//              << " wrist position: " << arm.wristPosition()
//              << " elbow position: " << arm.elbowPosition() << std::endl;
//
//    // Get fingers
    const FingerList fingers = hand.fingers();
      Vector3 index_pos = Vector3(0,0,0);
      Vector3 thumb_pos = Vector3(0,0,0);
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
        const Finger finger = *fl;
//      std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
//                << " finger, id: " << finger.id()
//                << ", length: " << finger.length()
//                << "mm, width: " << finger.width() << std::endl;
        
        //The left hand pinch is used to draw or not draw
        if(hand.isLeft()){
            if(finger.type() == finger.TYPE_INDEX){
                // Get finger bones
                for (int b = 0; b < 4; ++b)
                {
                    Bone::Type boneType = static_cast<Bone::Type>(b);
                    Bone bone = finger.bone(boneType);
                    if(boneType == bone.TYPE_DISTAL){
                        index_pos.x = bone.prevJoint().x;
                        index_pos.y = bone.prevJoint().y;
                        index_pos.z = bone.prevJoint().z;
                    }
                }
            }
            else if(finger.type() == finger.TYPE_THUMB){
                for (int b = 0; b < 4; ++b)
                {
                    Bone::Type boneType = static_cast<Bone::Type>(b);
                    Bone bone = finger.bone(boneType);
                    if(boneType == bone.TYPE_DISTAL){
                        thumb_pos.x = bone.prevJoint().x;
                        thumb_pos.y = bone.prevJoint().y;
                        thumb_pos.z = bone.prevJoint().z;
                    }
                }
            }
            index_pos.print("index");
            thumb_pos.print("thumb");
            cout << "distance :" << index_pos.distance(thumb_pos) << endl;
            //if the distance is less than 60, then change draw mode
            if(index_pos.distance(thumb_pos) < 60){
                draw_mode = true;
            }
            else{
                draw_mode = false;
            }
        }
        //The pen is your index finger
        if(finger.type() == finger.TYPE_INDEX){
            // Get finger bones
            for (int b = 0; b < 4; ++b)
            {
                Bone::Type boneType = static_cast<Bone::Type>(b);
                Bone bone = finger.bone(boneType);
                
                if(boneType == bone.TYPE_DISTAL){
//                        std::cout << std::string(6, ' ') <<  boneNames[boneType]
//                            << " bone, start: " << bone.prevJoint()
//                            << ", end: " << bone.nextJoint()
//                            << ", direction: " << bone.direction() << std::endl;
                    if(hand.isRight()){
                        pos.x = bone.prevJoint().x-50;
                        if((bone.prevJoint().y - 100) < 0){
                            pos.y = 0;
                        }
                        else{
                            pos.y = bone.prevJoint().y - 100;
                        }
                        pos.z = bone.prevJoint().z;

                        x += pos.x;
                        y += pos.y;
                        z += pos.z;
                        if(draw_mode){
                            if(count == 30){
                                x = x / 30;
                                y = y / 30;
                                z = z / 30;
                                pos.print("pushing pos");
//                                pos.x = x;
//                                pos.y = y;
//                                pos.z = z;
                                sample_points.push_back(Vector3(x,y,z));
                                corresponding_colors.push_back(color);
                                count = 0;
                            }
                            else{
                                count++;
                            }
                        }
                    }
                }
            }
        }
        if(finger.type() == finger.TYPE_MIDDLE){
            // Get finger bones
                for (int b = 0; b < 4; ++b) {
                    Bone::Type boneType = static_cast<Bone::Type>(b);
                    Bone bone = finger.bone(boneType);
                    if(boneType == bone.TYPE_DISTAL){
                        //                        std::cout << std::string(6, ' ') <<  boneNames[boneType]
                        //                            << " bone, start: " << bone.prevJoint()
                        //                            << ", end: " << bone.nextJoint()
                        //                            << ", direction: " << bone.direction() << std::endl;
                        if(hand.isLeft() && color_mode){
                            color.x = abs(bone.prevJoint().x);
                            color.y = abs(bone.prevJoint().y-100);
                            color.z = abs(bone.prevJoint().z);
                            color.normalize();
                            color.print("color is ");
                        }
                    }
                }

        }
    }
  }

  // Get tools
  const ToolList tools = frame.tools();
  for (ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
    const Tool tool = *tl;
    std::cout << std::string(2, ' ') <<  "Tool, id: " << tool.id()
              << ", position: " << tool.tipPosition()
              << ", direction: " << tool.direction() << std::endl;
  }

  // Get gestures
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/2) {
          clockwiseness = "clockwise";
          color_mode = true;
        } else {
          clockwiseness = "counterclockwise";
          color_mode = false;
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        std::cout << std::string(2, ' ')
                  << "Circle id: " << gesture.id()
                  << ", state: " << stateNames[gesture.state()]
                  << ", progress: " << circle.progress()
                  << ", radius: " << circle.radius()
                  << ", angle " << sweptAngle * RAD_TO_DEG
                  <<  ", " << clockwiseness << std::endl;
        break;
      }
      case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        std::cout << std::string(2, ' ')
          << "Swipe id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", direction: " << swipe.direction()
          << ", speed: " << swipe.speed() << std::endl;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        std::cout << std::string(2, ' ')
          << "Key Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << tap.position()
          << ", direction: " << tap.direction()<< std::endl;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        std::cout << std::string(2, ' ')
          << "Screen Tap id: " << gesture.id()
          << ", state: " << stateNames[gesture.state()]
          << ", position: " << screentap.position()
          << ", direction: " << screentap.direction()<< std::endl;
        break;
      }
      default:
        std::cout << std::string(2, ' ')  << "Unknown gesture type." << std::endl;
        break;
    }
  }

  if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    //std::cout << std::endl;
  }
}

void LeapListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void LeapListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void LeapListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void LeapListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void LeapListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}
