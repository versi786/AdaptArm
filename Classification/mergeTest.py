import sys

if(len(sys.argv) != 4):
	print 'Usage python mergeTest.py <input file 0> <input file 1> <output file> '
	sys.exit(0)

readArr = [open(sys.argv[1], 'r'), open(sys.argv[2], 'r')];
write = open(sys.argv[3], 'w');

for i in xrange(len(readArr)):
	read = readArr[i]
	index = str(i);
	for line in read:
		try:
			string = index + ", " + str(int(line)) + "\r\n"
			write.write(string);
		except Exception, e:
			print e
	read.close()
write.close()

