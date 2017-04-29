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
		stream.exceptions(std::ifstream::badbit);
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
		result.objname = objpath;
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
		if (stream.eof())
		{
			return object;
		}
		else
		{
			throw std::exception("Error parsing object. Read failed before end of file.");
		}
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
		OBJMesh mesh;
		std::unordered_map<VertexDef, int, VertexDef::hash, VertexDef::equal_to> meshvertset; //collect distinct vertices

		//later create actual vertices out of these and put them into the mesh
		std::vector<VertexDef> meshverts; //for tracking order of insertion		
		std::vector<Index> meshindices;	//vertex index of one of the vertex defs above

		std::string command;
		if (!(istreamhelper::peekString(stream, command)))
		{
			throw std::exception("Error parsing mesh.");
		}

		if (command == "g") //if we have a grouped mesh extract its name first
		{
			if (!(stream >> command >> mesh.name))
			{
				throw std::exception("Error parsing mesh.");
			}
		}
		else
		{
			mesh.name = "UNGROUPED";
		}

		//now process faces
		while (istreamhelper::peekString(stream, command))
		{
			if (command == "f") //yay we found a face
			{
				Face face = parseFace(stream);
				//process face data and build mesh
				for (int i = 0; i < 3; i++)
				{
					//add vertexdefs and indices
					auto it = meshvertset.find(face.verts[i]);
					if (it != meshvertset.end())	//if vertex def exists already, just get the index and push it onto index array
					{
						meshindices.push_back(it->second);
					}
					else //if not, push a index pointing to the last pushed vertex def, push vertex def and insert it into the set
					{
						meshindices.push_back(meshverts.size());
						meshverts.push_back(face.verts[i]);
						meshvertset.insert(std::make_pair(face.verts[i], meshverts.size() - 1));
					}					
				}
			}
			else if (command == "g" || command == "o") //found next mesh group
			{
				fillMesh(mesh, cache, meshverts, meshindices);
				return mesh;
			}
			else
			{
				stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
		if (stream.eof())
		{
			fillMesh(mesh, cache, meshverts, meshindices);
			return mesh;
		}
		else
		{
			throw std::exception("Error parsing mesh. Read failed before end of file.");
		}
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
		std::string command;
		Face face;
		if ((stream >> command) && command == "f")
		{
			std::string vstr1, vstr2, vstr3;
			if (stream >> vstr1 >> vstr2 >> vstr3)
			{
				face.verts.push_back(parseVertex(vstr1));
				face.verts.push_back(parseVertex(vstr2));
				face.verts.push_back(parseVertex(vstr3));
				return face;
			}
			else
			{
				throw std::exception("Error parsing face");
			}
		}
		else
		{
			throw std::exception("Error parsing face");
		}
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

OBJLoader::VertexDef OBJLoader::parseVertex(const std::string& vstring)
{
	try
	{
		//buffer for v, vt, vn index
		std::string att[3];

		int attct = 0;

		for (int i = 0; i < vstring.length(); i++)
		{
			if (isdigit(vstring[i]))
			{
				att[attct] += vstring[i];
			}
			else if (vstring[i] == '/')
			{
				attct++;
			}
			else
			{
				throw std::exception("Error parsing vertex.");
			}
		}

		VertexDef vert = {
			(att[0].length() > 0 ? std::stoi(att[0]) - 1 : -1),
			(att[1].length() > 0 ? std::stoi(att[1]) - 1 : -1),
			(att[2].length() > 0 ? std::stoi(att[2]) - 1 : -1)
		};
		return vert;
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}

void OBJLoader::fillMesh(OBJMesh & mesh, DataCache & cache, std::vector<VertexDef> vdefs, std::vector<Index>& indices)
{
	//assemble the mesh from the collected indices
	//create vertex from cache data
	bool hasverts = true;
	bool hasuvs = true;
	bool hasnormals = true;
	for (int i = 0; i < vdefs.size(); i++)
	{
		Vertex vert;

		if (vdefs[i].p_idx == -1)
			hasverts = false;

		if (vdefs[i].uv_idx == -1)
			hasuvs = false;

		if (vdefs[i].n_idx == -1)
			hasnormals = false;

		if (vdefs[i].p_idx > -1 && vdefs[i].p_idx < cache.positions.size())
			vert.position = cache.positions[vdefs[i].p_idx];
		else
			throw std::exception("Missing position in object definition");

		if (vdefs[i].uv_idx > -1 && vdefs[i].uv_idx < cache.uvs.size())
			vert.uv = cache.uvs[vdefs[i].uv_idx];
		else
			throw std::exception("Missing texture coordinate in object definition");

		if (vdefs[i].n_idx > -1 && vdefs[i].n_idx < cache.normals.size())
			vert.normal = cache.normals[vdefs[i].n_idx];
		else
			throw std::exception("Missing normal in object definition");

		mesh.vertices.push_back(vert);
	}
	mesh.indices = std::move(indices);	
	mesh.hasPositions = hasverts;	
	mesh.hasUVs = hasuvs;	
	mesh.hasNormals = hasnormals;
}

void OBJLoader::recalculateNormals(OBJMesh & mesh)
{
}

void OBJLoader::recalculateTangents(OBJMesh & mesh)
{
}

bool istreamhelper::peekString(std::istream& stream, std::string& out)
{
	try
	{
		if (stream.eof())
		{
			return false;
		}
		auto spos = stream.tellg();
		if (stream >> out)
		{
			stream.seekg(spos);
			return true;
		}
		else
		{
			if(!stream.eof())
				stream.seekg(spos);
			return false;
		}
	}
	catch (std::exception ex)
	{
		throw ex;
	}
}
