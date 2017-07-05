import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

class WavePlot:
	def __init__(self, loadedData):
		self.rawData = loadedData
		self.extractNumChannel()

	def extractNumChannel(self):
		totalSamplePoints = len(self.rawData.index)
		self.nonZero = 0
		for i in range(10):
			if not np.all(self.rawData.ix[:,i]==0):
				self.nonZero += 1
		self.period = float(1.0 / (3000000.0 / (14.0*(self.nonZero+2.0))))
		print "Period detected : ", self.period

	def updateChannelCount(self, channelSel):
		self.channelCount = 0
		for i in range(len(channelSel)):
			if(channelSel[i].get() == 1):
				self.channelCount += 1
		print "Updated channel count: ", self.channelCount

	def plotSelected(self, channelSel):
		print "Plotting.."
		self.updateChannelCount(channelSel)
		channels = []
		count = 0
		for i in range(len(channelSel)):
			if(channelSel[i].get() == 1):
				yValues = self.rawData.ix[:,i] / 256.0 * 1.2
				xValues = yValues.index*self.period
				if not channels:
					channels.append(plt.subplot(self.channelCount, 1, count+1))
				else:
					channels.append(plt.subplot(self.channelCount, 1, count+1, sharex=channels[0]))
				plt.ylabel('Channel ' + str(i+1) + '(V)')
				plt.grid(linestyle=':')
				plt.ylim(0, 1.2)
				plt.plot(xValues, yValues)
				count += 1
				plt.subplots_adjust(left=0.03, bottom=0.04, right=0.99, top=0.99, wspace=0, hspace=0.50)
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		print "Opening plot!"
		plt.xlabel("Time in seconds")
		plt.show()

	def plotAll(self):
		print "Plotting all channels.."
		channels = []
		for i in range(10):
			yValues = self.rawData.ix[:,i] / 256.0 * 1.2
			xValues = yValues.index*self.period
			if not channels:
				channels.append(plt.subplot(10, 1, i+1))
			else:
				channels.append(plt.subplot(10, 1, i+1, sharex=channels[0]))
			plt.ylabel('Channel ' + str(i+1) + ' (V)')
			plt.ylim(0, 1.2)
			plt.grid(linestyle=':')
			plt.plot(xValues, yValues)
			plt.subplots_adjust(left=0.03, bottom=0.04, right=0.99, top=0.99, wspace=0, hspace=0.50)
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		print "Opening plot!"
		plt.xlabel("Time in seconds")
		plt.show()

	def plotDiffData(self, diffSel, chnA, chnB):
		print "Plotting differential channels..."
		self.updateChannelCount(diffSel)
		channels = []
		for i in range(5):
			if(diffSel[i].get() == 1):
				yValues = ((self.rawData.ix[:,chnA[i].current()]) - (self.rawData.ix[:,chnB[i].current()])) / 256.0 * 1.2
				xValues = yValues.index*self.period
				if not channels:
					channels.append(plt.subplot(self.channelCount, 1, i+1))
				else:
					channels.append(plt.subplot(self.channelCount, 1, i+1, sharex=channels[0]))
				plt.ylabel('Channel ' + str(chnA[i].current()+1) + ' minus ' + str(chnB[i].current()+1) + ' (V)')
				plt.ylim(-1.2, 1.2)
				plt.grid(linestyle=':')
				plt.plot(xValues, yValues)
				plt.subplots_adjust(left=0.03, bottom=0.04, right=0.99, top=0.99, wspace=0, hspace=0.50)
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		print "Opening plot!"
		plt.xlabel("Time in seconds")
		plt.show()

	def plotThreshold(self, preThreshold, postThreshold, moreless, maxSpikes, threshold, channelSelectedA, channelSelectedB):
		print "Plotting Threshold channels..."
		timeFrameMS = postThreshold - preThreshold
		numPreThreshold =  int(preThreshold/(self.period*1000.0))
		numPostThreshold = int(postThreshold/(self.period*1000.0))
		xValues = []
		for i in range(numPostThreshold-numPreThreshold+1):
			xValues.append(i*self.period)
		if channelSelectedB < 0:
			yValues_all = self.rawData.ix[:,channelSelectedA] / 256.0 * 1.2
		else:
			yValues_all = (self.rawData.ix[:,channelSelectedA] - self.rawData.ix[:,channelSelectedB]) / 256.0 * 1.2
		self.start = 0-numPreThreshold
		for i in range(maxSpikes):
			yValues = self.extractData(yValues_all, numPreThreshold, numPostThreshold, threshold, moreless)
			if not yValues.empty and len(xValues) == len(yValues):
				plt.plot(xValues, yValues)
			else:
				break
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		print "Opening plot!"
		plt.subplots_adjust(left=0.03, bottom=0.04, right=0.99, top=0.99, wspace=0, hspace=0.50)
		plt.ylabel("Volts")
		plt.xlabel("Time in seconds")
		plt.grid(linestyle=':')
		plt.show()

	def extractData(self, yValues_all, numPreThreshold, numPostThreshold, threshold, moreLess):
		print "Extracting data..."
		emptyarray = pd.DataFrame({'A' : []})
		if moreLess == 0:
			for i in range(self.start, len(yValues_all)):
				if yValues_all[i] > threshold:
					self.start = i+numPostThreshold
					return yValues_all[i+numPreThreshold:i+numPostThreshold+1]
		else:
			for i in range(self.start, len(yValues_all)):
				if yValues_all[i] < threshold:
					self.start = i+numPostThreshold
					return yValues_all[i+numPreThreshold:i+numPostThreshold+1]
		return emptyarray

	def plotFFT(self, channelFFTSel, baselineStart, baselineEnd, signalStart, signalEnd):
		print "Plotting.."
		self.updateChannelCount(channelFFTSel)
		channels = []
		count = 0
		for i in range(len(channelFFTSel)):
			if(channelFFTSel[i].get() == 1):
				if not channels:
					channels.append(plt.subplot(self.channelCount, 1, count+1))
				else:
					channels.append(plt.subplot(self.channelCount, 1, count+1, sharex=channels[0], sharey=channels[0]))
				#Baseline plot
				yValues = self.rawData.ix[:,i] / 256.0 * 1.2
				m = np.fft.fft(yValues[(int)(baselineStart/self.period):(int)(baselineEnd/self.period)])
				P2 = abs(m/m.size)
				P1 = P2[2:-m.size/2]
				P1[2:-1] = 2 * P1[2:-1]
				xValues = np.fft.fftfreq(P1.size, self.period)
				plt.plot(xValues, P1)
				#Baseline plot
				#FFT plot
				m = np.fft.fft(yValues[(int)(signalStart/self.period):(int)(signalEnd/self.period)])
				P2 = abs(m/m.size)
				P1 = P2[2:-(m.size/2)]
				P1[2:-1] = 2 * P1[2:-1]
				xValues = np.fft.fftfreq(P1.size, self.period)
				plt.plot(xValues, P1)
				
				baselineLegend = mpatches.Patch(color='blue', label='Baseline FFT')
				signalLegend = mpatches.Patch(color='orange', label='Signal FFT')
				plt.legend(handles=[baselineLegend, signalLegend])
				plt.ylabel('Channel ' + str(i+1) + ' FFT Analysis')
				plt.grid(linestyle=':')
				plt.xlim(2, 200)
				count += 1
				plt.subplots_adjust(left=0.03, bottom=0.04, right=0.99, top=0.99, wspace=0, hspace=0.50)
				#FFT plot
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		print "Opening plot!"
		plt.xlabel("Frequency in Hertz")
		plt.show()