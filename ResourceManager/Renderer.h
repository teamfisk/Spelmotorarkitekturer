#pragma once
class Model;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(const Model * model);
};

