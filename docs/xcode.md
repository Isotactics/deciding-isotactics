# Creating and Running an Apple Xcode Project File

cmake can use the build configuration to create an Apple Xcode project file (*.xcodeproj).

### TL;DR
From the project's home directory (where build.sh is located):

    mkdir -p xcode && cd xcode && cmake -G Xcode .. && open isotactics.xcodeproj

This command creates and opens an Xcode project file under 'xcode/isotactics.xcodeproj'.

### What you need
- cmake 3.6.3,
- Xcode 8.3.3, and
- boost 1.55 (installed via homebrew).

### Creating the Project file

  You will first create the directory 'xcode' if it doesn't yet exist, and move into it.
  Creating the project file inside the build directory is necessary for an out-of-source build. If you create the project file from the home directory Xcode will perform an in-source build which clutters up the folders containing the source code. You also should not use the 'build' directory to prevent interaction between the build script 'build.sh' and Xcode.

    mkdir xcode
    cd xcode

  cmake has compatibility with Xcode built in, so we just need to tell cmake to create the
  Xcode project file using the -G flag. You'll also need to tell Xcode the root path of the project.

    cmake -G Xcode ..

  This will create the Xcode project file 'xcode/isotactics.xcodeproj'. You can either
  double click the project file to open it, or use 'open':

    open isotactics.xcodeproj

### Building and Running the Project in Xcode

By default Xcode will use the build target 'Debug', meaning it will create a binary

    bin/Debug/iso-decision

This is a different binary location to the shipped build script 'build.sh' which does not use build targets and outputs the binary into

    bin/iso-decision

### Building

In Xcode you can build all targets by pressing 'cmd+b' or through the menu 'Product/Build'. By default the 'ALL_BUILD' target is selected, meaning Xcode will build all targets. You select a specific target to build via menu for example 'Product/Scheme/iso-decision'.

### Running

To run for example the iso-decision target you first need to select it. Go to the menu and select 'Product/Scheme/iso-decision' or any other target you want to build. Now you can either press 'cmd+r' or select 'Product/Run' from the menu to build & run the target.

If you want to change the current **working directory** or pass **arguments** to the executable you need to edit the scheme. Press 'cmd+<' or select 'Product/Scheme/Edit Scheme' from the menu. On the left you select 'Run'. On the right top menu under 'arguments' you can set arguments passed to the executable. In the same menu under 'options' you can also change the working directory.  

### Viewing Header Files in Xcode

By default Xcode will not show the raw directory tree in the navigator on the left. The navigator will only include source files (*.c & *.cpp), but no headers. To view these directories you have to manually add them by right-clicking for example the directory 'iso-lib' and then select 'Add files to isotactics'. Select the 'iso-lib/include' folder and it will appear in the navigator.

### Clean up Xcode

To clean up the Xcode project first invoke 'sh build.sh clean' to remove the binaries. Then simply delete the 'xcode' directory:

    rm -rf xcode
