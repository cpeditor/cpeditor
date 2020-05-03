# CP Editor

<img src=assets/icon.ico height="80" width="80">

CP Editor is a Qt-based, lightweight and cross-platform code editor specially designed for competitive programming.

It makes your competitive coding life easier :grin: by automating many things for you.

[Features](https://cpeditor.github.io/) | [Installation](doc/INSTALL.md) | [Usage](doc/MANUAL.md) | [Changelog](doc/CHANGELOG.md) | [Contributing](CONTRIBUTING.md) | [FAQ](#faq) | [中文](README_zh-CN.md) | [Русский](README_ru-RU.md)

---

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-11-orange.svg)](#contributors-sparkles)
<!-- ALL-CONTRIBUTORS-BADGE:END -->
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ce0f297f31f74485b0d340949d08d605)](https://www.codacy.com/gh/cpeditor/cpeditor)
[![GitHub All Releases](https://img.shields.io/github/downloads/cpeditor/cpeditor/total?label=downloads%40all)](https://github.com/cpeditor/cpeditor/releases)
[![Slack Workspace](https://img.shields.io/badge/join-slack-success)](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)
[![Telegram Group](https://img.shields.io/badge/join-telegram%20chat-success)](https://t.me/cpeditor)
[![Help wanted issues](https://img.shields.io/github/issues/cpeditor/cpeditor/help%20wanted)](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

|  type  |                           branch                           |                                                                                                       build                                                                                                        |                                                                   downloads                                                                    |                                                         AUR                                                         |
| :----: | :--------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------: |
| alpha  | [master](https://github.com/cpeditor/cpeditor/tree/master) | [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=master&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Amaster) |                                                                       -                                                                        | [![AUR version](https://img.shields.io/aur/version/cpeditor-git)](https://aur.archlinux.org/packages/cpeditor-git/) |
|  beta  |   [v6.3](https://github.com/cpeditor/cpeditor/tree/v6.3)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.3&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.3)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.3.1/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.3.1) |                                                          -                                                          |
| stable |   [v6.2](https://github.com/cpeditor/cpeditor/tree/v6.2)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.2&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.2)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.2.5/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.2.5) |     [![AUR version](https://img.shields.io/aur/version/cpeditor)](https://aur.archlinux.org/packages/cpeditor/)     |

## Get Started

- [Releases](https://github.com/cpeditor/cpeditor/releases)
- [Install in different ways](doc/INSTALL.md)
- [Get Started and Tips](doc/MANUAL.md)

## Contributing

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Contributing Guidelines](CONTRIBUTING.md)
- [Good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- [Help wanted issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

## Get help

1. Read the [manual](doc/MANUAL.md) and the [FAQ](#faq) first, also go through the menus and preferencecs to see if there are what you are looking for.
2. Ask Google first if it's not very editor-related. (e.g. How to install Clang Format? What does this compilation error mean?)
3. Search in the [Issues](https://github.com/cpeditor/cpeditor/issues) and make sure your bug report/feature request is not duplicated.
4. Please follow the issue template and provide detailed information, it will help us to give you better feedback.
5. If it's a feature request or bug report rather than a question, please open an issue instead of asking on Telegram or Slack so that it can be tracked easier.

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Ask on Telegram](https://t.me/cpeditor)
- [Ask on Slack](https://join.slack.com/t/cpeditor/shared_invite/zt-dke1v9xd-zr~QeXJhCzbM9FFOjx6sMA)

## FAQ

- I am using Java and the editor can't run my codes.
   - You have to use a **non-public** class named **a** for your solution.
- I get **DLL Missing error** when launching the application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
- How to use whole-application dark theme?
   - Now you can use whole-application dark theme on KDE, macOS and Windows 10. You need to set the system theme to dark theme, then CP Editor will use dark theme, too. However, on Windows 10, if you use the dark theme, you may need to restart the application after changing the theme of the code editor.
- How to fetch testcases from the websites?
   - You have to install [Competitive Companion](https://github.com/jmerle/competitive-companion) on your browser, and use it on the website while CP Editor is running.
- How to submit to CF inside the editor?
   - You have to either parse the problem from Competitive Companion, or set the problem URL in the right-click menu of the tabs. Then you'll see the submit button.
- I am using it on Codeforces but the submit button is not clickable.
   - It's because the editor can't run the `cf` command. Please make sure it's in the PATH or set the path to it in the Preferences.
- When I click the Submit button, I get some message saying template is required?
   - `cf` tool requires you to configure it before you use it. Please run `cf config` to set the username & password and add a template.
- I got an invalid payload when parsing sample testcases?
   - Please try restarting the browser and the editor. It's known that there are few people always fail on this even after restarting, and Firefox is more stable than Google Chrome. We're really sorry if it always fails, you can change a browser as for now, we will try our best to investigate it.
- How to enable error linting in the code editor?
   - You need to install a [Language Server program](https://microsoft.github.io/language-server-protocol/implementors/servers/) and set the settings in Preferences -> Extensions -> Language Server. You can learn about more details [here](doc/MANUAL.md).

## Contributors :sparkles:

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

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
      <a href="https://github.com/m3h3d1"><img src="https://avatars0.githubusercontent.com/u/39239695?v=4" width="100px;" alt=""/></a><br /><a href="https://github.com/m3h3d1"><sub><b>m3h3d1</b></sub></a><br /><a href="#ideas-m3h3d1" title="Ideas, Planning, & Feedback">🤔</a>
    </td>
  </tr>
</table>

<!-- markdownlint-enable -->
<!-- prettier-ignore-end -->
<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!

License
----

[GNU General Public License Version 3.0](https://github.com/cpeditor/cpeditor/blob/master/LICENSE)
