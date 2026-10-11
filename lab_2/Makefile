CC = cc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic -pthread

main: main.c matrix.c parallel.c matrix.h
	$(CC) $(CFLAGS) main.c matrix.c parallel.c -o main -lm

clean:
	rm -f main
THREADS ?= 10
SIZE ?= 2048

stats: main
	@echo "Логических ядер: $$(sysctl -n hw.logicalcpu)"
	@echo "Рабочих потоков: $(THREADS)"
	@./main -m $(SIZE) -n $(SIZE) -k $(SIZE) -t $(THREADS) > .stats_output.txt 2>&1 & \
	pid=$$!; \
	echo "PID процесса: $$pid"; \
	top -pid $$pid -stats pid,command,threads,cpu; \
	wait $$pid; \
	cat .stats_output.txt; \
	rm -f .stats_output.txt


.PHONY: clean
.PHONY: stats
