set CFLAGS=-Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
set LCC=C:\gbdk\bin\lcc.exe

mkdir build
%LCC% %CFLAGS% -c -o build/mbacl.o mbacl.c
%LCC% %CFLAGS% -c -o build/comm.o comm.s
%LCC% %CFLAGS% -c -o build/testapp.o testapp.c
%LCC% %CFLAGS% -o testapp.gbc build/testapp.o build/mbacl.o build/comm.o
