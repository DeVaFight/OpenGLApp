#include "Plane.h"



Plane::Plane()
{
}

Plane::Plane(const ShapesBuilder builder): Shape()
{
	sourceShapeType = builder.m_shapeType;
	state = builder.m_view;
	light = builder.m_light;
	outsideLight = builder.m_pos;
	GLfloat g_vertex_buffer_data[] = {
		-.5f, -.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,//0
		.5f, -.5f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //1
		-.5f,  -.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//2
		.5f, -.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f//3
	};
	unsigned int indexes[] = {
		0, 2, 3,
		0, 1, 3,
	};
	CreateShape(g_vertex_buffer_data, indexes);
	CreateType();
}

void Plane::CreateShape(const GLfloat * points, unsigned int * orderIndex)
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	texture = new Texture("Images\\BrickMedievalBlocks0129_2_M.jpg");
	specularTexture = new Texture("Images\\BrickMedievalBlocks0129_2_M_specular_map.jpg");
	va = new VertexArray();
	vb = new VertexBuffer(points, sizeof(float) * 4 * (3 + 2 + 3));

	layout = new VertexBufferLayout();
	layout->Push<float>(3); // vertexes
	layout->Push<float>(2); //textcoorssd
	layout->Push<float>(3); // normals
	va->AddBuffer(*vb, *layout);
	ib = new IndexBuffer(orderIndex, 6); //indexes count
}

void Plane::GenerateShaders()
{
	shaders.push_back(new Shader("Shaders\\DiffuseVertexShader.vert"));
	shaders.push_back(new Shader("Shaders\\DiffuseFragmentShader.frag"));
	sm.AddShadersToProgram(shaders);
	sm.Bind();
	CreateMVPMatrix();
	shaders[0]->SetUniformMat4f("model", model, sm.GetProgram());
	shaders[0]->SetUniformMat4f("projection", proj, sm.GetProgram());
	shaders[0]->SetUniformMat4f("view", view, sm.GetProgram());
	texture->Bind();
	shaders[1]->SetUniform1i("material.diffuse", 0, sm.GetProgram());
	specularTexture->Bind(1);
	shaders[1]->SetUniform1i("material.specular", 1, sm.GetProgram());
	shaders[1]->SetUniform3f("light.lightPos", outsideLight.x, outsideLight.y, outsideLight.y, sm.GetProgram());
	shaders[1]->SetUniform3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f), sm.GetProgram());
	shaders[1]->SetUniform3f("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f), sm.GetProgram());
	shaders[1]->SetUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f), sm.GetProgram());

	shaders[1]->SetUniform1f("light.constant", 1.0f, sm.GetProgram());
	shaders[1]->SetUniform1f("light.linear", 0.2f, sm.GetProgram());
	shaders[1]->SetUniform1f("light.quadratic", 0.2f, sm.GetProgram());

	shaders[1]->SetUniform3f("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f), sm.GetProgram());
	shaders[1]->SetUniform1f("material.shininess", 32.0f, sm.GetProgram());

	shaders[1]->SetUniform3f("camPos", camEyePos, sm.GetProgram());
	shaders[1]->SetUniform3f("sunLight", 0.99f, 0.71f, 0.32f, sm.GetProgram());
	shaders[1]->SetUniform3f("skyLight", 0.51f, 0.80f, 0.92f, sm.GetProgram());
	texture->UnBind();
	specularTexture->UnBind();
}
void Plane::GetSourceLight(glm::vec3 lightSource)
{
	m_sourceLight = lightSource;
}
void Plane::CreateMVPMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(50.0f, 1.0f, 5.0f));
	proj = glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 100.0f);
	//mvp = proj * view * model;
}
void Plane::TurnOffShapeElements()
{
	va->UnBind();
	vb->UnBind();
	ib->UnBind();
	sm.UnBind();
}

void Plane::Update()
{
	sm.Bind();
	texture->Bind();
	specularTexture->Bind(1);
	shaders[0]->SetUniformMat4f("model", model, sm.GetProgram());
	shaders[0]->SetUniformMat4f("projection", proj, sm.GetProgram());
	shaders[0]->SetUniformMat4f("view", view, sm.GetProgram());
	shaders[1]->SetUniform3f("camPos", camEyePos, sm.GetProgram());
	shaders[1]->SetUniform3f("light.lightPos", outsideLight.x, outsideLight.y, outsideLight.y, sm.GetProgram());
	sm.UnBind();
	sm.Bind();
	ib->Bind();
	va->Bind();
}


Plane::~Plane()
{
}
