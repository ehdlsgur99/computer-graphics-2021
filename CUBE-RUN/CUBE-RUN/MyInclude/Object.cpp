#include "common.h"

Object::Object(const char* objFilename, glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate)
	: m_vScale(scale), m_vRotate(rotate), m_vPivot(translate)
{
	// load obj file
	ObjectBox objBox = ObjectManager::GetInstance()->checkObjFile(objFilename);
	if (strcmp(objBox.objName.c_str(), objFilename) != 0)
		loadObject(objFilename);
	else
	{
		m_iVAO = objBox.VAO;
		m_iVBO = objBox.VBO;
		m_vVertexes.resize((int)(objBox.m_vVertexes.size()));
		std::copy(objBox.m_vVertexes.begin(), objBox.m_vVertexes.end(), m_vVertexes.begin());
	}



	// set object color
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_real_distribution<float> uid(0.0f, 1.0f);

	m_vColor = glm::vec3(uid(dre), uid(dre), uid(dre));

	setScale(scale);
	setRotate(rotate);
	setTranslate(translate);

	updateModelTransfrom();

	initailizeBuffer();

	if (strcmp(objBox.objName.c_str(), objFilename) != 0)
	{
		ObjectManager::GetInstance()->addObjtoBox(objFilename, m_iVAO, m_iVBO, m_vVertexes);
	}
}

Object::Object(const char* objFilename, glm::vec3 scale, glm::vec3 rotate, glm::vec3 translate, glm::vec3 color)
{
	// load obj file
	loadObject(objFilename);

	// set object color
	m_vColor = color;

	setScale(scale);
	setRotate(rotate);
	setTranslate(translate);

	updateModelTransfrom();

	initailizeBuffer();
}

Object::~Object()
{
	m_vVertexes.clear();
	//delete[] m_iElementBuffer;

	glDeleteBuffers(1, &m_iVBO);

	glDeleteVertexArrays(1, &m_iVAO);

}

void Object::initailizeBuffer()
{
	glGenVertexArrays(1, &m_iVAO);
	glGenBuffers(1, &m_iVBO);
	//glGenBuffers(1, &m_iEBO);


	glBindVertexArray(m_iVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);

	glBufferData(GL_ARRAY_BUFFER, m_vVertexes.size() * sizeof(Vertexes), &m_vVertexes[0], GL_STATIC_DRAW);

	// vtxes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexes), (void*)0);
	glEnableVertexAttribArray(0);

	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexes), (void*)offsetof(Vertexes, Normal));
	glEnableVertexAttribArray(1);

	// texture coordinate
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertexes), (void*)offsetof(Vertexes, TexCoordinate));
	glEnableVertexAttribArray(2);
}

void Object::loadObject(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");

	if (fp == NULL) {
		std::cerr << "Invalid Object File!!\n";
		exit(100);
	}

	char buff[100];
	int faceNum = 0;
	std::vector<glm::vec3> vtx;
	std::vector<glm::vec3> nor;
	std::vector<glm::vec2> tex;

	while (!feof(fp)) {
		if (fscanf(fp, "%s", buff) == 3) printf("hello");

		// vertex or element buffer
		if (buff[0] == 'v' && buff[1] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(1);
			vtx.push_back(pos);

		}
		// vertex normal
		else if (buff[0] == 'v' && buff[1] == 'n' && buff[2] == '\0') {
			glm::vec3 pos;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &pos.z) != 3) exit(1);
			nor.push_back(pos);
		}
		// vertex texture coordinate
		else if (buff[0] == 'v' && buff[1] == 't' && buff[2] == '\0') {
			glm::vec2 pos;
			float temp;
			if (fscanf(fp, "%f %f %f", &pos.x, &pos.y, &temp) != 3) exit(1);
			tex.push_back(pos);
		}
		else if (buff[0] == 'f' && buff[1] == '\0') {
			Vertexes temp;
			int v, t, n;
			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.Position = vtx[v - 1];
			temp.Normal = nor[n - 1];
			temp.TexCoordinate = tex[t - 1];
			m_vVertexes.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.Position = vtx[v - 1];
			temp.Normal = nor[n - 1];
			temp.TexCoordinate = tex[t - 1];
			m_vVertexes.push_back(temp);

			if (fscanf(fp, "%d/%d/%d", &v, &t, &n) != 3) exit(1);
			temp.Position = vtx[v - 1];
			temp.Normal = nor[n - 1];
			temp.TexCoordinate = tex[t - 1];
			m_vVertexes.push_back(temp);
		}

		memset(buff, NULL, sizeof(buff));
	}

	for (int i = 0; i < faceNum * 3; ++i) {
		m_iElementBuffer[i]--;
	}


	vtx.clear();
	nor.clear();
	tex.clear();

	fclose(fp);
}

void Object::draw()
{
	glBindVertexArray(m_iVAO);

	glDrawArrays(GL_TRIANGLES, 0, m_vVertexes.size());

	glBindVertexArray(0);
}

glm::vec3 Object::getScaleVec() const
{
	return m_vScale;
}

glm::vec3 Object::getRotateVec() const
{
	return m_vRotate;
}

glm::vec3 Object::getTranslateVec() const
{
	return m_vPivot;
}

void Object::setTranslate(glm::vec3 vOffset)
{
	m_vPivot = vOffset;

	m_mTranslate = glm::translate(glm::mat4(1.0f), m_vPivot);
	Object::updateModelTransfrom();
}

void Object::setScale(glm::vec3 vOffset)
{
	m_vScale = vOffset;

	m_mScale = glm::scale(glm::mat4(1.0f), vOffset);
	Object::updateModelTransfrom();
}

void Object::setRotate(glm::vec3 vOffset)
{
	m_vRotate = vOffset;
	m_mRotate = glm::rotate(glm::mat4(1.0f), glm::radians(m_vRotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_mRotate = glm::rotate(m_mRotate, glm::radians(m_vRotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_mRotate = glm::rotate(m_mRotate, glm::radians(m_vRotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	Object::updateModelTransfrom();
}

void Object::updateModelTransfrom()
{
	m_mSRTModel = m_mTranslate * m_mRotate * m_mScale;
}

glm::mat4 Object::getModelTransform() const
{
	return m_mSRTModel;
}

glm::mat4 Object::getScaleTransform() const
{
	return m_mScale;
}

glm::mat4 Object::getRotateTransform() const
{
	return m_mRotate;
}

glm::mat4 Object::getTranslateTransform() const
{
	return m_mTranslate;
}

void Object::uniformModelingMat(unsigned int shaderNum)
{
	unsigned int modelLocation = glGetUniformLocation(shaderNum, "modelTransform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_mSRTModel));
}

void Object::uniformColor(unsigned int shaderNum)
{
	unsigned int color = glGetUniformLocation(shaderNum, "objectColor");
	glUniform3fv(color, 1, glm::value_ptr(m_vColor));
}