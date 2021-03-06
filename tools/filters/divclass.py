#!/usr/bin/env python

from pandocfilters import toJSONFilter, RawBlock, Div, stringify
import re

def html(x):
    return RawBlock('html', x)

def latex(s):
    return RawBlock('latex', s)

def mk_div(k, v, f, m):
    begin = re.compile("\..*\[")
    end = re.compile("\.\]")
    if k == "Para":
        value = stringify(v)
        res = begin.match(value)
        if type(res) != type(None):
            return html(r'<div class="%s">' % res.group()[1:-1])
        res = end.match(value)
        if type(res) != type(None):
            return html(r'</div>')

if __name__ == "__main__":
    toJSONFilter(mk_div)
