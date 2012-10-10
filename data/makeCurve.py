#!/bin/env python

import sys

def Main():
    line = sys.stdin.readline()
    switching = True
    while line:
        lineToks = line.strip().split(" ")
        if len(lineToks) == 3:
            try:
                float(lineToks[0])
                if switching:
                    print "cvs.clear();"
                switching = False
                print "cvs.push_back(vec3(", ",".join(lineToks), "));"
            except ValueError:
                switching = True
        else:
            if not switching:
                print "_tubes.push_back(Tube());"
                print "_tubes.back().cvs = cvs;"
                print "_tubes.back().radius = .1;"
                print "_tubes.back().uniformWidth = true;"
                print "_tubes.back().Init();"
            switching = True
        line = sys.stdin.readline()
        

if __name__ == "__main__":
    Main()

