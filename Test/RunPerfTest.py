import subprocess
import sys
from CatchXMLFileToGNUPlotData import CatchXMLFileToGNUPlotData

# Used when running this script directly form a console
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: arg1\n")
        print("arg1 is the path to the XML file created by catch.")
        input()
        sys.exit(0)
    else:
        #arg1 = "[StackAllocator]"
        arg1 = sys.argv[1]
        args = ["../bin/Test.exe", arg1, '-d', 'yes', "-r", "xml", "-o", arg1 + ".xml"]

        p = subprocess.Popen(args)
        p.wait()

        CatchXMLFileToGNUPlotData(arg1 + ".xml")

        input("Program done. Press any key to continue.")
