# CP Editor

<img src=assets/icon.ico height="80" width="80">

CP Editor is a Qt-based code editor specially designed for competitive programming.

It makes your competitive coding life easier :grin: by automating many things for you.

## [<--- LEARN MORE ABOUT CP EDITOR --->](https://cpeditor.github.io/)

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ce0f297f31f74485b0d340949d08d605)](https://www.codacy.com/gh/cpeditor/cpeditor)
[![GitHub All Releases](https://img.shields.io/github/downloads/cpeditor/cpeditor/total?label=downloads%40all)](https://github.com/cpeditor/cpeditor/releases)
[![Telegram Group](https://img.shields.io/badge/join-telegram%20chat-success)](https://t.me/cpeditor)
[![Help wanted issues](https://img.shields.io/github/issues/cpeditor/cpeditor/help%20wanted)](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

|  type  |                           branch                           |                                                                                           build                                                                                            |                                                                   downloads                                                                    |
| :----: | :--------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: |
| alpha  | [master](https://github.com/cpeditor/cpeditor/tree/master) | [![GitHub Workflow Status](https://img.shields.io/github/workflow/status/cpeditor/cpeditor/CI:%20Build%20Test/master)](https://github.com/cpeditor/cpeditor/actions?query=branch%3Amaster) |                                                                       -                                                                        |
|  beta  |   [v6.1](https://github.com/cpeditor/cpeditor/tree/v6.1)   |   [![GitHub Workflow Status](https://img.shields.io/github/workflow/status/cpeditor/cpeditor/CI:%20Build%20Test/v6.1)](https://github.com/cpeditor/cpeditor/actions?query=branch%3Av6.1)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.1.2/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.1.2) |
| stable |   [v6.0](https://github.com/cpeditor/cpeditor/tree/v6.0)   |   [![GitHub Workflow Status](https://img.shields.io/github/workflow/status/cpeditor/cpeditor/CI:%20Build%20Test/v6.0)](https://github.com/cpeditor/cpeditor/actions?query=branch%3Av6.0)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.0.6/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.0.6) |

## Get Started

- [Releases](https://github.com/cpeditor/cpeditor/releases)
- [Install instructions](doc/INSTALL.md)
- [Get Started and Tips](doc/MANUAL.md)

## Contributing

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Contributing Guidelines](doc/CONTRIBUTING.md)
- [Good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- [Help wanted issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

## Get help

1. Read the [manual](doc/MANUAL.md) first, also go through the menus to see if there are what you are looking for.
2. Ask Google first if it's not very editor-related. (e.g. How to install Clang Format?)
3. Search in the [Issues](https://github.com/cpeditor/cpeditor/issues) and make sure your bug report/feature request is not duplicated.
4. Please follow the issue template and provide detailed information, it will help us to give you better feedback.
5. If it's a feature request or bug report rather than a question, please open an issue instead of asking on Telegram so that it can be tracked easier.

- [Open an issue](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Ask on Telegram](https://t.me/cpeditor)

## FAQ

- I am using Java and the editor can't run my codes.
   - You have to use a **non-public** class named **a** for your solution.
- I get **DLL Missing error** when launching the application?
   - Please download  [Microsoft Visual C++ Redistributable for Visual Studio 2015, 2017 and 2019](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
- How to use whole-application dark theme?
   - It's only available on some platforms including KDE and MacOS.
- I am using it on Codeforces but the submit button is not clickable.
   - It's because the editor can't run the `cf` command. Please make sure it's in the PATH or set the path to it in the Preferences.
- When I click the Submit button, I get some message saying template is required?
   - `cf` tool requires you to configure it before you use it. Please run `cf config` to set the username & password and add a template.
- I got an invalid payload when parsing sample testcases?
   - Please try restarting the browser and the editor. It's known that there are few people always fail on this even after restarting, and Firefox is more stable than Google Chrome. We're really sorry if it always fails, you can change a browser as for now, we will try our best to investigate it.

License
----

[GNU General Public License Version 3.0](https://github.com/cpeditor/cpeditor/blob/master/LICENSE)
