#!/bin/bash

gnuplot -e "
	set title '$1';
	set hidden3d;
	set dgrid3d 30,30 splines;
	set logscale x 2;
	set multiplot;
	set ytics 30,10,60;
	set xtics center offset 0,-0.5;
	set ytics center offset 0,-0.5;
	set zlabel 'Power (W)' rotate by 90;
	splot '$2' w l notitle lc rgb '#000000';
	set cbrange [0:10];
	set cblabel 'SoC (%)' offset 0.0,0.0;
	set cbtics (0,5,10);
	set cbtics ('0' 0, '50' 5, '100' 10);
	set format x '';
	set format y '';
	set format z '';
	unset ztics;
	set palette rgb 10,13,33;
	splot '$3' w pm3d at b notitle;
	unset multiplot;
	unset output;
	pause -1
"
