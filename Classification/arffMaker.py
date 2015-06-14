import sys
import movingWindow




#MAIN
# collectionTime = 60 #time in seconds
sampFreq = 1000 #Hz
winLenSeconds = 0.25#seconds
winDisplacementSeconds = 0.125 # seconds
classifications = ["rest", "flex"]

# dataLength = collectionTime * sampFreq #samples aka len(data)
winLen = int(sampFreq * winLenSeconds) #samples = frequency * secondsOfWindow
winDisplacement = int(sampFreq * winDisplacementSeconds) #seconds

if(len(sys.argv) != 3):
	print 'Usage: python arffMaker.py <input file> <output file>'
	sys.exit(0)

read = open(sys.argv[1], 'r')
write = open(sys.argv[2], 'w')



#split data into different lists based on classification
data = []
temp = []
classification = 0
for line in read:
	split = line.split(',')
	#print str(split)
	if(len(split) != 2):
		continue
	if classification == int(split[0]):
		temp.append(int(split[1]))
	else:
		classification += 1
		data.append(temp)
		temp = []
		temp.append(int(split[1]))
#append last classification
data.append(temp)
#print 'input file split into different lists\n', data


#write header of arff file
write.write('@RELATION test\r\n')
write.write('\r\n')
write.write('@ATTRIBUTE max NUMERIC\r\n')
write.write('@ATTRIBUTE min NUMERIC\r\n')
write.write('@ATTRIBUTE range NUMERIC\r\n')
write.write('@ATTRIBUTE distance NUMERIC\r\n')
write.write('@ATTRIBUTE entropy NUMERIC\r\n')
#write.write('@ATTRIBUTE FFT_intensity NUMERIC\r\n')
write.write('@ATTRIBUTE class {')
for i in xrange(len(classifications)):
	write.write(classifications[i])
	if i != (len(classifications) - 1):
		write.write(', ')
write.write('}\r\n')
write.write('\r\n')
write.write('@DATA\r\n')



#write and calculate DATA
max_func = movingWindow.make_max()
min_func = movingWindow.make_min()
range_func = movingWindow.make_range()
distance_func = movingWindow.make_distance()
entropy_func = movingWindow.make_entropy()
FFT_intensity_func = movingWindow.make_FFT_intensity()



for i in xrange(len(data)):
	classData = data[i]
	maxList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, max_func)
	minList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, min_func)
	rangeList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, range_func)
	distanceList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, distance_func)
	entropyList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, entropy_func)
	FFT_intensityList = movingWindow.MWFCount(classData, len(classData), winLen, winDisplacement, FFT_intensity_func)
	classification = classifications[i]
	for j in xrange(len(maxList)):
		write.write(
		str(maxList[j])
		+', ' + str(minList[j])
		+', ' + str(rangeList[j])
		+', ' + str(distanceList[j])
		+', ' + str(entropyList[j])
#		+', ' + str(FFT_intensityList[j])
		+', ' + classification
		+ '\r\n')


#close both files
read.close()
write.close()