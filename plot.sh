#!/bin/bash

gnuplot -e "
	set title 'first layer model';
	set multiplot layout 2,1 rowsfirst title \"$1\";
	set datafile separator comma;
        set key autotitle columnhead;
	set ylabel \"W\";
	set xlabel \"s\";
	plot \"$2\" using (\$0/10):1 w l title \"power [W]\";
	set title 'third layer model';
	set ylabel \"%\";
	set xlabel \"s\";
	plot \"$3\" using (\$0/1000):1 w l title \"SoC [%]\";
	unset multiplot;
	pause -1
"
