Compiled with Visual Studio (2017 was used for this one)

Packages needed:
1) GLFW (https://www.glfw.org/download.html)
2) GLAD (https://glad.dav1d.de/)
	Set up for gl 4.4, core profile, no extensions; might need to replace glad.c in the repo with glad.c generated by  the tool
3) Freetype (https://freetype.org/)
	Use the prebuilt static for x64 (https://github.com/ubawurinna/freetype-windows-binaries/releases/tag/v2.13.0)

Update inclusion catalogs and library catalogs for the ones where the libs were set up.