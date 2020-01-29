## Introduction

You are here to help CP Editor? Awesome, feel welcome and read the following sections in order to know how to work on something and where to get help from. Your contribution matters a lot, CP Editor is getting better and better because of people like you.

Following these guidelines helps to communicate that you respect the time of the developers managing and developing this open-source project. In return, they should reciprocate that respect in addressing your issue, assessing changes, and helping you finalize your pull requests.

## What kinds of contributions are we looking for?

- If you like CP Editor, star this project and share it with your friends.
- Report bugs or request new features by [opening issues](https://github.com/coder3101/cp-editor/issues/new/choose). Please follow the issue templates and provide as much information as possible, this will help the developers to find the reason of the bug and understand what feature you are requesting for exactly.
- Answer questions from other people in issues/pull requests/telegram group if you are familiar with those topics.
- Improve documentation like README.md and CONTRIBUTING.md, write tutorials and blogs on how to use CP Editor.
- Write codes for new features or fixing bugs.

## Your First Contribution

**Working on your first Pull Request?** You can learn how from this *free* series [How to Contribute to an Open Source Project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github). Basically, you have to fork this repository, write codes on your fork, open a pull request when you are finished, wait for reviews and apply suggestions from the maintainers.

**First time to developing?** It's a big difference between developing and competitive programming, but it's not hard. The first thing you should know is Object-Oriented Programming. Basically, you need to know how classes work in C++, and you should know it's a rule to put the class definitions in the `.h`/`.hpp` files, and put the implementation of the functions in the `.cpp` files. The source code of the CP Editor is a good example. Google for what you want to implement is usually helpful. Copy and modify the source codes if you want to implement something similar to an existing feature. [VS Code](https://code.visualstudio.com/) (or [Visual Studio](https://visualstudio.microsoft.com/) on Windows only) is probably a good choice if you are looking for an IDE for developing.

**First time to Qt?** You have to install Qt>=5.12 and CMake>=3.5 first. Linux users may install them using the package manager on your system. Windows users may download Qt [here](https://www.qt.io/offline-installers) and it's recommended to use [Visual Studio](https://visualstudio.microsoft.com/) and install the CMake plugin. Read the [build from source](README.md#build-from-source) section to learn how to build. Read the [Qt docs](https://doc.qt.io/) on how to use Qt built-in widgets, how to design the UI and how to use the [signals and slots](https://doc.qt.io/qt-5/signalsandslots.html). Usually, you don't need anything from the std namespace, for example, use `QString` instead of `std::string`.

**Don't know where to start?** You can start by looking through the [issues](https://github.com/coder3101/cp-editor/issues) and find one you are interested in. Ask in the [Telegram group](https://t.me/cpeditor) or under the issues if you have any questions.

## Before Writing Codes

Please open an issue (describe the feature/bug like normal issues, and say that you are going to work on it, probably with a draft on how you would like to implement it) before writing codes for a pull request, for these reasons:

1. You may get help from others.
2. Others can know you are working on it, this will save possible duplicate works.
3. You will be notified if this feature is not going to be accepted, or the "bug" is actually not a bug, etc., thus saving your time.

## Contributing rules

- Remember that the goal is to build an editor for competitive programmers, not for developers.
- Do one thing in a pull request. If you want to add multiple features/fix multiple bugs, open multiple pull requests. You may need to create multiple branches to open multiple pull requests, you can learn git branching [here](https://learngitbranching.js.org/).
- Please write [clear commit messages](https://chris.beams.io/posts/git-commit/). It's recommended to use [commitizen/cz-cli](https://github.com/commitizen/cz-cli) to format your commit messages.
- Use Clang Format 9 (use `clang-format --version` to check your version, you may need to set the clang-format binary path for your IDE, for example, the default version of clang-format in VS Code may be clang-format-6) and `.clang-format` in the root directory to format your codes.
- Don't remove features unless it's necessary. Add an option to disable some feature instead.
- Keep the settings file backward compatible all the time.
- Everyone should obey the [Code of Conduct](CODE_OF_CONDUCT.md) in order to build a harassment-free community.

> There may be clang-format-6 only for Ubuntu 18.04, you can read [this blog](http://mickey-happygolucky.hatenablog.com/entry/2020/01/06/133319) for how to install clang-format-9 on Ubuntu 18.04.

## FAQ

- How to add a setting?
  - You have to add the UI part in `ui/preferencewindow.ui`, add get/set functions in `include/SettingsManager.hpp` and `src/SettingsManager.cpp`, update settings and probably extract setting to UI in `src/preferencewindow.cpp`.
- How to implement XXX?
  - Google for it and read the Qt docs usually helps. If you still have questions, ask in the [Telegram group](https://t.me/cpeditor).
- What do these codes mean/do?
  - Unfortunately, CP Editor doesn't have a document up to now, so it's normal to have questions on the codes. You can ask in the [Telegram group](https://t.me/cpeditor), and it's welcome to help us write a document.