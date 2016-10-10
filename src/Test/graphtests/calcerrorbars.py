import sys

if len(sys.argv) < 2:
    print("You need to specify at least one .dat file")
    sys.exit()

filelist = []
    
for filename in sys.argv:
    if filename.find(".dat") >= 0:
        i = sum = 0
        with open(filename) as file:
            for line in file:
                parts = line.split(" ")
                if len(parts) < 2:
                    break
                column = float(parts[1].rstrip())
                if i == 0:
                    max = column
                    min = column
                if column > max:
                        max = column
                if column < min:
                    min = column
                sum = sum + column
                i = i + 1
            filelist.append(str(sys.argv.index(filename)) + \
                            " " + \
                            filename + \
                            " " + \
                            str(sum/i) + \
                            " " + \
                            str(min) + \
                            " " + \
                            str(max) + \
                            "\n")

f = open("test.dat", "w")
#for item in list:
    #if len(item) < 1:
    #    break
    #f.write(item)
    #f.write(" ")
    #f.write(str(min) + " " + str(max))
    #f.write("\n")
for item in filelist:
    f.write(item)
f.close()

