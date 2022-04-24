main: main.c unittest.c
	gcc $^ -o $@
clean:
	rm main