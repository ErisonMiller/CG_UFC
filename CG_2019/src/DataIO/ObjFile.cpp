#include "ObjFile.h"
#include <iostream>
#include <fstream>
#include <sstream>


//DEFAULT CONSTRUCTOR
ObjFile::ObjFile()
{
	objList.clear();
	allVertices.clear();
}

//DESTRUCTOR
ObjFile::~ObjFile()
{
}

//CLEAR OBJ VARIABLE TYPE
void ObjFile::ClearCurrentObj(obj& _obj)
{
	_obj.Name = "";
	_obj.Vertices.clear();
	_obj.Faces.clear();
}

//READ .OBJ FILE
bool ObjFile::ReadObjFile(const std::string& inName)
{
	std::ifstream inFile(inName);
	if (!inFile.is_open())
	{
		std::cerr << "\n\t!!! FILE COULD NOT BE OPENED !!!\n" << std::endl;
		return false;
	}

	//Initialize
	objList.clear();
	allVertices.clear();
	//Read a file line by line
	std::string line;
	obj currentObj;
	bool object = false;

	while (!inFile.eof())
	{
		getline(inFile, line);

		/*-------------------* OBJECT *-------------------*/

		if (line.substr(0, 1) == "o")
		{
			//add last obj, if it exists
			//and clear the current obj
			if (object == true)
			{
				objList.push_back(currentObj);
				ClearCurrentObj(currentObj);
			}

			object = true;							//activate a new obj
			currentObj.Name = line.substr(2);		//'o' name
			std::cout << "\n\tReading Object ...: " << currentObj.Name << std::endl;
		}
		else if (object == true)
		{

			/*-------------------* VERTEX *-------------------*/

			if (line.substr(0, 1) == "v")
			{
				CRAB::Vector4Df vertex;
				line = line.substr(1);
				std::istringstream s(line);
				s >> vertex.x;
				s >> vertex.y;
				s >> vertex.z;
				vertex.w = 1.0f;
				currentObj.Vertices.push_back(vertex);
				allVertices.push_back(vertex);
			}

			/*--------------------* FACE *--------------------*/

			if (line.substr(0, 1) == "f")
			{
				int vertex;
				Face face;
				line = line.substr(1);
				std::istringstream s(line);
				s >> vertex;
				face.v1 = allVertices[vertex - 1];
				s >> vertex;
				face.v2 = allVertices[vertex - 1];
				s >> vertex;
				face.v3 = allVertices[vertex - 1];
				currentObj.Faces.push_back(face);
			}
		}
	}
	//END OF FILE

	//last valid object
	if (object == true)
		objList.push_back(currentObj);

	std::cout << "\n\tFile [" << inName << "] has been opened successfully!" << std::endl;

	return true;
}

//WRITE .OBJ FILE
void ObjFile::WriteObjFile() const
{

}
