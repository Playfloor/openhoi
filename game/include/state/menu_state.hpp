// Copyright 2018-2020 the openhoi authors. See COPYING.md for legal info.

#pragma once

#include <OgreRectangle2D.h>

#include "gui/options_dialog.hpp"
#include "state.hpp"

namespace openhoi {

// Main menu state class
class MenuState final : public State {
 public:
  // MenuState destructor
  ~MenuState(){};

  // Called when the state is started up
  void startup();

  // Called when the state is shutting down
  void shutdown();

  // Used to create the scene
  void createScene();

  // Used to update the scene
  void updateScene(const Ogre::FrameEvent& evt);

  // Used to update the GUI of the scene
  void updateGui();

  // Used to remove the scene
  void removeScene();

 private:
  // Create background image
  void createBackground();

  // Create logo
  void createLogo();

  std::string backgroundImageName;
  std::unique_ptr<Ogre::Rectangle2D> backgroundImageRect;
  std::unique_ptr<OptionsDialog> optionsDialog;
};

}  // namespace openhoi