![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/38b6f3ba1d8ecd946eac329807001c5faad6462c/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using OpenGL for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Load the X.sln file into Visual studio, hit compile, done.

Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering...
![Screenshot of the development application showing GUI and simple 3D scene manager with shadow rendering.](https://github.com/DavidCradock/X/blob/e0d565a4164a812c36f54f97c38803a583d7033d/github_images/github_screenshot_2023_07_03.png)

Screenshot of Benchmark application showing lots of 2D images being rendered...
![Screenshot of Benchmark application showing lots of 2D images being rendered.](https://github.com/DavidCradock/X/blob/e0d565a4164a812c36f54f97c38803a583d7033d/github_images/github_screenshot_2023_07_13.png)

Screenshot of "X:Default:FontGenerator" container window...
![Screenshot of the X:Default:FontGenerator container window generating and displaying font files from a font installed in the operating system.](https://github.com/DavidCradock/X/blob/38a31307b9ecdecf7225171ee055127a4ebc05a9/github_images/githun_font_generator_window_2023_08_02.png)

** TODO **
- Forward slashes for all file names/paths
- Private implementation of private members and methods.
- added fileManager and fileMemory, complete manager
- implement message system and remove SCWindow onToggleFullscreen. Why?
- Add A star pathfinding.
- Instead of using x->pWindow->getDims/getWidth etc... get the width of the back buffer frame buffer resource instead.
- Add quad tree culling of all entities.
- Add QuadTree to 2D world/layers
- Set automatic positioning for shadows depthmap rendering in the scene manager
- Test CFrustum class and culling of COctTree
- Implement SceneManagerOctTree.
- Add 2D and 3D particle systems.
- Rewrite joy part of input.
- Add padding to vector classes and test speed increase.
- Add mouse wheel support for CGUITextScroll object.
- Add physics engine.
- GUI rewrite
- Things to improve...
- Use texture atlas
- Add more frame buffer usage to speed up everything
- Remove the reflection thing, it's crap
- Add blurring of background
- Call containers, windows. Then set decorations on/off.
- Add screen resize handling code (For fullscreen and window resize)
- The current method of checking button presses is aweful, too many if statements
- Read up on "messaging" data structure then a widget will send a message to a message handler.
- Add alignment (left, right, centreX, top, centreY, bottom)
- UI: Add a scrollable sub container which has a scroll bar if the contents added do not fit.
- UI: Implement Button
- UI: Implement ButtonImage
- UI: Implement Checkbox
- UI: Implement Colour selector
- UI: Implement Container
- UI: Implement DraggableDock.
- UI: Implement DraggableItem.
- UI: Implement Image
- UI: Implement ImageAnimated
- UI: Implement ImageDepthbuffer
- UI: Implement ImageFramebuffer
- UI: Implement Linegraph
- UI: Implement Listbox
- UI: Implement Manager
- UI: Implement Menu.
- UI: Implement Progress bar
- UI: Implement Slider
- UI: Implement Tab and TabContainer.
- UI: Implement Taskbar.
- UI: Implement Text
- UI: Implement TextScroll
- UI: Implement Theme
- UI: Implement Tooltip
- REMOVE GUI.

** Random notes **
The measurement of force uses the unit known as the Newton after that guy who has a nasty experience with an apple.
One Newton is the force required to make a one kilogram mass move from rest to a speed of one meter per second.