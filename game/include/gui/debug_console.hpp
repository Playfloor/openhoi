// Copyright 2018-2019 the openhoi authors. See COPYING.md for legal info.

#pragma once

#include <OgreLog.h>
#include <imgui.h>

#include <string>
#include <vector>

#include "gui/gui_element.hpp"

namespace openhoi {

// Debug console GUI element
class DebugConsole final : public GuiElement {
 public:
  // Creates the debug console
  DebugConsole();

  // Destroys the debug console
  ~DebugConsole();

  // Add log to debug console
  void addLog(Ogre::LogMessageLevel lml, const char* fmt, ...);

  // Clear debug console log
  void clearLog();

  // Draw the debug console (inherited from GuiElement)
  virtual void draw();

 private:
  // Command line input callback
  int textEditCallback(ImGuiInputTextCallbackData* data);

  char inputBuffer[256];
  std::vector<std::string> items;
  std::vector<std::string> history;
  int historyPos;
  ImGuiTextFilter filter;
};

}  // namespace openhoi