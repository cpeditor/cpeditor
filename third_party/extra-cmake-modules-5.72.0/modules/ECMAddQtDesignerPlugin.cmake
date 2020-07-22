#.rst:
# ECMAddQtDesignerPlugin
# ----------------------
#
# This module provides the ``ecm_add_qtdesignerplugin`` function for generating
# Qt Designer plugins for custom widgets. Each of those widgets is described
# using a second function ``ecm_qtdesignerplugin_widget``.
#
# ::
#
#   ecm_add_qtdesignerplugin(<target_name>
#       NAME <name>
#       WIDGETS <widgetid> [<widgetid2> [...]]
#       LINK_LIBRARIES <lib> [<lib2> [...]]
#       INSTALL_DESTINATION <install_path>
#       [OUTPUT_NAME <output_name>]
#       [DEFAULT_GROUP <group>]
#       [DEFAULT_HEADER_CASE <SAME_CASE|LOWER_CASE|UPPER_CASE>]
#       [DEFAULT_HEADER_EXTENSION <header_extension>]
#       [DEFAULT_ICON_DIR <icon_dir>]
#       [INCLUDE_FILES <include_file> [<include_file2> [...]]]
#       [SOURCES <src> [<src2> [...]]]
#       [COMPONENT <component>]
#   )
#
# ``NAME`` specifies the base name to use in the generated sources.
# The default is <target_name>.
#
# ``WIDGETS`` specifies the widgets the plugin should support. Each widget has
# to be defined before by a call of ``ecm_qtdesignerplugin_widget`` with the
# respective <widgetid>, in a scope including the current call.
#
# ``LINK_LIBRARIES`` specifies the libraries to link against. This will be at
# least the library providing the widget class(es).
#
# ``INSTALL_DESTINATION`` specifies where the generated plugin binary will be
# installed.
#
# ``OUTPUT_NAME`` specifies the name of the plugin binary. The default is
# "<target_name>".
#
# ``DEFAULT_GROUP`` specifies the default group in Qt Designer where the
# widgets will be placed. The default is "Custom".
#
# ``DEFAULT_HEADER_CASE`` specifies how the name of the header is derived from
# the widget class name.  The default is "LOWER_CASE".
#
# ``DEFAULT_HEADER_EXTENSION`` specifies what file name extension is used for
# the header file derived from the class name.  The default is "h".
#
# ``DEFAULT_ICON_DIR`` specifies what file name extension is used for
# the header file derived from the class name.  The default is "pics".
#
# ``INCLUDE_FILES`` specifies additional include files to include with the
# generated source file. This can be needed for custom code used in
# initializing or creating widgets.
#
# ``SOURCES`` specifies additional source files to build the plugin from.
# This can be needed to support custom code used in initializing or
# creating widgets.
#
# ``COMPONENT`` specifies the installation component name with which the install
# rules for the generated plugin are associated.
#
# ::
#
#   ecm_qtdesignerplugin_widget(<widgetid>
#       [CLASS_NAME <class_name>]
#       [INCLUDE_FILE <include_file>]
#       [CONTAINER]
#       [ICON <iconfile>]
#       [TOOLTIP <tooltip>]
#       [WHATSTHIS <whatsthis>]
#       [GROUP <group>]
#       [CREATE_WIDGET_CODE_FROM_VARIABLE <create_widget_code_variable>]
#       [INITIALIZE_CODE_FROM_VARIABLE <initialize_code_variable]
#       [DOM_XML_FROM_VARIABLE <dom_xml_variable>]
#       [IMPL_CLASS_NAME <impl_class_name>]
#       [CONSTRUCTOR_ARGS_CODE <constructor_args_code>]
#       [CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE <constructor_args_code_variable>]
#   )
#
# ``CLASS_NAME`` specifies the name of the widget class, including namespaces.
# The default is "<widgetid>".
#
# ``INCLUDE_FILE`` specifies the include file to use for the class of this
# widget. The default is derived from <class_name> as configured by the
# ``DEFAULT_HEADER_*`` options of ``ecm_add_qtdesignerplugin``, also replacing
# any namespace separators with "/".
#
# ``CONTAINER`` specifies, if set, that this widget is a container
# for other widgets.
#
# ``ICON`` specifies the icon file to use as symbol for this widget.
# The default is "{lowercased <class_name>}.png" in the default icons dir as
# configured by the ``DEFAULT_ICON_DIR`` option of
# ``ecm_add_qtdesignerplugin``, if such a file exists.
#
# ``TOOLTIP`` specifies the tooltip text to use for this widget. Default is
# "<class_name> Widget".
#
# ``WHATSTHIS`` specifies the What's-This text to use for this widget.
# Defaults to the tooltip.
#
# ``GROUP`` specifies the group in Qt Designer where the widget will be placed.
# The default is set as configured by the ``DEFAULT_GROUP`` option of
# ``ecm_add_qtdesignerplugin``.
#
# ``CREATE_WIDGET_CODE_FROM_VARIABLE`` specifies the variable to get from the
# C++ code to use as factory code to create an instance of the widget,
# for the override of
# ``QDesignerCustomWidgetInterface::createWidget(QWidget* parent)``.
# The default is "return new <impl_class_name><constructor_args_code>;".
#
# ``INITIALIZE_CODE_FROM_VARIABLE`` specifies the variable to get from the C++
# code to use with the override of
# ``QDesignerCustomWidgetInterface::initialize(QDesignerFormEditorInterface* core)``.
# The code has to use the present class member ``m_initialized`` to track and
# update the state. The default code simply sets ``m_initialized`` to
# ``true``, if it was not before.
#
# ``DOM_XML_FROM_VARIABLE`` specifies the variable to get from the string to
# use with the optional override of
# ``QDesignerCustomWidgetInterface::domXml()``.
# Default does not override.
#
# ``IMPL_CLASS_NAME`` specifies the name of the widget class to use for the
# widget instance with Qt Designer. The default is "<class_name>".
#
# ``CONSTRUCTOR_ARGS_CODE`` specifies the C++ code to use for the constructor
# arguments with the default of ``CREATE_WIDGET_CODE_FROM_VARIABLE``. Note
# that the parentheses are required. The default is "(parent)".
#
# ``CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE`` specifies the variable to get from
# the C++ code instead of passing it directly via ``CONSTRUCTOR_ARGS_CODE``.
# This can be needed if the code is more complex and e.g. includes ";" chars.
#
#
#
# Example usage:
#
# .. code-block:: cmake
#
#   ecm_qtdesignerplugin_widget(FooWidget
#       TOOLTIP "Enables to browse foo."
#       GROUP "Views (Foo)"
#   )
#
#   set(BarWidget_CREATE_WIDGET_CODE
#   "
#       auto* widget = new BarWidget(parent);
#       widget->setBar("Example bar");
#       return widget;
#   ")
#
#   ecm_qtdesignerplugin_widget(BarWidget
#       TOOLTIP "Displays bars."
#       GROUP "Display (Foo)"
#       CREATE_WIDGET_CODE_FROM_VARIABLE BarWidget_CREATE_WIDGET_CODE
#   )
#
#   ecm_add_qtdesignerplugin(foowidgets
#       NAME FooWidgets
#       OUTPUT_NAME foo2widgets
#       WIDGETS
#           FooWidget
#           BarWidget
#       LINK_LIBRARIES
#           Foo::Widgets
#       INSTALL_DESTINATION "${KDE_INSTALL_QTPLUGINDIR}/designer"
#       COMPONENT Devel
#   )
#
# Since 5.62.0.

#=============================================================================
# SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

include(CMakeParseArguments)

# helper method
# escapes string for C++ code
function(_ecm_qtdesignerplugin_escape_cpp_string _varName input)
    string(REPLACE "\"" "\\\"" _string ${input})
    set(${_varName} "${_string}" PARENT_SCOPE)
endfunction()

# To make the data about the widgets available to the function ecm_add_qtdesignerplugin,
# variables are created in the scope of the caller of this method, protected by
# a namespace for this macro file, and otherwise from the widget id and the property id:
# ECM_QTDESIGNERPLUGIN_${widget}_${property}
function(ecm_qtdesignerplugin_widget widget)
    set(options
        CONTAINER
    )
    set(oneValueArgs
        CLASS_NAME
        INCLUDE_FILE
        ICON
        TOOLTIP
        WHATSTHIS
        GROUP
        CREATE_WIDGET_CODE_FROM_VARIABLE
        INITIALIZE_CODE_FROM_VARIABLE
        DOM_XML_FROM_VARIABLE
        IMPL_CLASS_NAME
        CONSTRUCTOR_ARGS_CODE
        CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE
    )
    set(multiValueArgs
    )
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARGS_CLASS_NAME)
        set(ARGS_CLASS_NAME "${widget}")
    endif()
    if(NOT ARGS_TOOLTIP)
        set(ARGS_TOOLTIP "${ARGS_CLASS_NAME} Widget")
    endif()
    if(NOT ARGS_WHATSTHIS)
        set(ARGS_WHATSTHIS "${ARGS_TOOLTIP}")
    endif()
    if(ARGS_CONTAINER)
        set(_is_container TRUE)
    else()
        set(_is_container FALSE)
    endif()
    if(ARGS_CONSTRUCTOR_ARGS_CODE AND ARGS_CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE)
        message(FATAL_ERROR "Either CONSTRUCTOR_ARGS_CODE or CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE can be passed when calling ecm_qtdesignerplugin_widget().")
    endif()
    if(NOT ARGS_CREATE_WIDGET_CODE_FROM_VARIABLE)
        if(NOT ARGS_IMPL_CLASS_NAME)
            set(ARGS_IMPL_CLASS_NAME "${ARGS_CLASS_NAME}")
        endif()
        if(ARGS_CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE)
            set(_constructor_args "${${ARGS_CONSTRUCTOR_ARGS_CODE_FROM_VARIABLE}}")
        elseif(ARGS_CONSTRUCTOR_ARGS_CODE)
            set(_constructor_args "${ARGS_CONSTRUCTOR_ARGS_CODE}")
        else()
            set(_constructor_args "(parent)")
        endif()
        set(_create_widget_code "        return new ${ARGS_IMPL_CLASS_NAME}${_constructor_args};")
    else()
        set(_create_widget_code "${${ARGS_CREATE_WIDGET_CODE_FROM_VARIABLE}}")
    endif()
    if(ARGS_ICON)
        if (NOT IS_ABSOLUTE ${ARGS_ICON})
            set(ARGS_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_ICON}")
        endif()
        if(NOT EXISTS "${ARGS_ICON}")
            message(FATAL_ERROR "No such file as passed with ICON when calling ecm_qtdesignerplugin_widget(): ${ARGS_ICON}")
        endif()
    endif()

    # store data about widget, so ecm_add_qtdesignerplugin can access it
    set(ECM_QTDESIGNERPLUGIN_${widget}_CLASS_NAME "${ARGS_CLASS_NAME}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_INCLUDE_FILE "${ARGS_INCLUDE_FILE}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_TOOLTIP "${ARGS_TOOLTIP}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_WHATSTHIS "${ARGS_WHATSTHIS}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_GROUP "${ARGS_GROUP}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_ICON "${ARGS_ICON}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_IS_CONTAINER "${_is_container}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_CREATE_WIDGET_CODE "${_create_widget_code}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_INITIALIZE_CODE "${${INITIALIZE_CODE_FROM_VARIABLE}}" PARENT_SCOPE)
    set(ECM_QTDESIGNERPLUGIN_${widget}_DOM_XML "${${ARGS_DOM_XML_FROM_VARIABLE}}" PARENT_SCOPE)
endfunction()

# helper method
function(_ecm_qtdesignerplugin_write_widget designer_src_file widget default_group rc_icon_dir)
    # prepare data
    set(_classname "${ECM_QTDESIGNERPLUGIN_${widget}_CLASS_NAME}")
    set(_factory_classname "${_classname}QtDesignerWidgetFactory")
    string(REPLACE "::" "__" _factory_classname "${_factory_classname}")
    set(ECM_QTDESIGNERPLUGIN_${widget}_FACTORY_CLASS_NAME "${_factory_classname}" PARENT_SCOPE)
    if(ECM_QTDESIGNERPLUGIN_${widget}_IS_CONTAINER)
        set(_is_container "true")
    else()
        set(_is_container "false")
    endif()
    _ecm_qtdesignerplugin_escape_cpp_string(_tooltip "${ECM_QTDESIGNERPLUGIN_${widget}_TOOLTIP}")
    _ecm_qtdesignerplugin_escape_cpp_string(_whatsthis "${ECM_QTDESIGNERPLUGIN_${widget}_WHATSTHIS}")
    set(_group ${ECM_QTDESIGNERPLUGIN_${widget}_GROUP})
    if(NOT _group)
        set(_group "${default_group}")
    endif()
    _ecm_qtdesignerplugin_escape_cpp_string(_group "${_group}")
    set(_dom_xml "${ECM_QTDESIGNERPLUGIN_${widget}_DOM_XML}")
    if(_dom_xml)
        string(REPLACE "\"" "\\\"" _dom_xml "${_dom_xml}")
        set(_dom_xml_method "    QString domXml() const override { return QStringLiteral(\"${_dom_xml}\"); }")
    else()
        set(_dom_xml_method)
    endif()
    set(_icon "${ECM_QTDESIGNERPLUGIN_${widget}_ICON}")
    if(_icon)
        get_filename_component(_icon_filename ${_icon} NAME)
        set(_icon_construct "QIcon(QStringLiteral(\":${rc_icon_dir}/${_icon_filename}\"))")
    else()
        set(_icon_construct "QIcon()")
    endif()
    set(_initialize_code "${ECM_QTDESIGNERPLUGIN_${widget}_INITIALIZE_CODE}")
    if(NOT _initialize_code)
        set(_initialize_code
"        Q_UNUSED(core);

        if (m_initialized) return;

        m_initialized = true;"
        )
    endif()

    # write code
    file(APPEND ${designer_src_file} "
class ${_factory_classname}
    : public QObject
    , public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    explicit ${_factory_classname}(QObject *parent = nullptr)
        : QObject(parent)
        , m_initialized(false)
    {}

    ~${_factory_classname}() override {}
    
public: // QDesignerCustomWidgetInterface API
    bool isInitialized() const override { return m_initialized; }
${_dom_xml_method}
    bool isContainer() const override { return ${_is_container}; }
    QIcon icon() const override { return ${_icon_construct}; }
    QString group() const override { return QStringLiteral(\"${_group}\"); }
    QString includeFile() const override { return QStringLiteral(\"${ECM_QTDESIGNERPLUGIN_${widget}_INCLUDE_FILE}\"); }
    QString name() const override { return QStringLiteral(\"${_classname}\"); }
    QString toolTip() const override { return QStringLiteral(\"${_tooltip}\"); }
    QString whatsThis() const override { return QStringLiteral(\"${_whatsthis}\"); }

    QWidget* createWidget(QWidget* parent) override
    {
${ECM_QTDESIGNERPLUGIN_${widget}_CREATE_WIDGET_CODE}
    }

    void initialize(QDesignerFormEditorInterface* core) override 
    {
${_initialize_code}
    }

private:
    bool m_initialized;
};
"
    )
endfunction()

# helper method
function(_ecm_qtdesignerplugin_write_icon_qrc_file rc_file rc_icon_dir)
    set(_icons ${ARGN})
    file(WRITE ${rc_file}
"<!DOCTYPE RCC><RCC version=\"1.0\">
<!-- DO NOT EDIT! Generated from ecm_add_qtdesignerplugin() -->
<qresource prefix=\"${rc_icon_dir}\">
"
    )
    foreach(_icon ${_icons})
        get_filename_component(_icon_filename ${_icon} NAME)
        file(APPEND ${rc_file} "<file alias=\"${_icon_filename}\">${_icon}</file>\n")
    endforeach()
    file(APPEND ${rc_file}
"</qresource>
</RCC>
"
    )
endfunction()

# This needs to be a macro not a function because of the nested
# find_package() call, which will set some variables.
macro(ecm_add_qtdesignerplugin target)
    set(options
    )
    set(oneValueArgs
        NAME
        OUTPUT_NAME
        INSTALL_DESTINATION
        DEFAULT_GROUP
        COMPONENT
        DEFAULT_HEADER_CASE
        DEFAULT_HEADER_EXTENSION
        DEFAULT_ICON_DIR
    )
    set(multiValueArgs
        WIDGETS
        LINK_LIBRARIES
        INCLUDE_FILES
        SOURCES
    )
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # args sanity check
    if (NOT ARGS_WIDGETS)
        message(FATAL_ERROR "No WIDGETS passed when calling ecm_add_qtdesignerplugin().")
    endif()
    foreach(_widget ${ARGS_WIDGETS})
        # using _CLASS_NAME as sample property to find if defined
        if (NOT ECM_QTDESIGNERPLUGIN_${_widget}_CLASS_NAME)
            message(FATAL_ERROR "Undefined widget passed when calling ecm_add_qtdesignerplugin(): ${_widget}")
        endif()
    endforeach()

    if(NOT ARGS_NAME)
        set(ARGS_NAME "${target}")
    endif()
    if(NOT ARGS_DEFAULT_GROUP)
        set(ARGS_DEFAULT_GROUP "Custom")
    endif()
    if(NOT ARGS_DEFAULT_HEADER_EXTENSION)
        set(ARGS_DEFAULT_HEADER_EXTENSION "h")
    endif()
    if(NOT ARGS_DEFAULT_HEADER_CASE)
        set(ARGS_DEFAULT_HEADER_CASE "LOWER_CASE")
    else()
        set(_allowed_values "LOWER_CASE" "UPPER_CASE" "SAME_CASE")
        list(FIND _allowed_values "${ARGS_DEFAULT_HEADER_CASE}" _index)
        if(_index EQUAL "-1")
            message(FATAL_ERROR "Unexpected value for DEFAULT_HEADER_CASE argument to ecm_add_qtdesignerplugin(): ${ARGS_DEFAULT_HEADER_CASE}")
        endif()
    endif()
    if(NOT ARGS_DEFAULT_ICON_DIR)
        set(ARGS_DEFAULT_ICON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pics")
    else()
        if (NOT IS_ABSOLUTE ${ARGS_DEFAULT_ICON_DIR})
            set(ARGS_DEFAULT_ICON_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_DEFAULT_ICON_DIR}")
        endif()
        if(NOT EXISTS "${ARGS_DEFAULT_ICON_DIR}")
            message(FATAL_ERROR "No such directory as passed with DEFAULT_ICON_DIR when calling ecm_add_qtdesignerplugin(): ${ARGS_DEFAULT_ICON_DIR}")
        endif()
    endif()

    # Check deps
    # peek at Qt5Core to learn about the version to decide whether Qt5UiPlugin is enough
    if(NOT Qt5Core_FOUND)
        find_package(Qt5Core QUIET CONFIG)
    endif()
    if(Qt5Core_VERSION VERSION_LESS "5.5.0")
        set(_qtdesigner_tobefound TRUE)
    elseif(Qt5Core_VERSION VERSION_LESS "5.9.0")
        set(_qtdesigner_tobefound TRUE)
        set(_qtuiplugin_tobefound TRUE)
    else()
        # Since Qt 5.9 only Qt5UiPlugin is needed
        set(_qtuiplugin_tobefound TRUE)
    endif()
    if(NOT Qt5Designer_FOUND AND _qtdesigner_tobefound)
        find_package(Qt5Designer QUIET CONFIG)
        set_package_properties(Qt5Designer PROPERTIES
            PURPOSE "Required to build Qt Designer plugins"
            TYPE REQUIRED
        )
    endif()
    if(NOT Qt5UiPlugin_FOUND AND _qtuiplugin_tobefound)
        find_package(Qt5UiPlugin QUIET CONFIG)
        set_package_properties(Qt5UiPlugin PROPERTIES
            PURPOSE "Required to build Qt Designer plugins"
            TYPE REQUIRED
        )
    endif()
    if (Qt5Designer_FOUND)
        set(_qtdesigner_tobefound FALSE)
    endif()
    if (Qt5UiPlugin_FOUND)
        set(_qtuiplugin_tobefound FALSE)
        # in some old versions Qt5UiPlugin does not set its _INCLUDE_DIRS variable. Fill it manually
        get_target_property(Qt5UiPlugin_INCLUDE_DIRS Qt5::UiPlugin INTERFACE_INCLUDE_DIRECTORIES)
    endif()

    # setup plugin only if designer/uiplugin libs were found, as we do not abort hard the cmake run otherwise
    if(NOT _qtdesigner_tobefound AND NOT _qtuiplugin_tobefound)
        set(_generation_dir "${CMAKE_CURRENT_BINARY_DIR}/${target}_ECMQtDesignerPlugin")
        file(MAKE_DIRECTORY "${_generation_dir}")
        set(_rc_icon_dir "/${ARGS_NAME}/designer")

        # process defaults for widgets
        foreach(_widget ${ARGS_WIDGETS})
            set(_class_name "${ECM_QTDESIGNERPLUGIN_${_widget}_CLASS_NAME}")
            # include file
            set(_include_file "${ECM_QTDESIGNERPLUGIN_${_widget}_INCLUDE_FILE}")
            if(NOT _include_file)
                set(_include_file "${_class_name}")
                if (ARGS_DEFAULT_HEADER_CASE STREQUAL "LOWER_CASE")
                    string(TOLOWER "${_include_file}" _include_file)
                elseif(ARGS_DEFAULT_HEADER_CASE STREQUAL "UPPER_CASE")
                    string(TOUPPER "${_include_file}" _include_file)
                endif()
                # turn any namespaces into dir levels
                string(REPLACE "::" "/" _include_file ${_include_file})
                set(ECM_QTDESIGNERPLUGIN_${_widget}_INCLUDE_FILE "${_include_file}.${ARGS_DEFAULT_HEADER_EXTENSION}")
            endif()
            # icon
            set(_icon "${ECM_QTDESIGNERPLUGIN_${_widget}_ICON}")
            if(NOT _icon)
                string(TOLOWER "${_class_name}" _icon)
                # handle any namespaces
                string(REPLACE "::" "_" _icon "${_icon}")
                set(_icon "${ARGS_DEFAULT_ICON_DIR}/${_icon}.png")
                if(EXISTS "${_icon}")
                    set(ECM_QTDESIGNERPLUGIN_${_widget}_ICON "${_icon}")
                endif()
            endif()
        endforeach()

        set(_plugin_src_file "${_generation_dir}/designerplugin.cpp")
        set(_srcs
            ${ARGS_SOURCES}
            ${_plugin_src_file}
        )

        set(_icons)
        foreach(_widget ${ARGS_WIDGETS})
            list(APPEND _icons "${ECM_QTDESIGNERPLUGIN_${_widget}_ICON}")
        endforeach()

        # generate qrc file with icons
        if (_icons)
            set(_rc_file "${_generation_dir}/designerplugin.rc")
            set(_rc_work_file "${_rc_file}.work")

            _ecm_qtdesignerplugin_write_icon_qrc_file("${_rc_work_file}" "${_rc_icon_dir}" ${_icons})
            # avoid rebuilding if there was no change
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_rc_work_file}" "${_rc_file}"
            )
            file(REMOVE "${_rc_work_file}")

            qt5_add_resources(_srcs ${_rc_file})
        endif()

        # generate source file
        set(_collection_classname "${ARGS_NAME}QtDesignerWidgetCollection")

        set(_include_files
            # classes inherited
            QDesignerCustomWidgetCollectionInterface
            QDesignerCustomWidgetInterface
            QObject
            # classes used
            QIcon
            QString
            ${ARGS_INCLUDE_FILES}
        )
        foreach(_widget ${ARGS_WIDGETS})
            list(APPEND _include_files ${ECM_QTDESIGNERPLUGIN_${_widget}_INCLUDE_FILE})
        endforeach()
        list(REMOVE_DUPLICATES _include_files)

        set(_plugin_src_work_file "${_plugin_src_file}.work")
        file(WRITE ${_plugin_src_work_file} "// DO NOT EDIT! Generated from ecm_add_qtdesignerplugin()\n\n")
        foreach(_include_file ${_include_files})
            if (NOT ${_include_file} MATCHES "^[\"<]")
                set(_include_file "<${_include_file}>")
            endif()
            file(APPEND ${_plugin_src_work_file} "#include ${_include_file}\n")
        endforeach()
        foreach(_widget ${ARGS_WIDGETS})
            _ecm_qtdesignerplugin_write_widget(${_plugin_src_work_file} ${_widget} ${ARGS_DEFAULT_GROUP} ${_rc_icon_dir})
        endforeach()
        file(APPEND ${_plugin_src_work_file} "
class ${_collection_classname}
    : public QObject
    , public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(
        QDesignerCustomWidgetCollectionInterface
    )

    Q_PLUGIN_METADATA(IID \"org.qt-project.Qt.QDesignerCustomWidgetInterface\")

public:
    explicit ${_collection_classname}(QObject* parent = nullptr);

public: // QDesignerCustomWidgetCollectionInterface API
    QList<QDesignerCustomWidgetInterface*> customWidgets() const override;

private:
    QList<QDesignerCustomWidgetInterface*> m_widgetFactories;
};

${_collection_classname}::${_collection_classname}(QObject* parent)
    : QObject(parent)
{
    m_widgetFactories = QList<QDesignerCustomWidgetInterface*>{
"
        )
        foreach(_widget ${ARGS_WIDGETS})
            file(APPEND ${_plugin_src_work_file} "        new ${ECM_QTDESIGNERPLUGIN_${_widget}_FACTORY_CLASS_NAME}(this),\n")
        endforeach()
        file(APPEND ${_plugin_src_work_file}
"    };
}

QList<QDesignerCustomWidgetInterface*> ${_collection_classname}::customWidgets() const
{
    return m_widgetFactories;
}

#include \"designerplugin.moc\"
"
        )

        # avoid rebuilding if there was no change
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_plugin_src_work_file}" "${_plugin_src_file}"
        )
        file(REMOVE "${_plugin_src_work_file}")

        # setup plugin binary
        add_library(${target} MODULE ${_srcs})
        if(Qt5UiPlugin_VERSION AND NOT Qt5UiPlugin_VERSION VERSION_LESS "5.9.0")
            list(APPEND ARGS_LINK_LIBRARIES Qt5::UiPlugin)
        else()
            # For Qt <5.9 include dir variables needed
            target_include_directories(${target}
                PRIVATE ${Qt5UiPlugin_INCLUDE_DIRS}
                PRIVATE ${Qt5Designer_INCLUDE_DIRS}
            )
        endif()
        if(NOT WIN32)
            # Since there are no libraries provided by this module,
            # there is no point including the build tree in RPath,
            # and then having to edit it at install time.
            set_target_properties(${target} PROPERTIES
                SKIP_BUILD_RPATH TRUE
                BUILD_WITH_INSTALL_RPATH TRUE
            )
        endif()
        if (ARGS_OUTPUT_NAME)
            set_target_properties(${target} PROPERTIES
                OUTPUT_NAME ${ARGS_OUTPUT_NAME}
            )
        endif()
        target_link_libraries(${target} ${ARGS_LINK_LIBRARIES})

        if (DEFINED ARGS_COMPONENT)
            set(_component COMPONENT ${ARGS_COMPONENT})
        else()
            set(_component)
        endif()

        install(TARGETS ${target} DESTINATION ${ARGS_INSTALL_DESTINATION} ${_component})
    endif()
endmacro()
