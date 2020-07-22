#!/usr/bin/env python
#
# SPDX-FileCopyrightText: 2016 by Shaheed Haque <srhaque@theiet.org>
# SPDX-FileCopyrightText: 2016 Stephen Kelly <steveire@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

"""SIP file generation rules engine."""

from __future__ import print_function

from abc import *
import argparse
import gettext
import inspect
import logging
import os
import re
import sys
import textwrap
import traceback
from copy import deepcopy
from clang.cindex import CursorKind

from clang.cindex import AccessSpecifier

class HelpFormatter(argparse.ArgumentDefaultsHelpFormatter, argparse.RawDescriptionHelpFormatter):
    pass


logger = logging.getLogger(__name__)
gettext.install(__name__)
_SEPARATOR = "\x00"

def _parents(container):
    parents = []
    parent = container.semantic_parent
    while parent and parent.kind != CursorKind.TRANSLATION_UNIT:
        parents.append(parent.spelling)
        parent = parent.semantic_parent
    if parents:
        parents = "::".join(reversed(parents))
    else:
        parents = os.path.basename(container.translation_unit.spelling)
    return parents


class Rule(object):
    def __init__(self, db, rule_number, fn, pattern_zip):
        #
        # Derive a useful name for diagnostic purposes.
        #
        caller = os.path.basename(inspect.stack()[3][1])
        self.name =  "{}:{}[{}],{}".format(caller, type(db).__name__, rule_number, fn.__name__)
        self.rule_number = rule_number
        self.fn = fn
        self.usage = 0
        try:
            groups = ["(?P<{}>{})".format(name, pattern) for pattern, name in pattern_zip]
            groups = _SEPARATOR.join(groups)
            #
            # We'll use re.match to anchor the start of the match, and so need a $ to anchor the end.
            #
            self.matcher = re.compile(groups + "$")
        except Exception as e:
            groups = ["{} '{}'".format(name, pattern) for pattern, name in pattern_zip]
            groups = ", ".join(groups)
            raise RuntimeError(_("Bad {}: {}: {}").format(self, groups, e))

    def match(self, candidate):
        return self.matcher.match(candidate)

    def trace_result(self, parents, item, original, modified):
        """
        Record any modification both in the log and the returned result. If a rule fired, but
        caused no modification, that is logged.

        :return: Modifying rule or None.
        """
        fqn = parents + "::" + original["name"] + "[" + str(item.extent.start.line) + "]"
        return self._trace_result(fqn, original, modified)

    def _trace_result(self, fqn, original, modified):
        """
        Record any modification both in the log and the returned result. If a rule fired, but
        caused no modification, that is logged.

        :return: Modifying rule or None.
        """
        if not modified["name"]:
            logger.debug(_("Rule {} suppressed {}, {}").format(self, fqn, original))
        else:
            delta = False
            for k, v in original.items():
                if v != modified[k]:
                    delta = True
                    break
            if delta:
                logger.debug(_("Rule {} modified {}, {}->{}").format(self, fqn, original, modified))
            else:
                logger.warn(_("Rule {} did not modify {}, {}").format(self, fqn, original))
                return None
        return self

    def __str__(self):
        return self.name


class AbstractCompiledRuleDb(object):
    __metaclass__ = ABCMeta

    def __init__(self, db, parameter_names):
        self.db = db
        self.compiled_rules = []
        for i, raw_rule in enumerate(db()):
            if len(raw_rule) != len(parameter_names) + 1:
                raise RuntimeError(_("Bad raw rule {}: {}: {}").format(db.__name__, raw_rule, parameter_names))
            z = zip(raw_rule[:-1], parameter_names)
            self.compiled_rules.append(Rule(self, i, raw_rule[-1], z))
        self.candidate_formatter = _SEPARATOR.join(["{}"] * len(parameter_names))

    def _match(self, *args):
        candidate = self.candidate_formatter.format(*args)
        for rule in self.compiled_rules:
            matcher = rule.match(candidate)
            if matcher:
                #
                # Only use the first matching rule.
                #
                rule.usage += 1
                return matcher, rule
        return None, None

    @abstractmethod
    def apply(self, *args):
        raise NotImplemented(_("Missing subclass"))

    def dump_usage(self, fn):
        """ Dump the usage counts."""
        for rule in self.compiled_rules:
            fn(str(rule), rule.usage)


class ContainerRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR CONTAINERS.

    These are used to customize the behaviour of the SIP generator by allowing
    the declaration for any container (class, namespace, struct, union) to be
    customized, for example to add SIP compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the container.

        1. A regular expression which matches the container name.

        2. A regular expression which matches any template parameters.

        3. A regular expression which matches the container declaration.

        4. A regular expression which matches any base specifiers.

        5. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def container_xxx(container, sip, matcher):
            '''
            Return a modified declaration for the given container.

            :param container:   The clang.cindex.Cursor for the container.
            :param sip:         A dict with the following keys:

                                    name                The name of the container.
                                    template_parameters Any template parameters.
                                    decl                The declaration.
                                    base_specifiers     Any base specifiers.
                                    body                The body, less the outer
                                                        pair of braces.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT body and annotations.

            :return: An updated set of sip.xxx values. Setting sip.name to the
                     empty string will cause the container to be suppressed.
            '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(ContainerRuleDb, self).__init__(db, ["parents", "container", "template_parameters", "decl", "base_specifiers"])

    def apply(self, container, sip):
        """
        Walk over the rules database for containers, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        parents = _parents(container)
        matcher, rule = self._match(parents, sip["name"],
                                    ", ".join(sip["template_parameters"]),
                                    sip["decl"],
                                    ", ".join(sip["base_specifiers"]))
        if matcher:
            before = deepcopy(sip)
            rule.fn(container, sip, matcher)
            return rule.trace_result(parents, container, before, sip)
        return None


class ForwardDeclarationRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR FORWARD DECLARATIONS.

    These are used to customize the behaviour of the SIP generator by allowing
    the forward declaration for any container (class, struct, union) to be
    customized, for example to add SIP compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the container.

        1. A regular expression which matches the container name.

        2. A regular expression which matches any template parameters.

        3. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def declaration_xxx(container, sip, matcher):
            '''
            Return a modified declaration for the given container.

            :param container:   The clang.cindex.Cursor for the container.
            :param sip:         A dict with the following keys:

                                    name                The name of the container.
                                    template_parameters Any template parameters.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT body and annotations.

            :return: An updated set of sip.xxx values. Setting sip.name to the
                     empty string will cause the container to be suppressed.
            '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(ForwardDeclarationRuleDb, self).__init__(db, ["parents", "container", "template_parameters"])

    def apply(self, container, sip):
        """
        Walk over the rules database for containers, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        parents = _parents(container)
        matcher, rule = self._match(parents, sip["name"],
                                    ", ".join(sip["template_parameters"]))
        if matcher:
            before = deepcopy(sip)
            rule.fn(container, sip, matcher)
            return rule.trace_result(parents, container, before, sip)
        return None


class FunctionRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR FUNCTIONS.

    These are used to customize the behaviour of the SIP generator by allowing
    the declaration for any function to be customized, for example to add SIP
    compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the function.

        1. A regular expression which matches the function name.

        2. A regular expression which matches any template parameters.

        3. A regular expression which matches the function result.

        4. A regular expression which matches the function parameters (e.g.
        "int a, void *b" for "int foo(int a, void *b)").

        5. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def function_xxx(container, function, sip, matcher):
            '''
            Return a modified declaration for the given function.

            :param container:   The clang.cindex.Cursor for the container.
            :param function:    The clang.cindex.Cursor for the function.
            :param sip:         A dict with the following keys:

                                    name                The name of the function.
                                    template_parameters Any template parameters.
                                    fn_result           Result, if not a constructor.
                                    parameters          The parameters.
                                    prefix              Leading keyworks ("static"). Separated by space,
                                                        ends with a space.
                                    suffix              Trailing keywords ("const"). Separated by space, starts with
                                                        space.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT annotations.

            :return: An updated set of sip.xxx values. Setting sip.name to the
                     empty string will cause the container to be suppressed.
            '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(FunctionRuleDb, self).__init__(db, ["container", "function", "template_parameters", "fn_result", "parameters"])

    def apply(self, container, function, sip):
        """
        Walk over the rules database for functions, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param function:            The clang.cindex.Cursor for the function.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        parents = _parents(function)
        matcher, rule = self._match(parents, sip["name"], ", ".join(sip["template_parameters"]), sip["fn_result"], ", ".join(sip["parameters"]))
        if matcher:
            sip.setdefault("code", "")
            before = deepcopy(sip)
            rule.fn(container, function, sip, matcher)
            return rule.trace_result(parents, function, before, sip)
        return None


class ParameterRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR FUNCTION PARAMETERS.

    These are used to customize the behaviour of the SIP generator by allowing
    the declaration for any parameter in any function to be customized, for
    example to add SIP compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the function enclosing the parameter.

        1. A regular expression which matches the function name enclosing the
        parameter.

        2. A regular expression which matches the parameter name.

        3. A regular expression which matches the parameter declaration (e.g.
        "int foo").

        4. A regular expression which matches the parameter initialiser (e.g.
        "Xyz:MYCONST + 42").

        5. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def parameter_xxx(container, function, parameter, sip, init, matcher):
            '''
            Return a modified declaration and initialiser for the given parameter.

            :param container:   The clang.cindex.Cursor for the container.
            :param function:    The clang.cindex.Cursor for the function.
            :param parameter:   The clang.cindex.Cursor for the parameter.
            :param sip:         A dict with the following keys:

                                    name                The name of the function.
                                    decl                The declaration.
                                    init                Any initialiser.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT annotations.

            :return: An updated set of sip.xxx values.
        '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(ParameterRuleDb, self).__init__(db, ["container", "function", "parameter", "decl", "init"])

    def apply(self, container, function, parameter, sip):
        """
        Walk over the rules database for parameters, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param function:            The clang.cindex.Cursor for the function.
        :param parameter:           The clang.cindex.Cursor for the parameter.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        parents = _parents(function)
        matcher, rule = self._match(parents, function.spelling, sip["name"], sip["decl"], sip["init"])
        if matcher:
            sip.setdefault("code", "")
            before = deepcopy(sip)
            rule.fn(container, function, parameter, sip, matcher)
            return rule.trace_result(parents, parameter, before, sip)
        return None


class TypedefRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR TYPEDEFS.

    These are used to customize the behaviour of the SIP generator by allowing
    the declaration for any typedef to be customized, for example to add SIP
    compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the typedef.

        1. A regular expression which matches the typedef name.

        2. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def typedef_xxx(container, typedef, sip, matcher):
            '''
            Return a modified declaration for the given function.

            :param container:   The clang.cindex.Cursor for the container.
            :param typedef:     The clang.cindex.Cursor for the typedef.
            :param sip:         A dict with the following keys:

                                    name                The name of the typedef.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT annotations.

            :return: An updated set of sip.xxx values. Setting sip.name to the
                     empty string will cause the container to be suppressed.
            '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(TypedefRuleDb, self).__init__(db, ["container", "typedef"])

    def apply(self, container, typedef, sip):
        """
        Walk over the rules database for typedefs, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param typedef:             The clang.cindex.Cursor for the typedef.
        :param sip:                 The SIP dict.
        """
        parents = _parents(typedef)
        matcher, rule = self._match(parents, sip["name"])
        if matcher:
            before = deepcopy(sip)
            rule.fn(container, typedef, sip, matcher)
            return rule.trace_result(parents, typedef, before, sip)
        return None


class VariableRuleDb(AbstractCompiledRuleDb):
    """
    THE RULES FOR VARIABLES.

    These are used to customize the behaviour of the SIP generator by allowing
    the declaration for any variable to be customized, for example to add SIP
    compiler annotations.

    Each entry in the raw rule database must be a list with members as follows:

        0. A regular expression which matches the fully-qualified name of the
        "container" enclosing the variable.

        1. A regular expression which matches the variable name.

        2. A regular expression which matches the variable declaration (e.g.
        "int foo").

        3. A function.

    In use, the database is walked in order from the first entry. If the regular
    expressions are matched, the function is called, and no further entries are
    walked. The function is called with the following contract:

        def variable_xxx(container, variable, sip, matcher):
            '''
            Return a modified declaration for the given variable.

            :param container:   The clang.cindex.Cursor for the container.
            :param variable:    The clang.cindex.Cursor for the variable.
            :param sip:         A dict with the following keys:

                                    name                The name of the variable.
                                    decl                The declaration.
                                    annotations         Any SIP annotations.

            :param matcher:         The re.Match object. This contains named
                                    groups corresponding to the key names above
                                    EXCEPT annotations.

            :return: An updated set of sip.xxx values. Setting sip.name to the
                     empty string will cause the container to be suppressed.
            '''

    :return: The compiled form of the rules.
    """
    def __init__(self, db):
        super(VariableRuleDb, self).__init__(db, ["container", "variable", "decl"])

    def apply(self, container, variable, sip):
        """
        Walk over the rules database for variables, applying the first matching transformation.

        :param container:           The clang.cindex.Cursor for the container.
        :param variable:            The clang.cindex.Cursor for the variable.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        parents = _parents(variable)
        matcher, rule = self._match(parents, sip["name"], sip["decl"])
        if matcher:
            sip.setdefault("code", "")
            before = deepcopy(sip)
            rule.fn(container, variable, sip, matcher)
            return rule.trace_result(parents, variable, before, sip)
        return None


class AbstractCompiledCodeDb(object):
    __metaclass__ = ABCMeta

    def __init__(self, db):
        caller = os.path.basename(inspect.stack()[2][1])
        self.name = "{}:{}".format(caller, type(self).__name__)
        self.db = db

    @abstractmethod
    def apply(self, function, sip):
        raise NotImplemented(_("Missing subclass"))

    def trace_result(self, parents, item, original, modified):
        """
        Record any modification both in the log and the returned result. If a rule fired, but
        caused no modification, that is logged.

        :return: Modifying rule or None.
        """
        fqn = parents + "::" + original["name"] + "[" + str(item.extent.start.line) + "]"
        self._trace_result(fqn, original, modified)

    def _trace_result(self, fqn, original, modified):
        """
        Record any modification both in the log and the returned result. If a rule fired, but
        caused no modification, that is logged.

        :return: Modifying rule or None.
        """
        if not modified["name"]:
            logger.debug(_("Rule {} suppressed {}, {}").format(self, fqn, original))
        else:
            delta = False
            for k, v in original.items():
                if v != modified[k]:
                    delta = True
                    break
            if delta:
                logger.debug(_("Rule {} modified {}, {}->{}").format(self, fqn, original, modified))
            else:
                logger.warn(_("Rule {} did not modify {}, {}").format(self, fqn, original))
                return None
        return self

    @abstractmethod
    def dump_usage(self, fn):
        raise NotImplemented(_("Missing subclass"))

    def __str__(self):
        return self.name

class MethodCodeDb(AbstractCompiledCodeDb):
    """
    THE RULES FOR INJECTING METHOD-RELATED CODE (such as %MethodCode,
    %VirtualCatcherCode, %VirtualCallCode and other method-level directives).

    These are used to customize the behaviour of the SIP generator by allowing
    method-level code injection.

    The raw rule database must be an outer dictionary as follows:

        0. Each key is the fully-qualified name of a "container" enclosing
        methods.

        1. Each value is an inner dictionary, each of whose keys is the name
        of a method.

    Each inner dictionary has entries which update the declaration as follows:

        "name":         Optional string. If present, overrides the name of the
                        method.
        "parameters":   Optional list. If present, update the argument list.

        "fn_result":    Optional string. If present, update the return type.

        "code":         Required. Either a string, with the %XXXCode content,
                        or a callable.

    In use, the database is directly indexed by "container" and then method
    name. If "code" entry is a string, then the other optional keys are
    interpreted as above. If "code" is a callable, it is called with the
    following contract:

        def methodcode_xxx(function, sip, entry):
            '''
            Return a modified declaration for the given function.

            :param function:    The clang.cindex.Cursor for the function.
            :param sip:         A dict with keys as for function rules and (string)
                                "code" keys described above.
            :param entry:       The inner dictionary entry.

            :return: An updated set of sip.xxx values.
            '''

    :return: The compiled form of the rules.
    """
    __metaclass__ = ABCMeta

    def __init__(self, db):
        super(MethodCodeDb, self).__init__(db)

        for k, v in self.db.items():
            for l in v.keys():
                v[l]["usage"] = 0

    def _get(self, item, name):

        parents = _parents(item)
        entries = self.db.get(parents, None)
        if not entries:
            return None

        entry = entries.get(name, None)
        if not entry:
            return None
        entry["usage"] += 1
        return entry

    def apply(self, function, sip):
        """
        Walk over the code database for functions, applying the first matching transformation.

        :param function:            The clang.cindex.Cursor for the function.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        entry = self._get(function, sip["name"])
        sip.setdefault("code", "")
        if entry:
            before = deepcopy(sip)
            if callable(entry["code"]):
                fn = entry["code"]
                fn_file = os.path.basename(inspect.getfile(fn))
                trace = "// Generated (by {}:{}): {}\n".format(fn_file, fn.__name__, {k:v for (k,v) in entry.items() if k != "code"})
                fn(function, sip, entry)
            else:
                trace = "// Inserted (by {}:{}): {}\n".format(_parents(function), function.spelling, {k:v for (k,v) in entry.items() if k != "code"})
                sip["code"] = entry["code"]
                sip["parameters"] = entry.get("parameters", sip["parameters"])
                sip["fn_result"] = entry.get("fn_result", sip["fn_result"])
            #
            # Fetch/format the code.
            #
            sip["code"] = trace + textwrap.dedent(sip["code"]).strip() + "\n"
            return self.trace_result(_parents(function), function, before, sip)
        return None

    def dump_usage(self, fn):
        """ Dump the usage counts."""
        for k in sorted(self.db.keys()):
            vk = self.db[k]
            for l in sorted(vk.keys()):
                vl = vk[l]
                fn(str(self) + " for " + k + "," + l, vl["usage"])

class ModuleCodeDb(AbstractCompiledCodeDb):
    """
    THE RULES FOR INJECTING MODULE-RELATED CODE (such as %ExportedHeaderCode,
    %ModuleCode, %ModuleHeaderCode or other module-level directives).

    These are used to customize the behaviour of the SIP generator by allowing
    module-level code injection.

    The raw rule database must be a dictionary as follows:

        0. Each key is the basename of a header file.

        1. Each value has entries which update the declaration as follows:

        "code":         Required. Either a string, with the %XXXCode content,
                        or a callable.

    If "code" is a callable, it is called with the following contract:

        def module_xxx(filename, sip, entry):
            '''
            Return a string to insert for the file.

            :param filename:    The filename.
            :param sip:         A dict with the key "name" for the filename,
                                "decl" for the module body plus the "code" key
                                described above.
            :param entry:       The dictionary entry.

            :return: A string.
            '''

    :return: The compiled form of the rules.
    """
    __metaclass__ = ABCMeta

    def __init__(self, db):
        super(ModuleCodeDb, self).__init__(db)
        #
        # Add a usage count and other diagnostic support for each item in the database.
        #
        for k, v in self.db.items():
            v["usage"] = 0

    def _get(self, filename):
        #
        # Lookup for an actual hit.
        #
        entry = self.db.get(filename, None)
        if not entry:
            return None
        entry["usage"] += 1
        return entry

    def apply(self, filename, sip):
        """
        Walk over the code database for modules, applying the first matching transformation.

        :param filename:            The file for the module.
        :param sip:                 The SIP dict (may be modified on return).
        :return:                    Modifying rule or None (even if a rule matched, it may not modify things).
        """
        entry = self._get(filename)
        sip.setdefault("code", "")
        if entry:
            before = deepcopy(sip)
            if callable(entry["code"]):
                fn = entry["code"]
                fn_file = os.path.basename(inspect.getfile(fn))
                trace = "\n// Generated (by {}:{}): {}".format(fn_file, fn.__name__, {k:v for (k,v) in entry.items() if k != "code"})
                fn(filename, sip, entry)
                sip["code"] = trace + sip["code"]
            else:
                sip["code"] = entry["code"]
            #
            # Fetch/format the code.
            #
            sip["code"] = textwrap.dedent(sip["code"]).strip() + "\n"
            fqn = filename + "::" + before["name"]
            self._trace_result(fqn, before, sip)

    def dump_usage(self, fn):
        """ Dump the usage counts."""
        for k in sorted(self.db.keys()):
            v = self.db[k]
            fn(str(self) + " for " + k, v["usage"])


class RuleSet(object):
    """
    To implement your own binding, create a subclass of RuleSet, also called
    RuleSet in your own Python module. Your subclass will expose the raw rules
    along with other ancilliary data exposed through the subclass methods.

    You then simply run the SIP generation and SIP compilation programs passing
    in the name of your rules file
    """
    __metaclass__ = ABCMeta

    @abstractmethod
    def container_rules(self):
        """
        Return a compiled list of rules for containers.

        :return: A ContainerRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def forward_declaration_rules(self):
        """
        Return a compiled list of rules for containers.

        :return: A ForwardDeclarationRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def function_rules(self):
        """
        Return a compiled list of rules for functions.

        :return: A FunctionRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def parameter_rules(self):
        """
        Return a compiled list of rules for function parameters.

        :return: A ParameterRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def typedef_rules(self):
        """
        Return a compiled list of rules for typedefs.

        :return: A TypedefRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def variable_rules(self):
        """
        Return a compiled list of rules for variables.

        :return: A VariableRuleDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def methodcode_rules(self):
        """
        Return a compiled list of rules for method-related code.

        :return: A MethodCodeDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def modulecode_rules(self):
        """
        Return a compiled list of rules for module-related code.

        :return: A ModuleCodeDb instance
        """
        raise NotImplemented(_("Missing subclass implementation"))

    def dump_unused(self):
        """Usage statistics, to identify unused rules."""
        def dumper(rule, usage):
            if usage:
                logger.info(_("Rule {} used {} times".format(rule, usage)))
            else:
                logger.warn(_("Rule {} was not used".format(rule)))

        for db in [self.container_rules(), self.forward_declaration_rules(), self.function_rules(),
                   self.parameter_rules(), self.typedef_rules(),
                   self.variable_rules(), self.methodcode_rules(), self.modulecode_rules()]:
            db.dump_usage(dumper)

    @abstractmethod
    def methodcode(self, container, function):
        """
        Lookup %MethodCode.
        """
        raise NotImplemented(_("Missing subclass implementation"))

    @abstractmethod
    def modulecode(self, filename):
        """
        Lookup %ModuleCode and friends.
        """
        raise NotImplemented(_("Missing subclass implementation"))


def container_discard(container, sip, matcher):
    sip["name"] = ""

def function_discard(container, function, sip, matcher):
    sip["name"] = ""

def parameter_transfer_to_parent(container, function, parameter, sip, matcher):
    if function.is_static_method():
        sip["annotations"].add("Transfer")
    else:
        sip["annotations"].add("TransferThis")

def param_rewrite_mode_t_as_int(container, function, parameter, sip, matcher):
    sip["decl"] = sip["decl"].replace("mode_t", "unsigned int")

def return_rewrite_mode_t_as_int(container, function, sip, matcher):
    sip["fn_result"] = "unsigned int"

def variable_discard(container, variable, sip, matcher):
    sip["name"] = ""

def parameter_strip_class_enum(container, function, parameter, sip, matcher):
    sip["decl"] = sip["decl"].replace("class ", "").replace("enum ", "")

def function_discard_impl(container, function, sip, matcher):
    if function.extent.start.column == 1:
        sip["name"] = ""

def typedef_discard(container, typedef, sip, matcher):
    sip["name"] = ""

def discard_QSharedData_base(container, sip, matcher):
    sip["base_specifiers"].remove("QSharedData")

def mark_forward_declaration_external(container, sip, matcher):
    sip["annotations"].add("External")

def container_mark_abstract(container, sip, matcher):
    sip["annotations"].add("Abstract")

def rules(project_rules):
    """
    Constructor.

    :param project_rules:       The rules file for the project.
    """
    import imp
    imp.load_source("project_rules", project_rules)
    #
    # Statically prepare the rule logic. This takes the rules provided by the user and turns them into code.
    #
    return getattr(sys.modules["project_rules"], "RuleSet")()
