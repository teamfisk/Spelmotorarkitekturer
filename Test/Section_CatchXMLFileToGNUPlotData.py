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

def CatchXMLFileToGNUPlotData(infile):
    tree = etree.parse(infile)
    root = tree.getroot()

    testgroup = root.getchildren()[0]
    testcases = testgroup.findall("TestCase")

    # figure out how many lines already exist in this file, so that the next line is
    # appened with the next linenumber
    for test in testcases:
        testname = test.attrib["name"]
        outfile = "./data/" + testname + ".txt"
        sections = test.findall("Section")

        with open(outfile, 'a') as f:
            for section in sections:
                sectionName = section.attrib["name"]
                result = section.find("OverallResults")
                dur = result.attrib["durationInSeconds"]
                f.write(str(sectionName))
                f.write(" ")
                f.write(str(dur))
                f.write("\n")




        

        
