# Classic Snake in 4K Windows executable

After watching this:
    
    [https://youtu.be/ExwqNreocpg]

I felt challenged, so I recreated a somewhat functional Snake clone in 4K for Windows.
Quite ugly, quite simplified, quite fast, quite small :)

I purposely chose not to use packers such as crinkler because where would the fun be?
I just wanted to stay below the 4K mark using plain C code, without resorting to ASM.
And I demonstrated it is possible, with a mix of (not too) careful coding and compiler/linking options.

The resulting executable was created using Visual Studio 2017, but it should be easy to recreate it on other versions.

Credits: 

- [https://github.com/revivalizer/64klibs](Ralph Brorsen) for his startup code
- [https://www.iquilezles.org/code/isystem1k4k/isystem1k4k.htm](Inigo Quilez) for the software renderer
- [https://lvgl.io/tools/imageconverter](LVGL converter) for the title screen conversion to bytes (yes, I am THAT lazy)


Enjoy!