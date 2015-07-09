from pyqtgraph.Qt import QtGui, QtCore
import numpy as np
import pyqtgraph as pg
from pyqtgraph.ptime import time
import serial
 
app = QtGui.QApplication([])
 
p = pg.plot()
p.setWindowTitle('live plot from serial')
curve = p.plot()
 
data = [0]
raw=serial.Serial("/dev/tty.usbmodem1411",9600)
#raw.open()
 
def update():
    global curve, data
    line = raw.readline()
    data.append(int(line))
    xdata = np.array(data, dtype='int16')
    curve.setData(xdata)
    app.processEvents()
 
timer = QtCore.QTimer()
timer.timeout.connect(update)
timer.start(0)
 
if __name__ == '__main__':
    import sys
    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
