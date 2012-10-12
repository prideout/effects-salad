#!/bin/env python

import sys

def Main():
    line = sys.stdin.readline()
    switching = True
    count = 0
    lines = []
    while line:
        lineToks = line.strip().split(" ")
        if len(lineToks) == 3:
            try:
                float(lineToks[0])
                if switching:
                    lines.append("cvs.clear();")
                switching = False
                lines.append("cvs.push_back(vec3(" + ",".join(lineToks) + "));")
                count += 1
            except ValueError:
                switching = True
        else:
            if not switching and count > 3:
                for l in lines:
                    print l
                print "_tubes.push_back(Tube());"
                print "_tubes.back().cvs = cvs;"
                print "_tubes.back().radius = .1;"
                print "_tubes.back().uniformWidth = true;"
                print "_tubes.back().Init();"
            switching = True
            lines = []
            count = 0
        line = sys.stdin.readline()
        

if __name__ == "__main__":
    Main()

