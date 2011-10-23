// ======================================================================== //
// Copyright (C) 2011 Benjamin Segovia                                      //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __PF_EVENT_HPP__
#define __PF_EVENT_HPP__

#include "sys/tasking.hpp"

namespace pf {

  /*! Contains the fields processed by the event handler */
  class InputEvent : public RefCount
  {
  public:
    InputEvent(InputEvent *previous = NULL);
    INLINE bool getKey(int32 key) {
      const int entry = key / 32; // int32 == 32 bits
      const int bit = key % 32;
      return (this->keys[entry] & (1u << bit)) != 0;
    }
    INLINE void setKey(int32 key) {
      const int entry = key / 32; // int32 == 32 bits
      const int bit = key % 32;
      if (key)
        this->keys[entry] |=  (1u << bit);
      else
        this->keys[entry] &= ~(1u << bit);
    }
    enum { MAX_KEYS = 256u };
    enum { KEY_ARRAY_SIZE = MAX_KEYS / sizeof(int32) };
    double time;                //!< Current time when capturing the events
    double dt;                  //!< Delta from the previous frame
    int32 keys[KEY_ARRAY_SIZE]; //!< Bitfield saying if key is pressed?
    int mouseX, mouseY;         //!< Absolute position of the mouse
    int mouseXRel, mouseYRel;   //!< Position of the mouse (relative to previous frame)
    int w, h;                   //!< Dimension of the window (if changed)
    char isMouseInit:1;         //!< Typically false when the mouse exits the windows
    char isResized:1;           //!< True if the user resized the window
  };

  /*! Record all events (keyboard, mouse, resizes) */
  class TaskEvent : public Task
  {
  public:
    /*! Previous events are used to get delta values */
    TaskEvent(Ref<InputEvent> current, Ref<InputEvent> previous = NULL);

    /*! Register all events relatively to the previous ones (if any) */
    virtual Task *run(void);

    /*! Mainly to initialize the underlying API */
    static void init(void);
    static void keyDown(unsigned char key, int x, int y);
    static void keyUp(unsigned char key, int x, int y);
    static void mouse(int button, int state, int x, int y);
    static void reshape(int w, int h);
    static void entry(int state);
    static void motion(int x, int y);

    /*! Both events are needed to handle deltas */
    Ref<InputEvent> current, previous;
  };
}

#endif /* __PF_EVENT_HPP__ */
