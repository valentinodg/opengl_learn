<p align='center'><img src="./openglx.svg" width="300"></p>

## opengl low-level 3d rendering api

### [dependencies]:
* **mesa3d** > 
	* ```sudo pacman -Sy mesa```
	* ```sudo xbps-install -S mesa```
* **glad** > https://glad.dav1d.de/
* **glfw** > 
	* ```sudo pacman -Sy glfw-x11```
	* ```sudo xbps-install -S glfw glfw-devel```
* **stb_image** > https://github.com/nothings/stb/blob/master/stb_image.h
* **glm** > https://github.com/g-truc/glm [0.9.9.7]
* **assimp** > http://assimp.org/index.php/downloads [3.1.1]
	* ```sudo pacman -Sy assimp```
	* ```sudo xbps-install -S assimp libassimp-devel```
	* or natively compile the lib >
		* ```sudo pacman -Sy minizip zziplib```
		* ```sudo xbps-install -S minizip-devel zziplib-devel```
		* ```cmake CMakeLists.txt```
		* ```make -jX``` (X: # of cpu cores > parallel processes)

#### [developed/tested on arch/void linux (using xorg server > x window system)]
