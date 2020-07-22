# SPDX-FileCopyrightText: 2014 Alex Merry <alex.merry@kde.org>
#
# Based on cmake.py from CMake:
# SPDX-FileCopyrightText: 2000-2013 Kitware Inc., Insight Software Consortium
#
# SPDX-License-Identifier: BSD-3-Clause

import os
import re

# Monkey patch for pygments reporting an error when generator expressions are
# used.
# https://bitbucket.org/birkenfeld/pygments-main/issue/942/cmake-generator-expressions-not-handled
from pygments.lexers import CMakeLexer
from pygments.token import Name, Operator
from pygments.lexer import bygroups
CMakeLexer.tokens["args"].append(('(\\$<)(.+?)(>)',
                                  bygroups(Operator, Name.Variable, Operator)))

# Monkey patch for sphinx generating invalid content for qcollectiongenerator
# https://bitbucket.org/birkenfeld/sphinx/issue/1435/qthelp-builder-should-htmlescape-keywords
from sphinx.util.pycompat import htmlescape
from sphinx.builders.qthelp import QtHelpBuilder
old_build_keywords = QtHelpBuilder.build_keywords
def new_build_keywords(self, title, refs, subitems):
  old_items = old_build_keywords(self, title, refs, subitems)
  new_items = []
  for item in old_items:
    before, rest = item.split("ref=\"", 1)
    ref, after = rest.split("\"")
    if ("<" in ref and ">" in ref):
      new_items.append(before + "ref=\"" + htmlescape(ref) + "\"" + after)
    else:
      new_items.append(item)
  return new_items
QtHelpBuilder.build_keywords = new_build_keywords


from docutils.parsers.rst import Directive, directives
from docutils.transforms import Transform
try:
    from docutils.utils.error_reporting import SafeString, ErrorString
except ImportError:
    # error_reporting was not in utils before version 0.11:
    from docutils.error_reporting import SafeString, ErrorString

from docutils import io, nodes

from sphinx.directives import ObjectDescription
from sphinx.domains import Domain, ObjType
from sphinx.roles import XRefRole
from sphinx.util.nodes import make_refnode
from sphinx import addnodes

class ECMModule(Directive):
    required_arguments = 1
    optional_arguments = 0
    final_argument_whitespace = True
    option_spec = {'encoding': directives.encoding}

    def __init__(self, *args, **keys):
        self.re_start = re.compile(r'^#\[(?P<eq>=*)\[\.rst:$')
        Directive.__init__(self, *args, **keys)

    def run(self):
        settings = self.state.document.settings
        if not settings.file_insertion_enabled:
            raise self.warning('"%s" directive disabled.' % self.name)

        env = self.state.document.settings.env
        rel_path, path = env.relfn2path(self.arguments[0])
        path = os.path.normpath(path)
        encoding = self.options.get('encoding', settings.input_encoding)
        e_handler = settings.input_encoding_error_handler
        try:
            settings.record_dependencies.add(path)
            f = io.FileInput(source_path=path, encoding=encoding,
                             error_handler=e_handler)
        except UnicodeEncodeError as error:
            raise self.severe('Problems with "%s" directive path:\n'
                              'Cannot encode input file path "%s" '
                              '(wrong locale?).' %
                              (self.name, SafeString(path)))
        except IOError as error:
            raise self.severe('Problems with "%s" directive path:\n%s.' %
                      (self.name, ErrorString(error)))
        raw_lines = f.read().splitlines()
        f.close()
        rst = None
        lines = []
        for line in raw_lines:
            if rst is not None and rst != '#':
                # Bracket mode: check for end bracket
                pos = line.find(rst)
                if pos >= 0:
                    if line[0] == '#':
                        line = ''
                    else:
                        line = line[0:pos]
                    rst = None
            else:
                # Line mode: check for .rst start (bracket or line)
                m = self.re_start.match(line)
                if m:
                    rst = ']%s]' % m.group('eq')
                    line = ''
                elif line == '#.rst:':
                    rst = '#'
                    line = ''
                elif rst == '#':
                    if line == '#' or line[:2] == '# ':
                        line = line[2:]
                    else:
                        rst = None
                        line = ''
                elif rst is None:
                    line = ''
            lines.append(line)
        if rst is not None and rst != '#':
            raise self.warning('"%s" found unclosed bracket "#[%s[.rst:" in %s' %
                               (self.name, rst[1:-1], path))
        self.state_machine.insert_input(lines, path)
        return []

class _ecm_index_entry:
    def __init__(self, desc):
        self.desc = desc

    def __call__(self, title, targetid):
        return ('pair', u'%s ; %s' % (self.desc, title), targetid, 'main')

_ecm_index_objs = {
    'manual':      _ecm_index_entry('manual'),
    'module':      _ecm_index_entry('module'),
    'find-module': _ecm_index_entry('find-module'),
    'kde-module':  _ecm_index_entry('kde-module'),
    'toolchain':   _ecm_index_entry('toolchain'),
    }

def _ecm_object_inventory(env, document, line, objtype, targetid):
    inv = env.domaindata['ecm']['objects']
    if targetid in inv:
        document.reporter.warning(
            'ECM object "%s" also described in "%s".' %
            (targetid, env.doc2path(inv[targetid][0])), line=line)
    inv[targetid] = (env.docname, objtype)

class ECMTransform(Transform):

    # Run this transform early since we insert nodes we want
    # treated as if they were written in the documents.
    default_priority = 210

    def __init__(self, document, startnode):
        Transform.__init__(self, document, startnode)
        self.titles = {}

    def parse_title(self, docname):
        """Parse a document title as the first line starting in [A-Za-z0-9<]
           or fall back to the document basename if no such line exists.
           Return the title or False if the document file does not exist.
        """
        env = self.document.settings.env
        title = self.titles.get(docname)
        if title is None:
            fname = os.path.join(env.srcdir, docname+'.rst')
            try:
                f = open(fname, 'r')
            except IOError:
                title = False
            else:
                for line in f:
                    if len(line) > 0 and (line[0].isalnum() or line[0] == '<'):
                        title = line.rstrip()
                        break
                f.close()
                if title is None:
                    title = os.path.basename(docname)
            self.titles[docname] = title
        return title

    def apply(self):
        env = self.document.settings.env

        # Treat some documents as ecm domain objects.
        objtype, sep, tail = env.docname.rpartition('/')
        make_index_entry = _ecm_index_objs.get(objtype)
        if make_index_entry:
            title = self.parse_title(env.docname)
            # Insert the object link target.
            targetid = '%s:%s' % (objtype, title)
            targetnode = nodes.target('', '', ids=[targetid])
            self.document.insert(0, targetnode)
            # Insert the object index entry.
            indexnode = addnodes.index()
            indexnode['entries'] = [make_index_entry(title, targetid)]
            self.document.insert(0, indexnode)
            # Add to ecm domain object inventory
            _ecm_object_inventory(env, self.document, 1, objtype, targetid)

class ECMObject(ObjectDescription):

    def handle_signature(self, sig, signode):
        # called from sphinx.directives.ObjectDescription.run()
        signode += addnodes.desc_name(sig, sig)
        return sig

    def add_target_and_index(self, name, sig, signode):
        targetid = '%s:%s' % (self.objtype, name)
        if targetid not in self.state.document.ids:
            signode['names'].append(targetid)
            signode['ids'].append(targetid)
            signode['first'] = (not self.names)
            self.state.document.note_explicit_target(signode)
            _ecm_object_inventory(self.env, self.state.document,
                                    self.lineno, self.objtype, targetid)

        make_index_entry = _ecm_index_objs.get(self.objtype)
        if make_index_entry:
            self.indexnode['entries'].append(make_index_entry(name, targetid))

class ECMXRefRole(XRefRole):

    # See sphinx.util.nodes.explicit_title_re; \x00 escapes '<'.
    _re = re.compile(r'^(.+?)(\s*)(?<!\x00)<(.*?)>$', re.DOTALL)
    _re_sub = re.compile(r'^([^()\s]+)\s*\(([^()]*)\)$', re.DOTALL)

    def __call__(self, typ, rawtext, text, *args, **keys):
        # CMake cross-reference targets may contain '<' so escape
        # any explicit `<target>` with '<' not preceded by whitespace.
        while True:
            m = ECMXRefRole._re.match(text)
            if m and len(m.group(2)) == 0:
                text = '%s\x00<%s>' % (m.group(1), m.group(3))
            else:
                break
        return XRefRole.__call__(self, typ, rawtext, text, *args, **keys)

class ECMDomain(Domain):
    """ECM domain."""
    name = 'ecm'
    label = 'ECM'
    object_types = {
        'module':      ObjType('module',      'module'),
        'kde-module':  ObjType('kde-module',  'kde-module'),
        'find-module': ObjType('find-module', 'find-module'),
        'manual':      ObjType('manual',      'manual'),
        'toolchain':   ObjType('toolchain',   'toolchain'),
    }
    directives = {}
    roles = {
        'module':      XRefRole(),
        'kde-module':  XRefRole(),
        'find-module': XRefRole(),
        'manual':      XRefRole(),
        'toolchain':   XRefRole(),
    }
    initial_data = {
        'objects': {},  # fullname -> docname, objtype
    }

    def clear_doc(self, docname):
        to_clear = []
        for fullname, (fn, _) in self.data['objects'].items():
            if fn == docname:
                to_clear.append(fullname)
        for fullname in to_clear:
            del self.data['objects'][fullname]

    def resolve_xref(self, env, fromdocname, builder,
                     typ, target, node, contnode):
        targetid = '%s:%s' % (typ, target)
        obj = self.data['objects'].get(targetid)
        if obj is None:
            # TODO: warn somehow?
            return None
        return make_refnode(builder, fromdocname, obj[0], targetid,
                            contnode, target)

    def get_objects(self):
        for refname, (docname, type) in self.data['objects'].items():
            yield (refname, refname, type, docname, refname, 1)

def setup(app):
    app.add_directive('ecm-module', ECMModule)
    app.add_transform(ECMTransform)
    app.add_domain(ECMDomain)
