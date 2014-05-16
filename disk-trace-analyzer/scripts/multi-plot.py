#!/usr/bin/env python

import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import csv as csv
import sys as sys
import os as os
import glob as glob

sns.set_style({"font.family": "sans-serif"})

# find all files
f, axes = plt.subplots(3, 6, figsize = (16,8))
plt.subplots_adjust(hspace=0.4)
files = glob.glob("*.csv")
fx = 0
fy = 0
for filename in files:
    print filename
    x0 = [1,2,3,4]
    y0 = [0]*100
    y0_day = []
    maxx = 0
    csvfile = open(filename, 'rb')
    next(csvfile)
    rows = csv.reader(csvfile)
    for row in rows:
        y0[int(row[0])-1] = int(row[1])
        maxx = max(maxx, int(row[0]))
    for i in range(0,4):
        y0_day.append(0)
        for j in range(0,23):
            y0_day[i] += y0[i*24+j]
    #y0 = y0[:maxx]
    #y0 = np.asarray(y0[:maxx])
    #for i in range(1,maxx+1):
    #    x0.append(i)
    #x0 = np.asarray(x0)
    x0 = np.asarray(x0)
    y0_day = np.asarray(y0_day)
    print x0, y0_day
    #axes[fx,fy].plot(x0,y0_day,label=filename) # line plot
    sns.barplot(x0,y0_day, ci=None, hline = 0, ax=axes[fy,fx])
    axes[fy,fx].set_xlabel(filename[9:-4])
    axes[fy,fx].ticklabel_format(axis='y', style='sci',
            scilimits=(2,2))
    if (maxx == 0):
        axes[fy,fx].set_ylim(0,5)
    fx += 1
    if (fx == 6):
        fx = 0
        fy += 1

#plt.yscale('log')
#plt.legend()
#sns.tsplot(gammas, "timepoint", "subject", "ROI", "BOLD signal")
plt.savefig("multi-plot.pdf", format='pdf')
sys.exit(0)
