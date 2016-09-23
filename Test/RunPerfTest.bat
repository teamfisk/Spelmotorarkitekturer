@echo off
REM How to use 	 	
REM ./RunPerfTest.bat MALLOC_SMallAllocationComparison
REM					  ^ the name of the test to run. Run ONE test at a time!
"../bin/Test.exe" %1 -o %1.xml -d "yes" -r xml
py .\CatchXMLFileToGNUPlotData.py "%1.xml" "%1.txt"

