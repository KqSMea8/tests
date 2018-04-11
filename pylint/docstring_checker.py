"""Checker for unneccessary use of str() in format() arguments."""

import six
import astroid

from pylint.checkers import BaseChecker, utils
from pylint.interfaces import IAstroidChecker

def register(linter):
    """Register checkers."""
    linter.register_checker(DocstringChecker(linter))

class DocstringChecker(BaseChecker):
    __implements__ = (IAstroidChecker,)

    POSITIONAL_MESSAGE_ID = 'str-used-on-positional-format-argument'
    KEYWORD_MESSAGE_ID = 'str-used-on-keyword-format-argument'

    name = 'doc-string-checker'
    symbol = "doc-string"
    priority = -1
    msgs = {'W9001': ('One line doc string on > 1 lines',
                      symbol+"-one-line",
                      'Used when a short doc string is on multiple lines'),
            'W9002': ('Doc string does not end with "." period',
                      symbol+"-end-with",
                      'Used when a doc string does not end with a period'),
            'W9003': ('All args with their types must be mentioned in doc string',
                      symbol+"-with-all-args",
                      'Used when not all arguments are in the doc string '),
            'W9004': ('triple quotes',
                      symbol+"-triple-quotes",
                      'Used when doc string does not use """'),
            'W9005': ('Missing docstring or docstring is too shorter',
                      symbol+"-missing",
                      'Add docstring longer >=10'),
            'W9006': ('docstring indent error, use 4 space for indent',
                      symbol+"-indent-error",
                      'Use 4 space for indent'),
            }
    options = ()

    def visit_functiondef(self, node):
        self._check_doc_string(node)
        self.all_args_in_doc(node)

    def visit_module(self, node):
        self._check_doc_string(node)

    def visit_classdef(self, node):
        self._check_doc_string(node)

    def _check_doc_string(self, node):
        self.missing_doc_string(node)
        self.one_line_one_one_line(node)
        self.has_period(node)
        self.triple_quotes(node)
        self.indent_style(node)

    def missing_doc_string(self, node):
        if len(node.body) <= 10:
            return True

        if node.doc is None or len(node.doc) < 10:
            self.add_message('W9005', node=node, line=node.tolineno)
        return False

    # FIXME(gongwb): give the docstring line-no
    def indent_style(self, node, indent=4):
        """check doc string indent style"""
        if node.doc is None:
            return True

        doc = node.doc
        lines = doc.splitlines()

        for l in lines:
            print l
            cur_indent = len(l) - len(l.lstrip())
            if cur_indent % indent != 0:
                self.add_message('W9006', node=node, line=node.fromlineno)
                return False

        return True

    def one_line_one_one_line(self,node):
        """One line docs (len < 80) are on one line"""

        doc = node.doc
        if doc is None:
            return True

        if len(doc) > 80: 
            return True
        elif sum(doc.find(nl) for nl in ('\n', '\r', '\n\r')) == -3: 
            return True
        else:
            self.add_message('W9001', node=node, line=node.tolineno)
            return False

        return True

    def has_period(self,node):
        """Doc ends in a period"""
        if node.doc is None:
            return True

        if not node.doc.strip().endswith('.'):
            self.add_message('W9002', node=node, line=node.tolineno)
            return False

        return True

    def _parse(self, node):
        from collections import defaultdict
        d = defaultdict(list)

        lines = node.doc.splitlines()
        state = "others"
        for l in lines:
            c = l.strip()
            if len(c) <= 0:
                continue

            print c, state
            if c.startswith("Args:"):
                state = "Args"
                continue
            elif c.startswith("Returns:"):
                state = "Returns"
                continue
            elif c.startswith("Raises:"):
                state = "Raises"
                continue
            elif c.startswith("Examples:"):
                state = "Examples"
                continue
            else:
                state = "others"

            d[state].append(c)

        return d

    def _arg_with_type(self, d):
        import re

        args={} #arg_name->arg_type
        for t in d['Args']:
            m = re.search('([A-Za-z0-9]+)\s?(\(.+\)):', t)
            if m:
                args[m.group[1]] = m.group[2]

        return args


    def all_args_in_doc(self,node):
        """All function arguments are mentioned in doc"""
        if len(node.body) <= 10:
            return True

        if not node.doc:
            return True

        args=[]
        for arg in node.args.get_children():
            if (not isinstance(arg, astroid.AssignName)) \
                or arg.name == "self":
                    continue
            args.append(arg.name)

        if len(args) <= 0:
            return True

        d = self._parse(node)
        print d
        if len(args) > 0 and len(d['Args']) <= 0:
            self.add_message('W9003', node=node, line=node.tolineno)
            return False

        parsed_args = self._arg_with_type(d)
        print parsed_args
        for t in args:
            if t not in parsed_args:
                self.add_message('W9003', node=node, line=node.tolineno)
                return False

        return True

    def triple_quotes(self,node): #This would need a raw checker to work b/c the AST doesn't use """
        """Doc string uses tripple quotes"""
        if node.doc is None:
            return True

        doc = node.doc.strip()
        if doc.endswith('"""') and doc.startswith('"""'): 
            return True
        else: 
            self.add_message('W9004', node=node, line=node.tolineno)
            return False

        return True
