![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.

Current progress...
![Screenshot of the development application showing GUI progress.](https://github.com/DavidCradock/X/blob/8b0174ddd5eb93baecca9c6d422111aac91df7e6/github_images/github_screenshot_2023_07_03.png)

** TODO **
- Add quaternions and position to CSMEntityBase along with helper functions to move stuff etc.
- implement more stuff into Camera
- implement SceneManagers (A brute force, render everything, but sorted by shader/textures) and SceneManagerOctTree, perhaps also SceneManagerQuadTree
- implement GUI (2D)
- particle systems
- Add in neural networks, steering behaviours, pathfinding
- Add and profile sorting of scene by materials
- add translation/rotation/scale of base entities
- Implement DraggableDock to GUI.
- Implement DraggableItem to GUI.
- Implement Menu to GUI.
- Implement Tab and TabContainer to GUI.
- Implement Taskbar to GUI.
- Create 2D sprite rendering
- Rewrite joy part of input
- Remove and replace glm with own code?
- Add padding to vector classes
- Create CResourceTexture2DAtlas class, copy from CResourceTexture2DAnimation then modify to include images of any size and allow spacing inbetween each image.
- Change CResourceTexture2DAnimation to CResourceTexture2DAtlas
- Check CImageAtlasPacker

