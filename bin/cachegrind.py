#!/usr/bin/env python
import re

csv = ''

def append(f):
    global csv

    fileHandle = open(f, 'r')
    contents = fileHandle.read()
    fileHandle.close()
    
    m = re.search('D\s+refs:\s+([\d,]+).*\(', contents);
    refs = m.group(1);
    refs = refs.replace(',', '')
    m = re.search('D1\s+misses:\s+([\d,]+).*\(', contents);
    misses = m.group(1);
    misses = misses.replace(',', '')

    csv += f + ',' + refs + ',' + misses + '\n'

append('cachegrind/PoolLinBaseline.txt')
append('cachegrind/PoolLin.txt')
append('cachegrind/PoolFragBaseline.txt')
append('cachegrind/PoolFrag.txt')
append('cachegrind/StandLinBaseline.txt')
append('cachegrind/StandLin.txt')
append('cachegrind/StandFragBaseline.txt')
append('cachegrind/StandFrag.txt')
print(csv)
