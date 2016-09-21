"../bin/Test.exe" SmallAllocationComparison -o SmallAllocationComparison.xml -d "yes" -r xml
py .\CatchXMLFileToGNUPlotData.py "SmallAllocationComparison.xml" "SmallAllocationComparison.txt"

