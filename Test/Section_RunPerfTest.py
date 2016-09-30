import subprocess
import sys
from CatchXMLFileToGNUPlotData import CatchXMLFileToGNUPlotData

# Used when running this script directly form a console
if __name__ == "__main__":
    if len(sys.argv) == 2 or len(sys.argv) == 3:
        testruns = 1
        if len(sys.argv) == 3:
            testruns = int(sys.argv[2])

        arg1 = sys.argv[1]
        outputfilepath = "./data/" + arg1 + ".xml"
        args = ["../bin/Test.exe", arg1, '-d', 'yes', "-r", "xml", "-o", outputfilepath]
        #arg1 = "[StackAllocator]"
        for i in range(testruns):
            p = subprocess.Popen(args)
            p.wait()

            CatchXMLFileToGNUPlotData(outputfilepath)
        #input("Program done. Press any key to continue.")
    else:
        print(len(sys.argv))
        print("Usage:")
        print("arg1 is the test(s) to run.")
        print("arg2 (optional) is the number of times to run the test.")
        sys.exit(0)
