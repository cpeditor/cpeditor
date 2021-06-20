# Settings

This directory contains the header/sources related to the Settings window.
Any code in this folder does not need to be under any namespace, 
It is because this directory is self sufficient and does not interacts with 
any outside code.

### Useful Information

**`settings.yaml`**

There're three parts.

#### presetMethod

Declare useful and universal methods.

* used for `getDefault`
    
    * `pickDefault`: pick the default value from `param.default.key`. If key is not in param.default, return the default value declared outside.
    
    * `genByRestrict`: get `param.restrict` as default value.

* used for `getParam`

    * `pickParam`: pick the param from `param.key`.

    * `takeParam`: pick the param from `param.param`.

    * `pickParamFromParam`: pick the param from `param.param.key`.

* used for `getTip`

    * `pickTip`: pick tip from `param.tip`. It should contain translated version. e.g. `tip: "!the tip"`.

#### pageInfo

Declare how to create the setting dialog.

If a group of setting is only supported on certain os, use `<xxx>` as prefix. xxx is the combine of `w`(windows), `u`(unix, linux), `m`(macos). Case insensitive.

```yaml
<wu>Options only for windows and unix:
- aaa
- bbb
```

Currently os restricting for a single option is not supported.

#### data

It includes description of all settings, including those not in the Settings Interface.

Each item in the array represents a configuration item, the structure is as follows:

```yaml
    name:
    # Required, internal key/name of the option

    type:
    # Required, data type of the option. Should be the type in the code.

    desc:
    # Optional, prompt text of the option in the setting interface. It will be translated. If not set, it'll be the internal name after replacing splash to space.

    default:
    # Optional, default value of the option.

    old:
    # Optional, previous key/name of the option for backward compatibility. It should be a list of all old keys.
    -   ~old key

    ui:
    # Optional, type of QtWidget to use in the settings interface.

    tip:
    # Optional, tip showed when hovering widget. It will be translated.

    param:
    # Optional, parameters passed for control creation in the setting interface.

    methods:
    # Optional, methods used for different reasons.

        ~method name:
        # onApply: invoked when user click apply button after changing this setting. Good to show warning messages or related information.
        # getDefault, getParam, getTip: invoked when need a default value/param/tip. Good to dynamic calculating those values.
            param:
                ~param name: ~type
            code: ~source code

        ~another method: ~preset method name
        # use methods in presetMethod

    depends:
    # Optional. If all dependency satisfied, this setting is enabled, otherwise it is disabled.
    -   name:
        # Name of key, this setting depends on. (Should be in same page. Upper of object in the page could be detected too.)
        checks:
        # a function that return true or false by checking the var(QVariant).

    immediateApply:
    # Optional， a boolean when enabled, this setting is applied as soon as changed by user. It does not require pressing Apply button

    sub:
    # children when type is Object
    - name:
}
```

about methods

If the code is too long, we suggest you to write them in `SettingsMethods.cpp` and call it.

* `onApply`

    * `parent`: the rootWidget

* `getParam`

    * `param`: raw param
    
    * `pre`: list of keys and names/descs
    
* `getDefault`

    * `param`: param from `getParam`
    
    * `rawparam`: raw param
    
    * `def`: raw default value

    * `pre`: list of keys and names/descs
    
* `getTip`

    * `param`: param from `getParam`
    
    * `rawparam`: raw param
    
    * `needtr`: whether to translate

    * `tip`: raw tip
    
    * `pre`: list of keys and names/descs

desc and tip will be formatted. Use @x (x is a digit) to refer the key or setting name. Use @@ to represent @. The string being replaced will use translated version automatically.

    * @0: The name or desc of current setting

    * @1: The key of the parent object

    * @2: The name or desc of parent object

    * ...

param and default can use any structure that allowed in yaml. `genSettings.py` will handle it.

To prevent misparsing of string, we use following rules:

* String with no prefix is considered as a raw string.

    * `hello` -> `"hello"`

* String starts with @ is considered as an expression of string.

    * `@tr(\"foobar\")` -> `tr("foobar")`

* String starts with # is considered as an expression of value(not string).

    * `#PathItem::Executable` -> `PathItem::Executable`

* String starts with % is considered as a raw string. This is used for raw string that have special char at the beginning.

    * `%#label` -> `"#label"`

* String starts with ! is considered as a raw string that need to be translated. It becomes a string list `[ raw str, translated str ]`.

    * `!tips` -> `QStringList{"tips", tr("tips")}`

* If a list is full of string, it becomes `QStringList`, otherwise `QVariantList`.

**`SettingWidget.hpp` `SettingWidget.cpp`**

It includes codes for value and widget wrapping.

* `SettingBase`

The most basic wrapping.

* `SettingTemplate<Type>`

The wrapping with type specified.

* `WrapperTemplate<Type, Widget>`

The wrapping with type and rootWidget specified.

* `XXXWrapper`

The subclass of Wrapper for the widget XXX.

**`genSettings.py`**

Script to generate `SettingsInfo.hpp`, `SettingsHelper.hpp` and `PreferencesWindow.cpp`.

It includes the default value used when you don't specify it in `Settings.yaml`.

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

It is designed for `PreferencesTemplate`.

**`SettingsHelper.hpp`**

It includes the easy functions to set and get options.

Include ```<generated/SettingsHelper.hpp>``` to use them.

* set: `SettingsHelper::setXXX`

* get: `SettingsHelper::getXXX` or `SettingsHelper::isXXX` according to type.

* for object

    * get keys: `SettingsHelper::queryXXX`

    * set keys: `SettingsHelper::setXXX`

    * set option YYY under key KKK: `SettingsHelper::getXXX(KKK).setYYY`

    * if it has restrict keys, then you can get by: `SettingsHelper::getXXX_YYY`

**`PreferencesWindow.cpp`**

Codes to generate setting dialog.

### Current supported types

The first ui in the list is the default ui.

* `bool`

    Default value: `false`

    * `QCheckBox`

* `QString`

    Default value: `""`

    * `QLineEdit`

    * `QPlainTextEdit`

    * `QComboBox`

        * param(`QStringList`): The pre-defined choices.

        ```yaml
        param:
        - item1
        - item2
        ```

    * `PathItem`

        Widget for file choosing.

        * param(`int`): The index of title and filter strings in code.

    * `ShortcutItem`

        Widget for shortcut choosing.

* `int`

    Default value: `0`

    * param(`QVariantList`): The range.

    ```yaml
    param:
    - 1
    - 100
    - 2 # optional, steps
    ```
    
    * `QSpinBox`
    
    * `QScrollBar`
    
        Horizontal only.
    
    * `QSlider`
    
        Horizontal only.
    
    * `TristateCheckBox`
    
        Checkbox with three states.
        
* `QFont`

    Default value: not defined yet

    * `FontItem`

        Widget for font choosing.

* `QVariantList`

    * `StringListsItem`

        Widget for several `QStringList`s.

        Suppose the number of of `QString`s in a single `QStringList` is `<col>`.

        value: `QVariantList`, each the element of `value` is a `QStringList` with `<col>` `QString`s.

        param: The param describes the header and `<col>`. It's a `QVariantList` with `<col>` elements, and each of the elements is a `QStringList` with two `QString`s, the first one is the title and the second one is the tooltip.

* `Object`

    Object are handled directly.

    * `default`: set preset keys.

    * `param.rename`: to allow user rename. default is disabled.

    * `param.action`: show more actions in `More` menu. Please refer `Snippet` for examples.

        * `name`: Name of the action. Should always be a translated string.

        * `method`: Name of method defined in `methods`.
            
            This method will be passed in two arguments.

            ```yaml
            widget: MapWidget*
            valid: bool        # if true, return whether this action is enabled. otherwise do the action.
            ```

            method with `valid=true` will be called when data changed or current selecting changed.

    * `param.restrict`: it means the object can only contains those keys.

    * `param.pass`: it will be passed to `SettingsWrapper` as param. Please refer `Language Config` for examples.

        * `param.pass.group`: use `QTabWidget` to group the settings.

            * `name`: Title for the tab. Should always be a translated string.

            * `target`: Options for this tab.

        * `param.pass.hide`: hide options for certain keys.

        ```yaml
        hide:
            opt:
            - key1
        ```

        It means when choosing `key1`, don't show the option `opt`.

### Adding Settings

Refer to the existing code for more details.

#### Adding a new type or a new widget

1. Add `XXXWrapper` in `SettingWidget`       (XXX represent the widget name).
2. Modify `createWrapper` in `SettingWidget`.
3. Optionally, add fallback default value in `genSettings.py`.

#### Adding a new setting

1. Add a new option in settings.yaml with above mentioned syntax.

#### Adding a new page or a new option

* Modify `pageInfo`.

#### Using a new setting

Use the static members from `SettingsHelper` namespace. This is auto-generated from `settings.yaml`.
