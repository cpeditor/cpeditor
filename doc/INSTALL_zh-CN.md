> 本页面由 [英文版本](INSTALL.md) 翻译而来，内容可能有所过时。

# 选择一种安装方式

1. [安装可执行文件](#安装可执行文件)

   这是最简单的方式，推荐大多数用户使用。

2. [从源代码构建](#从源代码构建)

   通过这种方式，你可以使用最新的还未发布的特性，同时为参与开发做好准备。这需要更多的步骤，也是在你的操作系统不被支持时的唯一方案。

3. [使用 Artifacts](#使用-artifacts)

    通过 Artifacts，无需自己构建便能使用最新的 feature。但是，并非所有平台都可以使用，而且只能用于更新，不能用于首次安装。

4. [从 AUR 构建](#从-AUR-构建)

   这是 Arch Linux 用户的推荐安装方法。你可以使用 AUR 安装器来进行更新，轻松享受最新的特性。并且，由于 AppImage 是在 Ubuntu 上构建的，只能使用 Fusion 主题，但如果是在使用 KDE 桌面环境的 Arch Linux 上构建的，就可以使用 Breeze（微风）等主题。

## 安装可执行文件

在 [发行版](https://github.com/cpeditor/cpeditor/releases) 中选择一个版本，下载适合你的平台的可执行文件。

如果你不方便在 GitHub 上下载，可以在 [Gitee](https://gitee.com/ouuan/cpeditor/releases) 上下载。

### Windows

下载 `setup.exe` 然后安装。你可能还需要安装 [适用于 Visual Studio 2015、2017 和 2019 的 Microsoft Visual C++ 可再发行软件包](https://support.microsoft.com/zh-cn/help/2977003/the-latest-supported-visual-c-downloads)。

### Linux

下载 `AppImage`，添加执行权限 (`chmod +x <AppImagePath>`), 然后就可以运行了。

### MacOS

下载 `.dmg` 文件然后安装。如果提醒“未识别的开发者”之类的，请依然允许 CP Editor 运行。可以在 `Settings -> Security -> General` 中将 CP Editor 添加到白名单里。

## 从源代码构建

1. 克隆代码仓库以及子组件:

	```sh
	git clone --recurse-submodules https://github.com/cpeditor/cpeditor.git
	cd cpeditor
	```

2. 安装 [Qt](https://www.qt.io/download)（5.15 或更高版本）, [CMake](https://cmake.org/download/)（3.12 或更高版本）以及 [Python3](https://www.python.org/downloads/)。
   - 在一些 Linux 发行版和 MacOS 系统上，你可以直接使用包管理器安装 Qt。例如，在 Arch Linux 上可以使用 `sudo pacman -S qt5-base`，在 MacOS 上可以使用 `brew install qt5`.=。
   - 你还可以使用 [aqtinstall](https://github.com/miurahr/aqtinstall) 来下载并安装。

3. 如果 CMake 提示找不到 Qt 路径，你应该设置环境变量：`CMAKE_PREFIX_PATH=%QtPath%/%QtVersion%/%Compiler%/lib/cmake`。例如，在 MacOS 上，你可以运行 `export CMAKE_PREFIX_PATH="/usr/local/Cellar/qt/5.15.0"`。

4. 运行下面的脚本来构建:

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

在 Linux 上，你会得到 `build/cpeditor`。

在 MacOS 上，你会得到 `build/cpeditor.app`。

在 Windows 上，你会得到 `build/cpeditor.exe` 或 `build/Release/cpeditor.exe`。你可能需要手动添加需要的 DLL，如果你安装过 CP Editor，可以从安装目录将 DLL 拷贝过来（或者将可执行文件复制到安装目录覆盖原来的）。

## 使用 Artifacts

如果你想要使用最新的没有发布的特性，而不想自己构建，你可以在 [GitHub Actions](https://github.com/cpeditor/cpeditor/actions) 中下载 Artifacts。

使用 Artifacts 是不被推荐的，因为它们可能比 beta 版本更不稳定，甚至可能包含写了一半还没完成的工作。在使用 Artifacts 前，你应该阅读 commit 记录，甚至是源码。

在 Windows/MacOS 上，你可以用 Artifact 替换之前的安装。在 MacOS 上，你需要运行 `cp path/to/Artifact cpeditor.app/Contents/MacOs`，其中 `cpeditor.app` 是已经安装好的文件。

在 Ubuntu 18.04 上，你需要安装 Qt 来运行 Artifact。其它发行版上可能无法运行 Artifact。

## 从 AUR 构建

这个方法仅适用于 Arch Linux（以及 Manjaro 等基于 Arch Linux 的发行版）。

CP Editor 在 AUR 上有两个包：稳定版 [cpeditor](https://aur.archlinux.org/packages/cpeditor/) 和 alpha 版 [cpeditor-git](https://aur.archlinux.org/packages/cpeditor-git/)。

就像安装其它 AUR 包一样，首先你需要有一个 AUR 安装器。如果你没有，可以通过 `sudo pacman -S yay` 安装一个。

接着，运行 `yay -S cpeditor` 或 `yay -S cpeditor-git`，选择默认选项，然后等着就好了。

安装完成后，你就可以在终端中使用 `cpeditor` 来运行 CP Editor 了。

# 开始使用

安装完成后，你可能想要知道如何 [开始使用](MANUAL_zh-CN.md#开始使用)。
