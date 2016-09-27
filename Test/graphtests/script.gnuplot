set title "Graph over time vs. amount of allocations"
set xlabel "Test Run"
set ylabel "Time"

# Make plot fancy with transparrent stuff
#set clip two
set style fill transparent solid 0.50 noborder
set style histogram clustered
set key inside left top vertical Left reverse enhanced autotitles 
#set key noinvert samplen 1 spacing 1 width 0 height 0 

set xtics rotate by 45 offset 0,-2

set boxwidth 0.5

plot "test.dat" every 2 using 1:3:xtic(2) title "Malloc" with boxes, \
     "test.dat" every 2 using 1:3:4:5 title "Error" with yerrorbars, \
     "test.dat" every 2:1:1 using 1:3:xtic(2) title "Stack" with boxes, \
     "test.dat" every 2:1:1 using 1:3:4:5 title "Error" with yerrorbars

pause -1 "Hit enter"


