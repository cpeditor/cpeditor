> 本页面由 [英文版本](CONTRIBUTING.md) 翻译而来，内容可能有所过时。

## 欢迎！

你是来帮助 CP Editor 变得更好的吗？好的想法是一切的开始，耐心地阅读下面的内容，相信你也可以做出自己的贡献。CP Editor 就是在像你一样的人的帮助下变得越来越好的。:smile:

## 我们需要什么样的贡献？

- 如果你觉得 CP Editor 还不错，可以给我们一个 star 并分享给身边的人。
- 通过 [提 issues](https://github.com/cpeditor/cpeditor/issues/new/choose) 来报告 bug 或者请求新特性。提 feature 时请严格遵守模板，这有助于我们弄清楚 bug 的原因，以及你到底需要什么样的新特性。
- 在 GitHub，[QQ 群](https://jq.qq.com/?_wv=1027&k=50eq8yF) 或其它平台上为他人解答问题。
- 写博客/教程来介绍 CP Editor。
- 帮助我们改进文档，包括 Markdown 文件和代码中的注释。
- 参与代码的编写。

## 第一次参与贡献？

**第一次使用 GitHub?** 你可以通过这个免费教程学习：[How to Contribute to an Open Source Project on GitHub](https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github)。简而言之，你需要 fork 这个代码仓库，在你的 fork 中修改代码，完成修改后提交 Pull Request，等待审核并采纳意见。

**第一次参与开发?** 工程开发和算法竞赛有着很大的区别，但并没有那么难以上手。首先，你需要了解面向对象编程：你需要知道 C++ 中类是怎样工作的，知道类的定义与函数实现是分开放在两个文件里的（在 `.h`/`.hpp` 文件中放置定义，在 `.cpp` 文件中放置函数实现）。你可以阅读 CP Editor 的源码，模仿它，在百度（如果能用 Google 更好）上搜索你想实现的功能。开发和算法竞赛使用的 IDE 也是不同的，对于工程开发来说，[VS Code](https://code.visualstudio.com/) 是一个不错的选择。

**第一次使用 Qt?** 不要怕，Qt 并不难上手。你可以阅读 [从源代码构建](https://cpeditor.org/docs/installation/build-from-source/) 来学习如何构建 CP Editor，阅读 [Qt 文档](https://doc.qt.io/) 来学习如何使用 Qt 组件，如何设计 UI，以及如何使用 [信号与插槽机制](https://doc.qt.io/qt-5/signalsandslots.html)。在使用 Qt 时，你一般不需要使用 `std` 命名空间中的函数 / 对象，例如，`std::string` 可以使用 `QString` 代替。

**不知道从哪开始？** 你可以查看 [issues](https://github.com/cpeditor/cpeditor/issues) 来找到你感兴趣的那个。[Good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) 是被认为对新手友好的 issues，你可以从它们开始。如果遇到了什么问题，可以在 [QQ 群](https://jq.qq.com/?_wv=1027&k=50eq8yF) 中或对应的 issue 中提问。

## 在开始写代码前

在开始写代码前，请先开一个 issue，在普通 issue 的基础上说明你会来解决它，可以附带你设想的解决方案。

原因有以下几点：

1. 你可能可以从他人处获得帮助。
2. 其他人可以知道你在试图解决这个问题，就可以避免可能的重复工作。
3. 如果你新增的 feature 不被接受，或者你发现的“bug”实际上并不是 bug，你可以被提醒，从而节约你的时间。

## 贡献守则

1. 记住，目标用户是算法竞赛选手，不是开发者。
2. 一个 PR 只干一件事。如果你需要添加多个 feature / 修复多个 bug，并且它们之间没有什么关联，请开多个 PR。你可能需要开多个分支来提交多个 PR，你可以在 [这里](https://learngitbranching.js.org/) 学习在不同的分支上工作。
3. 请在 master 分支上添加特性，在最早的含有该 bug 的活动（master, beta 和 stable）分支上修复 bug。
4. 请撰写 [清晰的 commit messages](https://chris.beams.io/posts/git-commit/)。推荐遵守 [约定式提交](https://www.conventionalcommits.org/zh-hans/) 的规范，你可以使用 [commitizen/cz-cli](https://github.com/commitizen/cz-cli) 来撰写提交信息。只不过，这些规范都是不必强行遵守的，关键是你的 commit messages 要能让别人和未来的自己明白每个 commit 中有哪些改动，以及为何进行这些改动。
5. 请使用 Clang Format 9（你可以通过 `clang-format --version` 来获取你的 Clang Format 版本）来格式化代码。你可能需要为你的 IDE 设置 clang-format 可执行文件路径，例如，VS Code 可能自带了 Clang Format 6，需要手动设置成你安装的 Clang Format 9。
6. 尽量不要移除已有的 feature，请增加一个启用 / 禁用的选项。
7. 永远保持设置文件的向下兼容。
8. Windows，Linux 和 Mac OS 都要支持。
9. 系统浅色主题/深色主题都要支持。
10. 请遵守 [行为准则](CODE_OF_CONDUCT.md)。

## FAQ

- 这是我第一次参与开发，我可以做得好吗？
  - 大家都是从第一次开始的。不要担心，其它 contributors 会在你遇到困难时给予帮助。只要你热爱开源并尝试，一切都会越来越好。
- 我需要学习使用 CMake 吗？
  - 不需要，你只需要知道如何 [从源代码构建](https://cpeditor.org/docs/installation/build-from-source/) 即可。
- 如何添加一个设置？
  - 你只需修改 [settings.json](src/Settings/settings.json) 和 [PreferencesWindow.cpp](src/Settings/PreferencesWindow.cpp)，你可以参照其它设置来进行修改。
- 有些文件在编译时缺失了。
  - 如果 CI 测试通过了，这往往是因为子组件更新了。

    你可以尝试执行下面的命令：

    ```sh
    git add .
    git reset --hard
    git submodule update --init
    ```

    你还可以尝试使用 `git rm` 删除所有子组件然后再添加，或者删除本地仓库后重新 clone。
