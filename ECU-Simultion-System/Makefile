CC = gcc

CFLAGS = -Ilib -fstack-usage

SRC = main.c lib/input.c lib/control.c lib/fault.c lib/mode.c lib/state.c lib/log.c lib/compare.c lib/error.c

OUT = ecu_project_exec
CSV_GEN = gen_csv

all: build analyze

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
	mkdir -p perf system
	-mv *.su lib/*.su perf/ 2>/dev/null

analyze: build
	mkdir -p perf system
	nm -S --size-sort $(OUT) | grep " T " > perf/size_raw.txt
	$(CC) lib/gen_csv.c -o $(CSV_GEN)
	./$(CSV_GEN)

run:
	./$(OUT)

clean:
	rm -f $(OUT) $(CSV_GEN)
	rm -f *.su lib/*.su perf/*.su
	rm -f perf/size_raw.txt
	rm -f system/module_size.csv