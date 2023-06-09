![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.

Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering...
![Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering.](https://github.com/DavidCradock/X/blob/8b0174ddd5eb93baecca9c6d422111aac91df7e6/github_images/github_screenshot_2023_07_03.png)

Screenshot of Benchmark application showing lots of 2D images being rendered...
![Screenshot of Benchmark application showing lots of 2D images being rendered.](https://github.com/DavidCradock/X/blob/a3010410cca1f2320c03d0e0bc02bbb0f61f15df/github_images/github_screenshot_2023_07_13.png)

** TODO **
- Add quaternions and position to CSMEntityBase along with helper functions to move stuff etc.
- Add translation/rotation/scale of base entities
- Implement more stuff into Camera of scene manager
- Implement SceneManagers (A brute force, render everything, but sorted by shader/textures) and SceneManagerOctTree, perhaps also SceneManagerQuadTree
- Add 2D and 3D particle systems
- Work on C2DEntityComplex of 2D sprite rendering
- Rewrite joy part of input
- Add padding to vector classes and test speed increase
- Add the ability to be able to add newlines to the text scroll GUI object.
- Change 2D stuff from using CVector2r to CVector2f
- Add mouse wheel support for CGUITextScroll object.
- Add CText class and exchange it for all the std::string usage
- Add generation of orthographic and perspective matrix
- Add in neural networks, steering behaviours, pathfinding
- Add physics engine.
- Remove and replace glm with own code?
- Implement better random number generator
- GUI...
- Implement DraggableDock to GUI.
- Implement DraggableItem to GUI.
- Implement Menu to GUI.
- Implement Tab and TabContainer to GUI.
- Implement Taskbar to GUI.
