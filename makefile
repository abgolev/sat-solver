sat: 
	g++ satsolver.cpp

run-3:
	./a.out < ./Benchmarks/test2.cnf

run-6:
	./a.out < ./Benchmarks/test.cnf

run-impossible:
	./a.out < ./Benchmarks/test3.cnf

run-20: 
	./a.out < ./Benchmarks/CNFFormulae/uf20-0156.cnf

run-50: 
	./a.out < ./Benchmarks/CNFFormulae/uf50-01.cnf

run-75: 
	./a.out < ./Benchmarks/CNFFormulae/uf75-01.cnf

run-hard:
	./a.out < ./Benchmarks/HardFormulae/100.430.975412199.cnf

run-unsolvable: 
	./a.out < ./Benchmarks/CNFFormulae/uuf75-01.cnf

run-multiple: 
	./a.out < ./Benchmarks/CNFFormulae/uf50-01.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-02.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-03.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-04.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-05.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-06.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-07.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-08.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-09.cnf
	./a.out < ./Benchmarks/CNFFormulae/uf50-10.cnf



