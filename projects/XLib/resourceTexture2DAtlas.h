#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "image.h"
#include "imageAtlas.h"
#include "colour.h"

namespace X
{
	// Upon construction, this is given a vector of strings holding each image filename to be added to one or more image atlases
	// This reduces texture re-binding
	// The atlas images are stored in memory so we can quickly reload onGLContextCreated()
	// This is NOT designed for holding and playing back large animation files, it's for animated sprites and such.
	// Images are sorted by dimensions when being placed inside the atlases, they are unsorted if they are all the same dimensions
	class CResourceTexture2DAtlas : public CResourceBase
	{
	public:
		CResourceTexture2DAtlas(const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages, unsigned int uiImageSpacing = 1, bool bFilteringNearest = false);
		~CResourceTexture2DAtlas();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Binds the correct atlas texture to the given texture unit, depending upon given image number
		// uiTextureUnit should be between 0-7
		// uiImageNumber should be a valid image number otherwise an exception occurs
		void bind(unsigned int uiTextureUnit = 0, unsigned int uiImageNumber = 0) const;

		// Binds the correct atlas texture to the given texture unit, depending upon given image name
		// uiTextureUnit should be between 0-7
		// strImageName should be a valid, otherwise an exception occurs
		void bind(unsigned int uiTextureUnit, const std::string& strImageName) const;

		// Binds the specified atlas texture index
		// uiTextureUnit should be between 0-7
		// uiAtlasImageNumber should be a valid image number otherwise an exception occurs
		void bindAtlas(unsigned int uiTextureUnit = 0, unsigned int uiAtlasImageNumber = 0) const;

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbind(unsigned int uiTextureUnit = 0) const;

		// Unbind all texture units
		void unbindAll(void) const;

		// Return total number of images in atlases
		unsigned int getNumImages(void) const;

		// Return total number of atlases used to contain each of the images
		unsigned int getNumAtlases(void) const;

		// Sets passed vecs to hold min and max texture coordinates within atlas texture for the specified image number
		// If an invalid image number is given, an exception occurs.
		void getTextureCoords(unsigned int uiImageNumber, CVector2f& vTCMin, CVector2f& vTCMax) const;

		// Sets passed vectors to hold texture coordinates within atlas texture for the specified image number
		// If an invalid image number is given, an exception occurs.
		void getTextureCoords(unsigned int uiImageNumber, CVector2f& vtcTopLeft, CVector2f& vtcTopRight, CVector2f& vtcBottomRight, CVector2f& vtcBottomLeft) const;

		// Sets passed vecs to hold min and max texture coordinates within atlas texture for the specified image name
		// If an invalid image name is given, an exception occurs.
		void getTextureCoords(const std::string& strImageName, CVector2f& vTCMin, CVector2f& vTCMax) const;

		// Sets passed vectors to hold texture coordinates within atlas texture for the specified image name
		// If an invalid image name is given, an exception occurs.
		void getTextureCoords(const std::string& strImageName, CVector2f& vtcTopLeft, CVector2f& vtcTopRight, CVector2f& vtcBottomRight, CVector2f& vtcBottomLeft) const;

		// Get dimensions of the image number
		// If an invalid image number is given, an exception occurs.
		CVector2f getImageDims(unsigned int uiImageNumber) const;

		// Get dimensions of the image name
		// If an invalid image name is given, an exception occurs.
		CVector2f getImageDims(const std::string& strImageName) const;

		// Returns the filename the given image number was created from.
		// If an invalid image number is given, an exception occurs.
		std::string getImageFilename(unsigned int uiImageNumber) const;

		// Returns whether when adding the image to the atlas, it was rotated or not
		// If an invalid image number is given, an exception occurs.
		bool getImageWasRotated(unsigned int uiImageNumber) const;

		// Returns whether when adding the image to the atlas, it was rotated or not
		// If an invalid image name is given, an exception occurs.
		bool getImageWasRotated(const std::string& strImageName) const;

		// Returns which atlas image the given image number is stored in
		// If an invalid image number is given, an exception occurs.
		unsigned int getImageStoredInAtlasNumber(unsigned int uiImageNumber) const;

		// Returns which atlas image the given image name is stored in
		// If an invalid image name is given, an exception occurs.
		unsigned int getImageStoredInAtlasNumber(const std::string& strImageName) const;

		// Returns image name at specified index;
		// If an invalid index is given, an exception occurs
		std::string getImageNameAtIndex(unsigned int uiImageNumber) const;

		// Returns image details of the named image name using the CImageAtlasPacker's hashmap
		// If an invalid image name is given, an exception occurs
		CImageAtlasDetails getImageDetails(const std::string& strImageName) const;

		// Returns a pointer to the named image's details
		// If an invalid image name is given, an exception occurs
		CImageAtlasDetails* getImageDetailsPointer(const std::string& strImageName) const;

		// Returns OpenGL texture ID for the named image stored within the texture atlas.
		// As an atlas can have more than one texture to store all of it's images, different images within an atlas may have a different texture ID
		// If an invalid image name is given, an exception occurs.
		unsigned int getImageTextureID(const std::string& strImageName) const;

		// Returns whether the named image name exists or not
		bool getImageNameExists(const std::string& strImageName) const;

		// Renders the contents of this texture atlas to a 2D quad with specified position and dimensions
		// uiAtlasImageNumber should be a valid number (Use getNumAtlases()) otherwise an exception occurs
		void renderAtlasTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, unsigned int uiAtlasImageNumber, CColour colour = CColour());

		// TODO Renders the named image stored in the atlas at the specified position and dimensions.
		// If dimensions are <= 0, then the dimensions of the image are used.
		// If the named image does not exist, an exception occurs
		void renderAtlasImageTo2dQuad(int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strImageInAtlasName, CColour colour = CColour());

		// Returns a pointer to the CImageAtlasPacker object so we can access the images
		CImageAtlasPacker* getImageAtlasPacker(void);
	private:
		mutable CImageAtlasPacker _mAtlases;			// Holds one or more atlas images containing all images
		std::vector<unsigned int> _mvAtlasTextureIDs;	// OpenGL texture ID for each generated atlas texture holding the smaller images.
		bool _mbFilteringNearest;						// Whether when generating the textures, we use nearest filtering or mipmipping
	};
}