> 本页面由 [英文版本](README.md) 翻译而来，内容可能有所过时。

# CP Editor

<img src=assets/icon.ico height="80" width="80">

CP Editor 是一个基于 Qt 的轻量级跨平台代码编辑器，专为算法竞赛设计。

它可以让你刷题/打比赛更加轻松 :grin: 因为它自动化了很多算法竞赛中的流程。

[特性](https://cpeditor.github.io/) | [安装](doc/INSTALL_zh-CN.md) | [使用](doc/MANUAL_zh-CN.md) | [更新日志](doc/CHANGELOG.md) | [贡献指南](CONTRIBUTING_zh-CN.md) | [FAQ](#faq) | [English](README.md) | [Русский](README_ru-RU.md)

---

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-11-orange.svg)](#contributors-sparkles)
<!-- ALL-CONTRIBUTORS-BADGE:END -->
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ce0f297f31f74485b0d340949d08d605)](https://www.codacy.com/gh/cpeditor/cpeditor)
[![GitHub All Releases](https://img.shields.io/github/downloads/cpeditor/cpeditor/total?label=downloads%40all)](https://github.com/cpeditor/cpeditor/releases)
[![Slack Workspace](https://img.shields.io/badge/join-slack-success)](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)
[![QQ Group](https://img.shields.io/badge/join-QQ%20chat-success)](https://jq.qq.com/?_wv=1027&k=50eq8yF)
[![Help wanted issues](https://img.shields.io/github/issues/cpeditor/cpeditor/help%20wanted)](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

|  类型  |                            分支                            |                                                                                                        构建                                                                                                        |                                                                     下载量                                                                     |                                                         AUR                                                         |
| :----: | :--------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------: |
| alpha  | [master](https://github.com/cpeditor/cpeditor/tree/master) | [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=master&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Amaster) |                                                                       -                                                                        | [![AUR version](https://img.shields.io/aur/version/cpeditor-git)](https://aur.archlinux.org/packages/cpeditor-git/) |
|  beta  |   [v6.3](https://github.com/cpeditor/cpeditor/tree/v6.3)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.3&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.3)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.3.1/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.3.1) |                                                          -                                                          |
| stable |   [v6.2](https://github.com/cpeditor/cpeditor/tree/v6.2)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.2&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.2)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.2.5/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.2.5) |     [![AUR version](https://img.shields.io/aur/version/cpeditor)](https://aur.archlinux.org/packages/cpeditor/)     |

## 开始使用

- [发行版](https://github.com/cpeditor/cpeditor/releases) ([Gitee 镜像](https://gitee.com/ouuan/cpeditor/releases))
- [各种安装方式](doc/INSTALL_zh-CN.md)
- [使用说明与提示](doc/MANUAL_zh-CN.md)

## 做出贡献

- [提 issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [贡献指南](CONTRIBUTING_zh-CN.md)
- [适合新手解决的 issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- [需要帮助的 issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

## 获取帮助

1. 请首先阅读 [使用指南](doc/MANUAL_zh-CN.md) 和 [FAQ](#faq)，并看看应用菜单和设置中有没有能帮到你的。
2. 如果你要问的问题和 CP Editor 本身不太相关，请尝试通过百度来解决问题（例如：如何安装 Clang Format？这个编译错误是什么意思？）。
3. 在 [Issues](https://github.com/cpeditor/cpeditor/issues) 中搜索，确保你是第一个发现这个问题/提出这个请求的。
4. 在提 issue 时，请务必遵守模板，提供详细的信息，这有助于更快地获得更加令您满意的答复。
5. 如果您要报告 bug / 请求新特性，而非询问如何使用，请提 issue 而非在 QQ 群中或 Slack 上提问。

- [提 issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [在 QQ 群中提问](https://jq.qq.com/?_wv=1027&k=50eq8yF) (1081647997)
- [在 Slack 上提问](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)

## FAQ

- 我在写 Java，代码不能正常运行。
   - 你的代码使用的类名需要是 `a`，而且不能是 public class。
- 打开程序时提示缺失 DLL。
   - 请下载并安装 [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)。
- 如何使整个界面都是暗色主题？
   - 现在你可以在 KDE, macOS 以及 Windows 10 上使用暗色主题。你需要将系统主题设置为暗色主题，这样的话 CP Editor 也会使用暗色主题。然而，在 Windows 10 上，如果你使用暗色主题，当你切换代码编辑器的主题时，你可能会需要重启 CP Editor。
- 如何从网站上获取样例？
   - 你需要在浏览器上安装 [Competitive Companion](https://github.com/jmerle/competitive-companion) 插件, 然后在打开 CP Editor 的情况下使用它（点击绿色加号）。
- 如何在应用内提交代码到 CF 上?
   - 你需要使用 Competitive Companion 获取 CF 题目的样例，或者在标签页的右键菜单中设置题目地址，然后就可以看到提交按钮了。
- 为什么提交按钮无法点击？
   - 这是因为 CP Editor 不能运行 `cf` 命令。你可以将 CF Tool 加入 PATH，或者在首选项中设置 CF Tool 的路径。
- 提交时让我设置 template，那是什么？
   - 请在命令行中使用 `cf config` 来设置用户名，密码和模板。
- 使用 Competitive Companion 时，提示 invalid payload，那是什么?
   - 一般来说多获取几次样例，重启浏览器和 CP Editor，甚至更换浏览器可以解决这个问题。

## 关于中文

这是一个面向全世界的项目，在 Issues / Pull Requests 中请尽量使用英文。如果对自己的英语没有信心 / 觉得中文更加方便的话，可以在使用中文的同时附上一份 Google/百度翻译。尽量不要机翻，如果是纯机翻请附上原文，防止翻译错误导致理解出现偏差。提交信息（commit messages）的标题必须使用英文，内容则同 Issues / Pull Requests。

## 关于 Gitee

本项目同时在 [GitHub](https://github.com/cpeditor/cpeditor) 和 [Gitee](https://gitee.com/ouuan/cpeditor) 上进行托管，其中 Gitee 仓库仅作为镜像使用，不保证总是与 GitHub 仓库同步，不开放 Issues / Pull Requests，如有需要请移步 [GitHub](https://github.com/cpeditor/cpeditor)，也可以进入 [QQ 群](https://jq.qq.com/?_wv=1027&k=50eq8yF) (1081647997) 来交流讨论。

## Contributors :sparkles:

感谢下面这些人的贡献：（[emoji key](https://allcontributors.org/docs/en/emoji-key)）

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center">
      <a href="https://github.com/ouuan"><img src="https://avatars2.githubusercontent.com/u/30581822?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/ouuan"><sub><b>Yufan You</b></sub></a><br /><a href="#maintenance-ouuan" title="Maintenance">🚧</a> <a href="https://github.com/cpeditor/cpeditor/commits?author=ouuan" title="Code">💻</a> <a href="https://github.com/cpeditor/cpeditor/commits?author=ouuan" title="Documentation">📖</a> <a href="#translation-ouuan" title="Translation">🌍</a> <a href="#ideas-ouuan" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/cpeditor/cpeditor/issues?q=author%3Aouuan" title="Bug reports">🐛</a> <a href="https://github.com/cpeditor/cpeditor/pulls?q=is%3Apr+reviewed-by%3Aouuan" title="Reviewed Pull Requests">👀</a>
    </td>
    <td align="center">
      <a href="https://github.com/coder3101"><img src="https://avatars2.githubusercontent.com/u/22212259?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/coder3101"><sub><b>Ashar</b></sub></a><br /><a href="#maintenance-coder3101" title="Maintenance">🚧</a> <a href="https://github.com/cpeditor/cpeditor/commits?author=coder3101" title="Code">💻</a> <a href="https://github.com/cpeditor/cpeditor/commits?author=coder3101" title="Documentation">📖</a> <a href="#ideas-coder3101" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/cpeditor/cpeditor/issues?q=author%3Acoder3101" title="Bug reports">🐛</a> <a href="https://github.com/cpeditor/cpeditor/pulls?q=is%3Apr+reviewed-by%3Acoder3101" title="Reviewed Pull Requests">👀</a>
    </td>
    <td align="center">
      <a href="https://github.com/neko-para"><img src="https://avatars1.githubusercontent.com/u/26452623?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/neko-para"><sub><b>liaohy</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/commits?author=neko-para" title="Code">💻</a> <a href="https://github.com/cpeditor/cpeditor/commits?author=neko-para" title="Documentation">📖</a> <a href="#translation-neko-para" title="Translation">🌍</a> <a href="#ideas-neko-para" title="Ideas, Planning, & Feedback">🤔</a>
    </td>
    <td align="center">
      <a href="https://github.com/sadykhzadeh"><img src="https://avatars1.githubusercontent.com/u/51178055?v=4" width="100px;" alt=""/></a><br /><a href="http://sadykhzadeh.github.io"><sub><b>Azer Sadykhzadeh</b></sub></a><br /><a href="#translation-sadykhzadeh" title="Translation">🌍</a>
    </td>
    <td align="center">
      <a href="https://github.com/IZOBRETATEL777"><img src="https://avatars0.githubusercontent.com/u/32099652?v=4" width="100px;" alt=""/></a><br /><a href="http://izobretatelsoft.moy.su/"><sub><b>IZOBRETATEL777</b></sub></a><br /><a href="#translation-IZOBRETATEL777" title="Translation">🌍</a>
    </td>
  </tr>
  <tr>
    <td align="center">
      <a href="https://github.com/Razdeep"><img src="https://avatars2.githubusercontent.com/u/32531173?v=4" width="100px;" alt=""/></a><br /><a href="https://www.linkedin.com/in/razdeeproychowdhury"><sub><b>Rajdeep Roy Chowdhury</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/commits?author=Razdeep" title="Code">💻</a> <a href="#ideas-Razdeep" title="Ideas, Planning, & Feedback">🤔</a>
    </td>
    <td align="center">
      <a href="https://github.com/testitem"><img src="https://avatars3.githubusercontent.com/u/19212916?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/testitem"><sub><b>queen bee shuttle</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/commits?author=testitem" title="Code">💻</a> <a href="#ideas-testitem" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/cpeditor/cpeditor/issues?q=author%3Atestitem" title="Bug reports">🐛</a>
    </td>
    <td align="center">
      <a href="https://github.com/polyomino-24"><img src="https://avatars0.githubusercontent.com/u/20138086?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/polyomino-24"><sub><b>polyomino</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/commits?author=polyomino-24" title="Documentation">📖</a>
    </td>
    <td align="center">
      <a href="https://github.com/DarkWarrior703"><img src="https://avatars0.githubusercontent.com/u/56077342?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/DarkWarrior703"><sub><b>DarkWarrior703</b></sub></a><br /><a href="#ideas-DarkWarrior703" title="Ideas, Planning, & Feedback">🤔</a>
    </td>
    <td align="center">
      <a href="https://github.com/nikhil-sarda"><img src="https://avatars1.githubusercontent.com/u/51774873?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/nikhil-sarda"><sub><b>Nikhil Sarda</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/issues?q=author%3Anikhil-sarda" title="Bug reports">🐛</a>
    </td>
  </tr>
  <tr>
    <td align="center">
      <a href="https://github.com/fedimser"><img src="https://avatars3.githubusercontent.com/u/14271071?v=4" width="100px;" alt=""/></a><br /><a href="http://fedimser.github.io"><sub><b>Dmytro Fedoriaka</b></sub></a><br /><a href="https://github.com/cpeditor/cpeditor/issues?q=author%3Afedimser" title="Bug reports">🐛</a>
    </td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

本项目使用 [all-contributors](https://github.com/all-contributors/all-contributors)，欢迎各种形式的贡献！

许可证
----

[GNU General Public License Version 3.0](https://github.com/cpeditor/cpeditor/blob/master/LICENSE)
