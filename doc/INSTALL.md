# Choose your way of installation

1. [Install from binaries](#Install-from-binaries)

   This is the easiest way, and is recommended for most users.

2. [Build from source](#Build-from-source)

   In this way, you can get the latest unreleased features and get ready for contributing. It needs more steps, and is the only choice if your OS is not officially supported.

3. [Use Artifacts](#Use-Artifacts)

    In this way, you can get the latest unreleased features without building by yourself. But not all operating systems support this, and it's only available for updating, not for the first installation.

4. [Build from AUR](#Build-from-AUR)

   This is the way recommended for Arch Linux users. You can use an AUR helper to update the package and get the latest features. What's more, since the AppImage is built on Ubuntu, you can only use the fusion theme, but if it's built on Arch Linux with KDE you can use other themes like Breeze.

## Install from binaries

Choose a version in the [Releases](https://github.com/cpeditor/cpeditor/releases), then download your binary.

### Windows

Download the `setup.exe` in the assets and install it. You may also need to install [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

### Linux

Download the `AppImage` in the assets, add execute permission to it (`chmod +x <AppImagePath>`), then run it.

### MacOS

Download the `.dmg` file in the assets and install it.

You may need to click "Yes" to access Filesystem several times. Apple may report that this application does not comes from recognised developer and prevent you from launching app. Please add an exception to the Editor from `Settings -> Security -> General`, and allow CP Editor to `run anyways`

## Build from source

1. Clone this repo and submodules:

	```sh
	git clone --recurse-submodules https://github.com/cpeditor/cpeditor.git
	cd cpeditor
	```

2. Install [Qt](https://www.qt.io/download) (5.12 or higher), [CMake](https://cmake.org/download/) (3.12 or higher) and [Python3](https://www.python.org/downloads/).
   - On some Linux distributions and MacOS, you can install from your package manager. For example, `sudo pacman -S qt5` on Arch Linux, `brew install qt5` on Mac OS.
   - You can also download the [offline installer](https://www.qt.io/offline-installers), or download from the [mirrors](https://download.qt.io/static/mirrorlist/). The path from the root of the mirror should be like `/qt/official_releases/qt/5.14/5.14.1/qt-opensource-<platform>-5.14.1.<suffix>` (or other versions).
   - You can also use [aqtinstall](https://github.com/miurahr/aqtinstall) to install Qt.

3. If CMake can't find the Qt installation path, you should set environment variable: `CMAKE_PREFIX_PATH=%QtPath%/%QtVersion%/%Compiler%/lib/cmake`. For example, on Mac, you can run something like `export CMAKE_PREFIX_PATH="/usr/local/Cellar/qt/5.14.1"`.

4. Run the following commands:

	- Linux/MacOS:

		```sh
		mkdir build
		cd build
		cmake .. -DCMAKE_BUILD_TYPE=Release
		cmake --build .
		```

	- Windows:

		```bat
		mkdir build
		cd build
		cmake ..
		cmake --build . --config Release
		```

On Linux, you will get `build/cpeditor`.

On Mac, you will get `build/cpeditor.app`.

On Windows, you will get `build/cpeditor.exe`, or `build/Release/cpeditor.exe`. You may need to gather the necessary DLLs. If you have installed CP Editor by a `setup.exe`, you can copy the DLLs from where CP Editor is installed (or copy the executable file to the installation path).

## Use Artifacts

If you want to use the latest version (even later than the beta release), but don't want to build from source, you can download Artifacts from [GitHub Actions](https://github.com/cpeditor/cpeditor/actions).

However, it's not recommended to use Artifacts, they are more unstable than beta releases, and may be a work in progress. You should read the commit logs and even the source codes before using Artifacts.

In order to run it on Windows/MacOS, you should install from [Releases](https://github.com/cpeditor/cpeditor/releases) first (usually any version is OK, unless something like DLL missing happens). On Windows, replace `cpeditor.exe` in your install folder with the Artifact. On Mac, run `cp path/to/Artifact cpeditor.app/Contents/MacOs`, where `cpeditor.app` is the one already installed on your machine.

In order to run it on Ubuntu 18.04, you need to install Qt. Other Linux distributions may not support binaries compiled on Ubuntu 18.04.

## Build from AUR

This method is only for Arch Linux (or other distros based on Arch Linux, including Manjaro) users.

There are two AUR packages for CP Editor: [cpeditor](https://aur.archlinux.org/packages/cpeditor/) for the stable version and [cpeditor-git](https://aur.archlinux.org/packages/cpeditor-git/) for the alpha version.

Just like installing other packages from AUR, you need an AUR helper, if you don't have one, you can install yay by `sudo pacman -S yay`.

Then, run either `yay -S cpeditor` or `yay -S cpeditor-git`, choose the default options, and wait until the build finishes.

After the installation, you'll be able to run CP Editor by `cpeditor` in your terminal.

# Get Started

You may want to [Get Started](MANUAL.md#Get-Started) after installing.
