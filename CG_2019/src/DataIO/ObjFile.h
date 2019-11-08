#pragma once
#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>
#include "Linear_Algebra.h"

//------------------------------------------------------------------------------
// TYPES
//------------------------------------------------------------------------------

struct Face
{
	CRAB::Vector4Df v1, v2, v3;
};
#define FaceList std::vector<Face>

//TODO: add material
struct obj
{
	std::string Name;
	std::vector<CRAB::Vector4Df> Vertices;
	FaceList Faces;
};

//------------------------------------------------------------------------------
// CLASSES
//------------------------------------------------------------------------------

class ObjFile
{
public:
	//DEFAULT CONSTRUCTOR
	ObjFile();
	//DESTRUCTOR
	~ObjFile();

	//CLEAR A OBJ VARIABLE TYPE
	void ClearCurrentObj(obj& _obj);
	//READ .OBJ FILE
	bool ReadObjFile(const std::string& inName);
	//WRITE .OBJ FILE
	void WriteObjFile() const;

	//VARIABLES
	std::vector<obj> objList; //vector of objets from OBJ File

private:
	std::vector<CRAB::Vector4Df> allVertices; /* Create a unique vector of OBJ points */
};

#endif // OBJFILE_H