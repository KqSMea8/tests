"""Checker for unneccessary use of str() in format() arguments."""

import six

import astroid


from pylint.checkers import BaseChecker, utils
from pylint.interfaces import IAstroidChecker

#from .common import BASE_ID
BASE_ID=99

def register(linter):
    """Register checkers."""
    linter.register_checker(format_str_checker(linter))

class format_str_checker(BaseChecker):
   """
   Checks that the arguments to format() are not str(something)

   Bad:
      "Bad {}".format(str(a))

      "Bad {nope}".format(nope=str(a))

   OK:
      "Good".format(a)

      "Good {yeah}".format(yeah=a)

   """

   __implements__ = (IAstroidChecker,)

   name = 'no-need-to-call-str-on-format-args-checker'

   POSITIONAL_MESSAGE_ID = 'str-used-on-positional-format-argument'
   KEYWORD_MESSAGE_ID = 'str-used-on-keyword-format-argument'
   msgs = {
        'W9910': (
            "format argument #%d should be called without str()",
            POSITIONAL_MESSAGE_ID,
            "format() already converts its arguments to strings",
        ),
        'W9911': (
            "format argument '%s' should be called without str()",
            KEYWORD_MESSAGE_ID,
            "format() already converts its arguments to strings",
        ),        
        }

   @utils.check_messages(POSITIONAL_MESSAGE_ID, KEYWORD_MESSAGE_ID)
   def visit_callfunc(self, node):
       """Called for every function call in the source code."""

       if not isinstance(node.func, astroid.Attribute):
          return
          
       if node.func.attrname != "format":
           # Be gone
           return

       if not self.linter.is_message_enabled(self.POSITIONAL_MESSAGE_ID, line=node.fromlineno):
           return

       # Check the positional args
       index = 0
       for child in node.args:
           index += 1
           if isinstance(child, astroid.Call):
               if child.func.name == "str":
                   self.add_message(self.POSITIONAL_MESSAGE_ID, args=index, node=node)

       if not self.linter.is_message_enabled(self.KEYWORD_MESSAGE_ID, line=node.fromlineno):
           return

       # Check the keyword args
       if not node.keywords:
           return
           
       for child in node.keywords:
            if isinstance(child.value, astroid.Call):
                if child.value.func.name == "str":
                   self.add_message(self.KEYWORD_MESSAGE_ID, args=child.arg, node=node)
                   
       return
