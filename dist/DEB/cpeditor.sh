#!/bin/sh
export LD_LIBRARY_PATH=/usr/lib/cpeditor:$LD_LIBRARY_PATH
export QT_PLUGIN_PATH=/usr/share/cpeditor/plugins/:$QT_PLUGIN_PATH
export QT_QPA_PLATFORM_PLUGIN_PATH=/usr/share/cpeditor/plugins/platforms:$QT_QPA_PLATFORM_PLUGIN_PATH

"/usr/share/cpeditor/bin/cpeditor" "$@"
