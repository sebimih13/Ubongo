#include "ResourceManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::string ResourceManager::PiecesFormat[15];
glm::vec3 ResourceManager::PiecesColor[15];

ResourceManager::ResourceManager()
{
	/*

		Pieces format: 4x4

		1 : **##		2:	**##		3:	**##		4:	*###		5:	****		6:	****
			#*##			*###			**##			*###			####			##*#
			#**#			*###			####			####			####			####
			####			####			####			####			####			####

		7:  ***#		8:	#**#		9:	***#		10:	****		11:	***#		12:	**##
			#**#			**##			####			###*			#*##			*###
			####			####			####			####			####			####
			####			####			####			####			####			####

	*/

	// 12 formats for pieces
	PiecesFormat[1] = "**###*###**#####";
	PiecesFormat[2] = "**##*###*#######";
	PiecesFormat[3] = "**##**##########";
	PiecesFormat[4] = "*###*###########";
	PiecesFormat[5] = "****############";
	PiecesFormat[6] = "****##*#########";
	PiecesFormat[7] = "***##**#########";
	PiecesFormat[8] = "#**#**##########";
	PiecesFormat[9] = "***#############";
	PiecesFormat[10] = "****###*########";
	PiecesFormat[11] = "***##*##########";
	PiecesFormat[12] = "**##*###########";

	// 12 colors for pieces
	PiecesColor[1] = glm::vec3(1.0f, 1.0f, 0.0f);
	PiecesColor[2] = glm::vec3(0.0f, 0.4f, 0.7f);
	PiecesColor[3] = glm::vec3(0.1f, 0.9f, 0.5f);
	PiecesColor[4] = glm::vec3(0.8f, 0.0f, 0.7f);
	PiecesColor[5] = glm::vec3(0.8f, 0.5f, 0.0f);
	PiecesColor[6] = glm::vec3(0.0f, 0.3f, 0.1f);
	PiecesColor[7] = glm::vec3(0.7f, 0.0f, 0.2f);
	PiecesColor[8] = glm::vec3(0.7f, 0.6f, 0.5f);
	PiecesColor[9] = glm::vec3(0.8f, 0.8f, 1.0f);
	PiecesColor[10] = glm::vec3(0.6f, 0.6f, 0.6f);
	PiecesColor[11] = glm::vec3(0.4f, 0.1f, 0.5f);
	PiecesColor[12] = glm::vec3(0.1f, 1.0f, 0.8f);
}

void ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = LoadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

void ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = LoadTextureFromFile(file, alpha);
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);

	for (auto iter : Textures)
		glDeleteProgram(iter.second.ID);
}

Shader ResourceManager::LoadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from file path
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);

		std::stringstream vShaderStream, fShaderStream;

		// read file's buffers contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER:: Failed to read shader from file" << std::endl;
	}

	// 2. create shader object from source code
	Shader shader;
	shader.Compile(vertexCode.c_str(), fragmentCode.c_str(), gShaderFile ? geometryCode.c_str() : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
	// create texture object
	Texture2D texture;

	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}

	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	// generate texture
	texture.Generate(width, height, data);

	// free image data
	stbi_image_free(data);
	return texture;
}

