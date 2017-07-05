from Tkinter import *
from tkFileDialog import askopenfilename
import pandas as pd
import matplotlib.pyplot as plt

class ControlPannel:
	def __init__(self, master):
		self.selectLabel = Label(master, text="Select channels")
		self.selectLabel.grid(row=0, column=0)

		self.channelSel = []
		for i in range(10):
			self.channelSel.append(IntVar())
			self.cb = Checkbutton(master, text="Channel " + str(i+1), variable=self.channelSel[i]).grid(row=i+1, column=0)
		self.channelSel.append(IntVar())
		self.cb = Checkbutton(master, text="Sync Pulse Channel", variable=self.channelSel[10]).grid(row=11, column=0)

		self.plotButton = Button(master, text="Plot!", fg="black", command=self.plotRawData).grid(row=13, column=0)
		self.plotAllButton = Button(master, text="Plot All!", fg="black", command=self.plotAllRawData).grid(row=14, column=0)

		Tk().withdraw()
		filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
		self.df = pd.read_csv(filename)
		# self.dfADC = pd.read_csv(filename[:-4]+"ADC.csv")

		#self.differential1Label = Label(master, text="Select 2 Channels for differential mode:").grid(row=0, column=1)
		#self.differential2Label = Label(master, text="(Top channel minus bottom channel)").grid(row=1, column=1)

	def plotRawData(self):
		plt.figure(1)
		self.countChannels()
		channels = []
		count = 0
		for i in range(10):
			if(self.channelSel[i].get() == 1):
				dfy = self.df.ix[:,i] * 0.000000195
				dfx = dfy.index*0.000239
				if not channels:
					channels.append(plt.subplot(self.channelCount+2, 1, count+1))
				else:
					channels.append(plt.subplot(self.channelCount+2, 1, count+1, sharex=channels[0]))
				plt.ylabel('Channel ' + str(i+1) + '(V)')
				plt.ylim(-0.005, 0.005)
				plt.plot(dfx, dfy)
				count += 1

		dfy = self.df.ix[:,10]
		dfx = dfy.index*0.000239
		channels.append(plt.subplot(self.channelCount+2, 1, self.channelCount+1, sharex=channels[0]))
		plt.ylabel("Sync Pulse")
		plt.plot(dfx, dfy)

		dfy = self.df.ix[:,11]
		dfx = dfy.index*0.000239
		channels.append(plt.subplot(self.channelCount+2, 1, self.channelCount+2, sharex=channels[0]))
		plt.ylabel("Frame Marker")
		plt.plot(dfx, dfy)

		# if(self.channelSel[10].get() == 1):
		# 	dfy = self.dfADC.ix[:,0] * 2.5 / 256.0
		# 	dfx = dfy.index*0.00004995
		# 	channels.append(plt.subplot(self.channelCount+1, 1, self.channelCount, sharex=self.channels[0]))
		# 	plt.ylabel("Sync Pulse")
		# 	plt.plot(dfx, dfy)
		plt.subplots_adjust(left=0.04, bottom=0.02, right=0.99, top=0.98, wspace=0, hspace=0.12)
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		plt.show()

	def plotAllRawData(self):
		plt.figure(1)
		# self.countChannels()
		channels = []

		for i in range(10):
			dfy = self.df.ix[:,i] * 0.000000195
			dfx = dfy.index*0.000239
			if not channels:
				channels.append(plt.subplot(12, 1, i+1))
			else:
				channels.append(plt.subplot(12, 1, i+1, sharex=channels[0]))
			# channels.append(plt.subplot(12, 1, i+1, sharex=channels[0]))
			plt.ylabel('Channel ' + str(i+1) + '(V)')
			plt.ylim(-0.005, 0.005)
			plt.plot(dfx, dfy)

		dfy = self.df.ix[:,10]
		dfx = dfy.index*0.000239
		channels.append(plt.subplot(12, 1, 11, sharex=channels[0]))
		plt.ylim(0, 1)
		plt.ylabel("Sync Pulse")
		plt.plot(dfx, dfy)

		dfy = self.df.ix[:,11]
		dfx = dfy.index*0.000239
		channels.append(plt.subplot(12, 1, 12, sharex=channels[0]))
		plt.ylabel("Frame Marker")
		plt.plot(dfx, dfy)
		# dfy = self.dfADC.ix[:,0] * 2.5 / 256.0
		# dfx = dfy.index*0.00004995
		# self.channels.append(plt.subplot(12, 1, 11, sharex=self.channels[0]))
		# plt.ylabel("Sync Pulse")
		# plt.plot(dfx, dfy)
		# plt.subplots_adjust(left=0.04, bottom=0.02, right=0.99, top=0.98, wspace=0, hspace=0.12)
		figManager = plt.get_current_fig_manager()
		figManager.window.showMaximized()
		plt.show()

	def countChannels(self):
		self.channelCount = 0
		for i in range(10):
			if(self.channelSel[i].get() == 1):
				self.channelCount = self.channelCount + 1
		print "Updated channel count: ", self.channelCount

	def __del__(self):
		self.destroy()