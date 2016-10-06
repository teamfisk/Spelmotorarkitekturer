#include "Renderer.h"
#include <GL\glew.h>
#include "Model.h"

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Render(const Model * model)
{
	GLenum vao = model->GetVAO();	

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model->GetIndicesCount(), model->GetIndexType(), model->GetIndices());
	glBindVertexArray(0);
}
