## Your first contribution

-   [How to contribute to an open source project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github)
-   [Build CP Editor from source](https://cpeditor.org/docs/installation/build-from-source/)
-   [Qt docs](https://doc.qt.io/)
-   [Discover issues](https://github.com/cpeditor/cpeditor/contribute)

## Before writing codes

If there isn't an open issue for what you are going to work on, please open an issue before writing codes for a pull request, for these reasons:

1.  You may get help from others.
2.  Others can know you are working on it, this will save possible duplicate works.
3.  You will be notified if this feature is not going to be accepted, or the "bug" is actually not a bug, etc., thus saving your time.

## Contributing rules

-   Remember that the goal is to build an editor for competitive programmers, not for developers.
-   Do one thing in a pull request. If you want to add multiple features/fix multiple bugs and they are not relevant, open multiple pull requests. You may need to create multiple branches to open multiple pull requests, you can learn git branching [here](https://learngitbranching.js.org/).
-   Add new features on the master branch, and fix bugs on the latest active (master, beta and stable) branch which has the bug. See also [the versioning rule](VERSIONING.md).
-   Please write [clear commit messages](https://chris.beams.io/posts/git-commit/). It's also recommended to write [Conventional Commits](https://www.conventionalcommits.org/). Since we are [squashing and merging pull requests](https://docs.github.com/en/free-pro-team@latest/github/collaborating-with-issues-and-pull-requests/about-pull-request-merges#squash-and-merge-your-pull-request-commits), the commit messages in a pull request is not so important, while the title of the pull request or the message of the only commit in a pull request is more crucial.
-   Use Clang Format and [`.clang-format`](.clang-format) to format your codes. If the codes without your modifications are changed after formatting, try updating your `clang-format`.
-   Don't remove features unless it's necessary. Add an option to disable them instead.
-   Keep the settings file backward compatible all the time.
-   Everyone should obey the [Code of Conduct](CODE_OF_CONDUCT.md) in order to build a harassment-free community.

## Documentation

The documentation is hosted at [cpeditor.github.io](https://github.com/cpeditor/cpeditor.github.io). You can read [its contributing guidelines](https://github.com/cpeditor/cpeditor.github.io/blob/hugo/CONTRIBUTING.md) for more information.

## Translations

The translations are in the [translations](translations) directory. It's recommended to use Qt Linguist to edit them. However, if you only want to fix a typo, you can use whatever text editor you like. In Qt Linguist, when a translation is finished, press <kbd>Ctrl + Enter</kbd> to mark it as finished and go to the next unfinished translation. If a text doesn't need translation, for example, "C++", you can leave the translation empty and mark it as finished. You should fix all warnings in Qt Linguist.

If you want to add a new language, you can ask the developers for help. For example, you can open an issue for the language you want (it will be better if you can contribute the translations, after we finish the preparation for you).

If you want to go a step further: The translated strings are wrapped in `tr()` in the source codes. When the codes are changed, you can run [updateTranslation.sh](tools/updateTranslation.sh) or [updateTranslation.bat](tools/updateTranslation.bat) to update the translations. You can read [Qt Linguist Manual](https://doc.qt.io/qt-5/qtlinguist-index.html) if you are interested in it.

## The Settings system

The basic structure of the settings system is:

1.  The information of the settings are stored in [settings.json](src/Settings/settings.json).
2.  `settings.json` is translated to `SettingsHelper.hpp` and `SettingsInfo.cpp` by [genSettings.py](src/Settings/genSettings.py).
3.  Settings are displayed in [PreferencesWindow](src/Settings/PreferencesWindow.hpp) and separated into several [PreferencesPage](src/Settings/PreferencesPage.hpp)s. Most of the settings are displayed in [PreferencesPageTemplate](src/Settings/PreferencesPageTemplate.hpp) which uses `SettingsInfo` to generate the page.
4.  In other parts of CP Editor, you can get the settings via the functions in `SettingsHelper.hpp` or use [SettingsManager](src/Settings/SettingsManager.hpp) to manage the settings. The purpose of `SettingsHelper.hpp` is to enable auto-complete during development and reduce the chance of misspelling the name of a setting. `SettingsManager` is usually used in cases where the name of the setting is a viarable, for example: `SettingsManager::get(QString("%1/Compile Command").arg(language)).toString()`.

To learn how to add/modify settings, you can refer to other settings or read the [documentation](src/Settings/README.md). Basically, you need to modify [settings.json](src/Settings/settings.json) and the part starts with `AddPageHelper(this)` in [PreferencesWindow.cpp](src/Settings/PreferencesWindow.cpp).

## Tips

-   Reuse codes when possible, even if the implementation is short and easy.
-   Use [DefaultFileManager](src/Settings/DefaultPathManager.hpp) when asking the user to choose a path. (With a few exceptions, including choosing a path in the preferences window, where the path is already saved in the settings.)
-   Check the stderr when testing. The stderr contains warnings from Qt and `qDebug()` in the codes, which is a quick way to catch common mistakes.
-   Use `git submodule update --init --recursive` to update submodules.
