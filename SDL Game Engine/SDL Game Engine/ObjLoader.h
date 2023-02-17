#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Position.h"

using namespace std;

struct Color {
	int red;
	int green;
	int blue;
	int alpha;
};
struct Vertex {
	int index;
	float x;
	float y;
	float z;
	float w;
};
struct Texture {
	float u;
	float v;
};
struct Normal {
	float x;
	float y;
	float z;
};
struct Face {
	int id;
	int capacity;
	Color color;
	bool visible;
	std::vector<int> Vertices;
	std::vector<int> Normals;
	std::vector<int> Textures;
};
struct Edge {
	int a;
	int b;
};
class Entity {
public:
	int edgeAmount;
	int vertexAmount;
	int faceAmount;
	int textureAmount;
	vector<Vertex> Vertices;
	vector<Texture> Textures;
	vector<Normal> Normals;
	vector<Normal> MeshNormals;
	vector<Face> Faces;
	vector<Edge> Edges;
	Position vertexToPosition(int index) {
		Position p;
		p.x = Vertices[index].x;
		p.y = Vertices[index].y;
		p.z = Vertices[index].z;
		p.w = Vertices[index].w;
		return p;
	}
	void setVertex(int index,Position p) {
		Vertices[index].x = p.x;
		Vertices[index].y = p.y;
		Vertices[index].z = p.z;
		Vertices[index].w = p.w;
	}
	void insertMeshNormal(float x, float y, float z) {
		Normal n;
		n.x = x;
		n.y = y;
		n.z = z;
		MeshNormals.push_back(n);
	}
	void hideFace(int i) {
		Faces[i].visible = false;
	}
	void removeFace(int i) {
		Faces.erase(Faces.begin() + i);
		faceAmount--;
		printf("remove face: %d\n", i);
	}
};

Entity loadEntity(std::string path);