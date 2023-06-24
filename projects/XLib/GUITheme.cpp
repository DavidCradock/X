#include "PCH.h"
#include "GUITheme.h"
#include "utilities.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	GUITheme::GUITheme()
	{
		mstrThemeName = "Default theme";

		// Images
		mImages.containerBlur =		"data/X/GUI/default/container_blur.png";
		mImages.containerColour =	"data/X/GUI/default/container_colour.png";
		mImages.containerGlow =		"data/X/GUI/default/container_glow.png";
		mImages.containerNormal =	"data/X/GUI/default/container_normal.png";
		mImages.reflection =		"data/X/GUI/default/reflection.png";

		// Colours
		mColours.containerTitlebarTextInFocus.set(1.0f, 1.0f, 1.0f, 1.0f);
		mColours.containerTitlebarTextNotInFocus.set(0.9f, 0.9f, 0.9f, 0.9f);

		mfBlurAmount = 0.5f;
		mfGlowAmount = 0.5f;
		mfNormalAmount = 0.5f;
		mfReflectionAmount = 0.5f;
	}

	void GUITheme::load(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = addFilenameExtension(".theme", strFilename);

		// Open file in text mode
		std::ifstream file;
		file.open(filename, std::ofstream::in);
		ThrowIfFalse(file.is_open(), "GUITheme::load(" + filename + ") failed to open file for reading.");

		// Filename
		std::string strWord;
		std::string strLine;
		char space;
		getline(file, strLine);

		// Theme name
		file >> strWord;	// "ThemeName:"
		space = file.get();	// Space
		getline(file, mstrThemeName);	// Themename

		// Images
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nImages\n"
		file >> strWord;	space = file.get();	getline(file, mImages.containerBlur);	// Description text, space character and the filename
		file >> strWord;	space = file.get();	getline(file, mImages.containerColour);
		file >> strWord;	space = file.get();	getline(file, mImages.containerGlow);
		file >> strWord;	space = file.get();	getline(file, mImages.containerNormal);
		file >> strWord;	space = file.get();	getline(file, mImages.reflection);

		// Colours
		getline(file, strLine);	// Empty line
		getline(file, strLine);	//"\nColours\n"
		_colourReadValues(file, mColours.containerTitlebarTextInFocus);
		_colourReadValues(file, mColours.containerTitlebarTextNotInFocus);
		getline(file, strLine);	// End of line

		getline(file, strLine);	// Empty line

		getline(file, strLine);	//"\nEffects\n"
		file >> strWord;	space = file.get();	file >> mfBlurAmount;
		file >> strWord;	space = file.get();	file >> mfGlowAmount;
		file >> strWord;	space = file.get();	file >> mfNormalAmount;
		file >> strWord;	space = file.get();	file >> mfReflectionAmount;

		ThrowIfTrue(file.fail(), "GUITheme::load(" + filename + ") failed whilst loading file.");
		file.close();
	}

	void GUITheme::save(const std::string& strFilename)
	{
		// Make sure filename given has the required extension and set to lowercase
		std::string filename = addFilenameExtension(".theme", strFilename);
		
		// Open file in text mode, deleting contents if existed before
		std::ofstream file;
		file.open(filename, std::ofstream::out | std::ofstream::trunc);
		ThrowIfFalse(file.is_open(), "GUITheme::save(" + filename + ") failed to open file for writing.");
		
		// Filename
		file << filename + "\n";

		// Theme name
		file << "ThemeName: " + mstrThemeName + "\n";

		// Images
		file << "\nImages\n";
		file << "ContainerBlur: " + mImages.containerBlur + "\n";
		file << "ContainerColour: " + mImages.containerColour + "\n";
		file << "ContainerGlow: " + mImages.containerGlow + "\n";
		file << "ContainerNormal: " + mImages.containerNormal + "\n";
		file << "Reflection: " + mImages.reflection + "\n";

		// Colours
		file << "\nColours\n";
		file << "containerTitlebarTextInFocus: " + _colourToString(mColours.containerTitlebarTextInFocus) + "\n";
		file << "containerTitlebarTextNotInFocus: " + _colourToString(mColours.containerTitlebarTextNotInFocus) + "\n";

		file << "\nEffects\n";
		file << "blurAmount: " + std::to_string(mfBlurAmount) + "\n";
		file << "glowAmount: " + std::to_string(mfGlowAmount) + "\n";
		file << "normalAmount: " + std::to_string(mfNormalAmount) + "\n";
		file << "reflectionAmount: " + std::to_string(mfReflectionAmount) + "\n";

		// Make sure there were no errors
		ThrowIfTrue(file.fail(), "GUITheme::save(" + filename + ") failed whilst saving file.");
		file.close();
	}

	void GUITheme::loadTextures(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->addTexture2D(mImages.containerBlur, mImages.containerBlur);
		pRM->addTexture2D(mImages.containerColour, mImages.containerColour);
		pRM->addTexture2D(mImages.containerGlow, mImages.containerGlow);
		pRM->addTexture2D(mImages.containerNormal, mImages.containerNormal);
		pRM->addTexture2D(mImages.reflection, mImages.reflection);
	}

	void GUITheme::unloadTextures(void)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		pRM->removeTexture2D(mImages.containerBlur);
		pRM->removeTexture2D(mImages.containerColour);
		pRM->removeTexture2D(mImages.containerGlow);
		pRM->removeTexture2D(mImages.containerNormal);
		pRM->removeTexture2D(mImages.reflection);
	}

	std::string GUITheme::_colourToString(GUIColour& colour)
	{
		std::string str;
		str = std::to_string(colour.red) + " " + std::to_string(colour.green) + " " + std::to_string(colour.blue) + " " + std::to_string(colour.alpha);
		return str;
	}

	void GUITheme::_colourReadValues(std::ifstream& stream, GUIColour& colour)
	{
		static char space;			// For storing a space character
		static std::string strWord;	// For storing a single word
		stream >> strWord;			// Description text (For example, containerTitlebarTextInFocus:)
		space = stream.get();		// Space character

		// Each colour value
		stream >> colour.red;
		stream >> colour.green;
		stream >> colour.blue;
		stream >> colour.alpha;
	}

}