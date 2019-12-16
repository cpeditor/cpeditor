# CP Editor

[![Build Status](https://travis-ci.org/coder3101/cp-editor2.svg?branch=master)](https://travis-ci.org/coder3101/cp-editor2)

<img src=https://i.imgur.com/31n9t8i.png height="80" width="80">

CP Editor is a Native Qt based code editor writter in C++. It make you only focus on your code and automates the compilation, executing and testing. It even fetches testcases for you from web!

### Screenshots
<img src=https://i.imgur.com/BSQ7d0K.jpg width="85%">  

### Installation

Head over to the [release page](https://github.com/coder3101/cp-editor2/releases) and download the binary/installer depending upon your OS.
- Windows users can find an `Installer` install it and start to code.
- Linux users can find an `AppImage` install or directly run it. 
  
  >See google on how to run AppImages if you are new to AppImages.
- Mac users please install from source for the time being.

For Chinese users who are not convenient to download on Github, there is an [alternative download link](https://pan.wzf2000.top/s/md70l8h0) (it's not guaranteed to be up to date).

### Initial setup

#### C/C++ Setup

- By default the editor uses `g++` as compiler. You can change your compiler command to change the way editor will compile your code (Settings => Compile Command). Any compiler of your choice will work as long as **its added to PATH** and works.

> Do not use `-o` option while changing compile command. Let editor itself generate the executable name.

- To use `code-format` you need to have `clang-format` (recommended) installed and **added to PATH**. If you want to use something else you can use, If you don't want to use code-format then ignore the *Formatter Error*.

> Any Error if occurs will be reported in `Message Section`. Try to read it and mitigate if possible. In most cases it will be related with your compiler or things not in PATH. 

#### Python Setup
- Since Python is interpreted language, we will need to change `Run Command`. By default it is empty, From Settings => Run Command, set it to `python`. Make sure Python is in PATH and is working. If newly added to PATH, A reboot may be needed for Editor to recognize it.

- Now Change Editor Language to Python. From Editor => Language => [Check] Python. This will enable Python syntax Highlighting and keyword completer.

- Now, start typing your wonderful code. Make sure you always use "Compile and Run", "Run" uses buffered script.

#### Java Setup
- Assuming that you have `javac` and `java` to your PATH, Enable Editor to use Java by Enabling Editor => Language => [Check] Java.

- Now change the Compile command to something like `javac`

- Now Change Run Command to `java`

- Now start to code.

> You **must not make your Java class public or give any other name apart from `a`**.


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
| CTRL+K       | Kill all running Testcases / Samples                         |
| CTRL+Shift+D | Run the program in separate window (Detached mode)           |


#### Building for source
> **Qt must be installed in order to compile from source**

```sh
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

#### FAQ 
- Where does the editor saves my code and binary executable?
    - Editor does not saves your files if you don't Save them yourself. You can find your binary executable at `C://Users/<NAME>/AppData/Local/Temp/a.exe` in windows and on NIX in `/tmp/a.out`. At those location the source file could also be located with name `sol.cpp`. 
- I am using it in Java and editor is not working.
   - Make sure `java` and `javac` are in PATH variables. Please Log/Restart your machine if it does not recognizes the Java Compiler. Now make sure your source code has **non-public class named a**. 
- I get DLL Missing error while launching application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).


License
----

[GNU General Public License Version 3.0](https://github.com/coder3101/cp-editor2/blob/master/LICENSE)
