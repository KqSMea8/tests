import astroid

from pylint.checkers import BaseChecker
from pylint.interfaces import IAstroidChecker

class DocStringChecker(BaseChecker):
    """
    PyLint AST based checker to eval compliance with PEP 257-ish conventions.
    """
    __implements__ = IAstroidChecker

    name = 'doc_string_checker'
    priority = -1
    msgs = {
        'C9910': (
            "format argument #%d should be called without str()",
            "123",
            "format() already converts its arguments to strings",
        ),
        'C9911': (
            "format argument '%s' should be called without str()",
            "346",
            "format() already converts its arguments to strings",
        ),        
        }

    """
    msgs = {'C9901': ('One line doc string on >1 lines',
                     ('Used when a short doc string is on multiple lines')),
            'C9902': ('One line doc string on >1 lines',
                     ('Used when a short doc string is on multiple lines')),
           }
    """
    options = (
        (
            'ignore-ints',
            {
                'default': False, 'type': 'yn', 'metavar' : '<y_or_n>',
                'help': 'Allow returning non-unique integers',
            }
        ),
    )

    def visit_function(self, node):
        if node.doc: self._check_doc_string(node)

    def visit_module(self, node):
        if node.doc: self._check_doc_string(node)

    def visit_class(self, node):
        if node.doc: self._check_doc_string(node)

    def _check_doc_string(self, node):
        self.one_line_one_one_line(node)
        self.has_period(node)
        self.all_args_in_doc(node)

    def one_line_one_one_line(self,node):
        """One line docs (len < 80) are on one line"""
        doc = node.doc
        if len(doc) > 80: return True
        elif sum(doc.find(nl) for nl in ('\n', '\r', '\n\r')) == -3: return True
        else:
            self.add_message('W9001', node=node, line=node.tolineno)

    def has_period(self,node):
        """Doc ends in a period"""
        if not node.doc.strip().endswith('.'):
            self.add_message('W9002', node=node, line=node.tolineno)

    def all_args_in_doc(self,node):
        """All function arguments are mentioned in doc"""
        if not hasattr(node, 'argnames'): return True
        for arg in node.argnames:
            if arg != 'self' and arg in node.doc: continue
            else: break
        else: return True
        self.add_message('W9003', node=node, line=node.tolineno)

    def triple_quotes(self,node): #This would need a raw checker to work b/c the AST doesn't use """
        """Doc string uses tripple quotes"""
        doc = node.doc.strip()
        if doc.endswith('"""') and doc.startswith('"""'): return True
        else: self.add_message('W9004', node=node, line=node.tolineno)

def register(linter):
    """required method to auto register this checker"""
    linter.register_checker(DocStringChecker(linter))
