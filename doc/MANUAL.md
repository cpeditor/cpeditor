# Get Started

We're glad that you have chosen our editor, let's get started now! :tada:

- Install `g++` / `python` / `java` depending on which language you are using. Make sure that they are in the system PATH so that you can run them directly in the command line. Or you can set the compile commands in the Preferences if they are not in the PATH.

- Set preferences in the Preferences (Options -> Preferences, or use the shortcut <kbd>Ctrl+P</kbd>).

- If you want to use the Competitive Companion for parsing examples from online platforms like Codeforces, install it [here](https://github.com/jmerle/competitive-companion).

- If you want to use CF-Tool for submitting on Codeforces inside the editor, install it [here](https://github.com/xalanq/cf-tool) and set the path to it in the Preferences if it's not in the PATH. Don't forget to configure it by `cf config` (set the user and the template).

- If you want to format your C++ codes, you need to install [Clang Format](http://releases.llvm.org/download.html). You have to make sure it's installed on the system PATH or set the path to it in the Preferences.

# Tips

## For Java users

You have to use a **non-public** class named **a** for your solution.

## Preferences

### Runtime Arguments

This is the arguments passed to your programs in the command line when running them. You usually don't need to set them.

### Clang Format

The content of the "Clang Format Style" setting is the same as a `.clang-format` file. You can read about it [here](https://clang.llvm.org/docs/ClangFormat.html).

### Snippets

Remember to save the snippets! The "OK" and "Apply" buttons don't save snippets.

You can use <kbd>Ctrl+S</kbd>, <kbd>Ctrl+N</kbd>, <kbd>Ctrl+D</kbd>, <kbd>Ctrl+R</kbd> for `Save`, `New`, `Delete` and `Rename`.

## Editor

Besides the shortcuts in the menu, you can use the [Read-only Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings) and the [Editing Key Bindings](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings). There are also some hidden shortcuts: <kbd>Tab</kbd> and <kbd>Shift+Tab</kbd> for indent (a selection is needed) and unindent, <kbd>Ctrl+Enter</kbd> and <kbd>Ctrl+Shift+Enter</kbd> for inserting line after and before.

## Tabs

You can use <kbd>Ctrl+Tab</kbd> and <kbd>Ctrl+Shift+Tab</kbd> to switch to the next/previous tab.

You can right-click on the tabs to get a useful context menu. The content of the menu is based on whether this tab is untitled or not, has problem URL or not.

## Testcases

Testlib checkers are compiled when changing the checker or changing the preferences, so you may have to wait for it after running.

## View mode

The view mode is helpful to focus on your codes, investigate the outputs and errors, or make the editor narrower so that you can split your screen into two parts.

You can set a hotkey for switching view modes in Preferences.

## Use CP Editor in the command line

You can use CP Editor in the command line, run `cpeditor --help` for more information.

You can open a non-existing file in the command line, or open all C++ files in a directory recursively.
