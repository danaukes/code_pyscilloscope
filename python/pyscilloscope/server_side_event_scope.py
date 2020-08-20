#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Aug  4 08:19:14 2020

@author: danaukes
"""


# Import libraries
import numpy
import PyQt5.Qt as qt
import PyQt5.QtGui as pg
import PyQt5.QtCore as pc
# import PyQt5.QtApp as qa
import PyQt5.QtWidgets as pw
# from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
# import serial

from sseclient import SSEClient 

class ServerSideEventScope(object):

    def __init__(self,url,window_width=300,data_width = 500):
    
        self.app = qt.QApplication([])

        self.url = url
        self.window_width = window_width
        self.data_width = data_width
 
        self.win = pg.GraphicsWindow(title="Pyscilloscope")
        p = self.win.addPlot(title="Time vs. Voltage")
        self.curve = p.plot()
        
        self.Xm = numpy.linspace(0,0,self.data_width)
        self.ptr = -self.window_width

        self.messages = SSEClient(self.url)

    def strip_special(self,s):
        substring = 'data":"'
        ii = s.find(substring)
        if ii>0:
            s = s[(ii+len(substring)):]
    
        substring = '","'
        ii = s.find(substring)
        if ii>0:
            s = s[:ii]
        return s
                       
    def update(self,msg):
        self.msg = msg
        data_s = self.strip_special(self.msg.data)
        self.values = [int(item) for item in data_s.split(',') if item != '']
        self.values_a = numpy.array(self.values)
        l = len(self.values_a)
        if l>0:
            self.values_a=self.values_a*3.3/4095
            self.Xm[:-l] = self.Xm[l:]
            self.Xm[-l:] = self.values_a[:self.data_width] # vector containing the instantaneous values      
            self.ptr += l                              # update x position for displaying the curve
            self.curve.setData(self.values_a)                     # set the curve with this data
            self.curve.setPos(0,0)                   # set x position in the graph to 0
            qt.QApplication.processEvents()    # you MUST process the plot now
            
    def run(self):
        for msg in self.messages: 
            self.update(msg)

if __name__=='__main__':
    
    pscope = ServerSideEventScope('https://api.particle.io/v1/devices/events?access_token=d335bc89d666834185edb810cd21a9ded2627613')
    pscope.run()
    
    qt.QApplication.exec_() # you MUST put this at the end
