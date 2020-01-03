# CP Editor

[![Build Status](https://travis-ci.org/coder3101/cp-editor2.svg?branch=master)](https://travis-ci.org/coder3101/cp-editor2)

<img src=https://i.imgur.com/31n9t8i.png height="80" width="80">

CP Editor is a Native Qt based code editor writter in C++. It make you only focus on your code and automates the compilation, executing and testing. It even fetches testcases for you from web!

<img src=https://i.imgur.com/jKOtf6W.jpg width="85%">  

### Installation

Head over to the [release page](https://github.com/coder3101/cp-editor2/releases) and download the binary/installer depending upon your OS.
- Windows users can find an `Installer` install it and start to code.
- Linux users can find an `AppImage` install or directly run it. 
  
  >Google on how to run App Images.
- Mac users please install from source for the time being.

For Chinese users who are not convenient to download on Github, there is an [alternative download link](https://pan.wzf2000.top/s/md70l8h0) (it's not guaranteed to be up to date).

### Initial setup

Just install `gcc`, `python` and `java` and put them in Environment PATH. Now you can use any language you want to use in the editor, If you want to use only one language, you can optionally install only one of the above programs. **You must install atleast one from above**.

> You **must not make your Java class public or give any other name apart from `a`**.

You have to ability to change compile command and run other commands by navigating to `Preferences => Languages`.


### Standard Keyboard Shortcuts

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
| CTRL+Shift+D | Run the program in separate window (Detached mode)           |

### Hotkeys

You can also set hotkeys for all of the above actions manually from `Preferences`. However the standard keys are fixed and cannot be changed, hotkys are additional shortcuts.

#### Building for source
> **Qt must be installed in order to compile from source**

```sh
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

#### Contribution
Contributors and contributions are welcomed! However to maintain the quality of code, we use `Coding Style`. So make sure that your changes are formatted using `clang-format`, using the format-file provided in the root of the directory.

#### FAQ 

- Where does the editor saves my code and binary executable?
    - Editor does not saves your files if you don't Save them yourself. You can find your binary executable at `C://Users/<NAME>/AppData/Local/Temp/a.exe` in windows and on NIX in `/tmp/a.out`. At those location the source file could also be located with name `sol.cpp`. 
- I am using it in Java and editor is not working.
   - Make sure `java` and `javac` are in PATH variables. Please Log/Restart your machine if it does not recognizes the Java Compiler. Now make sure your source code has **non-public class named a**. 
- I get **DLL Missing error** while launching application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).


License
----

[GNU General Public License Version 3.0](https://github.com/coder3101/cp-editor2/blob/master/LICENSE)
