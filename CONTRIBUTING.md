## Welcome!

You are here to help CP Editor? Awesome, feel free and read the following sections in order to know how to work on something and where to get help from. Your contribution matters a lot, CP Editor is getting better and better because of people like you. :smile:

## What kinds of contributions are we looking for?

- If you like CP Editor, star this project and share it with your friends.
- Report bugs or request new features by [opening issues](https://github.com/cpeditor/cpeditor/issues/new/choose). Please follow the issue templates and provide detailed information, this will help the developers to find the reason of the bug and understand what feature you are requesting for exactly.
- Help other people in issues/pull requests/[Telegram group](https://t.me/cpeditor)/other platforms if you are able to help them.
- Write tutorials and blogs on how to use CP Editor.
- Improve documentation, including the Markdown files and the comments in the codes.
- Write codes for new features or bug fixes.

## Your First Contribution

**Working on your first Pull Request?** You can learn how from this *free* series [How to Contribute to an Open Source Project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github). Basically, you have to fork this repository, write codes on your fork, open a pull request when you are finished, wait for reviews and apply suggestions from the maintainers.

**First time to developing?** There's a big difference between developing and competitive programming, but it's not hard. The first thing you should know is Object-Oriented Programming. Basically, you need to know how classes work in C++, and you should know it's a rule to put the class definitions in the `.h`/`.hpp` files, and put the implementation of the functions in the `.cpp` files. The source code of the CP Editor is a good example. Google for what you want to implement is usually helpful. Copy and modify the source codes if you want to implement something similar to an existing feature. If you are looking for an IDE for developing, [VS Code](https://code.visualstudio.com/) is probably a good choice.

**First time to Qt?** Don't be afraid, Qt is not hard to get started. You have to install Qt>=5.12 and CMake>=3.5 first. Linux users may install them using the package manager on your system. Windows users may download Qt [here](https://www.qt.io/offline-installers) and it's recommended to use [Visual Studio](https://visualstudio.microsoft.com/) and install the CMake plugin. Read the [build from source](doc/INSTALL.md#Build-from-source) section to learn how to build. Read the [Qt docs](https://doc.qt.io/) on how to use Qt built-in widgets, how to design the UI and how to use the [signals and slots](https://doc.qt.io/qt-5/signalsandslots.html). Usually, you don't need anything from the std namespace, for example, use `QString` instead of `std::string`.

**Don't know where to start?** You can start by looking through the [issues](https://github.com/cpeditor/cpeditor/issues) and find one you are interested in. The [good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) may be suitable for you. Ask in the [Telegram group](https://t.me/cpeditor) or under the issues if you have any questions.

## Before Writing Codes

Please open an issue (describe the feature/bug like normal issues, and say that you are going to work on it, probably with a draft on how you would like to implement it) before writing codes for a pull request, for these reasons:

1. You may get help from others.
2. Others can know you are working on it, this will save possible duplicate works.
3. You will be notified if this feature is not going to be accepted, or the "bug" is actually not a bug, etc., thus saving your time.

## Contributing rules

- Remember that the goal is to build an editor for competitive programmers, not for developers.
- Do one thing in a pull request. If you want to add multiple features/fix multiple bugs and they are not relevant, open multiple pull requests. You may need to create multiple branches to open multiple pull requests, you can learn git branching [here](https://learngitbranching.js.org/).
- Add new features on the master branch, and fix bug on the latest active (master, beta and stable) branch which has the bug.
- Please write [clear commit messages](https://chris.beams.io/posts/git-commit/). It's also recommended to write [Conventional Commits](https://www.conventionalcommits.org/). You can use [commitizen/cz-cli](https://github.com/commitizen/cz-cli) to write commit messages. However, the rules are optional, the key is that, others and yourself in the future should understand what has been changed and why these changes are made.
- Use Clang Format 9 (use `clang-format --version` to check your version, you may need to set the clang-format binary path for your IDE, for example, the default version of clang-format in VS Code may be clang-format-6) and `.clang-format` in the root directory to format your codes. It's OK if you are not able to install Clang Format 9, because your codes will be automatically formatted after pushing to GitHub.
- Don't remove features unless it's necessary. Add an option to disable some feature instead.
- Keep the settings file backward compatible all the time.
- Windows, Linux and Mac OS should be supported.
- Both light/dark system themes should be supported.
- Everyone should obey the [Code of Conduct](CODE_OF_CONDUCT.md) in order to build a harassment-free community.

## FAQ

- Can I do it well if I've just started?
  - Everyone knows nothing at the beginning. Don't worry, our cool contributors will help you when you meet difficulties. If you :heart: open source and keep trying, everything will be better and better.
- Do I need to learn about CMake?
  - No, you only need to know how to [build from source](doc/INSTALL.md#Build-from-source).
- How to add a setting?
  - You have to add the UI part in `ui/preferencewindow.ui`, add get/set functions in `include/Core/SettingsManager.hpp` and `src/Core/SettingsManager.cpp`, add codes in `applySettingsToui`, `extractSettingsFromui` and `isUnsavedChanges` in `src/preferencewindow.cpp`. Copy-pasting is very helpful.
- Some files are missing when compiling.
  -  If the CI tests are passed, it's probably because the submodules are updated. You can run the following commands:
  
    ```sh
    git add .
    git reset --hard
    git submodule update --init
    ```

    You can also try `git rm` all submodules and then init them again, or even delete the repo on your disk and clone again.
