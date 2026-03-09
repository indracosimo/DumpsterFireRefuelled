#include "geometry.h"
#include "renderer.h"
#include "textureLoader.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../MeshManager.h"
#include "glm/gtc/type_ptr.hpp"
#include "../shaders/LightManager.h"
#include "../shaders/BasicShadowMapper.h"


renderer::renderer(unsigned int width, unsigned int height) : SCR_WIDTH(width), SCR_HEIGHT(height)
{
	glEnable(GL_DEPTH_TEST);
	mainShader = new Shader("src/shaders/vertexShader.vs", "src/shaders/fragmentShader.fs");
	//
	// shadowMapper = new ShadowMapper();
	shadowMapper = new BasicShadowMapper();
	
	setupCube();
	setupQuad();
	setupFramebuffer();
	loadTextures();

	std::cout << "texColorBuffer ID: " << texColorBuffer << std::endl;
	std::cout << "texture1 ID: " << texture1 << std::endl;
}

renderer::~renderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete mainShader;
	//delete crtShader;
	delete shadowMapper;
}

void renderer::setupCube()
{
	float* vertices = getCubeVertices();
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, getCubeVertexCount() * sizeof(float), vertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texcoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void renderer::setupQuad()
{
	float* vertices = getQuadVertices();
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, getQuadVertexCount() * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void renderer::setupFramebuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderer::loadTextures()
{
	texture1 = loadTexture("assets/images/concrete.jpg");
}

void renderer::render(const std::vector<CubeTransform>& cubes, camera& cam) 
{
	Mesh* planeMesh = MeshManager::GetInstance().GetMesh("assets/obj/Viking_House.obj");
	
	LightManager& lightMgr = LightManager::GetInstance();

	light* shadowLight = nullptr;
	for (int i = 0; i < lightMgr.GetLightCount(); i++)
	{
		light* L = lightMgr.GetLight(i);
		if (!L) continue;
		if (!L->bEnabled) continue;
		if (!L->bCastShadows) continue;
		if (L->type != LightType::DIRECTIONAL) continue;

		shadowLight = L;
		break;
	}
	
    for (int i = 0; i < lightMgr.GetLightCount(); i++)
    {
        light* currentLight = lightMgr.GetLight(i);
        if (!currentLight || !currentLight->bCastShadows)
            continue;

        // shadowMapper->initializeShadowMap(*currentLight);

        if (currentLight->type != LightType::DIRECTIONAL)
            continue;

        // shadowMapper->beginShadowPass(*currentLight, cam.GetViewMatrix(), cam.GetProjectionMatrix(45.0f, 0.1f, 100.0f), cubes );
if (shadowLight)
{
	shadowMapper->Init(*shadowLight);
	shadowMapper->BeginDirectionalLight(*shadowLight);
	
        for (const auto& cube : cubes)
        {
        	glm::mat4 model(1.0f);
        	model = glm::translate(model, cube.position);
        	model = glm::rotate(model, cube.rotation.x, glm::vec3(1,0,0));
        	model = glm::rotate(model, cube.rotation.y, glm::vec3(0,1,0));
        	model = glm::rotate(model, cube.rotation.z, glm::vec3(0,0,1));
        	model = glm::scale(model, cube.scale);

        	//glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
            // GLint modelLoc = glGetUniformLocation(shadowMapper->shadowDepthShader->ID, "modelMatrix");
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        	//shadowMapper->Init(*shadowLight);
        	//shadowMapper->BeginDirectionalLight(*shadowLight);
        	shadowMapper->SetModelMatrix(model);
        //	mesh->DrawMesh();
        	
            if (!cube.meshPath.empty())
            {
                Mesh* mesh = MeshManager::GetInstance().GetMesh(cube.meshPath);
                if (mesh && mesh->vertexCount > 0)
                    mesh->DrawMesh();
            }
            else
            {
                glBindVertexArray(VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        if (planeMesh)
        {
            glm::mat4 model(1.0f);
			// GLint modelLoc = glGetUniformLocation(shadowMapper->shadowDepthShader->ID,"modelMatrix");
            // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        	shadowMapper->SetModelMatrix(model);
            planeMesh->DrawMesh();
        }
    	shadowMapper->EndDirectionalLight(SCR_WIDTH, SCR_HEIGHT);
        // shadowMapper->endShadowPass();
    }

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainShader->use();
	
	if (shadowLight)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowLight->shadowDepthTexture);
		mainShader->setInt("shadowMap", 2);

		mainShader->setMat4("shadowMapMatrix", shadowLight->shadowMapMatrix);
		mainShader->setVec2("shadowMapTexelSize",
			1.0f / (float)shadowLight->shadowMapResolution.x,
			1.0f / (float)shadowLight->shadowMapResolution.y
		);
		mainShader->setFloat("shadowCalcBias", 0.005f);
		mainShader->setBool("bUseShadowMap", true);

		glActiveTexture(GL_TEXTURE0);
	}
	else
	{
		mainShader->setBool("bUseShadowMap", false);
	}
}
    	
	
#pragma region Lighting
	
	mainShader->use();
	glm::mat4 view = cam.GetViewMatrix();
	glm::mat4 projection = cam.GetProjectionMatrix(45.0f, 0.1f, 100.0f);

	glUniformMatrix4fv(glGetUniformLocation(mainShader->ID, "viewMatrix"),
					   1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(glGetUniformLocation(mainShader->ID, "projectionMatrix"),
					   1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(glGetUniformLocation(mainShader->ID, "eyePosition"),
			 1, glm::value_ptr(cam.Position));
	int activeLights = std::min(16, lightMgr.GetLightCount());
	glUniform1i(glGetUniformLocation(mainShader->ID, "activeLights"), activeLights);
	
	glUniform4fv(glGetUniformLocation(mainShader->ID, "global_ambient"), 1, glm::value_ptr(lightMgr.globalAmbient));
	// mainShader->setInt("texture1", 0);

	//light arrays
	std::vector<int> types(16, 0);
	std::vector<int> enabled(16, 0);
	std::vector<glm::vec3> positions(16, glm::vec3(0.0f));
	std::vector<glm::vec3> directions(16, glm::vec3(0.0f));
	std::vector<glm::vec4> diffuses(16, glm::vec4(0.0f));
	std::vector<glm::vec4> specular(16, glm::vec4(0.0f));
	std::vector<glm::vec3> attenuation(16, glm::vec3(1.0f, 0.0f, 0.0f));
	std::vector<float> cutoffs(16, 0.0f);
	std::vector<int> exponents(16, 0);

	for (int i = 0; i < activeLights; i++)
	{
		const light* Light = lightMgr.GetLight(i);
		if (!Light) continue;
		
		types[i] = (int)Light->type;
		enabled[i]    = Light->bEnabled ? 1 : 0;
		positions[i] = Light->position;
		directions[i] = Light->direction;
		diffuses[i] = Light->diffuse;
		specular[i] = Light->specular;
		attenuation[i] = Light->attenuation;
		cutoffs[i] = Light->spotCutoff;
		exponents[i] = Light->spotExponent;
	}

	//uniform arrays
	glUniform1iv(glGetUniformLocation(mainShader->ID, "light_type"), 16, types.data());
	glUniform1iv(glGetUniformLocation(mainShader->ID, "light_enabled"), 16, enabled.data());
	glUniform3fv(glGetUniformLocation(mainShader->ID, "light_position"), 16, glm::value_ptr(positions[0]));
	glUniform3fv(glGetUniformLocation(mainShader->ID, "light_direction"), 16, glm::value_ptr(directions[0]));
	glUniform4fv(glGetUniformLocation(mainShader->ID, "light_diffuse"), 16, glm::value_ptr(diffuses[0]));
	glUniform4fv(glGetUniformLocation(mainShader->ID, "light_specular"), 16, glm::value_ptr(specular[0]));
	glUniform3fv(glGetUniformLocation(mainShader->ID, "light_attenuation"), 16, glm::value_ptr(attenuation[0]));
	glUniform1fv(glGetUniformLocation(mainShader->ID, "light_spotCutoff"), 16, cutoffs.data());
	glUniform1iv(glGetUniformLocation(mainShader->ID, "light_spotExponent"), 16, exponents.data());

	// Bind shadow map for first directional light
	// if (activeLights > 0)
	// {
	// 	light* firstLight = lightMgr.GetLight(0);
	// 	if (firstLight && firstLight->type == LightType::DIRECTIONAL && firstLight->bCastShadows)
	// 	{
	// 		shadowMapper->bindShadowMap(*firstLight, mainShader, 2); // Use texture unit 2
	// 		glUniform1i(glGetUniformLocation(mainShader->ID, "bUseShadowMap"), 1);
	// 	}
	// 	else
	// 	{
	// 		glUniform1i(glGetUniformLocation(mainShader->ID, "bUseShadowMap"), 0);
	// 	}
	// }

#pragma endregion

	glActiveTexture(GL_TEXTURE0);

	for (const auto& cube : cubes)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cube.textureID);
		GLint albedoLoc = glGetUniformLocation(mainShader->ID, "albedoMap");
		glUniform1i(albedoLoc, 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, cube.specularMapID);
		GLint specularLoc = glGetUniformLocation(mainShader->ID, "specularMap");
		glUniform1i(specularLoc, 1);

		GLint texLoc = glGetUniformLocation(mainShader->ID, "texture1");
		glUniform1i(texLoc, 0);
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cube.position);
		model = glm::rotate(model, cube.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, cube.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cube.rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, cube.scale);

		GLint modelLoc = glGetUniformLocation(mainShader->ID, "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
		GLint normalMatrixLoc = glGetUniformLocation(mainShader->ID, "normalMatrix");
		glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		
		GLint viewPosLoc = glGetUniformLocation(mainShader->ID, "viewPos");
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(cam.Position));

		GLint lightColorLoc = glGetUniformLocation(mainShader->ID, "lightColor");
		glUniform3f(lightColorLoc, 1.0f, 0.9f, 0.8f);
		
		Mesh* meshToDraw = nullptr;
		if (!cube.meshPath.empty())
		{
			meshToDraw = MeshManager::GetInstance().GetMesh(cube.meshPath);
		}

		if (meshToDraw && meshToDraw->vertexCount > 0)
		{
			meshToDraw->DrawMesh();
		}
		else
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

	glBindVertexArray(0);
}


void setupVertexBuffer(std::vector<Vertex>& vertices, GLuint& VAO, GLuint& VBO, GLuint vertexCount)
{
	vertexCount = vertices.size();
	//gen and bind vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//gen and bind vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
						 (void*)offsetof(Vertex, position));
    
	// texcoords location 1
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						 (void*)offsetof(Vertex, uv));
    
	// Normal location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
						 (void*)offsetof(Vertex, normal));
    
	// unbind vao
	glBindVertexArray(0);
}

void setupPhongUniforms(GLuint shaderProgram, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	//  uniform locations
	GLint modelLoc = glGetUniformLocation(shaderProgram, "modelMatrix");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "viewMatrix");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projectionMatrix");
	GLint normalMatrixLoc = glGetUniformLocation(shaderProgram, "normalMatrix");
	GLint eyePosLoc = glGetUniformLocation(shaderProgram, "eyePosition");
    
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
	// calc and set normal matrix
	// glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));
	glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    
	// eye position
	glm::vec3 eyePos(0.0f, 5.0f, 10.0f);
	glUniform3fv(eyePosLoc, 1, glm::value_ptr(eyePos));
    
	//  lighting uniforms
	GLint lightPosLoc = glGetUniformLocation(shaderProgram, "light_position");
	glUniform3f(lightPosLoc, 10.0f, 25.0f, 15.0f);
    
	GLint lightAttLoc = glGetUniformLocation(shaderProgram, "light_attenuation");
	glUniform3f(lightAttLoc, 1.0f, 0.1f, 0.01f);
    
	GLint ambientLoc = glGetUniformLocation(shaderProgram, "light_ambient");
	glUniform4f(ambientLoc, 0.2f, 0.2f, 0.2f, 1.0f);
    
	GLint diffuseLoc = glGetUniformLocation(shaderProgram, "light_diffuse");
	glUniform4f(diffuseLoc, 1.0f, 0.9f, 0.8f, 1.0f);
    
	GLint specularLoc = glGetUniformLocation(shaderProgram, "light_specular");
	glUniform4f(specularLoc, 0.9f, 0.8f, 0.7f, 1.0f);
    
	GLint shineLoc = glGetUniformLocation(shaderProgram, "materialShininess");
	glUniform1i(shineLoc, 32);
}
