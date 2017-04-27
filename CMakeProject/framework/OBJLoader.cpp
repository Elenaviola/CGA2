#include "OBJLoader.h"



OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

OBJResult OBJLoader::loadOBJ(const std::string & objpath)
{
	OBJResult result;
	try
	{
		std::ifstream stream(objpath, std::ios_base::in);
		stream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		std::string command = "";
		while (istreamhelper::peekString(stream, command))
		{			
			if (command == "o")
			{
				result.objects.push_back(parseObject(stream));
			}
			else
			{				
				stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
		return result;
	}
	catch (std::exception ex)
	{
		std::cerr << "Error: Loading OBJ failed: " << ex.what() << "\n";
		throw ex;
	}
}

OBJObject OBJLoader::parseObject(std::ifstream & stream)
{
	try
	{
		OBJObject object;
		std::string command;
		DataCache cache;

		//get object name
		if (!(stream >> command))
			throw std::exception("Error parsing object.");

		if (command != "o")
			throw std::exception("Error parsing object.");

		if (!(stream >> object.name))
			throw std::exception("Error parsing object name.");


		while (istreamhelper::peekString(stream, command))
		{
			//Fill cache
			if (command == "v")			//position
			{
				cache.positions.push_back(parsePosition(stream));
			}
			else if (command == "vt")	//uv
			{
				cache.uvs.push_back(parseUV(stream));
			}
			else if (command == "vn")	//normal
			{
				cache.normals.push_back(parseNormal(stream));
			}

			//meshes, groups and faces
			else if (command == "g") //grouped mesh
			{
				object.meshes.push_back(parseMesh(cache, stream));
			}
			else if (command == "f") //no mesh groups
			{
				object.meshes.push_back(parseMesh(cache, stream));
			}

			//stop condition
			else if (command == "o") //next object found
			{
				return object;
			}

			//ignore everything else
			else
			{
				stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}

		} //eof reached. model should be complete
		return object;
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

glm::vec3 OBJLoader::parsePosition(std::ifstream & stream)
{
	try
	{
		std::string command;
		glm::vec3 pos;
		if (!(stream >> command >> pos.x >> pos.y >> pos.z) || command != "v")
		{
			throw std::exception("Error parsing v command.");
		}
		return pos;
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

glm::vec3 OBJLoader::parseNormal(std::ifstream & stream)
{
	try
	{
		std::string command;
		glm::vec3 normal;
		if (!(stream >> command >> normal.x >> normal.y >> normal.z) || command != "vn")
		{
			throw std::exception("Error parsing vn command.");
		}
		return normal;
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

glm::vec2 OBJLoader::parseUV(std::ifstream & stream)
{
	try
	{
		std::string command;
		glm::vec2 uv;
		if (!(stream >> command >> uv.x >> uv.y) || command != "vt")
		{
			throw std::exception("Error parsing vt command.");
		}
		return uv;
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

OBJMesh OBJLoader::parseMesh(DataCache & cache, std::ifstream & stream)
{
	try
	{
		
	}
	catch (std::exception ex)
	{

		throw ex;
	}
}

OBJLoader::Face OBJLoader::parseFace(std::ifstream & stream)
{
	try
	{
		
	}
	catch (std::exception ex)
	{

		throw ex;
	}
}

OBJLoader::RawVertex OBJLoader::parseVertex(std::ifstream & stream)
{
	try
	{
		
	}
	catch (std::exception ex)
	{

		throw ex;
	}
}

void OBJLoader::generateNormals(OBJMesh & mesh)
{
}

void OBJLoader::generateTangents(OBJMesh & mesh)
{
}

bool istreamhelper::readToNextDelim(std::istream & stream, std::string & out, const std::vector<char>& delimiters)
{
	return false;
}

std::vector<std::string> istreamhelper::splitString(const std::string & in, const std::vector<char>& delimiters)
{
	return std::vector<std::string>();
}

bool istreamhelper::peekString(std::istream & stream, std::string& out)
{
	try
	{
		auto spos = stream.tellg();
		if (stream >> out)
		{
			stream.seekg(spos);
			return true;
		}
		else
		{
			stream.seekg(spos);
			return false;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}
