run: main
	@./main messdaten-rinex/20150990.24g

main: main.c
	@gcc main.c -o main
