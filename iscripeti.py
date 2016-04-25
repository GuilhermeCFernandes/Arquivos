import re

expr = re.compile(r"[^\>]+\>([^\<]+)\<");
f = open("output.txt", "r")

for l in f:
    m = re.match(expr, l)
    if m:
        print(m.group(1))
