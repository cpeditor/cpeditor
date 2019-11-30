# CP Editor

<img src=https://i.imgur.com/k9BdVwZ.png height="80" width="80">


CP Editor is a Native Qt based code editor writter in C++. It make you only focus on your code and automates the compilation, executing and testing.

  - Very Very Fast 
  - Dark theme code editor
  - Code formatting with `clang-format`
  - Checking and validating `stdout` with Expected values
  - Automatically Format code on braces
  - Ability to add source template file


### Screenshots
<img src=https://i.imgur.com/ACTrdoW.jpg width="49%" height="50%"> <img src=https://i.imgur.com/VlZ5vW2.jpg width="49%" height="50%"> 

### Installation
> Optionally You need clang/gcc and clang-format. You can install from [here](http://releases.llvm.org/download.html#9.0.0).

Head over to the [release page](https://github.com/coder3101/cp-editor2/releases) and download the binary/installer depending upon your OS.
- Windows users can find an `Installer` install it and start to code.
- Linux users can find an `AppImage` install or directly run it. 
  
  >See google on how to run AppImages if you are new to AppImages.
- Mac users please install from source for the time being.

### Initial setup
- By default the editor uses `clang++` as compiler. You may need to change it to something like minGW `g++` or `gcc`for windows if you don't have clang installed. For that `Settings => Change Compile command`. Any compiler of your choice will work as long as **its added to PATH** and works.

> Do not use `-o` option while changing compile command. Let editor itself generate the executable name.

- To use `code-format` you need to have `clang-format` (recommended) installed and **added to PATH**. If you want to use something else you can use, If you don't want to use code-format then ignore the *Formatter Error*.

> Any Error if occurs will be reported in `Message Section`. Try to read it and mitigate if possible. In most cases it will be related with your compiler or things not in PATH. 

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


#### Building for source
> **Qt must be installed in order to compile from source**

```sh
$ mkdir build
$ cmake build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

#### FAQ 
- Where does the editor saves my code and binary executable?
    - Editor does not saves your files if you don't Save them yourself. You can find your binary executable at `C://Users/<NAME>/AppData/Local/Temp/a.exe` in windows and on NIX in `/tmp/a.out`. At those location the source file could also be located with name `sol.cpp`. 
- I code in Other than C++/C. Will you add support for it?
   - Maybe I will in future but there should not be any ETA. It should automatically work for other languages that are compiled like `Rust`, `GoLang` etc (except Java) if you intelligently change the compile and run commands. However the syntax highlighting support will be not correct.
- I get DLL Missing error while launching application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).



License
----

[GNU General Public Licenser Version 3.0](https://github.com/coder3101/cp-editor2/blob/master/LICENSE)
