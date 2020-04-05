# CP Editor

<img src=assets/icon.ico height="80" width="80">

CP Editor - это легкий кросс-платформенный редактор кода на базе Qt, специально разработанный для спортивного программирования.

Этот редактор облегчает жизнь спортивного прораммиста :grin: автоматизируя многие вещи.

[Возможности](https://cpeditor.github.io/) | [Установка](doc/INSTALL_ru-RU.md) | [Использование](doc/MANUAL_ru-RU.md) | [Список изменений](doc/CHANGELOG.md) | [Сотрудничество](CONTRIBUTING_ru-RU.md) | [FAQ](#faq) | [English](README.md) | [中文](README_zh-CN.md)

---

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ce0f297f31f74485b0d340949d08d605)](https://www.codacy.com/gh/cpeditor/cpeditor)
[![Все релизы GitHub](https://img.shields.io/github/downloads/cpeditor/cpeditor/total?label=downloads%40all)](https://github.com/cpeditor/cpeditor/releases)
[![Группа в Телеграм](https://img.shields.io/badge/join-telegram%20chat-success)](https://t.me/cpeditor)
[![Помощь в отлове багов](https://img.shields.io/github/issues/cpeditor/cpeditor/help%20wanted)](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

|  type  |                           branch                           |                                                                                                       build                                                                                                        |                                                                   downloads                                                                    |
| :----: | :--------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------------------------: |
| alpha  | [master](https://github.com/cpeditor/cpeditor/tree/master) | [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=master&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Amaster) |                                                                       -                                                                        |
|  beta  |   [v6.2](https://github.com/cpeditor/cpeditor/tree/v6.2)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.2&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.2)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.2.2/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.2.2) |
| stable |   [v6.1](https://github.com/cpeditor/cpeditor/tree/v6.1)   |   [![GitHub Workflow Status](https://github.com/cpeditor/cpeditor/workflows/CI:%20Build%20Test/badge.svg?branch=v6.1&event=push)](https://github.com/cpeditor/cpeditor/actions?query=event%3Apush+branch%3Av6.1)   | [![Downloads](https://img.shields.io/github/downloads/cpeditor/cpeditor/6.1.4/total)](https://github.com/cpeditor/cpeditor/releases/tag/6.1.4) |

## Начало

- [Релизы](https://github.com/cpeditor/cpeditor/releases)
- [Установка разными способами](doc/INSTALL_ru-RU.md)
- [Начало и советы](doc/MANUAL_ru-RU.md)

## Сотрудничество

- [Открыть вопрос](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Правила внесения вклада](CONTRIBUTING_ru-RU.md)
- [Good first issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22)
- [Help wanted issues](https://github.com/cpeditor/cpeditor/issues?q=is%3Aissue+is%3Aopen+label%3A%22help+wanted%22)

## Получить помощь

1. Прочитайте в первую очередь [мануал](doc/MANUAL_ru-RU.md) и [FAQ](#faq), кроме того, просмотрите меню и настройки, чтобы узнать, есть ли то, что вы ищете.
2. Сначала поищите в Google или Яндекс. Может проблема не связана с редактором? Например, "как установить ClangFormat?" или "что такое ошибка компиляции?"
3. Поищите в [Issues](https://github.com/cpeditor/cpeditor/issues) и убедитесь, что такой ошибки/запроса функции еще не было.
4. Пожалуйста, следуйте шаблону вопроса и предоставьте подробную информацию, т.к. это поможет нам дать наиболее качественный ответ на ваш вопрос.
5. Если это запрос функции или сообщение об ошибке, а не вопрос, пожалуйста, создайте тему в [Issues](https://github.com/cpeditor/cpeditor/issues) для более быстрого баг-трекинга, а не спрашивайте в Telegram.

- [Открыть вопрос](https://github.com/cpeditor/cpeditor/issues/new/choose)
- [Спросить в Telegram](https://t.me/cpeditor)

## FAQ

- Я использую Java. Редактор не запускать мой код. Что делать?
   - Вы должны использовать **non-public** класс **а** для своего решения.
- Я получил ошибку **DLL Missing error** во время запуска программы. Что делать?
   - Пожалуйста, установите [Распространяемый компонент Microsoft Visual C++ для Visual Studio 2015, 2017 и 2019](https://support.microsoft.com/ru-ru/help/2977003/the-latest-supported-visual-c-downloads).
- Как включить темную тему во всем приложении?
   - Темная тема доступна только на некоторых платформах, включая KDE и Mac OS. Просто установите системную тему, и CP Editor будет использовать эту же тему. Также можно использовать различные темы с помощью опции командной строки `--style`, но это уже особенность, [которую предоставляет Qt](https://doc.qt.io/qt-5/qstyle.html#details), а не CP Editor. Она работает НЕ НА ВСЕХ платформах.
- Как получить входные/выходные данные с веб-сайтов?
   - Вам нужно установить [Competitive Companion](https://github.com/jmerle/competitive-companion) на Ваш браузер, и использовать его на веб-сайте, пока открыт CP Editor.
- Как послать решение в CF внутри редактора?
   - Вы должны либо отправить задачу через Competitive Companion, либо установить URL-адрес задачи в контекстном меню вкладок. Затем вы увидите кнопку "Submit".
- Я использую его на Codeforces, но кнопка "Submit" неактивна.
   - Это потому, что редактор не может выполнить команду `cf`. Пожалуйста, убедитесь, что он находится в PATH или установите путь в Настройках.
- Когда я нажимаю кнопку "Submit", я получаю какое-то сообщение, что необходим шаблон?
   - Инструмент `cf` требует предварительной настройки. Пожалуйста, запустите `cf config`, чтобы установить имя пользователя и пароль. Затем добавить шаблон.
- Я получил ошибку при копировании входных/выходных данных?
   - Пожалуйста, попробуйте перезагрузить браузер и редактор. Мы знаем, что есть несколько человек, которые получают ошибку даже после перезагрузки (чаще на Google Chrome, чем на Firefox). Нам очень жаль, но если у Вас ещё есть эта ошибка, попробуйте изменить браузер, а пока мы постараемся сделать все возможное, чтобы такой ошибки не было.

Лицензия
----

[GNU General Public License Version 3.0](https://github.com/cpeditor/cpeditor/blob/master/LICENSE)
