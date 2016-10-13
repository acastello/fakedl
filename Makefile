all: libfakedl_32.so libfakedl_64.so wov3_32.dll.so wov3_64.dll.so

libfakedl_32.so: fakedl.c
	gcc -shared -fPIC -ldl $^ -o $@ -m32
libfakedl_64.so: fakedl.c
	gcc -shared -fPIC -ldl $^ -o $@ -m64
wov3_32.dll.so: wov3.c
	winegcc -shared -fPIC $^ -o $@ -m32
wov3_64.dll.so: wov3.c
	winegcc -shared -fPIC $^ -o $@ -m64
test: all
	LIBGL_SHOW_FPS=1 LD_PRELOAD=./libfakedl_32.so wine ~/World\ of\ Warcraft\ 3.3.5a/Wow.exe
