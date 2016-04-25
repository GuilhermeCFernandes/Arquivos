import re

expr = re.compile(r"[^\>]+\>([^\<]+)\<");
f = open("output.txt", "r")

while True:
    l = f.read()
    if not l:
        break
    m = re.match(expr, l)
    if m:
        print(m.group(1))
