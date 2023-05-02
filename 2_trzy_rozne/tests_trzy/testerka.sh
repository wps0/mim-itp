gcc -g -O3 prog.c -o prog.o
# g++ -O2 -std=c++11 prog.cpp arczaw.c -o prog.o
g++ -g -O3 gen.cpp -o gen.o
gcc -g -O1 wzor.c -o wzor.o
#g++ -O2 -std=c++11 wzor.cpp arczaw.c -o wzor.o

rm in/* out/*

for i in {1..3000}; do
	echo $i | ./gen.o > in/test$i.in
	./wzor.o < "in/test$i.in" > "out/test$i.out"
	(./prog.o < "in/test$i.in") > ./test.out
	out=$(diff ./test.out out/test$i.out)
	if [[ "$out" = "" ]]; then
		echo Test$i: OK bledow $ile
		rm in/test$i.in
		rm out/test$i.out
	else
		echo "BLAD! Test#$i"
		let ile=ile+1
	fi
done
