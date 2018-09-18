trios: scanner.fl token.c main.c parser.c ntable.c ptable.c
	flex -o scanner.c scanner.fl 
	gcc -o trios main.c token.c scanner.c parser.c ntable.c ptable.c -lfl
	rm scanner.c
