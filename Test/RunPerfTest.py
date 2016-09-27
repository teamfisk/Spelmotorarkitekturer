import subprocess
from CatchXMLFileToGNUPlotData import CatchXMLFileToGNUPlotData

arg1 = "[StackAllocator]"
#arg1 = sys.argv[1]
args = ["../bin/Test.exe", arg1, '-d', 'yes', "-r", "xml", "-o", arg1 + ".xml"]

#print(str(args))
#input()
p = subprocess.Popen(args)
p.wait()

CatchXMLFileToGNUPlotData(arg1 + ".xml")

input("Program done. Press any key to continue.")
