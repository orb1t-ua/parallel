rm outfile.txt
touch outfile.txt

./pi1.out 10000 >> outfile.txt
./pi1.out 100000 >> outfile.txt
./pi1.out 1000000 >> outfile.txt
./pi1.out 10000000 >> outfile.txt

./pi2.out $1 10000 >> outfile.txt
./pi2.out $1 100000 >> outfile.txt
./pi2.out $1 1000000 >> outfile.txt
./pi2.out $1 10000000 >> outfile.txt

./pi3.out $1 10000 >> outfile.txt
./pi3.out $1 100000 >> outfile.txt
./pi3.out $1 1000000 >> outfile.txt
./pi3.out $1 10000000 >> outfile.txt

./pi4.out $1 10000 >> outfile.txt
./pi4.out $1 100000 >> outfile.txt
./pi4.out $1 1000000 >> outfile.txt
./pi4.out $1 10000000 >> outfile.txt

./pi5.out $1 10000 >> outfile.txt
./pi5.out $1 100000 >> outfile.txt
./pi5.out $1 1000000 >> outfile.txt
./pi5.out $1 10000000 >> outfile.txt
