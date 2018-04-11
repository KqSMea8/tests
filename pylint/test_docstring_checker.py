import docstring_checker
import pylint.testutils
import astroid
import pytest
import sys

class TestDocstring(pylint.testutils.CheckerTestCase):
    CHECKER_CLASS = docstring_checker.DocstringChecker

    def test_one_line(self):
        func_node = astroid.extract_node('''
        def test(): 
            """get 
            news.
            """
            if True:
                return 5
            return 5
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9001' == got[0][0]

    def test_end_with(self):
        func_node = astroid.extract_node('''
        def test(): 
            """get news"""
            if True:
                return 5
            return 5
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9002' == got[0][0]

    def test_args(self):
        func_node = astroid.extract_node('''
        def test(scale, mean): 
            """get news.
            Args:
                scale (int): scale is the number.
            """
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9003' == got[0][0]

    def test_missing(self):
        func_node = astroid.extract_node('''
        def test(): 
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9005' == got[0][0]

    def test_indent(self):
        func_node = astroid.extract_node('''
        def test(): 
            """ get get get get get get get get
              get get get get get get get get.
            """
            pass 
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9006' == got[0][0]

    def test_with_resturns(self):
        func_node = astroid.extract_node('''
        def test(): 
            """get news.
            Args:
                scale (int): scale is the number.
            """
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            return mean
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9007' == got[0][0]

    def test_with_raises(self):
        func_node = astroid.extract_node('''
        def test(): 
            """get news.
            Args:
                scale (int): scale is the number.
            """
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            mean=scale
            raise ValueError('A very specific bad thing happened.')
        ''')

        self.checker.visit_functiondef(func_node)
        got = self.linter.release_messages()
        print got
        assert len(got) == 1
        assert 'W9008' == got[0][0]

