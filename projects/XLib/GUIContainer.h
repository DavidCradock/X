#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "GUIButton.h"
#include "GUIText.h"
#include "GUITextEdit.h"
#include "GUISlider.h"
#include "GUILineGraph.h"
#include "GUIProgressBar.h"
#include "GUIImage.h"
#include "GUIImageAnimated.h"

namespace X
{
	class GUIContainer : public GUIBaseObject
	{
	public:
		GUIContainer();

		// Render this container and each of it's objects
		void render(const std::string& strFramebufferToSampleFrom);

		// Updates this container, returning true if the mouse is over this container.
		// Will only return true if the mouse isn't over a previously updated container.
		bool update(bool bMouseIsOverContainerWhichIsAboveThisOne);

		std::string mstrThemename;	// Theme name used by this container
		bool mbContainerIsWindow;	// If true, this container is set as a window, which enables dragging and rendering of a window's borders, titlebar text etc
		
		// Sets whether this container is visible or not
		void setVisible(bool bVisible);

		// Returns whether this container is visible or not
		bool getVisible(void);

		// Add a button to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the button is the offset from the top left corner of the container's centre area not including the buttons edge images
		GUIButton* addButton(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);
		
		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIButton* getButton(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeButton(const std::string& strName);

		// Add text to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		GUIText* addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIText* getText(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeText(const std::string& strName);

		// Add text edit to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		GUITextEdit* addTextEdit(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUITextEdit* getTextEdit(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeTextEdit(const std::string& strName);

		// Add slider to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// If the width is greater than height, it is set as a horizontal slider, else vertical.
		// fTabRatio is a value which is multiplied by the width/height(Depending on orientation) of the slider's dims, to obtain tab dimensions
		GUISlider* addSlider(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio = 0.05f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUISlider* getSlider(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeSlider(const std::string& strName);

		// Add line graph to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		GUILineGraph* addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUILineGraph* getLineGraph(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeLineGraph(const std::string& strName);

		// Add progress bar to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		GUIProgressBar* addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIProgressBar* getProgressBar(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeProgressBar(const std::string& strName);

		// Add static image to this container and return a pointer to it
		// If the name already exists, an exception occurs
		// The position of the object is the offset from the top left corner of the container's centre area not including the container's edge images
		// strImageFilename is the name of a file used to create a ResourceTexture2D resource added to ResourceManager.
		// An image is simply that. It is not clickable. If you want an image which is clickable, use the GUIButtonImage widget
		// If strImageFilename couldn't be loaded, an exception occurs
		// Passing a value of less than zero to either fWidth or fHeight will set the widget to the size of the image.
		GUIImage* addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth = -1.0f, float fHeight = -1.0f);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		GUIImage* getImage(const std::string& strName);

		// Removes the named object from the container
		// If the named object doesn't exist, this silently fails
		void removeImage(const std::string& strName);

		std::string mstrTitleText;	// Title text
	private:
		bool _mbWindowBeingMoved;	// Whether this window is being moved or not
		
		std::map<std::string, GUIButton*> _mmapButtons;				// Hashmap for each added button
		std::map<std::string, GUIText*> _mmapTexts;					// Hashmap for each added text
		std::map<std::string, GUITextEdit*> _mmapTextEdits;			// Hashmap for each added text edit
		std::map<std::string, GUISlider*> _mmapSliders;				// Hashmap for each added slider
		std::map<std::string, GUILineGraph*> _mmapLineGraphs;		// Hashmap for each added line graph
		std::map<std::string, GUIProgressBar*> _mmapProgressBars;	// Hashmap for each added progress bar
		std::map<std::string, GUIImage*> _mmapImages;				// Hashmap for each added image

		glm::vec4 _mvTextColour;	// Current colour of the titlebar text
		bool _mbVisible;			// Whether this container is shown or not

		// Called from render() to render this container
		void _renderContainer(const std::string& strFramebufferToSampleFrom);

	};
}