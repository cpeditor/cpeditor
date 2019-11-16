# CP Editor

<img src=https://i.imgur.com/k9BdVwZ.png height="80" width="80">


CP Editor is a Native Qt based code editor writter in C++. It make you only focus on your code and automates the compilation, executing and testing.

  - Very Very Fast 
  - Dark theme code editor
  - Code formatting with `clang-format`
  - Checking and validating `stdout` with Expected values


### Screenshots
<img src=https://i.imgur.com/ACTrdoW.jpg width="49%" height="50%"> <img src=https://i.imgur.com/VlZ5vW2.jpg width="49%" height="50%"> 

### Installation
> Optionally You need clang and clang-format. You can install from [here](http://releases.llvm.org/download.html#9.0.0).

Head over to the [release page](https://github.com/coder3101/cp-editor2/releases) and download the binary/installer depending upon your OS.
- Windows users can find an `Installer` install it and start to code.
- Linux users can find an `AppImage` install or directly run it. 
  >See google on how to run AppImages if you are new to AppImages.
- Mac users please install from source for the time being.
 
#### Building for source
> **Qt must be installed in order to compile from source**

```sh
$ mkdir build
$ cmake build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
```

#### Common Issues
- When I launch application it says in message "Compiler will not work..."?
    - By default editor will use `g++` as the compiler. Please install it and add it to your PATH variable. Alternatively you can change the compile command by `Settings > Change Compile Commands` and setting some other compiler like `clang++`
- When I launch application it says in message "Formatting will not work..."?
   - By default editor uses `clang-format` to re-format the source, add it to your environment and error should go away. Alternatively you can ignore this error if you don't care about your code formatting.



License
----

GNU General Public Licenser Version 3.0

**Free Software, Hell Yeah!**
