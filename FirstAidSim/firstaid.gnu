#
# $Id: simple.dem,v 1.7 2012/11/18 23:12:22 sfeam Exp $
#
# Requires data files "[123].dat" from this directory,
# so change current working directory to this directory before running.
# gnuplot> set term <term-type>
# gnuplot> load 'simple.dem'
#
set title "Waiting Times for Emergencies" font ",20"
set xlabel 'Simulation Run Number'
set ylabel 'Time [minutes]'

f(x) = mean_y
fit f(x) 'keyfigures.dat' u 1:2 via mean_y

g(x) = mean_z
fit g(x) 'keyfigures.dat' u 1:4 via mean_z

set label 1 gprintf("Mean = %g", mean_y) at 2, 120
set label 2 gprintf("Mean = %g", mean_z) at 2, 100

plot mean_y w l lt 3, mean_z, \
     "keyfigures.dat" using 1:2 title 'Avg. Normal Emergencies' with linespoints, \
     "keyfigures.dat" using 1:4 title 'Avg. Urgent Emergencies' with linespoints