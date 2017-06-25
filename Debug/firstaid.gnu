#
# $Id: simple.dem,v 1.7 2012/11/18 23:12:22 sfeam Exp $
#
# Requires data files "[123].dat" from this directory,
# so change current working directory to this directory before running.
# gnuplot> set term <term-type>
# gnuplot> load 'simple.dem'
#
set fit quiet
set terminal wxt size 1024,524 enhanced font 'Verdana,10' persist
set title "Waiting Times for Emergencies" font ",20"
set xlabel 'Simulation Run Number'
set ylabel 'Time [minutes]'
set y2label 'Workload [%]'
set y2range [0:100]
set y2tics 10 nomirror tc lt 2

f(x) = mean_y
fit f(x) 'keyfigures.dat' u 1:2 via mean_y

g(x) = mean_z
fit g(x) 'keyfigures.dat' u 1:4 via mean_z


set label 1 gprintf("Avg. Normal Emergencies (all Runs) = %g", mean_y) at graph 0.01, 0.98
set label 2 gprintf("Avg. Urgent Emergencies (all Runs) = %g", mean_z) at graph 0.01, 0.94

set grid
set style histogram rowstacked gap 0
set style fill transparent solid 0.5 border

plot mean_y w l lt 3, mean_z, \
     "keyfigures.dat" using 1:2 title 'Avg. Normal Emergencies' with linespoints, \
     "keyfigures.dat" using 1:4 title 'Avg. Urgent Emergencies' with linespoints, \
	 "keyfigures.dat" using 1:($6*100) with boxes lc rgb"gray" title 'Doctor Workload' axes x2y2