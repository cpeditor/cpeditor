## UNRELEASED

### Added

### Fixed

-   Fixed an issue where in some cases CP Editor could not get full data from Google Chrome browsers. (#345 and #624)

## v6.7

### Added

-   Open recent files. (#527)
-   Reveal the executable file in file manager. (#537)
-   Use a dialog to open new contests. (#539 and #467)
-   Choose detached execution terminal on Linux (#554 and #474)
-   Add "save to file" in the context menu of the test cases. (#529 and #560)
-   Customizable head comments. (#203 and #561)
-   Now in C++, you can use relative paths to include headers files. (#565)
-   Now you can set different compile commands and time limits for each tab by setting them in the context menu of the tabs. (#565)
-   Now you can use the time limit from Competitive Companion as the time limit for the corresponding tab. (#565)
-   Now the compiler works in the same directory as the source file, so that you can use relative paths in the compile command. For example, you can add `grader.cpp` to compile the source file with the grader in the same directory. (#565)
-   TLE and RE verdicts. (#566)
-   Now the preferences window pops up at the first launch of CP Editor. (#614)

### Fixed

-   Fixed the diff viewer crashes when the output is empty. (#517)
-   Fixed the background color of the diff viewer is black on Windows with the Dark Fusion theme. (#519 and #520)
-   Fixed the old problem URL is not restored when opening an old file with that option on. (#522)
-   Fixed the performance issue that makes the editor extremely slow when there are many lines. (cpeditor/QCodeEditor#27)
-   Now the code snippets won't be in the log file. (#536)
-   Fixed crash when compiling twice in a row with large test cases. (#549 and #550)
-   Fixed a hypothetical Undefined Behaviour in testlib real number checker. (#586 and #587)
-   Now you can use CF Tool to submit to Gym problems. (#591 and #592)
-   Fixed an issue where Message Box font was getting reset when using Fusion styles. (#604 and #612)
-   Fixed Windows sometimes restarts during the setup. (#545 and #619)

### Changed

-   Now the working directory when executing programs is:

    1.  The path of the executable file for C++;
    2.  The class path for Java;
    3.  The temp file path for Python.

    If you write to files with relative paths in your codes, you can use "Reveal Executable File" to find the outputs.

## v6.6

### The Portable Version

Now we provide the portable version on Windows. The portable version saves the config file in the same directory as the executable file of CP Editor, instead of the system config directory, and you can run it without installtion.

With the portable version, you can easily store it in something like a USB disk, and the configs are always with you no matter you run it on which machine. However, the file paths in the settings can be broken when you change the machine.

### Added

- On Windows, install VC Redist along with the application.
- Now you can use `${tempdir}` as an alternative of `${tmpdir}` in C++ executable file path setting and Java class file path setting.
- Add the translation system and Simplified Chinese translation. (#377 and #384)
- In-application UI style setting and built-in light/dark style. (#265 and #404)
- Now you can choose to run on empty test cases. (#408, #420, #426 and #427)
- Now testcases with empty outputs can also be checked. (#208 and #430)
- Auto-save time interval.
- Now you can auto-save the current session, besides saving it when the application exists. (#437 and #442)
- Now the width of the input/output/expected of each test case is adjustable, and the maximum height of a test case can be set in the preferences. (#414 and #444)
- Now you can use proxy to check for updates. (#448)
- Now you can set a custom font for the whole application. (#169 and #453)
- Now Java programmers can use a public class as your main class. (#459 and #461)
- A warning when it failed to start compilation. (#463)
- Now you can change the text codec of the compiler output. (#469)
- Now you can use the mouse wheel to change the font of the code editor. (#475)
- A flexible system to set and change default paths for several path choosing actions. (#483)
- Settings to control whether to load external file changes. (#486)
- Separated settings for format on manual save and format on auto-save. (#488)
- An option to disable toast messages of CF Tool. (#489)
- Add Russian translations. (#494)
- Now the test cases will be elided if they are too long. (#491)
- Duplicate Tab (in the context menu of tabs). (#481 and #505)

### Fixed

- Fix the wrong default setting for C++ executable file path.
- Fix the displayed size of the default font is not the same as the actual size of the default font. (#425)
- Fix false external file changes when auto-save is enabled. (#392)

### Changed

- The default font is set to the system fixed-width font instead of the font named "Monospace". (#422)
- Now the session is stored in a separate file, which was stored along with the settings. So now when exporting/importing settings, the session won't be included, but you can export/load the session separately. (#437 and #442)
- Now the hidden (not in the search result) pages are skipped when navigating pages in the preferences window via Ctrl+Tab and Ctrl+Shift+Tab. (#447)
- Now when you open the Find/Replace dialog with some text selected in the code editor and the text-to-find changes, the undo history of the text-to-find won't be cleared. (#460)
- Now the default main class name of Java is `Main` instead of `a`. (#461)
- Now the version displayed in `cpeditor --version` is `X.Y.Z.rXX.gGITHASH` if the current commit (HEAD) has no tag, otherwise, it is the actual version. (#468)
- Now the manual URL contains the commit hash instead of the version number, so that it works on the master branch. (#468)
- Now Tab Jump Out Parentheses is disabled by default. (#499)
- The default shortcut to format codes is changed to `Ctrl+Shift+I`. (#512)

### Improved

- Now restoring the last session is faster. (#509)

## v6.5

### Added

- Now you can change the font of the message logger. (#331 and #334)
- Now besides the name of a setting, you can also search for the desciption, tool tip and help information of a setting in the preferences window. (#337)
- Now you can use Alt to choose the menu in the main window. (#344)
- Now it's optional to save the file on compilation and execution or not.
- Now you can choose the path of the executable file for C++ and the path of the parent directory of the class file for Java. (#271)
- Now you can get the git commit hash when executing `cpeditor --version` in the terminal.
- Now the application catches SIGINT, SIGTERM and SIGHUP on Linux/macOS and catches CTRL_C_EVENT, CTRL_BREAK_EVENT and CTRL_CLOSE_EVENT on Windows, it be will gracefully closed when receiving these signals. (#178 and #268) Warning: It's reported that on some environments it doesn't always work.
- Now you can restore the problem URL when opening a file previously with a problem URL, and/or open the old file when parsing an old problem URL. (#199)
- Now the update checker shows changelogs and provides a button to cancel the update operation and a button to directly download binaries. (#361)
- Now you can set a initial position for the code template. (#352)
- Use tab to jump out of a parenthesis. (#256)
- Parentheses settings for each parenthesis in each language. (#206)
- A button to clear the messages.
- Export and import settings. (#336)
- Ask whether to override an existing snippet or not when there's a conflict in the snippet names. (#244)

### Fixed

- Fixed some unexpected file saving. (#353)
- Fix a bug which makes the application crash when quiting in some scenarios. (fa5259b)
- Fix a bug which makes the saved test cases are not loaded correctly.
- Fix the screen is not scrolled to show the cursor in some situations.
- Fix change of Editor theme on Windows Dark mode required a restart of application. (#232)

### Changed

- Open an empty untitled tab when the open file length limit is exceeded. (#353)
- When parsing a problem while it is already parsed and opened in a tab, the new test cases will override the old test cases.
- If you click "Cancel" when saving all files, the action will be aborted instead of saving the next file.
- The Qt version is upgraded to 5.15. (#374 and #378)

## v6.4

### Linting with Language Server

Now error linting is available with Language Server. Linting can be helpful when you want to find errors in your code and fix them quickly. You need to install a Language Server program to enable linting. You can set delay for linting, the path to the Languae Server program and its arguments in the Preferences window. You can set different settings for different languages. Please check the [manual](MANUAL.md) for more information on how to set this up.

### Added

- Add Ctrl+Shift+D for duplicating the current line or the selection. (#237)
- Add tooltips for many widgets. (#211)
- Now you can add pairs of test cases from files. (#204 and #246)
- Now you can choose where to save the test case files in Preferences->File Path->Testcases. (#176)
- Now when saving the source file, if the parent directory of the file does not exist, it will be automatically created.
- Now you can set different default file paths for different problem URLs. (#200)
- Now you can set font for test cases.
- Now you can enable extra bottom margin with the height of a page. (#230)

### Changed

- Word selection in editor is highlighted with change of background color instead of an underline. Underlined text now represent linting of type "Hint".
- The shortcut of Detached Run is changed from Ctrl+Shift+D to Ctrl+Alt+D. (#237)
- Now after deleting the current line/selected lines, the cursor will keep its place if possible, instead of moving to the start of the previous line.

### Fixed

- Fix switching to split view mode instead of editor view mode when clicking "Run", "Compile" or "Compile and Run".
- Fix preferences window loses focus after picking the font. (#260)
- Fix Ctrl+Enter and Ctrl+Shift+Enter in the code editor.
- Fix lost of code snippets and hot exit status on some platforms.
- Fix the unexpected behaviour of Shift+Enter.

## v6.3

### The new Preferences Window!

The old preferences window is not very extendable, which has been a big obstacle to add new features for a long time. In this version, we bring a brand new preferences window for you.

In the new preferences window, the settings are organized well in a tree structure. Now you can save the settings for each page separately, and get warned when you try to leave a page without saving the changes. You can discard the changes on the current page, or restore the settings on the current page to the default. What's more, you can search for a setting and go to the corresponding page easily.

If you want to backup your settings file or do something with it, we have to tell you that it has been moved from the home directory to the config directory (on Linux, from `$HOME` to `$HOME/.config/CP Editor`). But don't worry, we have been working on keeping backward compatibility, so you won't lose your settings when switching from the old version to the new version.

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
