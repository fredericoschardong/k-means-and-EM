#!/usr/bin/gnuplot
reset

text_color = "black"

my_font = "SVBasic Manual, 12"

my_line_width = "2"
my_axis_width = "1.5"
my_ps = "1.2"

# set default point size
set pointsize my_ps

# set the style for the set 1, 2, 3...
set style line 1 linecolor rgbcolor "blue" linewidth 2 pt 7

# this is to use the user-defined styles we just defined.
set style increment user

# set the color and font of the text of the axis
set xtics textcolor rgb text_color font my_font
set ytics textcolor rgb text_color font my_font

# set the color and font (and a default text) for the title and each axis
set title "k-means" textcolor rgb text_color font my_font

# set the text color and font for the label
set label textcolor rgb text_color font my_font

# set the color and width of the axis border
set border 31 lw 1.2 lc rgb text_color

# set grid color
set grid lc rgb "#d5e0c9"
 
set terminal png size 800,600; 

set output "k-means.png"
set palette model RGB defined ( 0 'red', 1 'green' )

plot "<./main" every ::::100 u 1:2:( $3 == 0 ? 0 : 1 ) with points palette notitle
