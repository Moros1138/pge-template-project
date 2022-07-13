# PGE Template Project

This is a template project for use with the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). It serves as a jumping off point for you to build your masterpiece application.

## Features

CMake script for cross-platform building. Tested environments include:

* Linux - with UNIX Makefiles, GNU GCC and LLVM Clang
* MacOS - with UNIX Makefiles, XCode and LLVM Clang
* Windows - with Visual Studio, NMake Makefiles, and MinGW Makefiles
* Emscripten - with UNIX Makefiles, NMake Makefiles, and MinGW Makefiles

# Preparing your Environment

## Linux

### **Requirements**

* C/C++ Toolchain for your Linux distro
* Mesa OpenGL Development Libraries
* libpng
* Git
* CMake

### **Ubuntu and Ubuntu based distros**

Update your package manager by issuing the following command:

``sudo apt update``

Install toolchain and required software by issuing the following command:
``sudo apt install build-essential libglu1-mesa-dev libpng-dev git cmake``

### **Arch and Arch based distros**

``pacman -Sy base-devel libpng mesa git cmake``

## MacOS

### **Requirements**

* XCode (Installed from App Store)
* [HomeBrew](https://brew.sh/)
* libpng
* CMake

Open the ``Terminal`` App from Finder. go to Applications -> Utilities

``brew install libpng cmake``

## Windows

### Requirements

The following will be required whether you use MinGW or Visual Studio.You will need to open Powershell, as Administrator.

Visit the [Chocolatey website](https://chocolatey.org/) for instructions on how to install Chocolatey.

Once you've got Chocolatey installed, we can install CMake:

``choco install cmake``

Say ``yes`` to all of the scripts Chocolatey wants you to run!

After the installation has completed, find the Cmake ``bin`` directory, it is typically ``C:\Program Files\CMake\bin`` and add it to your path!

Confirm CMake is installed and in your path by issuing the following command in a Command Prompt:

``cmake --version``

If you recieve an ``command not found`` error double check that you have actually added CMake to your path.

## MinGW

Install MinGW via ``choco install mingw`` from Powershell as Administrator


## Visual Studio / NMake

Download and install [Visual Studio: Community Edition](https://visualstudio.microsoft.com/downloads/).

Ensure that you have installed the Desktop C++ option!

# **Usage**

IF YOU HAVE MADE IT HERE AND YOU HAVE NOT SET UP YOUR DEVELOPMENT ENVIRONMENT, GO BACK UP AND READ THE INSTRUCTIONS AGAIN!

## Linux / MacOS (with default toolchains)

Open a Terminal and navigate to the directory which you downloaded the project. Issue the following command:

``cmake .``

CMake will generate UNIX Makefiles you can use to build the project, like so:

``make``

The compiled binary will be located in ``bin`` directory.

## Linux / MacOS (Emscripten)

**These instructions assume you have Emscripten installed, activated, and have the environment set up for an active Terminal.**

Open a Terminal and navigate to the directory which you downloaded the project. Issue the following command:

``emcmake cmake .``

Emscripten's ``emcmake`` utility will invoke CMake with all the magic required to make it work with Emscripten. Generating UNIX Makefiles you can use to build the project, like so:

``make``

The compiled HTML, Javascript, WebAssembly, and Data will be in the ``bin`` directory.

If you lack some sort of live server extension to your IDE, you can view it using the ``emrun`` utility, like so:

``emrun bin/PROJECTNAME.html``

This command should launch the project in your default web browser.

## Windows (MinGW)

Open the ``Command Prompt`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

``cmake . -G "MinGW Makefiles"``

CMake will generate MinGW Makefiles you can use to build the project, like so:

``mingw32-make``

The compiled binary will be located in the ``bin`` directory.

## Windows (NMake)

Open the ``x64 Native Tools Command Prompt for VS 2022`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

``cmake . -G "NMake Makefiles"``

CMake will generate NMake Makefiles you can use to build the project, like so:

``nmake``

The compiled binary will be located in ``bin`` directory.

## Windows (Visual Studio)

Open the ``Command Prompt`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

``cmake .``

CMake will generate a Visual Studio solution and project. You can use it like any other Visual Studio Project.

