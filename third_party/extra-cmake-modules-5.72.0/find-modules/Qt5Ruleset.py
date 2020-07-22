#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2016 Shaheed Haque <srhaque@theiet.org>
# SPDX-FileCopyrightText: 2016 Stephen Kelly <steveire@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

from __future__ import print_function

import rules_engine

def container_rules():
    return [
        #
        # SIP does not seem to be able to handle these.
        #
        [".*", "(QMetaTypeId|QTypeInfo)<.*>", ".*", ".*", ".*", rules_engine.container_discard],
        #
        # SIP does not seem to be able to handle templated containers.
        #
        [".*", ".*<.*", ".*", ".*", ".*", rules_engine.container_discard],
    ]

def function_rules():
    return [
        #
        # Discard functions emitted by QOBJECT.
        #
        [".*", "metaObject|qt_metacast|tr|trUtf8|qt_metacall|qt_check_for_QOBJECT_macro|qt_check_for_QGADGET_macro", ".*", ".*", ".*", rules_engine.function_discard],
        #
        # SIP does not support operator=.
        #
        [".*", "operator=", ".*", ".*", ".*", rules_engine.function_discard],

        [".*", ".*", ".*", ".*", ".*::QPrivateSignal.*", rules_engine.function_discard],
        #
        # TODO: Temporarily remove any functions which require templates. SIP seems to support, e.g. QPairs,
        # but we have not made them work yet.
        #

        [".*", ".*", ".+", ".*", ".*", rules_engine.function_discard],
        [".*", ".*<.*>.*", ".*", ".*", ".*", rules_engine.function_discard],

        [".*", ".*", ".*", ".*", ".*std::function.*", rules_engine.function_discard],
        [".*", ".*", ".*", ".*", ".*std::numeric_limits.*", rules_engine.function_discard],
        [".*", ".*", ".*", ".*", ".*QPair.*", rules_engine.function_discard],
        [".*", ".*", ".*", ".*QPair.*", ".*", rules_engine.function_discard],

        [".*", ".*", ".*", ".*", ".*QDebug.*", rules_engine.function_discard],
        [".*", ".*", ".*", ".*QDebug.*", ".*", rules_engine.function_discard],

        [".*", ".*", ".*", ".*", ".*QExplicitlySharedDataPointer.*", rules_engine.function_discard],
        [".*", ".*", ".*", ".*QExplicitlySharedDataPointer.*", ".*", rules_engine.function_discard],

        [".*", ".*", ".*", ".*", ".*Private.*", rules_engine.function_discard],
        [".*", ".*", ".*", "mode_t", ".*", rules_engine.return_rewrite_mode_t_as_int],
        [".*", "d_func", ".*", ".*", ".*", rules_engine.function_discard],

        [".*", "operator\|", ".*", ".*", ".*", rules_engine.function_discard],
    ]

def parameter_rules():
    return [
        #
        # Annotate with Transfer or TransferThis when we see a parent object.
        #
        [".*", ".*", ".*", r"[KQ][A-Za-z_0-9]+\W*\*\W*parent", ".*", rules_engine.parameter_transfer_to_parent],
        [".*", ".*", ".*", "mode_t.*", ".*", rules_engine.param_rewrite_mode_t_as_int],
        [".*", ".*", ".*", ".*enum .*", ".*", rules_engine.parameter_strip_class_enum],
    ]

def variable_rules():
    return [
        #
        # Discard variable emitted by QOBJECT.
        #
        [".*", "staticMetaObject", ".*", rules_engine.variable_discard],
        #
        # Discard "private" variables.
        #
        [".*", "d_ptr", ".*", rules_engine.variable_discard],
        [".*", "d", ".*Private.*", rules_engine.variable_discard],
    ]

def typedef_rules():
    return []

def forward_declaration_rules():
    return []

class RuleSet(rules_engine.RuleSet):
    """
    SIP file generator rules. This is a set of (short, non-public) functions
    and regular expression-based matching rules.
    """
    def __init__(self):
        self._container_db = rules_engine.ContainerRuleDb(container_rules)
        self._forward_declaration_db = rules_engine.ForwardDeclarationRuleDb(forward_declaration_rules)
        self._fn_db = rules_engine.FunctionRuleDb(function_rules)
        self._param_db = rules_engine.ParameterRuleDb(parameter_rules)
        self._typedef_db = rules_engine.TypedefRuleDb(typedef_rules)
        self._var_db = rules_engine.VariableRuleDb(variable_rules)
        self._methodcode = rules_engine.MethodCodeDb({})
        self._modulecode = rules_engine.ModuleCodeDb({})

    def container_rules(self):
        return self._container_db

    def forward_declaration_rules(self):
        return self._forward_declaration_db

    def function_rules(self):
        return self._fn_db

    def parameter_rules(self):
        return self._param_db

    def typedef_rules(self):
        return self._typedef_db

    def variable_rules(self):
        return self._var_db

    def methodcode_rules(self):
        return self._methodcode

    def modulecode_rules(self):
        return self._modulecode

    def methodcode(self, function, sip):
        return self._methodcode.apply(function, sip)

    def modulecode(self, filename, sip):
        return self._modulecode.apply(filename, sip)
