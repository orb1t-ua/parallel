Building:
	./build.sh on a system with c++11 and pthreads
Running:
	qsub ./mysub.sh
	(use ./run.sh <threads> for systems other than hpc)
	results will be found under ~/pi/outfile.txt on hpc
	our simply outfile.txt using run.sh