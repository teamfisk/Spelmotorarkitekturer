"../bin/Test.exe" %1 -o %1.xml -d "yes" -r xml
py .\CatchXMLFileToGNUPlotData.py "%1.xml" "%1.txt"

