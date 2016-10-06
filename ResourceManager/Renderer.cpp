#include "Renderer.h"
#include <GL\glew.h>
#include "Model.h"
#include "InstanceData.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::AddToRender(Model * model, InstanceData* instancedata)
{	
	renderList.emplace_back(std::make_tuple(model, instancedata));
}

void Renderer::Render()
{
	for(unsigned int i = 0; i < renderList.size(); i++)
	{
		Model* model = std::get<0>(renderList[i]);
		glBindVertexArray(model->GetVAO());
		glDrawElements(GL_TRIANGLES, model->GetIndicesCount(), model->GetIndexType(), model->GetIndices());
		glBindVertexArray(0);
	}
}
