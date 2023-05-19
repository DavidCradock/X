![X project logo. A character X, extruded slightly and placed against a quad within the 3D application Blender.](https://github.com/DavidCradock/X/blob/ed253cf27bfdadf152ca4013c0d976c04d54021c/github_social_image.png)
# My current personal project.
## Written in C++ for Windows 11, using Vulkan for rendering.
### It's early days, but I'm enjoying programming again after about a nine year gap due to a health issue and this project will develop into a nice little game development framework.
** Build help **
- Download the source as a zip and extract somewhere.
- Download and install Visual studio
- Download the Vulkan SDK and install from https://www.lunarg.com/vulkan-sdk/
- Within Solution Explorer, right click on X, click properties and add the Vulkan SDK include and lib dir...
- C++/General additional include directories and add c:\VulkanSDK\1.3.246.1\Include\ or equivalent
- Linker/Additional Library Directories and add C:\VulkanSDK\1.3.246.1\Lib or equivalent
**Current features**
- None lol (It DOES have a WinMain though, that exits straight away)
