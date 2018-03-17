execute=./a.out
experiment1:
	gcc -w create_file.c
	$(execute)
experiment2:
	gcc -w -pthread -o a.out disk.c
	$(execute) 1 1
	$(execute) 1 2
	$(execute) 1 4
	$(execute) 1 8 
experiment3:
	gcc -w -pthread -o a.out disk.c
	$(execute) 2 1
	$(execute) 2 2
	$(execute) 2 4
	$(execute) 2 8
experiment4:
	gcc -w -pthread -o a.out disk.c
	$(execute) 3 1
	$(execute) 3 2
	$(execute) 3 4
	$(execute) 3 8
experiment5:
	gcc -w -pthread -o a.out memory.c
	$(execute) 1 1
	$(execute) 1 2
	$(execute) 1 4
	$(execute) 1 8
experiment6:
	gcc -w -pthread -o a.out memory.c
	$(execute) 2 1
	$(execute) 2 2
	$(execute) 2 4
	$(execute) 2 8
experiment7:
	gcc -w -pthread -o a.out memory.c
	$(execute) 3 1
	$(execute) 3 2
	$(execute) 3 4
	$(execute) 3 8
experiment8:
	gcc -w -mavx2 -pthread -o a.out cpu.c
	$(execute) 1 1
	$(execute) 1 2
	$(execute) 1 4
	$(execute) 1 8
experiment9:
	gcc -w -mavx2 -pthread -o a.out cpu.c
	$(execute) 2 1
	$(execute) 2 2
	$(execute) 2 4
	$(execute) 2 8

