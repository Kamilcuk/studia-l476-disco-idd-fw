#!/bin/python3

import sys
import time
import matplotlib.pyplot as plt
import collections
import re
import fileinput
import numpy as np
import traceback
import select

class plot:
    def __init__(self):
        plt.ion()
        self.fig = plt.figure()
        self.ax = self.fig.add_subplot(111)
        self.ax.set_title("Current vs time")
        self.ax.set_xlabel("Time [seconds]")
        self.ax.set_ylabel("Current [A]")
        self.ln, = self.ax.plot([], [])
        self.cnt = 0
    def replot(self, xdata, ydata):
        self.ln.set_data(xdata, ydata)
        if self.cnt % 100 == 0:
            self.cnt = 0
            self.ax.relim()
            self.ax.autoscale_view(True,True,True)
        self.fig.canvas.draw()
        self.fig.canvas.start_event_loop(0.00001)
        #plt.pause(0.1)
    def show(self):
        plt.show(block=True)

def main():
    p = plot()
    times = []
    currents = []
    while 1:
        try: 
            line = sys.stdin.readline()
            if not select.select([sys.stdin,],[],[],0.0)[0]:
                print("No data")
            t,_,_,c,_ = line.split()
        except (ValueError, UnicodeDecodeError):
            print("%s" % line)
            continue
        times.append(float(t))
        currents.append(float(c))
        p.replot(times, currents)
    p.show()

if __name__ == "__main__":
    sys.exit(main())
	
