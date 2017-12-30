import sys
lines = [line.rstrip('\n') for line in open(sys.argv[1])]
for l in lines:
    print "<a href=http://www.iciba.com/{0}>{1}</href><br>".format(l,l)
