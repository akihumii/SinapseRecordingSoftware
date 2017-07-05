from Tkinter import *
from controlpanel import ControlPannel
import tkMessageBox

def onClose():
	root.destroy()
	sys.exit(1)
	os._exit(1)

root = Tk()
cp = ControlPannel(root)
root.protocol("WM_DELETE_WINDOW", onClose)
root.mainloop()
