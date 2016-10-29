#include <GL/glew.h>
#include "Renderer.h"
#include "InstanceData.h"
#include "Model.h"

Renderer::Renderer()
{
	glGenSamplers(1, &linearSampler);
	glSamplerParameteri(linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(linearSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


Renderer::~Renderer()
{
}
//
//void Renderer::AddToRender(Model * model, InstanceData* instancedata)
//{	
//	renderList.emplace_back(std::make_tuple(model, instancedata));
//}

void Renderer::Render(Model * model, const glm::mat4x4& worldMat, Texture* texture, GLuint programHandle)
{	
	glUniformMatrix4fv(glGetUniformLocation(programHandle, "world"), 1, GL_FALSE, glm::value_ptr(worldMat));	
	texture->Bind();
	glBindSampler(0, linearSampler);
	

	glBindVertexArray(model->GetVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->GetIndexBuffer());
	glDrawElements(GL_TRIANGLES, model->GetIndicesCount(), model->GetIndexType(), (GLvoid*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0	);
	glBindVertexArray(0);

	//for(unsigned int i = 0; i < renderList.size(); i++)
	//{
	//	Model* model = std::get<0>(renderList[i]);
	//	InstanceData* instanceData = std::get<1>(renderList[i]);

	//	// TODO: Upload instance data.

	//	glBindVertexArray(model->GetVAO());
	//	glDrawElements(GL_TRIANGLES, model->GetIndicesCount(), model->GetIndexType(), model->GetIndices());
	//	glBindVertexArray(0);
	//}
}