
import sys

from PyQt5 import QtCore

sys.path.append(sys.argv[1])

import PyTest.CppLib

mo = PyTest.CppLib.MyObject()

assert(mo.addThree(39) == 42)

assert(mo.addThree([38, 39, 40]) == [41, 42, 43])

assert(mo.addThree("SomeString") == "DefaultSomeStringThree")

assert(mo.findNeedle(["One", "Two", "Three"], "Two") == 1)
assert(mo.findNeedle(["One", "Two", "Three"], "Four") == -1)
assert(mo.findNeedle(["One", "Two", "Three"], "Th") == 2)
assert(mo.findNeedle(["One", "Two", "Three"], "Th", QtCore.Qt.MatchExactly) == -1)

assert(mo.const_parameters(30) == 15)
assert(mo.const_parameters(30, mo) == 10)

assert(mo.qtEnumTest(QtCore.Qt.MatchContains | QtCore.Qt.MatchStartsWith) == 3)
assert(mo.localEnumTest(PyTest.CppLib.MyObject.Val2) == 2)

lfd = PyTest.CppLib.LocalFwdDecl(18)

assert(mo.localFwdDecl(lfd) == 18)

import PyTest.ExternalLib

efd = PyTest.ExternalLib.ExternalFwdDecl(18)

assert(mo.externalFwdDecl(efd) == 18)

assert(mo.localListDecl([1, 5, 7]) == 13)

lfdl = [PyTest.CppLib.LocalFwdDecl(3), PyTest.CppLib.LocalFwdDecl(6)]

assert(mo.localDeclListDecl(lfdl) == 9)

#
# Verify that an enum with attributes can be read.
#
assert(PyTest.CppLib.Foo == 0)
assert(PyTest.CppLib.Bar == 2)

class Reactor(QtCore.QObject):
    def __init__(self, obj):
        QtCore.QObject.__init__(self)
        self.gotPrivateSlotCalledSignal = False
        self.gotProtectedSlotCalledSignal = False
        self.gotPublicSlotCalledSignal = False

        obj.privateSlotCalled.connect(self.react_to_privateSlotCalled)
        obj.protectedSlotCalled.connect(self.react_to_protectedSlotCalled)
        obj.publicSlotCalled.connect(self.react_to_publicSlotCalled)

    def react_to_privateSlotCalled(self):
        self.gotPrivateSlotCalledSignal = True

    def react_to_protectedSlotCalled(self):
        self.gotProtectedSlotCalledSignal = True

    def react_to_publicSlotCalled(self):
        self.gotPublicSlotCalledSignal = True

class Emitter(QtCore.QObject):
    privateTrigger = QtCore.pyqtSignal()
    protectedTrigger = QtCore.pyqtSignal()
    publicTrigger = QtCore.pyqtSignal()

    def __init__(self, obj):
        QtCore.QObject.__init__(self)
        self.privateTrigger.connect(obj.privateSlot1)
        self.protectedTrigger.connect(obj.protectedSlot1)
        self.publicTrigger.connect(obj.publicSlot1)

    def emitSignalForPublic(self):
        self.publicTrigger.emit()

    def emitSignalForPrivate(self):
        self.privateTrigger.emit()

    def emitSignalForProtected(self):
        self.protectedTrigger.emit()

e = Emitter(mo)

r = Reactor(mo)

assert(not r.gotPrivateSlotCalledSignal)
assert(not r.gotProtectedSlotCalledSignal)
assert(not r.gotPublicSlotCalledSignal)

e.emitSignalForPrivate()

assert(r.gotPrivateSlotCalledSignal)

e.emitSignalForProtected()

assert(r.gotProtectedSlotCalledSignal)

e.emitSignalForPublic()

assert(r.gotPublicSlotCalledSignal)

assert(PyTest.CppLib.SomeNS.EnumValueOne == 1)
assert(PyTest.CppLib.SomeNS.EnumValueTwo == 2)

assert(PyTest.CppLib.SomeNS.useEnum() == 1.0)
assert(PyTest.CppLib.SomeNS.useEnum(PyTest.CppLib.SomeNS.EnumValueOne) == 1.0)
assert(PyTest.CppLib.SomeNS.useEnum(PyTest.CppLib.SomeNS.EnumValueTwo) == 2.0)

assert(PyTest.CppLib.SomeNS.customMethod([2, 3, 5]) == 10)

assert(PyTest.CppLib.anotherCustomMethod([2, 3, 5]) == 52)

sdo = PyTest.CppLib.SubdirObject()
assert(sdo.mul(5, 6) == 30)

visible = PyTest.CppLib.Visible()
assert visible.visible_fn()

try:
    assert visible.invisible_fn()
    assert False
except AttributeError as e:
    assert str(e) == "'Visible' object has no attribute 'invisible_fn'"
try:
    invisible = PyTest.CppLib.Invisible()
    assert False
except AttributeError as e:
    assert str(e) == "module 'PyTest.CppLib' has no attribute 'Invisible'"
