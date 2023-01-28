md bin
cl /Febin\note-git.exe utils.cpp git.cpp main.cpp /link user32.lib
del utils.obj
del git.obj
del main.obj