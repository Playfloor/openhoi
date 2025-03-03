// Copyright 2018-2019 the openhoi authors. See COPYING.md for legal info.

#include "hoibase/helper/os.hpp"

#include <OgreLogManager.h>
#include <stdio.h>

#include <array>
#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include <memory>

namespace openhoi {

// Execute command line program and return result
std::string OS::executeCommand(std::string command) {
  std::string result;
  std::array<char, 128> buffer;

#ifdef OPENHOI_OS_WINDOWS
  std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"),
                                                 _pclose);
#else
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"),
                                                pclose);
#endif

  if (!pipe) throw std::runtime_error("popen() failed!");

  while (fgets(buffer.data(), (int)buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }

  boost::trim_right(result);
  boost::trim_left(result);
  return result;
}

#if defined(OPENHOI_OS_LINUX) || defined(OPENHOI_OS_BSD)
// Check if system is running systemd
bool OS::isRunningSystemd() {
  std::string pid = OS::executeCommand("ps -ax | grep \"[s]ystemd\"");
  return !pid.empty();
}
#endif

// Checks if we are currently running inside a virtual machine
bool OS::isRunningInVirtualMachine() {
#if defined(OPENHOI_OS_LINUX) || defined(OPENHOI_OS_BSD)
  // Check for installed and running systemd
  if (OS::isRunningSystemd()) {
    // Yes, detect virtualization with help of systemd
    std::string virt = OS::executeCommand("systemd-detect-virt");
    if (!virt.empty() && virt != "none") {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          (boost::format(
               "*** RUNNING INSIDE VIRTUALIZATION PLATFORM '%s' ***") %
           virt)
              .str());
      return true;
    }
  }
#  ifdef OPENHOI_OS_LINUX
  else {
    // Do some (more or less reliable) checks for virtualization platforms

    std::string dmesgOutput = OS::executeCommand("dmesg | grep VirtualBox");
    if (!dmesgOutput.empty()) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          "*** RUNNING INSIDE VIRTUALIZATION PLATFORM 'VirtualBox' ***");
      return true;
    }
    dmesgOutput = OS::executeCommand("dmesg | grep 'Virtual CPU'");
    if (!dmesgOutput.empty()) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          "*** RUNNING INSIDE VIRTUALIZATION PLATFORM 'Virtual PC' ***");
      return true;
    }
    dmesgOutput = OS::executeCommand("dmesg | grep 'VMware Virtual'");
    if (!dmesgOutput.empty()) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          "*** RUNNING INSIDE VIRTUALIZATION PLATFORM 'VMware' ***");
      return true;
    }
    dmesgOutput = OS::executeCommand("dmesg | grep 'Xen virtual'");
    if (!dmesgOutput.empty()) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          "*** RUNNING INSIDE VIRTUALIZATION PLATFORM 'Xen' ***");
      return true;
    }
    dmesgOutput = OS::executeCommand("dmesg | grep QEMU");
    if (!dmesgOutput.empty()) {
      Ogre::LogManager::getSingletonPtr()->logMessage(
          "*** RUNNING INSIDE VIRTUALIZATION PLATFORM 'QEmu' ***");
      return true;
    }
  }
#  endif
#endif

  // Return FALSE in case no checks were applied (e.g. OS not supported) or if
  // we are not inside a VM
  return false;
}

}  // namespace openhoi
