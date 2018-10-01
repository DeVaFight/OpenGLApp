#include "Shape.h"
class Plane: public Shape
{
public:
	Plane();
	void CreateShape(const GLfloat* points, unsigned int* orderIndex) override;
	void TurnOffShapeElements() override;
	void Update(glm::mat4 camView) override;
	~Plane();
private:
	void CreateMVPMatrix() override;
	void GenerateShaders() override;
};

