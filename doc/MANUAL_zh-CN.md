> 本页面由 [英文版本](MANUAL.md) 翻译而来，内容可能有所过时。

# 开始使用

我们为您选择 CP Editor 感到高兴，我们现在就开始吧！:tada:

- 安装你想使用的语言的编译器：`g++` / `python` / `java`。

- 在 Preferences 中修改设置（Options -> Preferences, 或者使用快捷键 <kbd>Ctrl+P</kbd>）。

- 如果你想从网站上获取样例，你需要在浏览器上安装 [Competitive Companion](https://github.com/jmerle/competitive-companion)。

- 如果你想在 CP Editor 内提交代码到 CF 上，你需要安装 [CF Tool](https://github.com/xalanq/cf-tool)。如果 CF Tool 不在 PATH 中，你可以在 Preferences->Extensions->CF Tool 中设置 CF Tool 的路径。不要忘了在命令行中使用 `cf config` 设置用户名，密码和模板.

- 如果你想格式化你的代码，你需要安装 [Clang Format](http://releases.llvm.org/download.html)。如果 clang-format 不在 PATH 中，你可以在 Preferences->Extensions->Clang Format 设置 clang-format 的路径。

# 提示

## CF Tool

你需要使用 Competitive Companion 获取题目样例，或者在标签页的右键菜单中设置题目地址，然后就有提交按钮了。如果 CF Tool 没有被找到，提交按钮就无法点击，可以在 Preferences->Extensions->CF Tool 中设置 CF Tool 的路径。

## 设置

### 快捷键

你可以分别使用 <kbd>Ctlr+D</kbd>, <kbd>Ctrl+R</kbd> 和 <kbd>Ctrl+S</kbd> 来恢复本页的默认设置，恢复到本次修改前的状态，保存本次修改。

你可以使用 <kbd>Ctlr+Tab</kbd> 和 <kbd>Ctlr+Shift+Tab</kbd> 来切换页面。

### Save files faster

当这个选项启用时，总是会用 [QFile](https://doc.qt.io/qt-5/qfile.html) 来保存文件, 未启用时有时会用 [QSaveFile](https://doc.qt.io/qt-5/qsavefile.html) 来保存文件。

启用这个选项可以使保存文件稍快一点，但如果在保存文件的过程中发生错误，原文件也会丢失（如果是没有权限的话不会丢失）。这种错误发生的概率非常低，所以基本上可以放心地启用这个选项。

### Runtime Arguments

这些参数会在运行时作为程序的命令行参数，一般情况下用不上。

### Clang Format

"Clang Format Style" 中的内容会被用作 `.clang-format` 文件。你可以在 [这里](https://clang.llvm.org/docs/ClangFormat.html) 学习如何编写 `.clang-format` 文件。

### Snippets

你可以使用 <kbd>Ctrl+N</kbd> 来添加 snippet, 使用 <kbd>Ctrl+W</kbd> 来删除当前 snippet，使用 <kbd>F2</kbd> 来重命名当前 snippet。

你可以在主窗口内使用 <kbd>Ctrl+T</kbd> 来使用 snippets。

## 代码编辑器中的快捷键

除了菜单中可以看到的快捷键，还有这些快捷键：

 - [（Qt）只读的快捷键](https://doc.qt.io/qt-5/qtextedit.html#read-only-key-bindings)
 - [（Qt）修改的快捷键](https://doc.qt.io/qt-5/qtextedit.html#editing-key-bindings)
 - 其它快捷键:
   - <kbd>Tab</kbd>（有选中部分时）/ <kbd>Shift+Tab</kbd>：缩进 / 减少缩进。
   - <kbd>Ctrl+Enter</kbd> / <kbd>Ctrl+Shift+Enter</kbd>：在在后 / 前插入一行。
   - <kbd>Shift+Delete</kbd>：删除当前行。

## 拖拽

你可以将文件拖入代码编辑器 / 测试点输入 / 测试点答案。

## 标签页

你可以使用 <kbd>Ctrl+Tab</kbd> 和 <kbd>Ctrl+Shift+Tab</kbd> 来切换到下一个/上一个标签页。

右键单击标签页可以打开一个菜单。菜单的内容会根据标签页是否有标题 / 是否设置了题目地址而变化。

## 测试点

Testlib checkers 是在更新设置 / 更换 checkers 时编译的，所以如果刚刚更新设置 / 更换 checkers，可能需要等几秒才能获取评测结果。

在测试点文本输入框右键单击打开的菜单中，可以从文件读取 / 在更大的窗口中编辑测试点。

可以点击评测状态来打开差异比较器。

## 视图模式（View）

不同的视图可以帮助你在专心写代码和调试之间切换，纯代码编辑器 / 纯 IO 模式还能使窗口更小以方便分屏。

你可以在设置中为切换视图模式设置快捷键。

## 在命令行中启动 CP Editor

CP Editor 支持一些命令行参数，具体可以执行 `cpeditor --help` 查看。

在 Windows 上，最好使用 git bash 等类 Linux 终端来执行命令。

你可以通过命令行打开一个不存在的文件（稍后可以保存在这个路径）作为一个标签页，或者打开一个文件夹内的所有 C++ 文件。
