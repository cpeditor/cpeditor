# Settings

此文件夹包含设置界面代码。由于这部分代码不会与外部代码有交互，故不需要处于任何命名空间。

### 简介

**`settings.yaml`**

包含三部分。

#### presetMethod

定义常用函数。

* 适用于`getDefault`

    * `pickDefault`: 从`param.default.key`中提取默认值。如果`key`不在`param.default`中，返回外部定义的默认值。

    * `genByRestrict`: 从`param.restrict`中提取默认值。

* 适用于`getParam`

    * `pickParam`: 从`param.key`中提取参数值。

    * `takeParam`: 从`param.param`中提取参数值。

    * `pickParamFromParam`: 从`param.param.key`中提取参数值。

* 适用于`getTip`

    * `pickTip`: 从`param.tip`中提取提示。它应该包含翻译的版本。例如 `tip: "!the tip"`。

#### pageInfo

定义如何创建设置对话框。

如果一组设置项只在部分系统上支持，可使用`<xxx>`作为前缀，其中的`xxx`是`w`(Windows)、`u`(Unix,Linux)、`m`(MacOS)的组合，且不区分大小写。

```yaml
<wu>Options only for windows and unix:
- aaa
- bbb
```

目前尚不支持针对单个设置项限定系统。

#### data

这里有所有设置的描述信息，包括不在设置界面内的设置。

yaml数组的每一项表示一个配置项，结构如下：

```yaml
    name:
    # 必须，选项的内部名称。

    type:
    # 必须，选项的数据类型。应当是代码中的类型。

    desc:
    # 可选，用于设置界面的提示文本。将会被翻译。如果未设置，将会使用将'/'替换为' '的内部名称。

    default:
    # 可选，选项的默认值。

    old:
    # 可选，用于向前兼容的旧键值列表。
    - ~旧键

    ui:
    # 可选，在设置界面使用的UI控件。

    tip:
    # 可选，鼠标指向控件时显示的提示。将会被翻译。

    param:
    # 可选，在设置界面创建时传递给控件的参数。

    methods:
    # 可选，用于各种用于的函数。

        ~函数名:
        # onApply: 当用户改变该设置后点击 应用 时被调用。适合用于显示警告或相关信息。
        # getDefault, getParam, getTip: 当需要默认值、参数或提示时被调用。适合用于需要动态计算的情况。
            param:
                ~参数名: ~类型
            code: ~代码

        ~另一个函数名: ~预定义函数名
        # 使用在 presetMethod 中定义的函数

    depends:
    # 可选，如果所有依赖均满足则会启用该选项，否则禁用之。
    -   name:
        # 依赖的名称。应当位于同一页，页面内对象上层的选项也可以被检测到。
        checks:
        # 检查var(QVariant)，返回真假值的函数。

    immediateApply:
    # 可选，为真时，选项会在被用户改变时直接应用，不需要用户按下 应用 按钮。

    sub:
    # 当类型为Object时定义子选项。
    - name:
}
```

关于函数

如果代码过长，我们建议将其写在`SettingsMethods.cpp`中然后调用它。

* `onApply`

    * `parent`: 根控件

* `getParam`

    * `param`: 原始参数
    
    * `pre`: 对象键、名称/提示列表
    
* `getDefault`

    * `param`: `getParam`的返回值
    
    * `rawparam`: 原始参数
    
    * `def`: 原始默认值

    * `pre`: 对象键、名称/提示列表
    
* `getTip`

    * `param`: `getParam`的返回值
    
    * `rawparam`: 原始参数
    
    * `needtr`: 是否需要翻译

    * `tip`: 原始提示信息
    
    * `pre`: 对象键、名称/提示列表

提示和鼠标指向时的提示均会被格式化。使用 @x （x是数字）来指代对象键或设置名称。使用 @@ 来表达 @。被替换的字符串将会自动使用被翻译的版本。

    * @0: 当前设置名称或提示

    * @1: 父对象的对象键

    * @2: 父对象的设置名称或提示

    * ……

参数值和默认值可以使用任何`yaml`支持的数据结构。`genSettings.py`会处理它们。

为了防止错误解析字符串，我们使用了以下规则：

* 没有前缀的字符串值会被当做一个原始字符串。

    * `hello` -> `"hello"`

* 有@前缀的字符串会被当做一个字符串的表达式。

    * `@tr(\"foobar\")` -> `tr("foobar")`

* 有#前缀的字符串会被当做一个非字符串的表达式。

    * `#PathItem::Executable` -> `PathItem::Executable`

* 有%前缀的字符串会被当做一个原始字符串。这是用于那些以特殊字符开头的字符串。

    * `%#label` -> `"#label"`

* 有!前缀的字符串会被当做一个需要翻译的原始字符串。它将被解析为一个形如`[原始字符串, 被翻译字符串]`的字符串列表。

    * `!tips` -> `QStringList{"tips", tr("tips")}`

* 如果一个列表内全是字符串，则会变成一个`QStringList`，否则变成`QVaraintList`。

**`SettingWidget.hpp` `SettingWidget.cpp`**

这里包含不同类型、控件的包装代码。

* `SettingBase`

最基本的封装。

* `SettingTemplate<Type>`

确定类型的封装。

* `WrapperTemplate<Type, Widget>`

确定类型和根控件类型的封装。

* `XXXWrapper`

XXX控件的子类。

**`genSettings.py`**

这里包含了在yaml中未设置默认值时的默认值。

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

专为PreferencesTemplate设计使用。

**`SettingsHelper.hpp`**

包含所有选项的简易设置/获取函数。

使用时请包含```<generated/SettingsHelper.hpp>```

* 设置: `SettingsHelper::setXXX`

* 获取: 根据选项的类型为`SettingsHelper::getXXX`或`SettingsHelper::isXXX`

* 对于对象类型

    * 获取键: `SettingsHelper::queryXXX`

    * 设置键: `SettingsHelper::setXXX`

    * 设置KKK键的YYY设置: `SettingsHelper::getXXX(KKK).setYYY`

    * 如果键值被限定，则你可以这样获取: `SettingsHelper::getXXX_YYY`

**`PreferencesWindow.cpp`**

生成设置对话框的代码。

### 目前已实现支持设置界面的类型信息

第一个为默认的ui。

* `bool`

    默认值`false`

    * `QCheckBox`

* `QString`

    默认值`""`

    * `QLineEdit`

    * `QPlainTextEdit`

    * `QComboBox`

        * 参数(`QStringList`): 预定义选项。
        
        ```yaml
        param:
        - item1
        - item2
        ```

    * `PathItem`

        选择文件的控件。

        * 参数(`int`): 标题和过滤器在代码中的索引。

    * `ShortcutItem`

        设置快捷键的控件。
    
* `int`

    默认值`0`

    参数(`QVariantList`): 数值范围。

    ```yaml
    param:
    - 1
    - 100
    - 2 # 可选，步长
    ```

    * `QSpinBox`

    * `QScrollBar`

        始终为横向。
    
    * `QSlider`

        始终为横向。
        
    * `TristateCheckBox`
    
        三态复选框。

* `QFont`

    默认值尚无

    * `FontItem`

        设置字体的控件。

* `QVariantList`

    * `StringListsItem`

        多个 `QStringList`。

        令一个 `QStringList` 中的 `QString` 数量为 `<col>`。

        value: `QVariantList`, 其中每个元素是一个包含 `<col>` 个 `QString` 的 `QStringList`。

        param: 这个参数描述了 UI 中的标题栏以及 `<col>` 的值。它是一个 有 `<col>` 个元素的 `QVariantList`，其中每个元素是一个包含两个 `QString` 的 `QStringList`，第一个 `QString` 是标题，第二个 `QString` 是 tooltip。

* `Object`

    对象是直接处理的。

    * `default`: 设置预设的键。

    * `param.rename`: 允许用户重命名。默认为禁用。

    * `param.action`: 在 更多 菜单中显示更多动作。请参考`Snippet`的示例。

        * `name`: 动作的名称。应当总是一个翻译后的字符串。

        * `method`: 定义在`methods`中的函数的名称。
            
            该函数将会传入两个参数。

            ```yaml
            widget: MapWidget*
            valid: bool        # 如果真，返回这个动作是否启用，否则执行动作。
            ```

            当数据发生变化或当前选择改变时，将会以`valid=true`调用该函数。

    * `param.restrict`: 令对象只能拥有这些键值。

    * `param.pass`: 将会作为参数传递给`SettingsWrapper`。请参考`Language Config`的示例。

        * `param.pass.group`: 使用`QTabWidget`来组织设置项。

            * `name`: 一页的标签。应当总是一个翻译后的字符串。

            * `target`: 这一页中的设置项。

        * `param.pass.hide`: 对特定键值隐藏选项。

        ```yaml
        hide:
            opt:
            - key1
        ```

        这意味着当选择`key1`时，不显示选项`opt`。
### 添加设置

具体的实现细节请参考已有代码。

#### 添加新类型/控件

1. 在`SettingWidget`中添加`XXXWrapper`，XXX建议为控件名。

2. 修改`SettingWidget`中的`createWrapper`。

3. \[可选\]在`genSettings.py`中为类型设定一个默认值。

#### 添加新设置

1. 在`setting.yaml`中添加新项，按需设置。

#### 添加新的设置页面/选项

* 修改`pageInfo`。

#### 在代码中使用

* 使用SettingsHelper中的设置/获取函数。