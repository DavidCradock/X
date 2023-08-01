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
	class CGUITooltip : public CGUIBaseObject
	{
	public:
		CGUITooltip();
		~CGUITooltip();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from each object's update method to update this object
		void update(void* pParentContainer, CGUIBaseObject* pGUIOwner, bool bMouseOverOwner);

		// Enable or disable this tooltip from being rendered
		void setEnabled(bool bEnabled);

		// Returns whether this tooltip is enabled or not
		bool getEnabled(void) const;

		// Set this tooltip to just contain text with the dimensions set to the width/height of the text and the tooltip border.
		// Removes all previously added objects
		// Also sets the tooltip as enabled.
		// Allows "\n" in the given string to start a newline.
		void setAsText(const std::string& strText);

		// Add text to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		CGUIText* addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIText* getText(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeText(const std::string& strName);

		// Removes all text from this tooltip
		void removeAllText(void);

		// Add line graph to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		CGUILineGraph* addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUILineGraph* getLineGraph(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeLineGraph(const std::string& strName);

		// Removes all line graphs from this tooltip
		void removeAllLineGraphs(void);

		// Add progress bar to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		CGUIProgressBar* addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIProgressBar* getProgressBar(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeProgressBar(const std::string& strName);

		// Removes all progress bars from this tooltip
		void removeAllProgressBars(void);

		// Add static image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strImageFilename is the name of a file used to create a CResourceTexture2DFromFile resource added to SCResourceManager.
		// An image is simply that. It is not clickable. If you want an image which is clickable, use the CGUIButtonImage widget
		// If strImageFilename couldn't be loaded, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		CGUIImage* addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImage* getImage(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImage(const std::string& strName);

		// Removes all images from this tooltip
		void removeAllImages(void);

		// Add animated image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// vecStrImageFilenames holds the names of each file used, for each frame to create a CResourceTexture2DAnimation resource added to SCResourceManager.
		// If one of the images couldn't be loaded, an exception occurs
		// Each of the images need to be the same dimensions and have the same number of colour channels, otherwise an exception occurs.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the images.
		// The CResourceTexture2DAnimation in SCResourceManager will have the same name as strName
		CGUIImageAnimated* addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageAnimated* getImageAnimated(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageAnimated(const std::string& strName);

		// Removes all animated images from this tooltip
		void removeAllImagesAnimated(void);

		// Add framebuffer image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strFBname is the name of the CResourceFramebuffer object resource added to SCResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the frame buffer.
		CGUIImageFramebuffer* addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageFramebuffer* getImageFramebuffer(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageFramebuffer(const std::string& strName);

		// Removes all image framebuffers from this tooltip
		void removeAllImageFramebuffers(void);

		// Add depthbuffer image to this tooltip and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the tooltip's centre area not including the tooltip's edge images
		// strDBname is the name of the CResourceDepthbuffer object resource added to SCResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the depth buffer.
		CGUIImageDepthbuffer* addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageDepthbuffer* getImageDepthbuffer(const std::string& strName) const;

		// Removes the named object from the tooltip
		// If the named object doesn't exist, this silently fails
		void removeImageDepthbuffer(const std::string& strName);

		// Removes all image depthbuffers from this tooltip
		void removeAllImageDepthbuffers(void);

		// Removes all objects from this tooltip
		void removeAll(void);

		// Resets all fade values back to non-visible
		void resetFade(void);

	private:
		CTimer _mTimer;			// Timer object used for timebased stuff
		bool _mbEnabled;		// Whether this tooltip is enabled or not
		CColour _mColour;		// Current colour of the tooltip, used for fading in/out
		float _mfTooltipDelay;	// Used to prevent tooltip from fading in until SCGUIManager::getTooltipDelay() has passed
		mutable std::map<std::string, CGUIText*> _mmapTexts;							// Hashmap for each added text
		mutable std::map<std::string, CGUILineGraph*> _mmapLineGraphs;					// Hashmap for each added line graph
		mutable std::map<std::string, CGUIProgressBar*> _mmapProgressBars;				// Hashmap for each added progress bar
		mutable std::map<std::string, CGUIImage*> _mmapImages;							// Hashmap for each added image
		mutable std::map<std::string, CGUIImageAnimated*> _mmapImageAnimateds;			// Hashmap for each added image animated
		mutable std::map<std::string, CGUIImageFramebuffer*> _mmapImageFramebuffers;	// Hashmap for each added image framebuffer 
		mutable std::map<std::string, CGUIImageDepthbuffer*> _mmapImageDepthbuffers;	// Hashmap for each added image depthbuffer 
	};
}