from tkinter import *
from tkinter.ttk import *
from controlpanel import ControlPannel

def onClose():
	root.destroy()
	sys.exit(1)
	os._exit(1)

root = Tk()
root.title("Neutrino Data Analyzer")
root.minsize(450,300)
root.maxsize(450,300)
cp = ControlPannel(root)
root.protocol("WM_DELETE_WINDOW", onClose)
root.mainloop()