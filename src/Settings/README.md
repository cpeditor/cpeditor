# Settings

This directory contains the header/sources related to the Settings window.
Any code in this folder does not need to be under any namespace, 
It is because this directory is self sufficient and does not interacts with 
any outside code.

### Useful Information

**`settings.json`**

It includes description of all settings, including those not in the Settings Interface.

Each item in the json array represents a configuration item, the structure is as follows:

```javascript
{
        "name": "",   // Required, internal key/name of the option
        "desc": "",   // Optional, prompt text of the option in the setting interface. If not set, it'll be the internal name after replacing splash to space.
        "type": "",   // Required, data type of the option. Should be the type in the code.
        "default": 0, // Optional, default value of the option.
        "old": [],    // Optional, previous key/name of the option for backward compatibility. It should be a list of all old keys.
        "ui": "",     // Optional, type of QtWidget to use in the settings interface.
        "param": ""   // Optional, parameters passed for control creation in the setting interface.
}
```

**`ValueWrapper.hpp` `ValueWrapper.cpp`**

It includes codes for value and widget wrapping.

* `ValueWidget`

The most basic wrapping. Currently it is not used. Use `Wrapper<Type>` instead.

* `Wrapper<Type>`

    Wrapper for type `Type`. It is specialized for type `bool`.

    It contains three pure virtual functon.

    * `init`

        It's called when the Settings window is creating widgets for initialization.
    
    * `get` `set`

        It's called when the Setting window is ready to get or set the data.
    
* `XXXWrapper`

    The subclass of Wrapper for the widget XXX.

* `createXXXWrapper`

    Function to create the chosen XXXWrapper according to the type (specified in `ui` in `Setting.json`).

**`genSettings.py`**

Script to generate `SettingsInfo.hpp` and `SettingsHelper.hpp`.

It includes the default value used when you don't specify it in `Settings.json`.

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

It is designed for `PreferencesPageTemplate`.

**`SettingsHelper.hpp`**

It includes the easy functions to set and get options.

Include ```<generated/SettingsHelper.hpp>``` to use them.

* set: `SettingsHelper::setXXX`

* get: `SettingsHelper::getXXX` or `SettingsHelper::isXXX` according to type.

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
        
        * e.g. ```"param": "QStringList {\"item1\", \"item2\"}```

    * `PathItem`

        Widget for file choosing.

        * param(`int`): The index of title and filter strings in code.

    * `ShortcutItem`

        Widget for shortcut choosing.

* `int`

    Default value: `0`

    param(`QVariantList`): The range.

    e.g. ```"param": "QVariantList {1, 100}"```

    * `QSpinBox`

    * `QScrollBar`

        Horizontal only.

    * `QSlider`

        Horizontal only.

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

### Adding Settings

Refer to the existing code for more details.

#### Adding a new type or a new widget

1. Add `XXXWrapper` in `ValueWrapper`       (XXX represent the widget name)
2. Add `createXXXWrapper` in `ValueWrapper` (XXX represent the type name)
3. Add checking code for this type in `PreferebcesPageTemplate`
4. Optionally, add fallback default valuein `genSettings.py`

#### Adding a new setting

1. Add a new option in settings.json with above mentioned syntax.

#### Adding a new page or a new option

* Add `addPage(path, options)` in the constructor of preference window. `path` is Path on Left of Preference window, and `options` is set of internal names/keys.
* Add a new Settings name to the existing `addPage()` options

#### Using a new setting

Use the static members from `SettingsHelper` namespace. This is auto-generated from `settings.json`.
