# CP Editor

[![Build Status](https://travis-ci.org/coder3101/cp-editor.svg?branch=master)](https://travis-ci.org/coder3101/cp-editor)

<img src=https://i.imgur.com/31n9t8i.png height="80" width="80">

**CP Editor** is a native Qt-based code editor written in C++. It's specially designed for competitive programming, unlike other editors/IDEs which are mainly for developers. It helps you focus on your coding and automates the compilation, executing and testing. It even fetches test cases for you from webpages and submits codes on Codeforces!

### Features

- Submit code on Codeforces inside the editor.
- Parse problem example test cases from over 30+ online platforms, and test on them with verdict feedback.
- You can use code snippets conveniently.
- Fast and memory efficient. Runs flawlessly on low-end devices.
- Code formating using tools like clang-format.
- Customizable hotkeys for actions in the editor. 
- Over 5 themes to choose from (Drakula, Monkai, Solarised, Solarised Dark and Light).
- Supports Multi tabs.

<img src=https://i.imgur.com/jKOtf6W.jpg width="85%">  

### Installation

Head over to the [release page](https://github.com/coder3101/cp-editor/releases) and download the binary/installer depending upon your OS.
- Windows users can find a `setup.exe` for installation.
- Linux users can find an `AppImage`:
  1. `chmod +x <AppImagePath>` or make it executable in GUI by right-clicking on it.
  2. Just run it, either from bash or in GUI.
- Mac users please install from source for the time being.

For Chinese users who are not convenient to download on Github, there is an [alternative download link](https://pan.wzf2000.top/s/md70l8h0) (it's not guaranteed to be up to date).

### Initial setup

- Install `g++` / `python` / `java` depending on which language you are using. Make sure that they are in the system PATH so that you can run them directly in the command line.

- Set the preferences in the preference window. Make sure that `clang-format` is in the system PATH if you want to use it.

- If you want to use the Competitive Companion for parsing examples from online platforms like Codeforces, install it [here](https://github.com/jmerle/competitive-companion). Don't forget to set the same port for the editor and the browser extension.

- If you want to use CF-Tool for submitting on Codeforces inside the editor, install it [here](https://github.com/xalanq/cf-tool). Don't forget to configure it by `cf config` (set the user and the template).

### For Java users

You **must not** make your Java class public, and the name of the class should be `a` .

### Keyboard Shortcuts

| Shortcut     | Description                                                  |
| ------------ | ------------------------------------------------------------ |
| CTRL+N       | Launches a new Session and resets the editor. Once you solved a problem you need to Launch a new Session for the next Problem. |
| CTRL+Q       | Quit and Exit Application                                    |
| CTRL+O       | Opens a new Source File into Editor                          |
| CTRL+S       | Saves the current content of editor to a File                |
| CTRL+Shift+S | Saves as                                                     |
| CTRL+Alt+L   | Formats the editor using `format_command`                    |
| CTRL+Shift+R | Compile and Run                                              |
| CTRL+Shift+C | Compile only                                                 |
| CTRL+R       | Run                                                          |
| CTRL+K       | Kill all running Test cases / Samples                        |
| CTRL+Shift+D | Run the program in a separate window (Detached mode)         |
| CTRL+T       | Use any saved snippet codes                                  |

The key bindings for the code editor are [here](https://doc.qt.io/qt-5.14/qtextedit.html#read-only-key-bindings) and [here](https://doc.qt.io/qt-5.14/qtextedit.html#editing-key-bindings).

You can also set hotkeys for all of the above actions manually from `Preferences`. However the standard keys are fixed and cannot be changed, hotkeys are additional shortcuts.

### Build from source

1. Clone this repo:

	```sh
	git clone https://github.com/coder3101/cp-editor.git
	cd cp-editor
	git submodule init
	git submodule update
	```

2. Install Qt (5.12 or higher) and CMake.

3. Set environment variable `CMAKE_PREFIX_PATH=%QtPath%/%QtVersion%/%Compiler%/lib/cmake`.

4. Run the following commands:
	
	- Linux:

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

### Contribution

If you found any bugs or have any feature requests, please open an issue [here](https://github.com/coder3101/cp-editor/issues), and don't forget to follow the issue templates. Users' responses are important for contributors to improve the project.

If you want a try on developing, you can install Qt and CMake, then fix bugs / make new features by yourself! After finishing the coding, just open a pull request [here](https://github.com/coder3101/cp-editor/pulls). Don't worry if you are not familiar with Qt / GUI programming, Google is your friend, and Qt is a [well documented](https://doc.qt.io/) library. Don't forget to use `.clang-format` file in the root directory to format the codes.

Contributions are always welcomed. Together we can build the best editor for ourselves.

### Telegram group

You can join the [Telegram group](https://t.me/cpeditor) for asking quetions about using the editor / developing.

### FAQ

- Where does the editor save my code and binary executable?
    - The editor does not save your files if you don't save them yourself. You can find your binary executable at `C://Users/<NAME>/AppData/Local/Temp/a.exe` in windows and on NIX in `/tmp/a.out`. At those locations, the source file could also be located with the name `sol.cpp`. 
- I am using it in Java and the editor is not working.
   - Make sure `java` and `javac` are in PATH variables. Please Log/Restart your machine if it does not recognize the Java Compiler. Now make sure your source code has **non-public class named a**.
- I get **DLL Missing error** while launching the application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
- What is Competitive Companion and how do I set it up to parse test cases from websites?
   - Please read this [blog](https://codeforces.com/blog/entry/72067) carefully. It has detailed instructions on setting up the competitive companion to parse test cases.
- What is CF-Tool and where to download it?
   - CF tools provide you to perform many actions like submitting code on Codeforces from the command line. It is a CLI application that can be downloaded from [here](https://github.com/xalanq/cf-tool/releases). It is a standalone executable, simply keep it in a directory and add that directory to PATH.
- I am using it on Codeforces but the submit button is not clickable.
   - Please reboot if you have already added `cf` to your PATH. If you haven't already, add `cf` to your PATH. Submit button will be active only if the editor finds that the `cf` tool is on PATH and invokable.
 - When I click on Submit on Codeforces, I get some message saying template is required?
    - `cf` tool requires that you first configure it before you use it. Please read this [blog](https://codeforces.com/blog/entry/72952) carefully to configure it for your device.

License
----

[GNU General Public License Version 3.0](https://github.com/coder3101/cp-editor/blob/master/LICENSE)