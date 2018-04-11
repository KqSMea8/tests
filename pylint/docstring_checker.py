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
    symbol = "invalid-doc-string-format"
    priority = -1
    msgs = {'W9001': ('One line doc string on > 1 lines',
                      symbol+"-one-line",
                      'Used when a short doc string is on multiple lines'),
            'W9002': ('Doc string does not end with "." period',
                      symbol+"-end-with",
                      'Used when a doc string does not end with a period'),
            'W9003': ('Not all args mentioned in doc string',
                      symbol+"-with-all-args",
                      'Used when not all arguments are in the doc string '),
            'W9004': ('triple quotes',
                      symbol+"-triple-quotes",
                      'Used when doc string does not use """'),
            'W9005': ('Missing docstring or docstring is too shorter',
                      symbol+"-miss-docstring",
                      'Add docstring longer >=10"""'),
            }
    options = ()

    def visit_functiondef(self, node):
        self._check_doc_string(node)
        self.all_args_in_doc(node)


    def visit_module(self, node):
        self._check_doc_string(node)

    def visit_classdef(self, node):
        self._check_doc_string(node)
        self.all_args_in_doc(node)

    def _check_doc_string(self, node):
        if len(node.body) <= 10:
            return

        if node.doc is None or len(node.doc) < 10:
            self.add_message('W9005', node=node, line=node.tolineno)
            return

        self.one_line_one_one_line(node)
        self.has_period(node)
        self.triple_quotes(node)

    def one_line_one_one_line(self,node):
        """One line docs (len < 80) are on one line"""

        doc = node.doc
        if len(doc) > 80: 
            return True
        elif sum(doc.find(nl) for nl in ('\n', '\r', '\n\r')) == -3: 
            return True
        else:
            self.add_message('W9001', node=node, line=node.tolineno)

    def has_period(self,node):
        """Doc ends in a period"""
        if not node.doc.strip().endswith('.'):
            self.add_message('W9002', node=node, line=node.tolineno)

    def all_args_in_doc(self,node):
        """All function arguments are mentioned in doc"""

        if len(node.body) <= 10:
            return True

        args=[]
        for arg in node.args.get_children():
            if not isinstance(arg, astroid.AssignName) 
                or arg.name == "self":
                    continue
            args.append(arg.name)

        if len(args) <= 0:
            return True

        kvs = {}
        lines = node.doc.splitlines()
        for l in lines:
            kv = l.trip().split(":", 1)
            if len(kv) <= 1:
                continue
            kvs[kv[0]] = kvs[kv[1]]

        if "Args:" not in kvs:
            return False

        for arg in args:
            if arg+" (" not in kvs:
                self.add_message('W9003', node=node, line=node.tolineno)
        return False

    def triple_quotes(self,node): #This would need a raw checker to work b/c the AST doesn't use """
        """Doc string uses tripple quotes"""
        doc = node.doc.strip()
        if doc.endswith('"""') and doc.startswith('"""'): 
            return True
        else: 
            self.add_message('W9004', node=node, line=node.tolineno)
