
#include "ShaderContainer.h"

#include <SpehsEngine/Console.h>
#include <SpehsEngine/Time.h>


spehs::Shader* buildShader(const ShaderName _name)
{
	spehs::GLSLProgram* shader;
	switch (_name)
	{
	//case ShaderName::Background:
	//	shader = new spehs::GLSLProgram();
	//	shader->compileShaders("Shaders/backgroundShader.vert", "Shaders/backgroundShader.frag");
	//	shader->addAttribute(spehs::VertexAttributePosition::VERTEX_POSITION, "vertexPosition");
	//	shader->addAttribute(spehs::VertexAttributePosition::VERTEX_COLOR, "vertexColor");
	//	shader->addAttribute(spehs::VertexAttributePosition::VERTEX_UV, "vertexUV");
	//	shader->linkShaders();
	//	return new spehs::Shader((int) ShaderName::Background, shader, new BackgroundUniforms(shader));
	//	break;

	default:
		return nullptr;
		break;
	}
}

void initShaders()
{
	//Background
	//shaderManager->pushShader(buildShader(ShaderName::Background));
}
void reloadShader(const ShaderName _shaderIndex)
{
	if (int(_shaderIndex) < NEXT_SHADER_INDEX)
	{
		spehs::console::error("Cannot reload default shaders!");
		return;
	}
	spehs::Shader* shader = buildShader(_shaderIndex);
	if (!shader)
	{
		spehs::console::error("Invalid shader index!");
		return;
	}
	shaderManager->reload((int) _shaderIndex, shader);
	spehs::console::log("Shader reloaded!");
}


//BackgroundUniforms::BackgroundUniforms(spehs::GLSLProgram* _shader) : Uniforms(_shader)
//{
//	textureLocation = shader->getUniformLocation("tex");
//	fieldRadiusLocation = shader->getUniformLocation("fieldRadius");
//}
//BackgroundUniforms::~BackgroundUniforms(){}
//void BackgroundUniforms::setUniforms()
//{
//	//fieldRadius = GameClient::instance->getFieldRadius();
//	spehs::bind2DTexture(textureDataID, 0);
//	spehs::setUniform_int(textureLocation, 0);
//	spehs::setUniform_float(fieldRadiusLocation, fieldRadius);
//	Uniforms::setUniforms();
//}