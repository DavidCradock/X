#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIButton.h"
#include "GUIButtonImage.h"
#include "GUICheckbox.h"
#include "GUIImage.h"
#include "GUIImageAnimated.h"
#include "GUIImageDepthbuffer.h"
#include "GUIImageFramebuffer.h"
#include "GUILineGraph.h"
#include "GUIProgressBar.h"
#include "GUIText.h"
#include "GUITextEdit.h"
#include "GUITextScroll.h"
#include "GUITheme.h"
#include "GUISlider.h"

namespace X
{
	// This is a GUI object which is added to containers to hold other GUI objects.
	// If any added objects do not fit within the container, then
	// scroll bars are shown and used to scroll up/down/left/right to get them into view.
	class CGUISubContainer : public CGUIBaseObject
	{
		friend class CGUIContainer;
	public:
		CGUISubContainer();
		~CGUISubContainer();

		// Render this sub container and each of it's objects
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		// Removes all objects from this container
		void removeAll(void);

		/**************************************************************************************************************************************************
		Buttons
		**************************************************************************************************************************************************/

		// Add a button to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the button is the offset from the top left corner of the container's centre area not including the buttons edge images
		CGUIButton* addButton(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIButton* getButton(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeButton(const std::string& strName);

		// Removes all buttons from this container
		void removeAllButtons(void);

		/**************************************************************************************************************************************************
		Text
		**************************************************************************************************************************************************/

		// Add text to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		CGUIText* addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIText* getText(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeText(const std::string& strName);

		// Removes all text from this container
		void removeAllText(void);

		/**************************************************************************************************************************************************
		Text edit
		**************************************************************************************************************************************************/

		// Add text edit to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		CGUITextEdit* addTextEdit(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUITextEdit* getTextEdit(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeTextEdit(const std::string& strName);

		// Removes all text edits from this container
		void removeAllTextEdits(void);

		/**************************************************************************************************************************************************
		Slider
		**************************************************************************************************************************************************/

		// Add slider to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// If the width is greater than height, it is set as a horizontal slider, else vertical.
		// fTabRatio is a value which is multiplied by the width/height(Depending on orientation) of the slider's dims, to obtain tab dimensions
		CGUISlider* addSlider(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio = 0.05f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUISlider* getSlider(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeSlider(const std::string& strName);

		// Removes all sliders from this container
		void removeAllSliders(void);

		/**************************************************************************************************************************************************
		Line graph
		**************************************************************************************************************************************************/

		// Add line graph to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		CGUILineGraph* addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUILineGraph* getLineGraph(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeLineGraph(const std::string& strName);

		// Removes all line graphs from this container
		void removeAllLineGraphs(void);

		/**************************************************************************************************************************************************
		Progress bar
		**************************************************************************************************************************************************/

		// Add progress bar to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		CGUIProgressBar* addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIProgressBar* getProgressBar(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeProgressBar(const std::string& strName);

		// Removes all progress bars from this container
		void removeAllProgressBars(void);

		/**************************************************************************************************************************************************
		Image
		**************************************************************************************************************************************************/

		// Add static image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strImageFilename is the name of a file used to create a CResourceTexture2DFromFile resource added to SCResourceManager.
		// An image is simply that. It is not clickable. If you want an image which is clickable, use the CGUIButtonImage widget
		// If strImageFilename couldn't be loaded, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		CGUIImage* addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth = -1.0f, float fHeight = -1.0f);

		// Add static image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strCResourceTexture2DFromImage is the name of a CResourceTexture2DFromImage resource which you must add to SCResourceManager before calling this
		// An image is simply that. It is not clickable. If you want an image which is clickable, use the CGUIButtonImage widget
		// If image has zero dims, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		// Usage:
		// CImage image;
		// image.createBlank(512, 512, 4);
		// // Do more stuff to image here
		// x->pResource->addTexture2DFromImage("MyImageResource", image);
		// CGUIContainer* pContainer = x->pGUI->getContainer("container name");
		// pContainer->addImageFromImage("MyGUIImage", 0, 0, "MyImageResource");
		CGUIImage* addImageFromImage(const std::string& strName, float fPosX, float fPosY, const std::string& strCResourceTexture2DFromImage, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImage* getImage(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		// If the image object was added with addImage() then this removes the CResourceTexture2DFromFile resource from the file manager
		// If the image object was added with addImageFromImage(), please remember, to remove to resource yourself from the resource manager.
		void removeImage(const std::string& strName);

		// Removes all images from this container
		// If the image object was added with addImage() then this removes the CResourceTexture2DFromFile resource from the file manager
		// If the image object was added with addImageFromImage(), please remember, to remove to resource yourself from the resource manager.
		void removeAllImages(void);

		/**************************************************************************************************************************************************
		Image animated
		**************************************************************************************************************************************************/

		// Add animated image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// vecStrImageFilenames holds the names of each file used, for each frame to create a CResourceTexture2DAnimation resource added to SCResourceManager.
		// If one of the images couldn't be loaded, an exception occurs
		// Each of the images need to be the same dimensions and have the same number of colour channels, otherwise an exception occurs.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the images.
		// The CResourceTexture2DAnimation in SCResourceManager will have the same name as strName
		CGUIImageAnimated* addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageAnimated* getImageAnimated(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeImageAnimated(const std::string& strName);

		// Removes all animated images from this container
		void removeAllImageAnimateds(void);

		/**************************************************************************************************************************************************
		Frame buffers
		**************************************************************************************************************************************************/

		// Add framebuffer image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strFBname is the name of the CResourceFramebuffer object resource added to SCResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the frame buffer.
		CGUIImageFramebuffer* addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageFramebuffer* getImageFramebuffer(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeImageFramebuffer(const std::string& strName);

		// Removes all image framebuffers from this container
		void removeAllImageFramebuffers(void);

		/**************************************************************************************************************************************************
		Text scroll
		**************************************************************************************************************************************************/

		// Add text scroll object to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// Please note: The name should be unique for all text scroll objects added to this container as it is used to create the unique framebuffer resource.
		// If the name is not unique, an exception occurs.
		CGUITextScroll* addTextScroll(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUITextScroll* getTextScroll(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeTextScroll(const std::string& strName);

		// Removes all text scrolls from this container
		void removeAllTextScrolls(void);

		/**************************************************************************************************************************************************
		Button image
		**************************************************************************************************************************************************/

		// Add button image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strImageFilenameUp/Over/Down are the name of a files used to create the CResourceTexture2DFromFile resource added to SCResourceManager.
		// If the images couldn't be loaded, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		CGUIButtonImage* addButtonImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilenameUp, const std::string& strImageFilenameOver, const std::string& strImageFilenameDown, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIButtonImage* getButtonImage(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeButtonImage(const std::string& strName);

		// Removes all button images from this container
		void removeAllButtonImages(void);

		/**************************************************************************************************************************************************
		Depth buffers
		**************************************************************************************************************************************************/

		// Add depthbuffer image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strDBname is the name of the CResourceDepthbuffer object resource added to SCResourceManager.
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the depth buffer.
		CGUIImageDepthbuffer* addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUIImageDepthbuffer* getImageDepthbuffer(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeImageDepthbuffer(const std::string& strName);

		// Removes all image depthbuffers from this container
		void removeAllImageDepthbuffers(void);

		/**************************************************************************************************************************************************
		Check boxes
		**************************************************************************************************************************************************/

		// Add a checkbox to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the checkbox is the offset from the top left corner of the container's centre area
		CGUICheckbox* addCheckbox(const std::string& strName, float fPosX, float fPosY);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUICheckbox* getCheckbox(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeCheckbox(const std::string& strName);

		// Removes all check boxes from this container
		void removeAllCheckboxes(void);

		/**************************************************************************************************************************************************
		Sub containers
		**************************************************************************************************************************************************/

		// Add a sub container to this container and return a pointer to it
		// If the name already exists, an exception occurs
		CGUISubContainer* addSubContainer(const std::string& strName, float fPosX, float fPosY, float fDimsX, float fDimsY);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		CGUISubContainer* getSubContainer(const std::string& strName) const;

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeSubContainer(const std::string& strName);

		// Removes all sub containers from this container
		void removeAllSubContainers(void);
	private:
		bool _mbVisible;			// Whether this sub container is shown or not
		std::string _mstrName;		// The name of the container, used to generate unique names for resources

		mutable std::map<std::string, CGUIButton*> _mmapButtons;						// Hashmap for each added button
		mutable std::map<std::string, CGUIText*> _mmapTexts;							// Hashmap for each added text
		mutable std::map<std::string, CGUITextEdit*> _mmapTextEdits;					// Hashmap for each added text edit
		mutable std::map<std::string, CGUISlider*> _mmapSliders;						// Hashmap for each added slider
		mutable std::map<std::string, CGUILineGraph*> _mmapLineGraphs;					// Hashmap for each added line graph
		mutable std::map<std::string, CGUIProgressBar*> _mmapProgressBars;				// Hashmap for each added progress bar
		mutable std::map<std::string, CGUIImage*> _mmapImages;							// Hashmap for each added image
		mutable std::map<std::string, CGUIImageAnimated*> _mmapImageAnimateds;			// Hashmap for each added image animated
		mutable std::map<std::string, CGUIImageFramebuffer*> _mmapImageFramebuffers;	// Hashmap for each added image framebuffer 
		mutable std::map<std::string, CGUITextScroll*> _mmapTextScrolls;				// Hashmap for each added text scroll
		mutable std::map<std::string, CGUIButtonImage*> _mmapButtonImages;				// Hashmap for each added button image
		mutable std::map<std::string, CGUIImageDepthbuffer*> _mmapImageDepthbuffers;	// Hashmap for each added image depthbuffer
		mutable std::map<std::string, CGUICheckbox*> _mmapCheckboxes;					// Hashmap for each added check box
		mutable std::map<std::string, CGUISubContainer*> _mmapSubContainers;			// Hashmap for each added sub container.
	};
}