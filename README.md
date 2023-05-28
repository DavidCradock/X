![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using Vulkan for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Download the Vulkan SDK and install from https://www.lunarg.com/vulkan-sdk/
- Within Solution Explorer, right click on X, click properties and add the Vulkan SDK include and lib directories...
- C++/General additional include directories and add c:\VulkanSDK\1.3.246.1\Include\ or equivalent
- Linker/Additional Library Directories and add C:\VulkanSDK\1.3.246.1\Lib or equivalent
- I decided to make it so you have to install the VulkanSDK and perform the above setup because the debug
- layers which are enabled on debug builds only work if we have the VulkanSDK installed anyway.
- We also need GLFW, which there's a copy of the prebuilt 64bit static binaries in the dependencies directory.
- Extract that file to somewhere and right click on X, click properties and add the include and lib directories...
- C++/General additional include directories and add C:\glfw-3.3.8.bin.WIN64\include or equivalent
- Linker/Additional Library Directories and add C:\glfw-3.3.8.bin.WIN64\lib-vc2022 or equivalent
**Current features**
- None lol (It DOES have a WinMain though!)
- 2013 05 27: My first Vulkan triangle.
![A screenshot of X display it's first Vulkan triangle.](https://github.com/DavidCradock/X/blob/200f84dbc67354e97e7b7fabf2b3106daf09888a/github_images/my_first_Vulkan_triangle.png)
