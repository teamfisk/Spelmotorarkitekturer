@echo off
REM How to use 	 										↓ number of times to run each test.
REM ./RunPerfTest.bat MALLOC_SMallAllocationComparison 10
REM					  ^ the name of the test(s) to run. It's possible to run multiple tests. Each test result will be exported into a seperate txt file.

IF NOT "%2"=="" THEN (
	SET arg2=1
) ELSE (
	SET arg2=%2
)

echo on

FOR %%A IN (1, 1, %arg2%) DO (
	ECHO %%A
	REM "../bin/Test.exe" %1 -o %1.xml -d "yes" -r xml
	REM py .\CatchXMLFileToGNUPlotData.py "%1.xml"
)

