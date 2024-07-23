# PGE Template Project v2.25

This is a template project for use with the [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). It serves as a jumping off point for you to build your masterpiece application.

## Features

CMake script for cross-platform building. Tested environments include:

* Linux - with UNIX Makefiles, GNU GCC and LLVM Clang
* MacOS - with UNIX Makefiles, XCode and LLVM Clang
* Windows - with Visual Studio, NMake Makefiles, and MinGW Makefiles
* Emscripten - with UNIX Makefiles, NMake Makefiles, and MinGW Makefiles

# Preparing your Environment

The instructions to prepare your environment have been broken up for convenience. Simply follow the instructions that are pertinent to your situation.

## Linux

### **Requirements**

* C/C++ Toolchain for your Linux distro
* CMake
* Git
* libpng
* Mesa OpenGL Development Libraries

### **Ubuntu and Ubuntu based distros**

Update your package manager by issuing the following command:

```
sudo apt update
```

Install toolchain and required software by issuing the following command:

```
sudo apt install build-essential cmake git libpng-dev libglu1-mesa-dev
```

### **Arch, Manjaro, and Arch based distros**

```
sudo pacman -Sy base-devel cmake git libpng mesa
```

## MacOS

### **Requirements**

* XCode
* [Homebrew Package Manager](https://brew.sh/)
* libpng
* CMake
* git

Install XCode from the App Store.

Open the ``Terminal`` App from Finder. go to Applications -> Utilities

Follow the instructions at the [Homebrew Website](https://brew.sh/) to install the Homebrew package manager.

Once Homebrew is installed, issue the following command to install ``cmake``,``libpng``, and ``git``:


```
brew install libpng 
brew install cmake
brew install git
```

## Windows

### Requirements

* Chocolatey
* CMake
* Toolchain (MinGW or Visual Studio / NMake)

The following will be required whether you use MinGW or Visual Studio.You will need to open Powershell, as Administrator.

Visit the [Chocolatey website](https://chocolatey.org/) for instructions on how to install Chocolatey.

Once you've got Chocolatey installed, we can install CMake:

```
choco install cmake
```

Say ``yes`` to all of the scripts Chocolatey wants you to run!

After the installation has completed, find the Cmake ``bin`` directory, it is typically ``C:\Program Files\CMake\bin`` and add it to your path!

Confirm CMake is installed and in your path by issuing the following command in a Command Prompt:

```
cmake --version
```

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

```
cmake . -B linux-build -G "Unix Makefiles"
```

CMake will generate UNIX Makefiles you can use to build the project, like so:

```
cmake --build linux-build
```

The compiled binary will be located in ``linux-build/bin`` directory.

**NOTE: if you're executing the program, ensure you have the correct working directory, which contains the executable!**

## MacOS (with XCode)

Open a Terminal and navigate to the directory which you downloaded the project. Issue the following command:

```
cmake . -B xcode-build -G "xcode"
```

CMake will generate an XCode project in ``xcode-build``. You can use it like any other XCode project.

## Linux / MacOS (Emscripten)

**These instructions assume you have Emscripten installed, activated, and have the environment set up for an active Terminal.**

Open a Terminal and navigate to the directory which you downloaded the project. Issue the following command:

```
emcmake cmake . -B emscripten-build
```

Emscripten's ``emcmake`` utility will invoke CMake with all the magic required to make it work with Emscripten. Generating UNIX Makefiles you can use to build the project, like so:

```
cmake --build emscripten-build
```

The compiled HTML, Javascript, WebAssembly, and Data will be in the ``emscripten-build/bin`` directory.

If you lack some sort of live server extension to your IDE, you can view it using the ``emrun`` utility, like so:

```
emrun path/to/build/bin/PROJECTNAME.html
```

This command should launch the project in your default web browser.

## Windows (MinGW)

Open the ``Command Prompt`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

```
cmake . -B mingw-build -G "MinGW Makefiles"
```

CMake will generate MinGW Makefiles you can use to build the project, like so:

```
cmake --build mingw-build
```

The compiled binary will be located in the ``mingw-build/bin`` directory.

**NOTE: if you're executing the program, ensure you have the correct working directory, which contains the executable!**

## Windows (NMake)

Open the ``x64 Native Tools Command Prompt for VS 2022`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

```
cmake . -B nmake-build -G "NMake Makefiles"
```

CMake will generate NMake Makefiles you can use to build the project, like so:

```
cmake --build nmake-build 
```

The compiled binary will be located in ``nmake-build/bin`` directory.

**NOTE: if you're executing the program, ensure you have the correct working directory, which contains the executable!**

## Windows (Visual Studio)

Open the ``Command Prompt`` prompt and navigate to the directory which you downloaded the project. Issue the following command:

```
cmake . -B vs-build
```

CMake will generate a Visual Studio solution and project in ``vs-build``. You can use it like any other Visual Studio Project.

