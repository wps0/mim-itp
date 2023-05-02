g++ -O3 -g prog.cpp -o prog.o
g++ -O3 -g gen.cpp -o gen.o
g++ -O3 -g spr2.cpp -o spr2.o

for i in {1..100000}; do
  echo -n Test$i:
	echo $i | ./gen.o > in/test$i.in
	(./prog.o < "in/test$i.in") > "out/test$i.out"
	(cat "in/test$i.in"; cat "in/test$i.in"; cat "out/test$i.out") > ./test.in
	./spr2.o < ./test.in > test.out
	out=$(cat ./test.out)
	if [[ "$out" = "OK" ]]; then
		echo " OK bledow $ile"
		rm in/test$i.in
		rm out/test$i.out
	else
		echo -n "BLAD!"
		let ile=ile+1
	fi
done
