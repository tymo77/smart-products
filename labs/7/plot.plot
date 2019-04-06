#!/usr/bin gnuplot


# Read CSVs
set datafile separator comma

set xlabel 'x (cm)'
set ylabel 'y (cm)'
set zlabel 'z (cm)'

 


# Plot the data
splot 'lidar_data.log' using 5:6:7:4 with points palette pointsize 3 pointtype 7

pause 5
reread
