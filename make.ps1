cl /std:c11 /W4 /WX /I include `
    .\src\main.c .\src\gl.c `
    /Fe"main.exe" `
    /link .\dependencies\glfw3dll.lib opengl32.lib User32.lib Gdi32.lib Shell32.lib

rm main.obj, gl.obj
