# Translation Guide for CP Editor

This document explains how to work with translations in CP Editor.

## Overview

CP Editor uses Qt6's translation system to provide internationalization support. The project currently supports the following languages:

- **Greek (Νέα Ελληνικά)** - `el_GR`
- **Spanish (Español)** - `es_MX`
- **Portuguese Brazilian (Português brasileiro)** - `pt_BR`
- **Russian (Русский)** - `ru_RU`
- **Chinese Simplified (简体中文)** - `zh_CN`
- **Chinese Traditional (正體中文)** - `zh_TW`

## File Structure

```
cpeditor/
├── translations/
│   ├── el_GR.ts    # Greek translation source
│   ├── es_MX.ts    # Spanish translation source
│   ├── pt_BR.ts    # Portuguese Brazilian translation source
│   ├── ru_RU.ts    # Russian translation source
│   ├── zh_CN.ts    # Chinese Simplified translation source
│   └── zh_TW.ts    # Chinese Traditional translation source
├── tools/
│   ├── updateTranslation.sh        # Full translation update script (Unix)
│   ├── updateTranslation.bat       # Full translation update script (Windows)
│   └── updateTranslation_simple.sh # Simple update script (works without full build)
└── CMakeLists.txt          # Build configuration with Qt6 translations
```

## How Translations Work

1. **Source Files (.ts)**: These are XML files containing the original text and translations
2. **Compiled Files (.qm)**: These are binary files generated from .ts files used at runtime
3. **Resource System**: .qm files are embedded into the application as Qt resources

## Building with Translations

### Automatic Build

When you build CP Editor normally, translations are automatically processed:

```bash
mkdir build && cd build
cmake ..
make  # or ninja
```

This will:
1. Generate .qm files from .ts files
2. Create a resource file containing all translations
3. Embed translations into the final executable

### Using the Translation Scripts

We provide translation update scripts in the `tools/` directory:

#### Full Update Scripts (require complete build)
```bash
# These scripts scan all sources including generated files
tools/updateTranslation.sh        # No location info
tools/updateTranslation.sh r      # With relative paths
tools/updateTranslation.sh a      # With absolute paths

# On Windows:
tools\updateTranslation.bat
tools\updateTranslation.bat r
tools\updateTranslation.bat a
```

**Features:**
- Scans source code, UI files, and generated files
- Includes third-party dialog translations
- Most comprehensive translation extraction
- **Requires:** Complete build environment with all dependencies

#### Simple Update Script (works without full build)
```bash
# This script works even if you can't build the full project
tools/updateTranslation_simple.sh     # No location info
tools/updateTranslation_simple.sh r   # With relative paths (recommended)
tools/updateTranslation_simple.sh a   # With absolute paths
```

**Features:**
- Scans source code and UI files only
- Works without build dependencies
- Automatically detects and includes available generated files
- Good for translators who don't need full development environment
- **Requires:** Only Qt Linguist tools (lupdate)

#### Which Script to Use?

- **Developers with full build environment:** Use `updateTranslation.sh`
- **Translators or contributors without dependencies:** Use `updateTranslation_simple.sh`
- **CI/CD or automated builds:** Use `updateTranslation.sh`
- **Quick translation updates:** Use `updateTranslation_simple.sh r`

#### Building .qm files and full project
```bash
mkdir build && cd build
cmake .. && make
```

## For Translators

### Getting Started

1. Clone the repository
2. Find your language file in `translations/[locale].ts`
3. Use Qt Linguist to edit the translation file:
   ```bash
   linguist translations/zh_CN.ts
   ```

### Using Qt Linguist

Qt Linguist is the recommended tool for editing translations:

1. **Install Qt Linguist**: Usually comes with Qt development tools
2. **Open the .ts file**: `File → Open → translations/[locale].ts`
3. **Translate strings**: Select untranslated items and provide translations
4. **Mark as finished**: Use `Ctrl+Shift+Enter` to mark translations as finished
5. **Save**: `Ctrl+S` to save your work

### Translation Guidelines

1. **Context matters**: Pay attention to the context provided for each string
2. **Preserve formatting**: Keep HTML tags, placeholders like `%1`, `%2` intact
3. **Keyboard shortcuts**: Maintain `&` characters for keyboard shortcuts when possible
4. **Testing**: Test your translations by building and running the application

### Adding a New Language

To add support for a new language:

1. **Create the .ts file**:
   ```bash
   cd translations
   cp en.ts [new_locale].ts
   ```

2. **Update CMakeLists.txt**:
   - Add the new locale to `I18N_TRANSLATED_LANGUAGES`
   - Add the new .ts file to the `TS_FILES` list

3. **Update Core/Translator.cpp**:
   - Add the language name to the `locales` map
   - Add any suffix to the `suffixes` map
   - Add language code to the `code` map

4. **Test the integration**:
   ```bash
   ./update_translations.sh all
   ```

## For Developers

### Adding Translatable Strings

Use Qt's translation macros in your code:

```cpp
// In class methods
tr("Hello World")

// In static contexts
QObject::tr("Static text")

// With context
QCoreApplication::translate("ClassName", "Text with context")

// With placeholders
tr("Processing file %1 of %2").arg(current).arg(total)
```

### Updating Translations

When you add new translatable strings:

1. **Extract new strings**:
   ```bash
   # Use the full script if you have a complete build environment
   tools/updateTranslation.sh r

   # Or use the simple script if build dependencies are missing
   tools/updateTranslation_simple.sh r
   ```

2. **Notify translators**: Let translators know about new strings to translate

3. **Build and test**:
   ```bash
   mkdir build && cd build
   cmake .. && make
   ```

### CMake Configuration

The translation system is configured in `CMakeLists.txt`:

```cmake
# Set up Qt6 translations
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES el_GR es_MX pt_BR ru_RU zh_CN zh_TW)

# Define translation files
set(TS_FILES
    translations/el_GR.ts
    translations/es_MX.ts
    translations/pt_BR.ts
    translations/ru_RU.ts
    translations/zh_CN.ts
    translations/zh_TW.ts
)

# Create lupdate target (for updating .ts files)
qt6_add_lupdate(cpeditor
    TS_FILES ${TS_FILES}
    SOURCES src ui
    NO_GLOBAL_TARGET
)

# Create lrelease target (for building .qm files)
qt6_add_translations(cpeditor
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/translations"
)
```

## How Translation Loading Works

CP Editor loads translations at startup in `src/Core/Translator.cpp`:

1. **Determine locale**: From user settings or system locale
2. **Load .qm file**: From embedded resources at `:/translations/cpeditor_[locale].qm`
3. **Install translator**: Make it active for the application

The resource path pattern is: `:/translations/[locale].qm`

## Troubleshooting

### Common Issues

1. **Translations not showing up**:
   - Check if .qm files are being generated in the build directory
   - Verify the resource file was created properly
   - Ensure the locale code matches between .ts filename and Translator.cpp

2. **Build errors with translations**:
   - Make sure Qt6 LinguistTools is installed
   - Check that all .ts files exist and are valid XML

3. **lupdate not extracting strings**:
   - Verify source paths in CMakeLists.txt
   - Make sure you're using Qt translation macros correctly

## License

Translation files are subject to the same license as CP Editor (GPL v3).
