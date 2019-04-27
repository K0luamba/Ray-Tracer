//internal includes
#include "common.h"
#include "ShaderProgram.h"
#include "LiteMath.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <random>

static GLsizei WIDTH = 512, HEIGHT = 512; //задаем размеры окна

using namespace LiteMath;
//начальное положение камеры
float3 g_camPos(0, 0, 5);
float  cam_rot[2] = {0,0};
int    mx = 0, my = 0;
bool keys[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //массив для клавиш: D A S W R F Z X C V 1
float mirrorPos = 0.0;
float lightPower = 0.75;
int AA = -1;
bool firstTime = true; //нужно для поэтапного рендеринга

void windowResize(GLFWwindow* window, int width, int height)
{
  WIDTH  = width;
  HEIGHT = height;
}

//реакция на движение курсора
static void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
  xpos *= 0.08f;
  ypos *= 0.08f;

  int x1 = int(xpos);
  int y1 = int(ypos);

  cam_rot[0] -= 0.25f*(y1 - my);	//Изменение угола поворота
  cam_rot[1] -= 0.25f*(x1 - mx);

  mx = int(xpos); //запоминаем координаты
  my = int(ypos);
}

void key_call()
{
        int speed = 1;
        if (keys[6] == 1) //ускорение
            speed = 3; 
        if (keys[0] == 1)
            g_camPos.x += speed * 0.1f; //вправо
        if (keys[1] == 1)
            g_camPos.x -= speed * 0.1f; //влево
        if (keys[2] == 1)
            g_camPos.z += speed * 0.1f; //назад
        if (keys[3] == 1)
            g_camPos.z -= speed * 0.1f; //вперед
        if (keys[4] == 1)
            g_camPos.y += speed * 0.1f; //вверх
        if (keys[5] == 1)
            g_camPos.y -= speed * 0.1f; //вниз
        //движение зеркала
        if (keys[7] == 1)
            mirrorPos -= 0.1f;
        if (keys[8] == 1)
            mirrorPos += 0.1f;
        //изменение освещенности
        if (keys[9] == 1)
            lightPower -= 0.01f;
        if (keys[10] == 1)
            lightPower += 0.01f; 
        //управление анти-алиасингом 
        if (keys[11] == 1)  
            AA *= -1;        
}

void key_set(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        //смотрим, что сейчас нажато
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
            keys[0] = 1;  
        if (key == GLFW_KEY_D && action == GLFW_RELEASE)
            keys[0] = 0; 
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
            keys[1] = 1;  
        if (key == GLFW_KEY_A && action == GLFW_RELEASE)
            keys[1] = 0; 
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
            keys[2] = 1;  
        if (key == GLFW_KEY_S && action == GLFW_RELEASE)
            keys[2] = 0; 
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
            keys[3] = 1;  
        if (key == GLFW_KEY_W && action == GLFW_RELEASE)
            keys[3] = 0; 
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
            keys[4] = 1;  
        if (key == GLFW_KEY_R && action == GLFW_RELEASE)
            keys[4] = 0; 
        if (key == GLFW_KEY_F && action == GLFW_PRESS)
            keys[5] = 1;  
        if (key == GLFW_KEY_F && action == GLFW_RELEASE)
            keys[5] = 0; 
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
            keys[6] = 1;  
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
            keys[6] = 0; 
        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
            keys[7] = 1;  
        if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
            keys[7] = 0; 
        if (key == GLFW_KEY_X && action == GLFW_PRESS)
            keys[8] = 1;  
        if (key == GLFW_KEY_X && action == GLFW_RELEASE)
            keys[8] = 0; 
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
            keys[9] = 1;  
        if (key == GLFW_KEY_C && action == GLFW_RELEASE)
            keys[9] = 0; 
        if (key == GLFW_KEY_V && action == GLFW_PRESS)
            keys[10] = 1;  
        if (key == GLFW_KEY_V && action == GLFW_RELEASE)
            keys[10] = 0; 
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
            keys[11] = 1;  
        if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
            keys[11] = 0; 
        key_call();  //непосредственно действия, связанные с движением
}

int initGL()
{
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
    //системная инфа
	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "My OpenGL ray marching sample", nullptr, nullptr); //отсюда появляется окно
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//определение реакции на движение мыши, окна, нажатие клавиш
    glfwSetCursorPosCallback (window, mouseMove); 
    glfwSetWindowSizeCallback(window, windowResize);
    glfwSetKeyCallback(window, key_set);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	if(initGL() != 0) 
		return -1;
	
    //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	//создание шейдерной программы из двух файлов с исходниками шейдеров
	//используется класс-обертка ShaderProgram
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "vertex.glsl";
	shaders[GL_FRAGMENT_SHADER] = "fragment.glsl";
	ShaderProgram program(shaders); GL_CHECK_ERRORS; //инициализация вспосогательного класса, определение шейдеров разных типов

    glfwSwapInterval(1); // force 60 frames per second
      
    //Создаем и загружаем геометрию поверхности
    //
    GLuint g_vertexBufferObject;
    GLuint g_vertexArrayObject;
    {
 
    //один квадрат, по сути грань, задает область видимости
    float quadPos[] =
    {
      -1.0f,  1.0f,	// v0 - top left corner
      -1.0f, -1.0f,	// v1 - bottom left corner
      1.0f,  1.0f,	// v2 - top right corner
      1.0f, -1.0f	  // v3 - bottom right corner
    };

    g_vertexBufferObject = 0;
    GLuint vertexLocation = 0; // simple layout, assume have only positions at location = 0

    glGenBuffers(1, &g_vertexBufferObject);                                                        GL_CHECK_ERRORS;
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);                                           GL_CHECK_ERRORS;
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(GLfloat), (GLfloat*)quadPos, GL_STATIC_DRAW);     GL_CHECK_ERRORS;

    glGenVertexArrays(1, &g_vertexArrayObject);                                                    GL_CHECK_ERRORS;
    glBindVertexArray(g_vertexArrayObject);                                                        GL_CHECK_ERRORS;

    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBufferObject);                                           GL_CHECK_ERRORS;
    glEnableVertexAttribArray(vertexLocation);                                                     GL_CHECK_ERRORS;
    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);                            GL_CHECK_ERRORS;

    glBindVertexArray(0);
  }

	//цикл обработки сообщений и отрисовки сцены каждый кадр до закрытия окна
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		//очищаем экран каждый кадр
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);               GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        // (?) вызов непосредственно программы отрисовки объектов из fragment.glsl
        program.StartUseShader();                           GL_CHECK_ERRORS; 

        float4x4 camRotMatrix   = mul(rotate_Y_4x4(-cam_rot[1]), rotate_X_4x4(+cam_rot[0]));
        float4x4 camTransMatrix = translate4x4(g_camPos);
        float4x4 rayMatrix      = mul(camRotMatrix, camTransMatrix);
        //std::cout << "rayMatrix:";
        //for(int k=0; k < 4; k++)
        //       std::cout << rayMatrix.row[k].x << " " << rayMatrix.row[k].y << " " << rayMatrix.row[k].z << " " << rayMatrix.row[k].w << std::endl;
        //отправляем в шейдеры глобальные параметры программы
        program.SetUniform("g_mirrorPos", mirrorPos);
        program.SetUniform("g_lightPower", lightPower);
        program.SetUniform("g_AA", AA);
        program.SetUniform("g_firstTime", firstTime);
        program.SetUniform("g_rayMatrix", rayMatrix);
        program.SetUniform("g_screenWidth" , WIDTH);
        program.SetUniform("g_screenHeight", HEIGHT);

        // очистка и заполнение экрана цветом
        //
        glViewport  (0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //задание конкретного фона
        glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw call
        //
        glBindVertexArray(g_vertexArrayObject); GL_CHECK_ERRORS;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  GL_CHECK_ERRORS;  // The last parameter of glDrawArrays is equal to VS invocations
        
        program.StopUseShader();

		glfwSwapBuffers(window); 
        if (firstTime)
            firstTime = false;
	} //end while

	//очищаем vboи vao перед закрытием программы
    //
	glDeleteVertexArrays(1, &g_vertexArrayObject);
    glDeleteBuffers(1,      &g_vertexBufferObject);

	glfwTerminate();
	return 0;
}
