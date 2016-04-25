import re
import os

expr = re.compile(r"[^\"]*\"([^\"]+)\"")
expr2 = re.compile(r".*\<span class\=\"itemprop\" itemprop\=\"genre\"\>([^\<]+)")

def getGenre(f):
    for line in f:
        m = re.match(expr2, line)
        if m:
            return m.group(1)
    return None

with open("links.txt", "r") as f:
    for line in f:
        m = re.match(expr, line)
        if m:
            os.system("curl \"http://www.imdb.com" + m.group(1) + "\" > tmp.html")
            with open("tmp.html") as f2:
                print(getGenre(f2))
