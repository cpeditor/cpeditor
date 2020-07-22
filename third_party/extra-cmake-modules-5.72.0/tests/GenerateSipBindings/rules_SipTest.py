
import os, sys

import rules_engine
sys.path.append(os.path.dirname(os.path.dirname(rules_engine.__file__)))
import Qt5Ruleset

def local_container_rules():
    return [
        [".*", "Shared", ".*", ".*", ".*", rules_engine.discard_QSharedData_base]
    ]

def local_forward_declaration_rules():
    return [
        [".*", "ExternalFwdDecl", ".*", rules_engine.mark_forward_declaration_external]
    ]

def local_function_rules():
    return [
        ["TypedefUser", "setTagPattern", ".*", ".*", ".*", rules_engine.function_discard],
    ]

def local_typedef_rules():
    return [
        [".*", "TagFormatter", rules_engine.typedef_discard],
    ]

def methodGenerator(function, sip, entry):
    sip["code"] = """
        %MethodCode
            sipRes = {} + myAcumulate(a0);
        %End
    """.format(entry["param"])


class RuleSet(Qt5Ruleset.RuleSet):
    def __init__(self):
        Qt5Ruleset.RuleSet.__init__(self)
        self._container_db = rules_engine.ContainerRuleDb(lambda: local_container_rules() + Qt5Ruleset.container_rules())
        self._forward_declaration_db = rules_engine.ForwardDeclarationRuleDb(lambda: local_forward_declaration_rules() + Qt5Ruleset.forward_declaration_rules())
        self._fn_db = rules_engine.FunctionRuleDb(lambda: local_function_rules() + Qt5Ruleset.function_rules())
        self._typedef_db = rules_engine.TypedefRuleDb(lambda: local_typedef_rules() + Qt5Ruleset.typedef_rules())
        self._modulecode = rules_engine.ModuleCodeDb({
            "cpplib.h": {
            "code": """
%ModuleCode
int myAcumulate(const QList<int> *list) {
    return std::accumulate(list->begin(), list->end(), 0);
}
%End\n
            """
            }
            })

        self._methodcode = rules_engine.MethodCodeDb({
            "SomeNS": {
                "customMethod": {
                    "code": """
                    %MethodCode
                        sipRes = myAcumulate(a0);
                    %End
                    """
                }
            },
            "cpplib.h": {
                "anotherCustomMethod": {
                    "code": methodGenerator,
                    "param": 42
                }
            }
            })
