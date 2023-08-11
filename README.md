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
- Add A star pathfinding.
- Instead of using x->pWindow->getDims/getWidth etc... get the width of the back buffer frame buffer resource instead.
- Add quad tree culling of all entities.
- Create and add C2DMap to layers.
- Add QuadTree to 2D world/layers
- Set automatic positioning for shadows depthmap rendering in the scene manager
- Test CFrustum class and culling of COctTree
- Implement SceneManagerOctTree.
- Add 2D and 3D particle systems.
- Rewrite joy part of input.
- Add padding to vector classes and test speed increase.
- Add mouse wheel support for CGUITextScroll object.
- Add physics engine.
- GUI...
- GUI: Change default theme font to a bit bigger as commas don�t render right.
- GUI: Implement Listbox
- GUI: Implement Colour selector
- GUI: Implement DraggableDock.
- GUI: Implement DraggableItem.
- GUI: Implement Menu.
- GUI: Implement Tab and TabContainer.
- GUI: Implement Taskbar.
- GUI: Add ability for the GUITooltip::setAsText() method to allow "\n" multiple lines.

** Random notes **
The measurement of force uses the unit known as the Newton after that guy who has a nasty experience with an apple.
One Newton is the force required to make a one kilogram mass move from rest to a speed of one meter per second.






