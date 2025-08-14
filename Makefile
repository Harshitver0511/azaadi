all:
	bison -d compiler.y
	flex compiler.l
	gcc lex.yy.c compiler.tab.c -o compiler
	gcc vm.c -o vm

run:
	./compiler < program.c
	./vm
