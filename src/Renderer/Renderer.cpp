#include <GL/glew.h>
#include "Renderer.h"
#include "InstanceData.h"
#include "Model.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}
//
//void Renderer::AddToRender(Model * model, InstanceData* instancedata)
//{	
//	renderList.emplace_back(std::make_tuple(model, instancedata));
//}

void Renderer::Render(Model * model)
{
	glBindVertexArray(model->GetVAO());
	glDrawElements(GL_TRIANGLES, model->GetIndicesCount(), model->GetIndexType(), model->GetIndices());
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