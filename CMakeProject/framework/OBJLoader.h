#ifndef _OBJ_LOADER_
#define _OBJ_LOADER_

#include "libheaders.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

//------------------------------ istream string helper ----------------------------------------

class istreamhelper
{
public:
	static bool readToNextDelim(std::istream& stream, std::string& out, const std::vector<char>& delimiters);
	static std::vector<std::string> splitString(const std::string& in, const std::vector<char>& delimiters);
	static bool peekString(std::istream& stream, std::string& out);
};

//------------------------------ Data Structures to hold the result ---------------------------
struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 tangent;
};

typedef GLuint Index;

class OBJMesh
{
public:
	OBJMesh() {}
	OBJMesh(const OBJMesh& other) :
		name(other.name),
		hasPositions(other.hasPositions),
		hasUVs(other.hasUVs),
		hasNormals(other.hasNormals),
		hasTangents(other.hasTangents),
		vertices(other.vertices),
		indices(other.indices)
	{}
	OBJMesh(OBJMesh&& other) :
		name(std::move(other.name)),
		hasPositions(other.hasPositions),
		hasUVs(other.hasUVs),
		hasNormals(other.hasNormals),
		hasTangents(other.hasTangents),
		vertices(std::move(other.vertices)),
		indices(std::move(other.indices))
	{}
	~OBJMesh() {}

	OBJMesh& operator=(const OBJMesh& other)
	{
		this->name = other.name;
		this->hasPositions = other.hasPositions;
		this->hasUVs = other.hasUVs;
		this->hasNormals = other.hasNormals;
		this->hasTangents = other.hasTangents;
		this->vertices = other.vertices;
		this->indices = other.indices;
	}
	OBJMesh& operator=(OBJMesh&& other)
	{
		this->name = std::move(other.name);
		this->hasPositions = other.hasPositions;
		this->hasUVs = other.hasUVs;
		this->hasNormals = other.hasNormals;
		this->hasTangents = other.hasTangents;
		this->vertices = std::move(other.vertices);
		this->indices = std::move(other.indices);
	}

	bool hasPositions;
	bool hasUVs;
	bool hasNormals;
	bool hasTangents;

	std::string name;

	std::vector<Vertex> vertices;
	std::vector<Index> indices;
};

class OBJObject
{
public:
	OBJObject() {}
	OBJObject(const OBJObject& other) :
		name(other.name),
		meshes(other.meshes)
	{}
	OBJObject(OBJObject&& other) :
		name(std::move(other.name)),
		meshes(std::move(other.meshes))
	{}
	~OBJObject() {}

	OBJObject& operator=(const OBJObject& other)
	{
	}
	OBJObject& operator=(OBJObject&& other)
	{
	}

	std::string name;
	std::vector<OBJMesh> meshes;
};

class OBJResult
{
public:
	OBJResult() {}
	~OBJResult() {}

	std::string objname;
	std::vector<OBJObject> objects;
};


class OBJLoader
{
private:
	OBJLoader();
	~OBJLoader();

public:
	static OBJResult loadOBJ(const std::string& objpath);

private:
	typedef struct DataCache
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
	} DataCache;

	typedef struct RawVertex
	{
		//if -1: undefined
		int p_idx;
		int uv_idx;
		int n_idx;
	};

	typedef struct Face
	{
		std::vector<RawVertex> verts;
	};

	//parsing helpers
	//o flag
	static OBJObject parseObject(std::ifstream& stream);	
	//v per o
	static glm::vec3 parsePosition(std::ifstream& stream);
	//vn per o
	static glm::vec3 parseNormal(std::ifstream& stream);
	//vt per o
	static glm::vec2 parseUV(std::ifstream& stream);
	//=> create raw v, vn, vt cache

	//parse f flags and call parseFace
	static OBJMesh parseMesh(DataCache& cache ,std::ifstream& stream);

	//parse face and generate vertices and indices for the mesh
	static Face parseFace(std::ifstream& stream);

	static RawVertex parseVertex(std::ifstream& stream);

public:
	//post processing
	static void generateNormals(OBJMesh& mesh);
	static void generateTangents(OBJMesh& mesh);
};

#endif