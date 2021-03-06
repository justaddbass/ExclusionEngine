#include "RenderingEngine.h"

#include <btBulletDynamicsCommon.h>

RenderingEngine::RenderingEngine() {
	attached_entity = CoreEngine::GetInstance().GetNullEntity();
	mouseSpeed = 0.0005f;
	fov = 45.0f;
	camera_style = FIRST_PERSON;
	isFocus = true;
	m_camera = new Camera();
	projection_matrix = Matrix4x4f(glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f));
}

void RenderingEngine::Init() {
	up = glm::vec3(0,1,0);
	shader = LoadShaders("res/basic.vs", "res/basic.fs");

	glUseProgram(shader);
	modelViewID = glGetUniformLocation(shader, "ModelViewMatrix");
	viewID = glGetUniformLocation(shader, "ViewMatrix");
	mvpID = glGetUniformLocation(shader, "MVP");
	textureID = glGetUniformLocation(shader, "texture_sampler");
	normalID = glGetUniformLocation(shader, "NormalMatrix");
	modelViewID = glGetUniformLocation(shader, "ModelViewMatrix");
	projectionID = glGetUniformLocation(shader, "ProjectionMatrix");

#if 0
	LightInfo light = LightInfo(glm::vec4(1.0,1.0,1.0, 0.0), glm::vec3(0.25), glm::vec3(0.25), glm::vec3(0.25));
	MaterialInfo material = MaterialInfo(glm::vec3(0.25), glm::vec3(0.25), glm::vec3(0.25), 0.5);

	const char* blockNames[] = {"Light", "Material"};
	const char* lightNames[] = {"Position", "La", "Ld", "Ls"};
	const char* materialNames[] = {"Ka", "Kd", "Ks", "Shininess"};
	GLuint ubo[2];
	glGenBuffers(2, ubo);
	for(unsigned int i = 0; i < 2; ++i) {
		GLuint blockIndex;
		GLint blockSize;
		blockIndex = glGetUniformBlockIndex(shader, blockNames[i]);
		glGetActiveUniformBlockiv(shader, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		GLubyte* blockBuffer = new GLubyte[blockSize];
		GLuint indices[4];
		GLint offsets[4];
		glBindBuffer(GL_UNIFORM_BUFFER, ubo[i]);
		if(i == 0) {
			glGetUniformIndices(shader, 4, lightNames, indices);
			glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offsets);
			std::memcpy(blockBuffer + offsets[0], (void*)&light.Position, sizeof(glm::vec4));
			std::memcpy(blockBuffer + offsets[1], (void*)&light.La, sizeof(glm::vec3));
			std::memcpy(blockBuffer + offsets[2], (void*)&light.Ld, sizeof(glm::vec3));
			std::memcpy(blockBuffer + offsets[3], (void*)&light.Ls, sizeof(glm::vec3));
		}
		else {
			glGetUniformIndices(shader, 4, materialNames, indices);
			glGetActiveUniformsiv(shader, 4, indices, GL_UNIFORM_OFFSET, offsets);
			std::memcpy(blockBuffer + offsets[0], (void*)&material.Ka, sizeof(glm::vec3));
			std::memcpy(blockBuffer + offsets[1], (void*)&material.Kd, sizeof(glm::vec3));
			std::memcpy(blockBuffer + offsets[2], (void*)&material.Ks, sizeof(glm::vec3));
			std::memcpy(blockBuffer + offsets[3], (void*)&material.Shininess, sizeof(GLfloat));
		}
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, ubo[i]);
		delete [] blockBuffer;
	}
#endif
	glUniform1i(textureID, 0);
}

Camera* RenderingEngine::GetCamera() {
	return m_camera;
}

void RenderingEngine::ComputeMatrices() {
    //int x_pos, y_pos;
    //SDL_GetMouseState(&x_pos, &y_pos);

    if(isFocus) {
        //SDL_WarpMouseInWindow(window->GetWindow(), _width/2, _height/2);
        //horizontalAngle += mouseSpeed * (_width/2 - x_pos);
        //verticalAngle += mouseSpeed * (_height/2 - y_pos);
    }

	// direction = glm::vec3(
	// 	cos(verticalAngle) * sin(horizontalAngle),
	// 	sin(verticalAngle),
	// 	cos(verticalAngle) * cos(horizontalAngle)
	// );
	//
	// right = glm::vec3(
	// 	sin(horizontalAngle - 3.14f/2.0f),
	// 	0,
	// 	cos(horizontalAngle - 3.14f/2.0f)
	// );
	//
	// front = glm::vec3(
	// 	sin(horizontalAngle - 3.13f/2.0),
	// 	0,
	// 	0
	// );


    if(camera_style == FREE_LOOK) {
		position = glm::vec3(0, 10, -50);

		if (is3D)
			projection_matrix = glm::perspective(fov, 4.0f / 3.0f, 0.1f, 100.0f);
		else
			projection_matrix = glm::mat4x4(1.0);
    	view_matrix = glm::lookAt(position, position+direction, up);
    }
    else if(camera_style == FIRST_PERSON) {
        m_camera->GetViewMatrix(view_matrix);
    }
    else { //THIRD_PERSON
		//glm::vec3 camera_pos = glm::vec3(
    }
}

Window* RenderingEngine::CreateWindow(const char* title, int width, int height) {
    //probably better to grab values from config script here than from inside main
    _width = width;
    _height = height;
    window = new Window(title, width, height);
    return window;
}

void RenderingEngine::RegisterEntity(Entity* entity) {
	entities.push_back(entity);
}

void RenderingEngine::Swap() {
    window->SwapBuffers();
}

void RenderingEngine::Render(Component& c) {
	mvp_matrix = projection_matrix * view_matrix * c.getModelMatrix();
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp_matrix[0]);
	c.Render();
}

void RenderingEngine::RenderScene() {
	ComputeMatrices();

	glUseProgram(shader);
	//view_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0, -15, -50.0));
	view_matrix.TranslateMatrix(Vec3f(0, -5, -25));
	//model_matrix = glm::mat4(1.0);
	//glm::mat4 modelViewMatrix = view_matrix * model_matrix;
	//glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &view_matrix[0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection_matrix[0]);
	//glUniformMatrix4fv(normalID, 1, GL_FALSE, &normalMatrix[0][0]);
	//glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &modelViewMatrix[0][0]);


	model_matrix.SetIdentity();
	mvp_matrix = projection_matrix * view_matrix * model_matrix;
	Matrix4x4f mv_matrix = view_matrix * model_matrix;
	//Matrix4x4f normalMatrix = glm::transpose(glm::inverse(mv_matrix));
	Matrix4x4f inverse_normal;
	mv_matrix.Inverse(inverse_normal);
	Matrix4x4f normalMatrix = inverse_normal.Transpose();
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp_matrix[0]);
	glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &mv_matrix[0]);
	glUniformMatrix4fv(normalID, 1, GL_FALSE, &normalMatrix[0]);
	//world->Render();

	for(std::vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i) {
		if((*i) == attached_entity && camera_style == FIRST_PERSON)
			continue;	//don't render attached entity in first person
		//btTransform entity_pos;
		//(*i)->getRigidBody()->getMotionState()->getWorldTransform(entity_pos);
		//btVector3 entity_vec = entity_pos.getOrigin();
		//entity_pos.getOpenGLMatrix(&model_matrix[0][0]);
		// model_matrix = glm::translate(glm::mat4(1.0), glm::vec3(entity_vec.x(), entity_vec.y(), entity_vec.z()));
		model_matrix.SetIdentity();
		model_matrix.TranslateMatrix((*i)->GetPosition());
		mvp_matrix = projection_matrix * view_matrix * model_matrix;
		Matrix4x4f mv_matrix = view_matrix * model_matrix;
		//Matrix4x4f normalMatrix = glm::transpose(glm::inverse(mv_matrix));
		Matrix4x4f inverse_normal;
		mv_matrix.Inverse(inverse_normal);
		Matrix4x4f normalMatrix = inverse_normal.Transpose();
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp_matrix[0]);
		glUniformMatrix4fv(modelViewID, 1, GL_FALSE, &mv_matrix[0]);
		glUniformMatrix4fv(normalID, 1, GL_FALSE, &normalMatrix[0]);
		(*i)->Render();
	}

	view_matrix.SetIdentity();
	view_matrix = Matrix4x4f(Mat3(m_camera->GetViewMatrix()));
	//projection_matrix =
	glUniformMatrix4fv(glGetUniformLocation(skybox->GetShader(), "view"), 1, GL_FALSE, &view_matrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(skybox->GetShader(), "projection"), 1, GL_FALSE, &projection_matrix[0]);
	//skybox->Draw();
}
