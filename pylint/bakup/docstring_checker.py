import astroid

from pylint import checkers
from pylint import interfaces
from pylint.checkers import utils


class UselessReturnChecker(checkers.BaseChecker):
    __implements__ = interfaces.IAstroidChecker

    name = 'doc_string_checker'

    msgs = {'W9001': ('One line doc string on >1 lines',
                     'Used when a short doc string is on multiple lines'),
            'W9002': ('Doc string does not end with "." period',
                     ('Used when a doc string does not end with a period')),
           }

    """
    name = 'useless-return'

    msgs = {
        'R2119': ("Useless return at end of function or method",
                  'useless-return',
                  'Emitted when a bare return statement is found at the end of '
                  'function or method definition'
                  ),
        }
     """



    @utils.check_messages('useless-return')
    def visit_functiondef(self, node):
        """
            Checks for presence of return statement at the end of a function
            "return" or "return None" are useless because None is the default
            return type if they are missing
        """
        # if the function has empty body then return
        if not node.body:
            return

        last = node.body[-1]
        if isinstance(last, astroid.Return):
            # e.g. "return"
            if last.value is None:
                self.add_message('useless-return', node=node)
            # e.g. "return None"
            elif isinstance(last.value, astroid.Const) and (last.value.value is None):
                self.add_message('useless-return', node=node)


def register(linter):
    """required method to auto register this checker"""
    linter.register_checker(UselessReturnChecker(linter))

