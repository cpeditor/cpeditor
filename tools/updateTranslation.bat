@echo off

if "%cd:~-5%" == "tools" cd ..

if "%1" == "r" (
	set LOCATIONS=relative
) else if "%1" == "a" (
	set LOCATIONS=absolute
) else (
	set LOCATIONS=none
)

set SOURCES=src build/generated build/cpeditor_autogen/ui build/third_party/QtFindReplaceDialog/dialogs/QtFindReplaceDialog_autogen

(for %%i in (el_GR es_MX pt_BR ru_RU zh_CN zh_TW) do (
	lupdate -no-obsolete %SOURCES% -locations %LOCATIONS% -tr-function-alias tr+=TRKEY -ts translations/%%i.ts
))

pause
