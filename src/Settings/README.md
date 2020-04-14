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
        "desc": "",   // Optional, prompt text of the option in the setting interface. If not set, the internal name is replaced with spaces
        "type": "",   // Required, data type of the option. Should be the type in the code.
        "default": 0, // Optional, default value of the option.
        "old": [],    // Optional, previous key/name of the option for backward compatibility. It should be a list of all old keys.
        "ui": "",     // Optional, type of QtWidget to use in the settings interface.
        "param": ""   // Optional, parameters passed for control creation in the setting interface.
 }
```
> This section is incomplete and more information will be added.

### Adding a new Settings

1. Add a new option in settings.json with above mentioned syntax.
2. Add `addPage(path, options)` in the constructor of preference window. `path` is Path on Left of Preference window, and `options` is set of internal names/keys.
3. Add a new Settings name to the existing `addPage()` options

To get settings values in Code use the static members from `SettingsHelper` class. This is auto-generated from `settings.json`.
