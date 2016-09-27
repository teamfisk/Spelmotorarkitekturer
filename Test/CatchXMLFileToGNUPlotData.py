#  input,
#  1. The path to the XML file created by catch
#  2. Name of output file

# this generates data
# run it inside the python script?
#Test.exe SmallAllocationComparison  -n StackPerf-release -r xml -d "yes" -o StackAllocatorPerfData.xml

# run test once
# -> transform xml data into gnuplot data
# -> append to output text file
# rerun until enough test runs have run

import xml.etree.ElementTree as etree
import sys
#len(sys.argv) != 2 or

if(len(sys.argv) != 2):
    print("Usage: arg1\n")
    print("arg1 is the path to the XML file created by catch.")
    sys.exit(0)

infile = sys.argv[1]

tree = etree.parse(infile)
root = tree.getroot()

testgroup = root.getchildren()[0]
testcases = testgroup.findall("TestCase")

# figure out how many lines already exist in this file, so that the next line is
# appened with the next linenumber
for test in testcases:
    outfile = test.attrib["name"] + ".txt"
    result = test.find("OverallResult")

    try:
    	with open(outfile) as f:
    		testrun = len(f.readlines())
    except FileNotFoundError:
    		testrun = 1

    with open(outfile, 'a') as f:
        dur = result.attrib["durationInSeconds"]
        f.write(str(testrun))
        f.write(" ")
        f.write(str(dur))
        f.write("\n")
        testrun += 1
