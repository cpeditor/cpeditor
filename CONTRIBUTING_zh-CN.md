## 你的第一次贡献

-   [如何在 GitHub 上为开源项目做贡献](https://app.egghead.io/courses/how-to-contribute-to-an-open-source-project-on-github)
-   [从源代码构建 CP Editor](https://cpeditor.org/zh/docs/installation/build-from-source/)
-   [Qt 文档](https://doc.qt.io/)
-   [探索 issues](https://github.com/cpeditor/cpeditor/contribute)

## 在开始写代码之前

如果你想解决的问题并没有对应的 issue，请在开始写代码之前开一个 issue，因为：

1.  你可能可以从他人处获得帮助。
2.  其他人可以知道你在试图解决这个问题，就可以避免可能的重复工作。
3.  如果你想添加的功能不被接受，或者你发现的 "bug" 实际上并不是一个 bug，你可以被提醒，从而节约你的时间。

## 贡献守则

-   记住，目标用户是算法竞赛选手，不是开发者。
-   一个 PR 只干一件事。如果你需要添加多个 feature / 修复多个 bug，并且它们之间没有什么关联，请开多个 PR。你可能需要开多个分支来提交多个 PR，你可以在 [这里](https://learngitbranching.js.org/) 学习如何在不同的分支上工作。
-   请在 master 分支上添加特性，在最早的含有该 bug 的活动分支（master, beta 和 stable）上修复 bug。参阅[版本规则](VERSIONING.md)。
-   请撰写 [清晰的 commit messages](https://chris.beams.io/posts/git-commit/)。推荐遵循 [Conventional Commits](https://www.conventionalcommits.org/zh-hans) 规范。因为我们使用 [Squash and merge](https://docs.github.com/cn/free-pro-team@latest/github/collaborating-with-issues-and-pull-requests/about-pull-request-merges#压缩与合并拉取请求提交)，一个 PR 中的 commit messages 没有那么重要，而 PR 的标题，或者是仅有一个 commit 的 PR 的 commit message 则比较重要。
-   请使用 Clang Format 以及 [`.clang-format`](.clang-format) 格式化代码。如果在格式化过程中发生了错误，请尝试将你的 `clang-format` 更新到[最新版本](https://releases.llvm.org/download.html)。
-   尽量不要移除已有的 feature，而应增加一个启用/禁用的选项。
-   永远保证设置文件的向下兼容性。
-   请遵守[行为准则](CODE_OF_CONDUCT.md)。

## 文档

文档在 [cpeditor.github.io](https://github.com/cpeditor/cpeditor.github.io) 维护。你可以阅读它的[贡献指南](https://github.com/cpeditor/cpeditor.github.io/blob/hugo/CONTRIBUTING.md)以获取更多信息。

## 翻译

翻译文本在 [translations](translations) 文件夹中。你应当用 Qt Linguist 编辑它们。只不过，如果你只是想修正一个笔误，你可以使用任何文本编辑器。在 Qt Linguist 中，当你完成了一条翻译，你可以按 <kbd>Ctrl + Enter</kbd> 来把当前条目标记为已完成并跳转到下一个未完成的条目。如果一个条目不需要翻译（例如，"C++"），你只需将翻译留空即可。你应当修复所有在 Qt Linguist 中显示的警告。

如果你想更深入地了解：需翻译的文本在源代码中用 `tr()` 括起；在代码改变后，你可以运行 [updateTranslation.sh](tools/updateTranslation.sh) 或 [updateTranslation.bat](tools/updateTranslation.bat) 来更新翻译文本。如果感兴趣的话，你可以阅读 [Qt Linguist 使用手册](https://doc.qt.io/qt-5/qtlinguist-index.html)。

## 设置系统

设置系统的基本结构如下：

1.  设置的信息存储在 [settings.json](src/Settings/settings.json)。
2.  `settings.json` 被 [genSettings.py](src/Settings/genSettings.py) 翻译成 `SettingsHelper.hpp` 和 `SettingsInfo.cpp`。
3.  设置通过 [PreferencesWindow](src/Settings/PreferencesWindow.hpp) 显示，并被分成若干个 [PreferencesPage](src/Settings/PreferencesPage.hpp)。大多数设置通过 [PreferencesPageTemplate](src/Settings/PreferencesPageTemplate.hpp) 显示，基于 `SettingsInfo` 自动生成页面。
4.  在 CP Editor 的代码中，你可以使用 `SettingsHelper.hpp` 中提供的函数或者 `SettingsManager` 来访问/修改设置。`SettingsHelper.hpp` 的作用是使设置可以被自动补全，以及减少笔误的可能性。`SettingsManager` 通常用于设置名称是变量的情景，例如：`SettingsManager::get(QString("%1/Compile Command").arg(language)).toString()`。

你可以参考其它设置来添加/修改设置项。你也可以参考[文档](src/Settings/README_zh-CN.md)。简单来说，你需要修改 [settings.json](src/Settings/settings.json) 以及 [PreferencesWindow.cpp](src/Settings/PreferencesWindow.cpp) 中以 `AddPageHelper(this)` 开头的那一段。

## 提示

-   尽可能地重用代码，即使代码很短。
-   如果你要询问用户一个文件路径，使用 [DefaultFileManager](src/Settings/DefaultPathManager.hpp)。（有少数例外，例如在设置页面中，路径本身就被保存在设置里。）
-   在调试时查看 stderr。stderr 包含 Qt 内部的警告以及使用 `qDebug()` 输出的信息，是一个捕捉常见错误的好方法。
-   使用 `git submodule update --init --recursive` 来更新子模组。
