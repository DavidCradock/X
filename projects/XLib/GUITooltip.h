#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIText.h"
#include "GUILineGraph.h"
#include "GUIProgressBar.h"
#include "GUIImage.h"
#include "GUIImageAnimated.h"
#include "GUIImageFramebuffer.h"
#include "GUIImageDepthbuffer.h"

namespace X
{
	// Hovering over widgets may show a tooltip, which may be as simple as a line of text, or could hold for example, text, images,
	// animated images, progress bars, line graphs frame buffers and depth buffers.
	// All the widgets which are non-interactable can be placed inside a tooltip of a widget.
	// As creating a fancy tooltip with many widgets could be complicated and time consuming to program, there are helper functions
	// which create various "tooltip templates" such as "just text" or "left aligned text box with right animated image".
	class GUITooltip : public GUIBaseObject
	{
	public:
		GUITooltip();
		~GUITooltip();

		// Called from GUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from each object's update method to update this object
		void update(void* pParentContainer, GUIBaseObject* pGUIOwner, bool bMouseOverOwner);

		// Enable or disable this tooltip from being rendered
		void setEnabled(bool bEnabled);

		// Returns whether this tooltip is enabled or not
		bool getEnabled(void);

		// Add text to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		GUIText* addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIText* getText(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeText(const std::string& strName);

		// Add line graph to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		GUILineGraph* addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUILineGraph* getLineGraph(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeLineGraph(const std::string& strName);

		// Add progress bar to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		GUIProgressBar* addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIProgressBar* getProgressBar(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeProgressBar(const std::string& strName);

		// Add static image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strImageFilename is the name of a file used to create a ResourceTexture2D resource added to ResourceManager.
		// An image is simply that. It is not clickable. If you want an image which is clickable, use the GUIButtonImage widget
		// If strImageFilename couldn't be loaded, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		GUIImage* addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIImage* getImage(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImage(const std::string& strName);

		// Add animated image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// vecStrImageFilenames holds the names of each file used, for each frame to create a ResourceTexture2DAnimation resource added to ResourceManager.
		// If one of the images couldn't be loaded, an exception occurs
		// Each of the images need to be the same dimensions and have the same number of colour channels, otherwise an exception occurs.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the images.
		// The ResourceTexture2DAnimation in ResourceManager will have the same name as strName
		GUIImageAnimated* addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIImageAnimated* getImageAnimated(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageAnimated(const std::string& strName);

		// Add framebuffer image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strFBname is the name of the ResourceFramebuffer object resource added to ResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the frame buffer.
		GUIImageFramebuffer* addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIImageFramebuffer* getImageFramebuffer(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageFramebuffer(const std::string& strName);

		// Add depthbuffer image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strDBname is the name of the ResourceDepthbuffer object resource added to ResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the depth buffer.
		GUIImageDepthbuffer* addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIImageDepthbuffer* getImageDepthbuffer(const std::string& strName);

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageDepthbuffer(const std::string& strName);

		private:
			bool _mbEnabled;		// Whether this tooltip is enabled or not
			std::map<std::string, GUIText*> _mmapTexts;								// Hashmap for each added text
			std::map<std::string, GUILineGraph*> _mmapLineGraphs;					// Hashmap for each added line graph
			std::map<std::string, GUIProgressBar*> _mmapProgressBars;				// Hashmap for each added progress bar
			std::map<std::string, GUIImage*> _mmapImages;							// Hashmap for each added image
			std::map<std::string, GUIImageAnimated*> _mmapImageAnimateds;			// Hashmap for each added image animated
			std::map<std::string, GUIImageFramebuffer*> _mmapImageFramebuffers;		// Hashmap for each added image framebuffer 
			std::map<std::string, GUIImageDepthbuffer*> _mmapImageDepthbuffers;		// Hashmap for each added image depthbuffer 
	};
}