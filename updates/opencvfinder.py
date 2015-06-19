

f = open('new','r')
for lines in f:
	print 'opkg install'+lines[10:len(lines)]