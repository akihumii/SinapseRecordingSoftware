from tkinter import *
from tkinter.ttk import *
from tkFileDialog import askopenfilename
import pandas as pd
from waveplot import WavePlot

class ControlPannel:
	def __init__(self, master):
		self.createLayout(master)

		Tk().withdraw()
		filename = askopenfilename() # show an "Open" dialog box and return the path to the selected file
		self.df = pd.read_csv(filename)
		for i in range(len(filename)-1, len(filename)-26, -1):
			if filename[i] == "/":
				titlename = "Neutrino Data Analyzer - " + filename[i+1:]
		master.title(titlename)

	def createLayout(self, master):
		notebook = ttk.Notebook(master)
		
		frame1 = Frame(notebook)
		frame2 = Frame(notebook)
		frame3 = Frame(notebook)
		frame4 = Frame(notebook)

		self.selChannel_init(frame1)
		self.diffChannel_init(frame2)
		self.threshold_init(frame3)
		self.FFT_init(frame4)

		notebook.add(frame1, text='Single End Mode')
		notebook.add(frame2, text='Differential Mode')
		notebook.add(frame3, text='Threshold Mode')
		notebook.add(frame4, text='FFT Mode')

		notebook.pack()

	def selChannel_init(self, master):
		selectLabel = Label(master, text="Select channels")
		selectLabel.grid(row=0, column=0)

		self.channelSel = []
		for i in range(10):
			self.channelSel.append(IntVar())
			cb = Checkbutton(master, text="Channel " + str(i+1), variable=self.channelSel[i]).grid(row=i+1, column=0, sticky=W+E+N+S)

		plotButton = Button(master, text="Plot!", command=self.plotRawData).grid(row=12, column=0, sticky=W+E)
		plotAllButton = Button(master, text="Plot All!", command=self.plotAllRawData).grid(row=13, column=0, sticky=W+E)

	def diffChannel_init(self, master):
		self.comboboxA = []
		self.comboboxvarA = []
		self.comboboxB = []
		self.comboboxvarB = []
		self.diffSel = []
		channels = ('Channel 1', 'Channel 2', 'Channel 3', 'Channel 4', 'Channel 5', 'Channel 6', 'Channel 7', 'Channel 8', 'Channel 9', 'Channel 10')
		for i in range(5):
			self.diffSel.append(IntVar())
			cb = Checkbutton(master, variable=self.diffSel[i]).grid(row=i, column=0, sticky=S)

			self.comboboxvarA.append(StringVar())
			self.comboboxA.append(ttk.Combobox(master, width=10, textvariable=self.comboboxvarA[i], values=channels, state="readonly"))
			self.comboboxA[i].current(0)
			self.comboboxA[i].grid(row=i,column=1, sticky=W+E)

			minusLabel = Label(master, text="minus").grid(row=i, column=2, sticky=N)

			self.comboboxvarB.append(StringVar())
			self.comboboxB.append(ttk.Combobox(master, width=10, textvariable=self.comboboxvarB[i], values=channels, state="readonly"))
			self.comboboxB[i].current(0)
			self.comboboxB[i].grid(row=i,column=3, sticky=W+E)

		plotButton = Button(master, text="Plot!", command=self.plotDiffData).grid(row=5, column=0, columnspan=4, sticky=W+E)

	def threshold_init(self, master):
		self.thresholdSel = IntVar()
		Radiobutton(master, variable=self.thresholdSel, text="Single Channel", value=0, command=self.thresholdSelChanged).grid(row=0, column=0, columnspan=2, sticky=W)
		Radiobutton(master, variable=self.thresholdSel, text="Differential Channels", value=1, command=self.thresholdSelChanged).grid(row=0, column=1, columnspan=2, sticky=W)		

		channels = ('Channel 1', 'Channel 2', 'Channel 3', 'Channel 4', 'Channel 5', 'Channel 6', 'Channel 7', 'Channel 8', 'Channel 9', 'Channel 10')
		self.thresholdSingleVar = StringVar()
		self.thresholdSingleVar.set('Channel 1')
		self.thresholdSingle = ttk.Combobox(master, textvariable=self.thresholdSingleVar, values=channels, state="readonly")
		self.thresholdSingle.grid(row=2, column=0, columnspan=4, sticky=W+E)

		self.thresholdVarA = StringVar()
		self.thresholdVarA.set('Channel 1')
		self.thresholdVarA = ttk.Combobox(master, textvariable=self.thresholdVarA, values=channels, state="disable")
		self.thresholdVarA.grid(row=3, column=0, sticky=W+E)

		minusLabel = Label(master, text="minus").grid(row=3, column=1)

		self.thresholdVarB = StringVar()
		self.thresholdVarB.set('Channel 1')
		self.thresholdVarB = ttk.Combobox(master, textvariable=self.thresholdVarB, values=channels, state="disable")
		self.thresholdVarB.grid(row=3, column=2, columnspan=2, sticky=W+E)

		thresholdLabel = Label(master, text="Threshold (uV):").grid(row=4, column=0)
		symbols = ('>', '<')
		self.moreLess = IntVar()
		self.moreLessCombo = ttk.Combobox(master, width=3, textvariable=self.moreLess, values=symbols, state="readonly")
		self.moreLessCombo.grid(row=4, column=1, sticky=W+E)
		self.moreLessCombo.current(0)

		self.thresholdValue = StringVar()
		ttk.Entry(master, width=15, textvariable=self.thresholdValue).grid(row=4, column=2, columnspan=2, sticky=W+E)

		self.preThreshold = IntVar()
		preThresholdLabel = Label(master, text="Pre-threshold timeframe (ms):").grid(row=5, column=0)
		preThresholdEntry = ttk.Entry(master, width=15, textvariable=self.preThreshold).grid(row=5,column=1, columnspan=3, sticky=W+E)

		self.postThreshold = IntVar()
		postThresholdLabel = Label(master, text="Post-threshold timeframe (ms):").grid(row=6, column=0)
		postThresholdEntry = ttk.Entry(master, width=15, textvariable=self.postThreshold).grid(row=6,column=1, columnspan=3, sticky=W+E)

		numMaxList = ('10 plots', '20 plots', '30 plots', '40 plots', '50 plots')
		numMaxLabel = Label(master, text="Maximum plots:").grid(row=7, column=0)
		self.numMax = IntVar()
		self.numMaxCombo = ttk.Combobox(master, width=8, textvariable=self.numMax, values=numMaxList, state="readonly")
		self.numMaxCombo.grid(row=7, column=1, columnspan=3, sticky=W+E)
		self.numMaxCombo.current(0)

		plotButton = Button(master, text="Plot!", command=self.plotThreshold).grid(row=8, column=0, columnspan=4, sticky=W+E)

	def FFT_init(self, master):
		selectFFTLabel = Label(master, text="Select channels")
		selectFFTLabel.grid(row=0, column=0)

		self.channelFFTSel = []
		for i in range(10):
			self.channelFFTSel.append(IntVar())
			cb = Checkbutton(master, text="Channel " + str(i+1), variable=self.channelFFTSel[i]).grid(row=i+1, column=0, sticky=W+E+N+S)

		FFT_baseLabel = Label(master, text="Baseline timeframe (seconds):").grid(row=0, column=1, columnspan=3)
		self.baselineStart = IntVar()
		self.baselineStartEntry = ttk.Entry(master, width=15, textvariable=self.baselineStart).grid(row=1, column=1)
		baselineLabel = Label(master, text=" to ").grid(row=1, column=2)
		self.baselineEnd = IntVar()
		self.baselineEndEntry = ttk.Entry(master, width=15, textvariable=self.baselineEnd).grid(row=1, column=3)

		FFT_signalLabel = Label(master, text="Timeframe of signal to FFT (seconds):").grid(row=2, column=1, columnspan=3)
		self.FFTSignalStart = IntVar()
		self.FFTSignalStartEntry = ttk.Entry(master, width=15, textvariable=self.FFTSignalStart).grid(row=3, column=1)
		FFTLabel = Label(master, text=" to ").grid(row=3, column=2)
		self.FFTSignalEnd = IntVar()
		self.FFTSignalEndEntry = ttk.Entry(master, width=15, textvariable=self.FFTSignalEnd).grid(row=3, column=3)

		plotButton = Button(master, text="Plot!", command=self.plotFFTData).grid(row=12, column=0, sticky=W+E)
		plotAllButton = Button(master, text="Plot All!", command=self.plotAllFFTData).grid(row=13, column=0, sticky=W+E)

	def thresholdSelChanged(self):
		if(self.thresholdSel.get() == 1):
			self.thresholdVarA.config(state="readonly")
			self.thresholdVarB.config(state="readonly")
			self.thresholdSingle.config(state="disable")
		else:
			self.thresholdVarA.config(state="disable")
			self.thresholdVarB.config(state="disable")
			self.thresholdSingle.config(state="readonly")

	def plotRawData(self):
		plot = WavePlot(self.df)
		plot.plotSelected(self.channelSel)

	def plotAllRawData(self):
		plot = WavePlot(self.df)
		plot.plotAll()

	def plotDiffData(self):
		plot = WavePlot(self.df)
		plot.plotDiffData(self.diffSel, self.comboboxA, self.comboboxB)

	def plotThreshold(self):
		plot = WavePlot(self.df)
		if int(self.preThreshold.get()) > 0:
			preThreshold = -int(self.preThreshold.get())
		else:
			preThreshold = int(self.preThreshold.get())
		if int(self.postThreshold.get()) < 0:
			postThreshold = -int(self.postThreshold.get())
		else:
			postThreshold = int(self.postThreshold.get())
		if self.thresholdSel.get() == 0:
			plot.plotThreshold(preThreshold, postThreshold, int(self.moreLessCombo.current()), \
				10 * (int(self.numMaxCombo.current()) + 1), float(self.thresholdValue.get()), \
				int(self.thresholdSingle.current()), -1)
		else:
			plot.plotThreshold(preThreshold, postThreshold, int(self.moreLessCombo.current()), \
				10 * (int(self.numMaxCombo.current()) + 1), float(self.thresholdValue.get()), \
				int(self.thresholdVarA.current()), int(self.thresholdVarB.current()))

	def plotFFTData(self):
		plot = WavePlot(self.df)
		plot.plotFFT(self.channelFFTSel, int(self.baselineStart.get()), int(self.baselineEnd.get()), int(self.FFTSignalStart.get()), int(self.FFTSignalEnd.get()))

	def plotAllFFTData(self):
		print "testing"