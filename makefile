h5fs: $(wildcard src/*.c) $(wildcard src/*.h)
	cc -Wall src/h5fs_main.c $(shell pkg-config fuse --libs --cflags) $(shell pkg-config hdf5 --libs --cflags) -o h5fs

clean:
	rm h5fs -f
