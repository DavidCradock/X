![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.
** TODO **
- In resourceLines, make it so we can setAsLineStrip and setAsLineList
- Add quaternions and position to SMEntityBase along with helper functions to move stuff etc.
- Add ResourceDepthbuffer and modify ResourceManager to have those.
- Add new shader to render the entities in the scene manager to a depth buffer, for shadow rendering.
- Also add shader to render entities which use shadow’s depth map.
- Add a bool to scene manager to toggle rendering of shadows and set the SM’s render method to render correct stuff.
- All this good stuff has to be renderered to a frame buffer so that gui can blur itself over the top of what is already renderered. So we need to add a default frame buffer X:default_framebuffer..
- Modify scenemanager's render method to accept a name of a framebuffer to render to and then within that method, bind that framebuffer as render target, render everything, unbind render target. Also allow passing empty name to just render to back buffer.
- implement more stuff into Camera
- implement SceneManagers (A brute force, render everything, but sorted by shader/textures) and SceneManagerOctTree, perhaps also SceneManagerQuadTree
- implement GUI (2D)
- implement shadows
- particle systems
- Add in neural networks, steering behaviours, pathfinding
- Add and profile sorting of scene by materials
- add translation/ rotation/scale of base entities
