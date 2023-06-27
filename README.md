![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.
** TODO **
- Add quaternions and position to SMEntityBase along with helper functions to move stuff etc.
- implement more stuff into Camera
- implement SceneManagers (A brute force, render everything, but sorted by shader/textures) and SceneManagerOctTree, perhaps also SceneManagerQuadTree
- implement GUI (2D)
- particle systems
- Add in neural networks, steering behaviours, pathfinding
- Add and profile sorting of scene by materials
- add translation/rotation/scale of base entities
- Decide whether all the GUI widgets/objects render their 9 borders either...
INSIDE the position + dims, so that the TOTAL gui area is equal to their set dims
OUTSIDE the position + dims, so that the TOTAL gui area is MORE THAN the set dims
I think the first option is best because...
When changing themes, the GUI layout is consistent
- Create a "render 9 images" method inside the GUIBaseObject class, removing from all widgets that do this themselves.
