# Get Started

We're glad that you have chosen our editor, let's get started now! :tada:

- Install `g++` / `python` / `java` depending on which language you are using. Make sure that they are in the system PATH so that you can run them directly in the command line. Or you can set the compile commands in the Preferences if they are not in the PATH.

- Set preferences in the Preferences (Options -> Preferences, or use the shortcut <kbd>Ctrl+P</kbd>).

- If you want to parse sample testcases from online platforms like Codeforces, please install [Competitive Companion](https://github.com/jmerle/competitive-companion) on your browser.

- If you want to submit to Codeforces inside the editor, please install [CF Tool](https://github.com/xalanq/cf-tool). If it's not in the system PATH, you can set the path to it in Preferences->Extra->Misc->CF Tool Command. Don't forget to configure it by `cf config` (set the user and the template).

- If you want to format your C++ codes, you need to install [Clang Format](http://releases.llvm.org/download.html). You have to make sure it's installed on the system PATH or set the path to it at Preferences->Formatting->Clang Format Command.

# Tips

## CF Tool

You have to either parse the problem from Competitive Companion, or set the problem URL in the right-click menu of the tabs. Then you'll see the submit button. The button is only clickable when CF Tool is found by the editor. You can set the path to it in Preferences->Extra->Misc->CF Tool Command.

## For Java users

You have to use a **non-public** class named **a** for your solution.

## Preferences

### Save files faster

If this option is enabled, [QFile](https://doc.qt.io/qt-5/qfile.html) will always be used to save files, otherwise [QSaveFile](https://doc.qt.io/qt-5/qsavefile.html) will be used in some situations.

Enable this will make saving files faster, but if error happens when writing to files, both the old file and the new one may lose. Note that the old file won't lose when the editor doesn't have permission to the file. This error happens with extremely low probability.

### Runtime Arguments

This is the arguments passed to your programs in the command line when running them. You usually don't need to set them.

### Clang Format

The content of the "Clang Format Style" setting is the same as a `.clang-format` file. You can read about it [here](https://clang.llvm.org/docs/ClangFormat.html).

### Snippets

You can use <kbd>Ctrl+S</kbd>, <kbd>Ctrl+N</kbd>, <kbd>Ctrl+D</kbd>, <kbd>Ctrl+R</kbd> for `Save`, `New`, `Delete` and `Rename`.

## Shortcuts

Besides the shortcuts in the menu, you can use the [Read-only Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings) and the [Editing Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings). There are also some hidden shortcuts: <kbd>Tab</kbd> (when there's a selection) and <kbd>Shift+Tab</kbd> for indent and unindent, <kbd>Ctrl+Enter</kbd> and <kbd>Ctrl+Shift+Enter</kbd> for inserting line after and before, <kbd>Shift+Delete</kbd> for deleting the current line.

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

It's better to use unix-like shells like git bash on Windows.

You can open a non-existing file as a tab (and you can save to the path later) in the command line, or open all C++ files in a directory recursively.
