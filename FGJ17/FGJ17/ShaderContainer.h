#pragma once

#include <SpehsEngine/ShaderManager.h>
#include <SpehsEngine/GLSLProgram.h>

#include <vector>

/*
CREATING NEW SHADERS:
1. Create new Uniforms class
2. Add ShaderName to enum
3. Add shader to list to be sent to engine
Order of initialization is important!
Access Uniform values with: shaderManager->getShader( %SHADER_NAME )->getCustomUniforms< %UNIFORM_CLASS >()-> %UNIFORM_VALUE
*/

enum class ShaderName : int
{
	//PLHD = NEXT_SHADER_INDEX,
};

extern void initShaders();
extern void reloadShader(const ShaderName _shaderIndex);

//class BackgroundUniforms : public spehs::Uniforms
//{
//public:
//	BackgroundUniforms(spehs::GLSLProgram* _shader);
//	~BackgroundUniforms();
//
//	void setUniforms();
//
//	float fieldRadius;
//
//private:
//	unsigned int textureLocation = 0;
//	unsigned int fieldRadiusLocation = 0;
//};