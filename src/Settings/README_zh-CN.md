# Settings

此文件夹包含设置界面代码。由于这部分代码不会与外部代码有交互，故不需要处于任何命名空间。

### 简介

**`settings.json`**

这里有所有设置的描述信息，包括不在设置界面内的设置。

json数组的每一项表示一个配置项，结构如下：

```javascript
{
    "name": "",   // 必需，选项的内部名称。
    "desc": "",   // 可选，选项在设置界面的提示文本。如果未设置，则将使用将'/'替换为' '的内部名称。
    "type": "",   // 必须，选项的数据类型。应为其在代码中的类型。
    "default": 0, // 可选，选项的默认值。
    "old": [],    // 可选，选项过去的名称。应为ini配置键的列表。
    "ui": "",     // 可选，选项在设置界面中使用的UI类型。
    "param": ""   // 可选，选项在设置界面的控件创建时传递的参数。
}
```

**`ValueWrapper.hpp` `ValueWrapper.cpp`**

这里包含不同类型、控件的包装代码。

* `ValueWidget`

    最基本的封装。目前无特殊作用，请使用`Wrapper<Type>`。

* `Wrapper<Type>`

    对于类型Type的封装。对bool有特化。
    
    * init

        设置界面创建对象时初始化的函数。
    
    * get/set

        获取/设置数据的函数。

* `XXXWrapper`

    XXX控件的子类。

* `createXXXWrapper`

    根据type（即json中的ui）创建对应的控件。

**`genSettings.py`**

这里包含了在json中未设置默认值时的默认值。

> ```python
> defs = {
>     'QString':'""',
>     'int': '0',
>     'bool': 'false',
>     'QRect': 'QRect()',
>     'QByteArray': 'QByteArray()'
> }
> ```

**`SettingsInfo.hpp`**

    专为PreferencesPageTemplate设计使用。

**`SettingsHelper.hpp`**

    包含所有选项的简易`设置/获取函数。

    使用时请包含```<generated/SettingsHelper.hpp>```

    * 设置：`SettingsHelper::setXXX`

    * 获取：根据选项的类型为`SettingsHelper::getXXX`或`SettingsHelper::isXXX`

### 目前已实现支持设置界面的类型信息

第一个为默认的ui

* `bool`

    默认值`false`

    * `QCheckBox`

* `QString`

    默认值`""`

    * `QLineEdit`

    * `QPlainTextEdit`

    * `QComboBox`

        * 参数为代码形式的QStringList，代表预定义选项。
        
        * 例如```"param": "QStringList {\"item1\", \"item2\"}```

    * `PathItem`

        选择文件的控件。

        * 参数为一个整数，代表标题和过滤器在代码中的索引。

    * `ShortcutItem`

        设置快捷键的控件。
    
* `int`

    默认值```0```

    所有参数均为包含两个```int```的```QList<QVariant>```，代表数值范围。

    例如```"param": "QList<QVariant> {1, 100}"```

    * `QSpinBox`

    * `QScrollBar`

        始终为横向。
    
    * `QSlider`

        始终为横向。

* `QFont`

    默认值尚无

    * `FontItem`

        设置字体的控件。

* `QList<QVariant>`

    * `StringListsItem`

        多个 `QStringList`。

        令一个 `QStringList` 中的 `QString` 数量为 `<col>`。

        value: `QList<QVairant>`, 其中每个元素是一个包含 `<col>` 个 `QString` 的 `QStringList`。

        param: 这个参数描述了 UI 中的标题栏以及 `<col>` 的值。它是一个 有 `<col>` 个元素的 `QList<QVairant>`，其中每个元素是一个包含两个 `QString` 的 `QStringList`，第一个 `QString` 是标题，第二个 `QString` 是 tooltip。

### 添加设置

具体的实现细节请参考已有代码。

#### 添加新类型/控件

1. 在ValueWrapper中添加XXXWrapper，XXX建议为控件名。

2. 在ValueWrapper最后加上createXXXWrapper，XXX建议为类型名。

3. 在PreferencesPageTemplate的构造函数中添加对该类型的判断。

4. \[可选\]在genSettings.py中为类型设定一个默认值。

#### 添加新设置

1. 在setting.json中添加新项，按需设置。

#### 添加新的设置页面/选项

* 在PreferencesWindow的构造函数添加addPage(path, options)

    * path为页面在左侧的路径

    * options为设置的内部名称（name）的列表

* 在已有的addPage的options中添加新的设置名称

#### 在代码中使用

* 使用SettingsHelper中的设置/获取函数