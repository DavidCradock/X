![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/38b6f3ba1d8ecd946eac329807001c5faad6462c/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### This project is intended to become a bunch of code which I can use to develop a game with.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.

** Features **
- 2D rendering
- 3D rendering
- AI (Neural networks, finite state machines and path finding)
- Audio playback
- Image loading/saving/editing
- 2D pixel based user interface
- Input using DirectX 8
- Math helper functions/classes
- Messaging
- Physics
- Spatial partitioning.

Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering...
![Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering.](https://github.com/DavidCradock/X/blob/e0d565a4164a812c36f54f97c38803a583d7033d/github_images/github_screenshot_2023_07_03.png)

Screenshot of Benchmark application showing lots of 2D images being rendered...
![Screenshot of Benchmark application showing lots of 2D images being rendered.](https://github.com/DavidCradock/X/blob/e0d565a4164a812c36f54f97c38803a583d7033d/github_images/github_screenshot_2023_07_13.png)

Screenshot of "X:Default:FontGenerator" container window...
![Screenshot of the X:Default:FontGenerator container window generating and displaying font files from a font installed in the operating system.](https://github.com/DavidCradock/X/blob/38a31307b9ecdecf7225171ee055127a4ebc05a9/github_images/githun_font_generator_window_2023_08_02.png)

** TODO **
Remove all unnessessary comments. Some of them are bloomin' obvious (// Set bool to true lol)
Add tooltips

Add a SCRenderer class and add state changing stuff such as glEnable(GL_BLEND), culling, scissor testing etc.
- Check CUIWindow::getTitlebarArea()
- UI rewrite
- Implement UIDefaultContainers stuff
- Add screen resize handling code (For fullscreen and window resize)
- The current method of checking button presses is aweful, too many if statements. Research observer pattern, or signal and slots or delegates.
- Add alignment (left, right, centreX, top, centreY, bottom)
- UI: Implement Colour selector
- UI: Implement DraggableDock.
- UI: Implement DraggableItem.
- UI: Implement ImageAnimated
- UI: Implement Listbox
- UI: Implement Manager
- UI: Implement Menu.
- UI: Implement Tab and TabContainer.
- UI: Implement Taskbar.
- UI: Implement Tooltip
- REMOVE GUI.

- Create a simple 2D image editor, I hate the tiny UI of my old version of Photoshop and all other 2D paint apps are awkward to use.
- Forward slashes for all file names/paths
- Private implementation of private members and methods.
-
- Added fileManager and fileMemory, now complete manager
- Instead of using x->pWindow->getDims/getWidth etc... get the width of the back buffer frame buffer resource instead.
- Look into atlas packer and fix double spacing issue.
- 
- Add A star pathfinding.
- Add quad tree culling of all entities.
- Add QuadTree to 2D world/layers
- Set automatic positioning for shadows depthmap rendering in the scene manager
- Test CFrustum class and culling of COctTree
- Implement SceneManagerOctTree.
- Add 2D and 3D particle systems.
- Rewrite joy part of input.
- Add padding to vector classes and test speed increase.
- Add physics engine.
- 

** Random notes **
The measurement of force uses the unit known as the Newton after that guy who has a nasty experience with an apple.
One Newton is the force required to make a one kilogram mass move from rest to a speed of one meter per second.