# Get Started

We're glad that you have chosen our editor, let's get started now! :tada:

- Install `g++` / `python` / `java` depending on which language you are using. Make sure that they are in the system PATH so that you can run them directly in the command line. Or you can set the compile commands in the Preferences if they are not in the PATH.

- Set preferences in the Preferences (Options -> Preferences, or use the shortcut <kbd>Ctrl+P</kbd>).

- If you want to parse sample testcases from online platforms like Codeforces, please install [Competitive Companion](https://github.com/jmerle/competitive-companion) on your browser.

- If you want to submit to Codeforces inside the editor, please install [CF Tool](https://github.com/xalanq/cf-tool). If it's not in the system PATH, you can set the path to it in Preferences->Extensions->CF Tool. Don't forget to configure it by `cf config` (set the user and the template).

- If you want to format your C++ codes, you need to install [Clang Format](http://releases.llvm.org/download.html). You have to make sure it's installed on the system PATH or set the path to it at Preferences->Extensions->Clang Format.

# Tips

## CF Tool

You have to either parse the problem from Competitive Companion, or set the problem URL in the right-click menu of the tabs. Then you'll see the submit button. The button is only clickable when CF Tool is found by the editor. You can set the path to it in Preferences->Extensions->CF Tool Command.

## Language Server

You need to install a [Language Server program](https://microsoft.github.io/language-server-protocol/implementors/servers/) for linting, auto-complete and some other features. Each programming language have a different Language Server program.

Here are the steps to set up a Language Server program for each language:

- **C++**:  If you have installed LLVM, you already have `clangd`, because it comes with the clang compiler. If you haven't, please install [LLVM](https://releases.llvm.org/download.html) prebuild binaries for Windows. On Linux and mac you can install it from your package manager. You need to set the start command of `clangd` in Preferences -> Extensions -> Language Server -> C++ Server -> Path, where the command can be the path to the executable file or just `clangd` if it's in the PATH.

- **Python**: You need to have `python` and `pip` installed and added to `PATH`. Install `python-language-server` pip module by running `pip install 'python-language-server[all]'`. After the installation is done, open CP Editor settings, go to Preferences -> Extensions -> Language Server -> Python Server. Set "Path" to `python` and set "Arguments" to `-m pyls`. Save the changes and you will have linting for Python.

- **Java**: You need to have `java` installed and added to `PATH`. Download [Eclipse.JDT.ls](http://download.eclipse.org/jdtls/snapshots/jdt-language-server-latest.tar.gz), extract it to any location (in a new directory), We call the path to that directory `<INSTALL_PATH>` (Something like `C://Users/Myname/Desktop/EclipseJDT.ls/`). In CP Editor, go to Preferences -> Extensions -> Language Server -> Java Server. Set "Path" to `java` and set "Arguments" to something like `-jar $INSTALL_PATH/plugins/org.eclipse.equinox.launcher_1.5.200.v20180922-1751.jar -configuration <INSTALL_PATH>/config_(linux|win|mac)`. For example, on Windows, the arguments should be something like `-configuration C://Users/Myname/Desktop/Eclipse.JDT.ls/config_win`.

  The full name of the jar file above, `org.eclipse.equinox.launcher_1.5.200.v20180922-1751.jar`, may change incrementally as the project version of JDT changes. If java complains about "jar not found", then look for the latest version of the `org.eclipse.equinox.launcher_*` jar in `<INSTALL_PATH>/plugins` and replace it in the command after `-jar`.

## Preferences

### Shortcuts

You can use <kbd>Ctlr+D</kbd>, <kbd>Ctrl+R</kbd> and <kbd>Ctrl+S</kbd> for "Default", "Reset" and "Apply" respectively.

You can use <kbd>Ctlr+Tab</kbd> and <kbd>Ctlr+Shift+Tab</kbd> to go through the preferences pages.

### Save files faster

If this option is enabled, [QFile](https://doc.qt.io/qt-5/qfile.html) will always be used to save files, otherwise [QSaveFile](https://doc.qt.io/qt-5/qsavefile.html) will be used in some situations.

Enable this will make saving files faster, but if error happens when writing to files, both the old file and the new one may lose. Note that the old file won't lose when the editor doesn't have permission to the file. This error happens with extremely low probability.

### Runtime Arguments

This is the arguments passed to your programs in the command line when running them. You usually don't need to set them.

### Clang Format

The content of the "Clang Format Style" setting is the same as a `.clang-format` file. You can read about it [here](https://clang.llvm.org/docs/ClangFormat.html).

### Snippets

You can use <kbd>Ctrl+N</kbd> for "Add", <kbd>Ctrl+W</kbd> for "Del" and <kbd>F2</kbd> for "Rename".

To use the snippets, you can press <kbd>Ctrl+T</kbd> in the main window.

## Editor Shortcuts

Besides the shortcuts in the menu, you can use the [Read-only Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings) and the [Editing Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#editing-key-bindings). There are also some hidden shortcuts: <kbd>Tab</kbd> (when there's a selection) and <kbd>Shift+Tab</kbd> for indent and unindent, <kbd>Ctrl+Enter</kbd> and <kbd>Ctrl+Shift+Enter</kbd> for inserting line after and before, <kbd>Shift+Delete</kbd> for deleting the current line.

## Drag and Drop

You can drag files and drop them into the main editor and the input and expected parts of testcases.

## Tabs

You can use <kbd>Ctrl+Tab</kbd> and <kbd>Ctrl+Shift+Tab</kbd> to switch to the next/previous tab.

You can right-click on the tabs to get a useful context menu. The content of the menu is based on whether this tab is untitled or not, has problem URL or not.

## Testcases

Testlib checkers are compiled when changing the checker or changing the preferences, so you may have to wait for it after running.

You can load testcases from files and edit testcases in bigger windows in the right-click menu of the testcases.

You can click the verdict to open the diff viewer.

## View mode

The view mode is helpful to focus on your codes, investigate the outputs and errors, or make the editor narrower so that you can split your screen into two parts.

You can set a hotkey for switching view modes in Preferences.

## Launch CP Editor in the command line

CP Editor supports some command-line options, run `cpeditor --help` for more information.

It's better to use Linux-like shells like git bash on Windows.

You can open a non-existing file as a tab (and you can save to the path later) in the command line, or open all C++ files in a directory recursively.
