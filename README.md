# MiniRenderer using C++
The goal of this project is to learn and understand how do Graphic API's such as OpenGL, DirectX & Vulkan work at a low level.
This project does not rely on external libraries (except the files mentioned below*) and it will not be working directly with the GPU, also I am not trying to implement the most optimal known algorithms but implementing my own approach, implementing other's more optimal algorithms will be the main focus in my future learning projects. The output of the renderer will be found in the file called
'output.tga' because I am not interested in doing it real time for now as it is, as I mentioned earlier, just to learn the basics of rendering at a low-level, painting pixel per pixel.
<br />
*The 'tgaimage.cpp' and 'tgaimage.h' files are written by [ssloy](https://github.com/ssloy).


For now, the MiniRenderer project allows for the loading of 3D models in the .obj format and drawing their wireframes:
<br />
![image](https://github.com/iago-cpalmer/MiniRenderer/assets/68129919/5281bd68-64a3-480a-af2e-45e11a257c2a)

<br />
Filling triangles.
<br />

![image](https://github.com/iago-cpalmer/MiniRenderer/assets/68129919/9a46843f-5134-4f24-88b7-e92f0329b134)

<br />
Temporary flat shading:
<br/>

![image](https://github.com/iago-cpalmer/MiniRenderer/assets/68129919/817fc112-3f51-45fc-aaf3-864838ab2724)

<br />
With Z-buffer algorithm (no changes visually from the previous one as there was a simple back-face culling):
<br />

![image](https://github.com/iago-cpalmer/MiniRenderer/assets/68129919/dbc41c9d-cd33-4a90-a392-cd720f2b8c6f)
<br />





