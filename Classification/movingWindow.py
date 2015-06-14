#!/usr/bin/python
import sys
#import serial
import math
from scipy.fftpack import fft

# length = 60
# sampFreq = 10
# winSize = 3
# winDisplacement = 1.5

def numWindows(dataLength, sampFreq, winLen, winDisplacement):
	numWindows = ((((dataLength/sampFreq)- winLen)/winDisplacement) + 1)
	return int(numWindows)

def MWF(data, dataLength, sampFreq, winLen, winDisplacement, func):
	l = []
	nw = numWindows(dataLength, sampFreq, winLen, winDisplacement)
	x = 0
	nw = int(round(nw))
	for j in xrange(0, nw):
		l.append(func(data[x:x + winLen]))
		x += winDisplacement
	return l

def numWindowsCount(samples, winLen, winDisplacement):
	numWindows = ((((samples)- winLen)/winDisplacement) + 1)
	return numWindows

def MWFCount(data, samples, winLen, winDisplacement, func):
	l = []
	nw = numWindowsCount(samples, winLen, winDisplacement)
	x = 0
	for j in xrange(0, nw):
		l.append(func(data[x:x + winLen]))
		x += winDisplacement
	return l

#nothing
def make_nothing():
	return lambda x: x

#Max
def make_max():
	return lambda x: max(x)

#Min
def make_min():
	return lambda x: min(x)

#range
def make_range():
	return lambda x: max(x) - min(x)

#Entropy
def entropy(data):
    val_freq     = {}
    data_entropy = 0.0

    # Calculate the frequency of each of the values in the target attr
    for record in data:
        if (val_freq.has_key(record)):
            val_freq[record] += 1.0
        else:
            val_freq[record]  = 1.0

    # Calculate the entropy of the data for the target attribute
    for freq in val_freq.values():
        data_entropy += (-freq/len(data)) * math.log(freq/len(data), 2) 
    return data_entropy
def make_entropy():
	return lambda x: entropy(x)
#Line length Avg. Absolute distance between points
def distance(lst):
	total = 0
	for i in xrange(len(lst) - 1):
		temp = abs(lst[i] - lst[i+1])
		total += temp
	return total/(len(lst) - 1)


def make_distance():
	return lambda x: distance(x)


#FFT
def FFT_intensity(data):
	x = fft(data)
	sum = 0.0
	for y in x:
		sum += math.sqrt(math.pow(y.real,2) + math.pow(y.imag,2))
	sum /= len(x)
	return sum

def make_FFT_intensity():
	return lambda x: FFT_intensity(x)



# data = [1,2,3,4,5,6]
# sampFreq = 1
# winLen = 3
# winDisplacement = 1

# nothing_func = make_nothing()
# nothingList = MWF(data, len(data), sampFreq, winLen, winDisplacement, nothing_func)
# print 'nothing: ', nothingList


# max_func = make_max()
# maxList = MWF(data, len(data), sampFreq, winLen, winDisplacement, max_func)
# print 'max: ', maxList

# min_func = make_min()
# minList = MWF(data, len(data), sampFreq, winLen, winDisplacement, min_func)
# print 'min: ', minList

# range_func = make_range()
# rangeList = MWF(data, len(data), sampFreq, winLen, winDisplacement, range_func)
# print 'range: ', rangeList

# lineLegnth_func = make_distance()
# distanceList = MWF(data, len(data), sampFreq, winLen, winDisplacement, lineLegnth_func)
# print 'distance: ', distanceList

# data = []
# j = 0
# for i in xrange(32):
# 	data.append(0)
# 	data.append(1)
# 	data.append(0)
# 	data.append(-1)
# print data 
# print len(data)
# FFT_intensity(data)
