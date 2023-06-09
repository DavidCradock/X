#include "PCH.h"
#include "resourceTriangle.h"
#include "log.h"

namespace X
{

	CResourceTriangle::CResourceTriangle()
	{
		vertexBufferObject = 0;
		vertexArrayObject = 0;
		elementBufferObject = 0;
		//onGLContextCreated();
	}

	CResourceTriangle::~CResourceTriangle()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceTriangle::onGLContextCreated(void)
	{
		update();
	}

	void CResourceTriangle::onGLContextToBeDestroyed(void)
	{
		if (vertexBufferObject)
		{
			glDeleteBuffers(1, &vertexBufferObject);
			vertexBufferObject = 0;
		}
		if (vertexArrayObject)
		{
			glDeleteVertexArrays(1, &vertexArrayObject);
			vertexArrayObject = 0;
		}
		if (elementBufferObject)
		{
			glDeleteBuffers(1, &elementBufferObject);
			elementBufferObject = 0;
		}
	}

	void CResourceTriangle::removeGeom(void)
	{
		vertices.clear();
		indices.clear();
	}

	void CResourceTriangle::_computeTangentsAndBinormals(void)
	{
		if (!vertices.size())
			return;

		// For each face
		for (unsigned int iIndex = 0; iIndex < indices.size(); iIndex += 3)
		{
			// Calculate the face's edges and delta UV coordinates
			glm::vec3 edge1 = vertices[indices[iIndex+1]].position - vertices[indices[iIndex]].position;
			glm::vec3 edge2 = vertices[indices[iIndex+2]].position - vertices[indices[iIndex]].position;
			glm::vec2 deltaUV1 = vertices[indices[iIndex+1]].texCoord - vertices[indices[iIndex]].texCoord;
			glm::vec2 deltaUV2 = vertices[indices[iIndex+2]].texCoord - vertices[indices[iIndex]].texCoord;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			vertices[indices[iIndex]].tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			vertices[indices[iIndex]].tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			vertices[indices[iIndex]].tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			vertices[indices[iIndex]].binormal.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			vertices[indices[iIndex]].binormal.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			vertices[indices[iIndex]].binormal.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		}

	}

	void CResourceTriangle::update(void)
	{
		if (!vertices.size())
			return;
		if (!indices.size())
			return;

		_computeTangentsAndBinormals();

		if (!vertexBufferObject)
			glGenBuffers(1, &vertexBufferObject);
		if (!vertexArrayObject)
			glGenVertexArrays(1, &vertexArrayObject);
		if (!elementBufferObject)
			glGenBuffers(1, &elementBufferObject);

		glBindVertexArray(vertexArrayObject);

		// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
		// GL_STATIC_DRAW: the data is set only once and used many times.
		// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.

		// Bind VBO and upload vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		// Indicies
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// Position
		glVertexAttribPointer(0,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			3,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)0);						// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(0);

		// Colour
		glVertexAttribPointer(1,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			4,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(3 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(1);

		// Texture coordinates
		glVertexAttribPointer(2,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			2,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(7 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(2);

		// Normal
		glVertexAttribPointer(3,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			3,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(9 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(3);

		// Tangent
		glVertexAttribPointer(4,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			3,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(12 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(4);

		// Binormal
		glVertexAttribPointer(5,			// Index. Specifies the index in the shader of the generic vertex attribute to be modified.
			3,								// Size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4.
			GL_FLOAT,						// Type. Specifies the data type of each component in the array. The symbolic constants GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, GL_UNSIGNED_INT_2_10_10_10_REV, GL_UNSIGNED_INT_10F_11F_11F_REV, GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT and GL_UNSIGNED_INT are accepted
			GL_FALSE,						// Normalized. Specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(Vertex),					// Stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)(15 * sizeof(float)));	// Pointer. Specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.
		glEnableVertexAttribArray(5);

		// Unbind stuff as we're done
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void CResourceTriangle::draw(bool bWireframeMode) const
	{
		if (!vertexArrayObject)
			return;

		if (0 == indices.size())
			return;

		if (0 == vertices.size())
			return;

		if (bWireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glBindVertexArray(vertexArrayObject);
		glDrawElements(
			GL_TRIANGLES,				// Mode. Specifies what kind of primitives to render. Symbolic constants GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY, GL_TRIANGLES_ADJACENCY and GL_PATCHES are accepted.
			(GLsizei)indices.size(),	// Count. Specifies the number of elements to be rendered.
			GL_UNSIGNED_INT,			// Type. Specifies the type of the values in indices. Must be one of GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT.
			0);							// Indicies. Specifies a pointer to the location where the indices are stored. NOTE: We're using element buffer objects and using the indicies in that, so this is 0.
		glBindVertexArray(0);
	}

	void CResourceTriangle::addVertex(const Vertex& newVertex)
	{
		vertices.push_back(newVertex);
	}

	void CResourceTriangle::addIndex(int newIndex)
	{
		indices.push_back(newIndex);
	}

	void CResourceTriangle::addQuad2D(const glm::vec2& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour, const glm::vec2& textureCoordinateBottomLeft, const glm::vec2& textureCoordinateBottomRight, const glm::vec2& textureCoordinateTopRight, const glm::vec2& textureCoordinateTopLeft)
	{
		// Indicies
		unsigned int iIndex = (unsigned int)vertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		// Bottom left
		Vertex vertex;	// Holds a vertex's data before adding to the vector
		vertex.position.x = vPosition.x;
		vertex.position.y = vPosition.y + vDimensions.y;
		vertex.position.z = 0.0f;
		vertex.colour = colour;
		vertex.texCoord = textureCoordinateBottomLeft;
		addVertex(vertex);

		// Top left
		vertex.position.y = vPosition.y;
		vertex.texCoord = textureCoordinateTopLeft;
		addVertex(vertex);

		// Top right
		vertex.position.x = vPosition.x + vDimensions.x;
		vertex.texCoord = textureCoordinateTopRight;
		addVertex(vertex);

		// Bottom right
		vertex.position.y = vPosition.y + vDimensions.y;
		vertex.texCoord = textureCoordinateBottomRight;
		addVertex(vertex);
	}

	void CResourceTriangle::addQuad2D(const CVector2r& vPosition, CVector2r& vDimensions, const CColour& colour, const CVector2r& textureCoordinateBottomLeft, const CVector2r& textureCoordinateBottomRight, const CVector2r& textureCoordinateTopRight, const CVector2r& textureCoordinateTopLeft)
	{
		// Indicies
		unsigned int iIndex = (unsigned int)vertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		// Bottom left
		Vertex vertex;	// Holds a vertex's data before adding to the vector
		vertex.position.x = (float)vPosition.x;
		vertex.position.y = float(vPosition.y + vDimensions.y);
		vertex.position.z = 0.0f;
		vertex.colour.r = colour.red;
		vertex.colour.g = colour.green;
		vertex.colour.b = colour.blue;
		vertex.colour.a = colour.alpha;
		vertex.texCoord.x = (float)textureCoordinateBottomLeft.x;
		vertex.texCoord.y = (float)textureCoordinateBottomLeft.y;
		addVertex(vertex);

		// Top left
		vertex.position.y = (float)vPosition.y;
		vertex.texCoord.x = (float)textureCoordinateTopLeft.x;
		vertex.texCoord.y = (float)textureCoordinateTopLeft.y;
		addVertex(vertex);

		// Top right
		vertex.position.x = float(vPosition.x + vDimensions.x);
		vertex.texCoord.x = (float)textureCoordinateTopRight.x;
		vertex.texCoord.y = (float)textureCoordinateTopRight.y;
		addVertex(vertex);

		// Bottom right
		vertex.position.y = float(vPosition.y + vDimensions.y);
		vertex.texCoord.x = (float)textureCoordinateBottomRight.x;
		vertex.texCoord.y = (float)textureCoordinateBottomRight.y;
		addVertex(vertex);
	}

	void CResourceTriangle::addGroundplane(const glm::vec3& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour, const glm::vec2& textureCoordinateFrontLeft, const glm::vec2& textureCoordinateFrontRight, const glm::vec2& textureCoordinateBackRight, const glm::vec2& textureCoordinateBackLeft)
	{
		// Indicies
		unsigned int iIndex = (unsigned int)vertices.size();
		addIndex(iIndex);		// BL
		addIndex(iIndex + 1);	// TL
		addIndex(iIndex + 2);	// TR

		addIndex(iIndex + 2);	// TR
		addIndex(iIndex + 3);	// BR
		addIndex(iIndex);		// BL

		glm::vec2 vHalfDims = vDimensions;
		vHalfDims *= 0.5f;

		// Front left
		Vertex vertex;	// Holds a vertex's data before adding to the vector
		vertex.position.x = vPosition.x + vHalfDims.x;
		vertex.position.y = vPosition.y;
		vertex.position.z = vPosition.z + vHalfDims.y;
		vertex.colour = colour;
		vertex.texCoord = textureCoordinateFrontLeft;
		addVertex(vertex);

		// Back left
		vertex.position.z = vPosition.z - vHalfDims.y;
		vertex.texCoord = textureCoordinateBackLeft;
		addVertex(vertex);

		// Back right
		vertex.position.x = vPosition.x - vHalfDims.x;
		vertex.texCoord = textureCoordinateBackRight;
		addVertex(vertex);

		// Front right
		vertex.position.z = vPosition.z + vHalfDims.x;
		vertex.texCoord = textureCoordinateFrontRight;
		addVertex(vertex);
	}

	void CResourceTriangle::addFromFile(const std::string& strGeometryFilename, bool bCallUpdate)
	{
		FILE* file = 0;
		errno_t err;

		err = fopen_s(&file, strGeometryFilename.c_str(), "r+b");
		ThrowIfTrue(bool(err != 0), "CResourceTriangle::addFromFile() failed to open file " + strGeometryFilename + " for loading.");

		// Read in number of vertices
		unsigned int iNumVertices = 0;
		size_t read = fread(&iNumVertices, sizeof(unsigned int), 1, file);
		ThrowIfTrue(bool(0 == read), "CResourceTriangle::addFromFile() failed to read from file " + strGeometryFilename);

		// Read in a vertex at a time, adding one at a time..
		/*
		Vertex vertex;
		for (unsigned int i = 0; i < iNumVertices; i++)
		{
			read = fread(&vertex, sizeof(Vertex), 1, file);
			ThrowIfTrue(bool(0 == read), "CResourceTriangle::addFromFile() failed to read from file " + strGeometryFilename);
			addVertex(vertex);
			addIndex(i);
		}
		*/
		// Load in all vertex data
		vertices.resize(iNumVertices);
		indices.resize(iNumVertices);
		ThrowIfTrue(iNumVertices != fread(vertices.data(), sizeof(Vertex), iNumVertices, file), "CResourceTriangle::addFromFile() failed to read from file " + strGeometryFilename);
		vertices.resize(iNumVertices);
		for (unsigned int i = 0; i < iNumVertices; i++)
		{
			indices.push_back(i);
		}

		fclose(file);

		if (bCallUpdate)
			update();
	}

	size_t CResourceTriangle::getNumVertices(void) const
	{
		return vertices.size();
	}

	size_t CResourceTriangle::getNumIndicies(void) const
	{
		return indices.size();
	}

	void CResourceTriangle::convertObj(const std::string filename) const
	{
		// Used to temporarily hold each line of data from file
		glm::vec3 vertex;
		glm::vec3 normal;
		glm::vec2 texCoord;
		unsigned int index[9];	// Index to vertex, texcoord, normal

		// Used to temporarily hold all unique items from the file
		std::vector<glm::vec3> vVertices;
		std::vector<glm::vec3> vNormals;
		std::vector<glm::vec2> vTexCoords;
		std::vector<unsigned int> vIndicesVertices;
		std::vector<unsigned int> vIndicesNormals;
		std::vector<unsigned int> vIndicesTexCoords;

		FILE* file = 0;
		errno_t err;
		err = fopen_s(&file, filename.c_str(), "r");
		ThrowIfTrue(bool(err != 0), "CResourceTriangle::convertObj() failed to open file " + filename);

		char strLine[255] = { 0 };
		char ch = 0;
		// Go through entire file, loading everything to the above vectors
		while (!feof(file))
		{
			// Get the beginning character of the current line in the file
			ch = fgetc(file);

			if ('v' == ch)	// Could be v(vertex), vn(normal or vt(texcoord)
			{
				ch = fgetc(file);
				if (' ' == ch)	// v(vertex)
				{
					fscanf_s(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
					fgets(strLine, 255, file);	// Read the rest of the line
					vVertices.push_back(vertex);
				}
				else if ('n' == ch)	// n(normal)
				{
					fscanf_s(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
					fgets(strLine, 255, file);	// Read the rest of the line
					vNormals.push_back(normal);
				}
				else if ('t' == ch)	// t(texcoord)
				{
					fscanf_s(file, "%f %f", &texCoord.x, &texCoord.y);
					fgets(strLine, 255, file);	// Read the rest of the line
					vTexCoords.push_back(texCoord);
				}
			}
			else if ('f' == ch)	// f vertex/texcoord/normal indicies
			{
				fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &index[0], &index[1], &index[2], &index[3], &index[4], &index[5], &index[6], &index[7], &index[8]);
				fgets(strLine, 255, file);	// Read the rest of the line
				vIndicesVertices.push_back(index[0]);
				vIndicesTexCoords.push_back(index[1]);
				vIndicesNormals.push_back(index[2]);
				vIndicesVertices.push_back(index[3]);
				vIndicesTexCoords.push_back(index[4]);
				vIndicesNormals.push_back(index[5]);
				vIndicesVertices.push_back(index[6]);
				vIndicesTexCoords.push_back(index[7]);
				vIndicesNormals.push_back(index[8]);
			}
			else if ('\n' == ch)	// Newline
			{
			}
			else // Don't care
			{
				fgets(strLine, 255, file);	// Read the rest of the line
			}
		}
		fclose(file);

		// Now we have everything loaded from the file,
		// save out to binary file.
		std::string strOutputFilename = filename;
		for (int i = 0; i < 3; ++i)
		{
			strOutputFilename.pop_back();
		}
		strOutputFilename += "geom";
		err = fopen_s(&file, strOutputFilename.c_str(), "w+b");
		ThrowIfTrue(bool(err != 0), "CResourceTriangle::convertObj() failed to open file " + filename + " for saving.");

		// Loop through each face
		Vertex geomVertex;
		unsigned int iNumVertices = (unsigned int)vIndicesVertices.size();
		size_t written = fwrite(&iNumVertices, sizeof(unsigned int), 1, file);
		ThrowIfTrue(bool(0 == written), "CResourceTriangle::convertObj() failed to write to file " + filename);

		for (unsigned int i = 0; i < vIndicesVertices.size(); ++i)
		{
			geomVertex.position = vVertices[vIndicesVertices[i] - 1];	// Vertex position
			geomVertex.normal = vNormals[vIndicesNormals[i] - 1];		// Normal
			geomVertex.texCoord = vTexCoords[vIndicesTexCoords[i] - 1];	// Texture coordinates
			geomVertex.colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			geomVertex.binormal = glm::vec3(0.0f, 1.0f, 0.0f);	// Just set to whatever as calculated during call to update()
			geomVertex.tangent = glm::vec3(0.0f, 1.0f, 0.0f);	// Just set to whatever as calculated during call to update()
			written = fwrite(&geomVertex, sizeof(Vertex), 1, file);
			ThrowIfTrue(bool(0 == written), "CResourceTriangle::convertObj() failed to write to file " + filename);
		}
		fclose(file);
	}
}