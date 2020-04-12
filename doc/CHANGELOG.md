## UNRELEASED

### The new Preferences Window!

The old preferences window is not very extendable, which has been a big obstacle to add new features for a long time. In this version, we bring a brand new preferences window for the users.

In the new preferences window, the settings are organized well in a tree structure. Now you can save the settings for each page separately, and get warned when you try to leave a page without saving the changes. You can discard the changes in the current page, or restore the default settings on the current page. What's more, you can search for a setting and go to the corresponding page easily.

If you want to backup your settings file or do something with it, we have to tell you that it has been moved from the home directory to the config directory (on Linux, from `$HOME` to `$HOME/.config/CP Editor`). But don't worry, we have worked on keeping backward compatibility, so you won't lose your settings when switching from the old version to the new version.

For the developers, now it's extremely easy to add a new setting. So we'll add more settings with some exciting features in the next version.

### Added

- Show stderr of the checker when the verdict is AC. (#207)
- System-wide Dark theme for Windows 10. (#215)
- Make it able to use makepkg to install on Linux.

### Changed

- Show the manual of the current version instead of the latest version.
- Set window icon in application level instead of window level, this affects the update check window.
- Remove the hard-to-use completer for Python.
- The "Enable hotkeys" option is removed, because it's useless —— you can simply either not use the hotkeys or not set the hotkeys (i.e. clear the hotkeys).

### Fixed

- Fix the wrong behavior of toggle comments, indent, delete line, etc. when wrap text is on. (#213)
- Add auto-close for single and double quotes. (#226)
- Don't require any undo for auto-close parentheses.
- Fix the low resolution issue on macOS. (#225)
- Fix Application failed to get installed on macOS 10.14 (#193)
- Show the manual of the current version instead of the latest version.
- Before this fix, the language is not set to C++ when saving a file with one of the C++ prefixes.
- Fix typos in the names of the editor themes.

## v6.2

### Added

- Clear buttons for keybindings in Preferences.
- You can edit input/expected in a bigger window, by the action in the right-click menu of the textedits.
- Show warning if Setting changes are not saved and setting window is closed.
- Add Shift+Delete as a shortcut for deleting the current line.
- Cut codes to at most 100000 characters when opening a file to avoid hang up.

### Changed

- "Load Input" and "Load Expected" buttons are moved to the right-click menu of the textedits. "Run" and "Delete" buttons take place of them (they were in the menu of the "More" button, now the "More" button is gone).

### Fixed

- Fix update check.
- Fix the wrong behavior of the built-in checker Ignore trailing spaces.
- Check whether CF Tool is started when submitting to CF.
- Support CF Tool v1.0.0. (#194)
- Fix this bug: the editor is cleared when formatting if Clang Format is not found. (#195)
- Fix untitled tab title with problem URL.
- Add "problem URL is not set" to the requirements of not saving hot exit status.

## v6.1

### Added

- You can show/hide each testcase, and only run the non-hidden testcases.
- Add a drop-down menu for each testcase. You can delete a testcase / run a single testcase from it.
- Add a drop-down menu for testcases. You can remove empty, remove all, hide AC, show all, hide all, inverse hide/show from it.
- Add "Set Codeforces URL" in the right-click menu of tabs. You can enter the contest ID and problem code instead of the whole URL.
- Warn when submitting to CF and the problem code is 0.
- A system-wide tray icon.
- Windows icon for all OSs.
- Add an option to save files faster.
- In the regex mode of Find/Replace dialog, you can use "\1", "\2" to refer to the 1st, 2nd capturing group in text to replace.

### Changed

- The "Hide AC" button is removed. (But a similiar action is added in the "More" menu of testcases.)
- Submitting to CF is enhanced. Now empty messages are not shown, some wrong verdicts are fixed, toast messages are shown only once per run. Some small bugs are also fixed.
- Now there's only one Diff Viewer per testcase. The output and expected will update automatically after each run. When pressing the diff button, the output and expected will update and the Diff Viewer will show and raise.

### Fixed

- Show line-breaks without "¶" in the Diff Viewer when "Display new-line characters in the Diff Viewer" option is not checked. (#181)
- Fix window size issue on Windows if the transparency is 100 and the window was maximized in the last session.
- Fix a bug which makes the editor crash if the text is modified when running.
- Fix wrong version comparing in update notifier.
- Fix wrong color when submitting to CF and gets "Pretests passed" verdict.
- Fix a bug which would cause multiple lines jammed into one line. (#187)

## v6.0

It's the first version after the versioning rules changed, so there's no change log before v6.0.
