#pragma once
#include "PCH.h"
#include "2DEntity.h"
#include "2DEntityComplex.h"

namespace X
{

	class C2DLayer
	{
	public:
		C2DLayer();
		~C2DLayer();

		// Add a new named object
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DEntity* addEntity(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getEntityExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntity* getEntity(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntity* getEntity(unsigned int uiIndex);

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeEntity(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeEntity(unsigned int uiIndex);

		// Removes all objects
		void removeAllEntitys(void);

		// Returns the total number of added objects
		int getNumEntitys(void);

		// Add a new named object
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DEntityComplex* addEntityComplex(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getEntityComplexExists(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntityComplex* getEntityComplex(const std::string& strUniqueName);

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntityComplex* getEntityComplex(unsigned int uiIndex);

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeEntityComplex(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeEntityComplex(unsigned int uiIndex);

		// Removes all objects
		void removeAllEntityComplexs(void);

		// Returns the total number of added objects
		int getNumEntityComplexs(void);
	private:
		std::map<std::string, C2DEntity*> _mmapEntitys;					// Each named entity
		std::map<std::string, C2DEntityComplex*> _mmapEntityComplexs;	// Each named entity complex
	};
}