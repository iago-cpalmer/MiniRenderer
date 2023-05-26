#include "Parser.h"

Vector3f ParseVector(std::string line)
{
	int spacePositions[4] = { -1, -1, -1, (int)(line.length() - 1) };
	int nspace = 0;
	for (unsigned int i = 0; i < line.length(); i++) {
		if (line.at(i) == ' ') {
			if (spacePositions[nspace] != (int)i) {
				spacePositions[nspace] = i;
				nspace++;
			}
		}
	}
	spacePositions[nspace] = line.length() - 1;

	Vector3f v = Vector3f(0, 0, 0);

    v.x = stof(line.substr(spacePositions[0] + 1, spacePositions[1] - spacePositions[0]));
    v.y = stof(line.substr(spacePositions[1] + 1, spacePositions[2] - spacePositions[1]));
    if (spacePositions[3] > spacePositions[2]) {
        v.z = stof(line.substr(spacePositions[2] + 1, spacePositions[3] - spacePositions[2]));
    }

	return v;

}

Vector3Int* ParseFace(std::string line)
{
	Vector3Int* face = new Vector3Int[3];
	std::vector<std::string> vertices;
	std::stringstream ss(line.substr(2));
	std::string vertex;
	while (std::getline(ss, vertex, ' '))
	{
		vertices.push_back(vertex);
	}
	for (int i = 0; i < 3; i++)
	{
		std::stringstream vss(vertices[i]);
		std::string v;
		int j = 0;
		while (std::getline(vss, v, '/'))
		{
			if (i == 0)
			{
				face[j].x = std::stoi(v);
			}
			else if (i == 1)
			{
				face[j].y = std::stoi(v);
			}
			else if (i == 2)
			{
				face[j].z = std::stoi(v);
			}
			j++;
		}
	}
	return face;
}
void NormalizeVertices(std::list<Vector3f>& vertices) {
	// Find the minimum and maximum values for each coordinate
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	float maxZ = std::numeric_limits<float>::lowest();

	for (const Vector3f& vertex : vertices) {
		minX = std::min(minX, vertex.x);
		minY = std::min(minY, vertex.y);
		minZ = std::min(minZ, vertex.z);
		maxX = std::max(maxX, vertex.x);
		maxY = std::max(maxY, vertex.y);
		maxZ = std::max(maxZ, vertex.z);
	}

	// Calculate the range (difference) for each coordinate
	float rangeX = maxX - minX;
	float rangeY = maxY - minY;
	float rangeZ = maxZ - minZ;

	for (Vector3f& vertex : vertices) {
		vertex.x = ((vertex.x - minX) / rangeX) * 2.0f - 1.0f;
		vertex.y = ((vertex.y - minY) / rangeY) * 2.0f - 1.0f;
		vertex.z = ((vertex.z - minZ) / rangeZ) * 2.0f - 1.0f;
	}
}


Model* ParseObj(std::string filePath)
{
	std::list<Vector3f> vertexs;
	std::list<Vector2f> textures;
	std::list<Vector3f> normals;
	std::list<Vector3Int*> faces;


	std::ifstream file(filePath);
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("v ") != std::string::npos)
		{
			// Vertexs
			vertexs.push_back(ParseVector(line));

		}
		else if (line.find("vt ") != std::string::npos)
		{
			// Texture coords
			Vector3f v3d = ParseVector(line);
			Vector2f v = {v3d.x, v3d.y};
			textures.push_back(v);

		}
		else if (line.find("vn ") != std::string::npos)
		{
			// Vertex normals
			normals.push_back(ParseVector(line));
		}
		else if (line.find("f ") != std::string::npos)
		{
			// Faces
			faces.push_back(ParseFace(line));
		}
	}
	NormalizeVertices(vertexs);

	Vector3f* vertexArr = new Vector3f[vertexs.size()];
	std::copy(vertexs.begin(), vertexs.end(), vertexArr);

	Vector2f* textureArr = new Vector2f[textures.size()];
	std::copy(textures.begin(), textures.end(), textureArr);

	Vector3f* normalArr = new Vector3f[normals.size()];
	std::copy(normals.begin(), normals.end(), normalArr);

	// Get the number of elements in the list
	int numElements = faces.size();

	// Create a double array of Vector3Int
	Vector3Int** faceArray = new Vector3Int * [numElements];
	for (int i = 0; i < numElements; i++)
	{
		faceArray[i] = new Vector3Int[3];
	}

	// Copy the data from the list to the double array
	int i = 0;
	for (auto& facePtr : faces)
	{
		faceArray[i][0] = *facePtr++;
		faceArray[i][1] = *facePtr++;
		faceArray[i][2] = *facePtr++;

		i++;
	}

	return new Model{ vertexArr, textureArr, normalArr, faceArray, (unsigned int)vertexs.size(), (unsigned int)textures.size(), (unsigned int)normals.size(), (unsigned int)faces.size() };
}