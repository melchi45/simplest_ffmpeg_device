# Simplest FFmpeg AVDevice example

The simplest FFmpeg based AVDevice example
Simplest FFmpeg Device

Lei Lei Lei Xiaohua
Leixiaohua1020@126.com
Communication University of China / Digital TV Technology
Communication University of China / Digital TV Technology
Http://blog.csdn.net/leixiaohua1020


This project contains two examples of libmpdevice based on FFmpeg:
Simplest_ffmpeg_grabdesktop: Screen recording.
Simplest_ffmpeg_readcamera: read the camera.


This Solutions contains 2 examples based on FFmpeg's libavdevice:
Simplest_ffmpeg_grabdesktop: Screen Capture.
Simplest_ffmpeg_readcamera: Read Camera.

## Prerequisites:

- Windows 10, 8.1, or 7
- Visual Studio 2017 or Visual Studio 2015 Update 3
- Git
- *Optional: CMake 3.10.2

```
set DXSDK_PATH="D:\Tools\DXSDK"
set PKG_CONFIG_EXECUTABLE="D:\Tools\pkg-config\bin\pkg-config.exe"
set PKG_CONFIG_PATH="D:\workspace\ffmpeg-example\vcpkg\installed\x86-windows\lib"
```

## vcpkg

about vcpkg from this url.
 - https://blogs.msdn.microsoft.com/vcblog/2016/09/19/vcpkg-a-tool-to-acquire-and-build-c-open-source-libraries-on-windows/
 - https://docs.microsoft.com/en-us/cpp/vcpkg

install vcpkg to D:\Tools folder
```
d:
mkdir Tools
cd D:\Tools\
```

Clone vcpkg repository, then run
```
git clone https://github.com/Microsoft/vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
```

Then, to hook up user-wide integration, run (note: requires admin on first use)
```
.\vcpkg integrate install
PS D:\Tools\vcpkg> .\vcpkg integrate install
Applied user-wide integration for this vcpkg root.

All MSBuild C++ projects can now #include any installed libraries.
Linking will be handled automatically.
Installing new libraries will make them instantly available.

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

Install any packages for x86(x86-windows), x64(x64-windows), arm(arm-windows), arm64(arm64-winodws) windows with
```
.\vcpkg install pthreads:x64-windows pthreads:x86-windows
.\vcpkg install zlib:x86-windows zlib:x64-windows
.\vcpkg install sdl2:x64-windows sdl2:x86-windows
.\vcpkg install ffmpeg:x64-windows ffmpeg:x86-windows
```

Finally, create a New Project (or open an existing one) in Visual Studio 2017 or 2015. All installed libraries are immediately ready to be `#include`'d and used in your project.
For CMake projects, simply include our toolchain file. See our [using a package](docs/examples/using-sqlite.md) example for the specifics.
## Tab-Completion / Auto-Completion
`Vcpkg` supports auto-completion of commands, package names, options etc. To enable tab-completion in Powershell, use
```
.\vcpkg integrate powershell
```
and restart Powershell.

Check packages list with
```
.\vcpkg list
```

## Build for Windows

Build with IDE
```
mkdir build
cd build
cmake ..  -G "Visual Studio 15 2017" -DVCPKG_TARGET_TRIPLET=x86-windows -DCMAKE_TOOLCHAIN_FILE="D:/Tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
```