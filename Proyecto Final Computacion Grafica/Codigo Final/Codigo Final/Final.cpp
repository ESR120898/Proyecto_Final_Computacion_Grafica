/*--------------------------------------------------------*/
/* ----------------- Proyecto Final ------------------*/
/*-----------------    2021-2   --------------------------*/
/*---------- Alumnos: Ibarra Mata Katherine --------------*/
/*----------------- Santiago Edwin ---------------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>

//Librerias de Musica
#include <irrKlang.h>
using namespace irrklang;

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 60.0f, 300.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Musica
ISoundEngine *SoundEngine = createIrrKlangDevice();

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//Keyframes (Manipulación y dibujo) variables     ------------------modificado empieza aqui
// posiciones del tiburon
float	mov_tibu_x = 0.0f,
		mov_tibu_y = 0.0f,
		mov_tibu_z = 0.0f,
		cabezatibu = 0.0f,
		colatibu = 0.0f;

float	mov_tibu_x_Inc = 0.0f,
		mov_tibu_y_Inc = 0.0f,
		mov_tibu_z_Inc = 0.0f,
		cabezatibuInc = 0.0f,
		colatibuInc = 0.0f;

// posiciones del Subeybaja-1
float	mov_syb_x = 0.0f,
		mov_syb_y = 0.0f,
		mov_syb_z = 0.0f,
		subeybaja01 = 0.0f,
		subeybaja02 = 0.0f;

float	mov_syb_x_Inc = 0.0f,
		mov_syb_y_Inc = 0.0f,
		mov_syb_z_Inc = 0.0f,
		subeybaja01Inc = 0.0f,
		subeybaja02Inc = 0.0f;

//Arreglos para la animacion del tiburon

#define MAX_FRAMES3 20
int i_max_steps3 = 60;
int i_curr_steps3 = 0;
typedef struct _frame3
{
	//Variables para GUARDAR Key Frames
	float mov_tibu_x;		//Variable para PosicionX
	float mov_tibu_y;		//Variable para PosicionY
	float mov_tibu_z;		//Variable para PosicionZ
	float cabezatibu;
	float colatibu;

}FRAME3;

FRAME3 KeyFrame3[MAX_FRAMES3];
int FrameIndex3 = 10;			//introducir datos
bool play3 = false;
int playIndex3 = 0;

void resetElements3(void)
{
	mov_tibu_x = KeyFrame3[0].mov_tibu_x;
	mov_tibu_y = KeyFrame3[0].mov_tibu_y;
	mov_tibu_z = KeyFrame3[0].mov_tibu_z;
	cabezatibu = KeyFrame3[0].cabezatibu;
	colatibu = KeyFrame3[0].colatibu;
}

void interpolation3(void)
{
	mov_tibu_x_Inc = (KeyFrame3[playIndex3 + 1].mov_tibu_x - KeyFrame3[playIndex3].mov_tibu_x) / i_max_steps3;
	mov_tibu_y_Inc = (KeyFrame3[playIndex3 + 1].mov_tibu_y - KeyFrame3[playIndex3].mov_tibu_y) / i_max_steps3;
	mov_tibu_z_Inc = (KeyFrame3[playIndex3 + 1].mov_tibu_z - KeyFrame3[playIndex3].mov_tibu_z) / i_max_steps3;
	cabezatibuInc = (KeyFrame3[playIndex3 + 1].cabezatibu - KeyFrame3[playIndex3].cabezatibu) / i_max_steps3;
	colatibuInc = (KeyFrame3[playIndex3 + 1].colatibu - KeyFrame3[playIndex3].colatibu) / i_max_steps3;
}

// Arreglo para la animacion del Subeybaja

#define MAX_FRAMES4 20
int i_max_steps4 = 60;
int i_curr_steps4 = 0;
typedef struct _frame4
{
	//Variables para GUARDAR Key Frames
	float mov_syb_x;		//Variable para PosicionX
	float mov_syb_y;		//Variable para PosicionY
	float mov_syb_z;		//Variable para PosicionZ
	float subeybaja01;
	float subeybaja02;

}FRAME4;

FRAME4 KeyFrame4[MAX_FRAMES4];
int FrameIndex4 = 9;			//introducir datos
bool play4 = false;
int playIndex4 = 0;

void resetElements4(void)
{
	mov_syb_x = KeyFrame4[0].mov_syb_x;
	mov_syb_y = KeyFrame4[0].mov_syb_y;
	mov_syb_z = KeyFrame4[0].mov_syb_z;
	subeybaja01 = KeyFrame4[0].subeybaja01;
	subeybaja02 = KeyFrame4[0].subeybaja02;
}

void interpolation4(void)
{
	mov_syb_x_Inc = (KeyFrame4[playIndex4 + 1].mov_syb_x - KeyFrame4[playIndex4].mov_syb_x) / i_max_steps4;
	mov_syb_y_Inc = (KeyFrame4[playIndex4 + 1].mov_syb_y - KeyFrame4[playIndex4].mov_syb_y) / i_max_steps4;
	mov_syb_z_Inc = (KeyFrame4[playIndex4 + 1].mov_syb_z - KeyFrame4[playIndex4].mov_syb_z) / i_max_steps4;
	subeybaja01Inc = (KeyFrame4[playIndex4 + 1].subeybaja01 - KeyFrame4[playIndex4].subeybaja01) / i_max_steps4;
	subeybaja02Inc = (KeyFrame4[playIndex4 + 1].subeybaja02 - KeyFrame4[playIndex4].subeybaja02) / i_max_steps4;

}

//Animacion del tiburon
void animate(void)
{
	if (play3)
	{
		if (i_curr_steps3 >= i_max_steps3) //end of animation between frames?
		{
			playIndex3++;
			if (playIndex3 > FrameIndex3 - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex3 = 0;
				play3 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps3 = 0; //Reset counter
								  //Interpolation
				interpolation3();
			}
		}
		else
		{
			//Draw animation
			mov_tibu_x += mov_tibu_x_Inc;
			mov_tibu_y += mov_tibu_y_Inc;
			mov_tibu_z += mov_tibu_z_Inc;
			cabezatibu += cabezatibuInc;
			colatibu += colatibuInc;
			i_curr_steps3++;
		}
	}

	//Animacion del Subeybaja
	if (play4)
	{
		if (i_curr_steps4 >= i_max_steps4) //end of animation between frames?
		{
			playIndex4++;
			if (playIndex4 > FrameIndex4 - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex4 = 0;
				play4 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps4 = 0; //Reset counter
								  //Interpolation
				interpolation4();
			}
		}
		else
		{
			//Draw animation
			mov_syb_x += mov_syb_x_Inc;
			mov_syb_y += mov_syb_y_Inc;
			mov_syb_z += mov_syb_z_Inc;
			subeybaja01 += subeybaja01Inc;
			subeybaja02 += subeybaja02Inc;
			i_curr_steps4++;
		}
	}
}
 // ----------------------------------------------modificado acaba aqui
void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);
	//SoundEngine->play2D("Musica/Muerte.mp3", true);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/derecha.jpg",
		"resources/skybox/izquierda.jpg",
		"resources/skybox/arriba.jpg",
		"resources/skybox/abajo.jpg",
		"resources/skybox/atras.jpg",
		"resources/skybox/fondo.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso_new/piso_new.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model modelcasa("resources/objects/casa01/Modelo_Casa.obj");
	Model ModelMedidor("resources/objects/Medidor/medidorluz-1.obj");
	Model SubeyBaja01("resources/objects/subeybaja/subeybaja_01.obj");
	Model SubeyBaja02("resources/objects/subeybaja/subeybaja_02.obj");
	Model SubeyBaja03("resources/objects/subeybaja/subeybaja_centro.obj"); //-------------modificado
	Model shark01("resources/objects/tiburon/cabeza.obj");  //-----------------modificado
	Model shark02("resources/objects/tiburon/cola.obj");  //------------------modificado
	Model pool("resources/objects/pool/pool01.obj"); //----------------------modifcado

	//Inicialización de KeyFrames para la animacion del tiburon -----modificado

	KeyFrame3[0].mov_tibu_x = 0;
	KeyFrame3[0].mov_tibu_y = 0;
	KeyFrame3[0].mov_tibu_z = 0;
	KeyFrame3[0].cabezatibu = 0;
	KeyFrame3[0].colatibu = 0;

	KeyFrame3[1].mov_tibu_x = 0;
	KeyFrame3[1].mov_tibu_y = 0;
	KeyFrame3[1].mov_tibu_z = 0;
	KeyFrame3[1].cabezatibu = 5;
	KeyFrame3[1].colatibu = 10;

	KeyFrame3[2].mov_tibu_x =50;
	KeyFrame3[2].mov_tibu_y =0;
	KeyFrame3[2].mov_tibu_z = 0;
	KeyFrame3[2].cabezatibu = -10;
	KeyFrame3[2].colatibu = 15;

	KeyFrame3[3].mov_tibu_x = 100;
	KeyFrame3[3].mov_tibu_y = 0;
	KeyFrame3[3].mov_tibu_z = 0;
	KeyFrame3[3].cabezatibu = 10;
	KeyFrame3[3].colatibu = -15;

	KeyFrame3[4].mov_tibu_x = 150;
	KeyFrame3[4].mov_tibu_y = 0;
	KeyFrame3[4].mov_tibu_z = 0;
	KeyFrame3[4].cabezatibu = -10;
	KeyFrame3[4].colatibu = 15;

	KeyFrame3[5].mov_tibu_x = 250;
	KeyFrame3[5].mov_tibu_y = 0;
	KeyFrame3[5].mov_tibu_z = 0;
	KeyFrame3[5].cabezatibu = 10;
	KeyFrame3[5].colatibu = -15;

	KeyFrame3[6].mov_tibu_x = 300;
	KeyFrame3[6].mov_tibu_y = 0;
	KeyFrame3[6].mov_tibu_z = 0;
	KeyFrame3[6].cabezatibu = -10;
	KeyFrame3[6].colatibu = 15;

	KeyFrame3[7].mov_tibu_x = 350;
	KeyFrame3[7].mov_tibu_y = 0;
	KeyFrame3[7].mov_tibu_z = 0;
	KeyFrame3[7].cabezatibu = 10;
	KeyFrame3[7].colatibu = -15;

	KeyFrame3[8].mov_tibu_x = 400;
	KeyFrame3[8].mov_tibu_y = 0;
	KeyFrame3[8].mov_tibu_z = 0;
	KeyFrame3[8].cabezatibu = -10;
	KeyFrame3[8].colatibu = 15;

	KeyFrame3[8].mov_tibu_x = 450;
	KeyFrame3[8].mov_tibu_y = -20;
	KeyFrame3[8].mov_tibu_z = 0;
	KeyFrame3[8].cabezatibu = -10;
	KeyFrame3[8].colatibu = 15;

	KeyFrame3[9].mov_tibu_x = 0;
	KeyFrame3[9].mov_tibu_y = 0;
	KeyFrame3[9].mov_tibu_z = 0;
	KeyFrame3[9].cabezatibu = -10;
	KeyFrame3[9].colatibu = 15;

	//Inicialización de KeyFrames para la animacion del Subeybaja
	
	KeyFrame4[0].mov_syb_x = 0;
	KeyFrame4[0].mov_syb_y = 0;
	KeyFrame4[0].mov_syb_z = 0;
	KeyFrame4[0].subeybaja01 = 0;
	KeyFrame4[0].subeybaja02 = 0;

	KeyFrame4[1].mov_syb_x = 0;
	KeyFrame4[1].mov_syb_y = 0;
	KeyFrame4[1].mov_syb_z = 0;
	KeyFrame4[1].subeybaja01 = 30;
	KeyFrame4[1].subeybaja02 = -30;

	KeyFrame4[2].mov_syb_x = 0;
	KeyFrame4[2].mov_syb_y = 0;
	KeyFrame4[2].mov_syb_z = 0;
	KeyFrame4[2].subeybaja01 = -30;
	KeyFrame4[2].subeybaja02 = 30;

	KeyFrame4[3].mov_syb_x = 0;
	KeyFrame4[3].mov_syb_y = 0;
	KeyFrame4[3].mov_syb_z = 0;
	KeyFrame4[3].subeybaja01 = 30;
	KeyFrame4[3].subeybaja02 = -30;

	KeyFrame4[4].mov_syb_x = 0;
	KeyFrame4[4].mov_syb_y = 0;
	KeyFrame4[4].mov_syb_z = 0;
	KeyFrame4[4].subeybaja01 = -30;
	KeyFrame4[4].subeybaja02 = 30;

	KeyFrame4[5].mov_syb_x = 0;
	KeyFrame4[5].mov_syb_y = 0;
	KeyFrame4[5].mov_syb_z = 0;
	KeyFrame4[5].subeybaja01 = 30;
	KeyFrame4[5].subeybaja02 = -30;

	KeyFrame4[6].mov_syb_x = 0;
	KeyFrame4[6].mov_syb_y = 0;
	KeyFrame4[6].mov_syb_z = 0;
	KeyFrame4[6].subeybaja01 = -30;
	KeyFrame4[6].subeybaja02 = 30;

	KeyFrame4[7].mov_syb_x = 0;
	KeyFrame4[7].mov_syb_y = 0;
	KeyFrame4[7].mov_syb_z = 0;
	KeyFrame4[7].subeybaja01 = 30;
	KeyFrame4[7].subeybaja02 = -30;

	KeyFrame4[8].mov_syb_x = 0;
	KeyFrame4[8].mov_syb_y = 0;
	KeyFrame4[8].mov_syb_z = 0;
	KeyFrame4[8].subeybaja01 = -30;
	KeyFrame4[8].subeybaja02 = 30;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//tiburon cabeza 
		
		model = glm::translate(glm::mat4(1.0f), glm:: vec3(300.0f, 17.0f, -380.0f));
		model = glm::scale(model, glm::vec3(0.5f)); //-------------------------------------------modificado
		model = glm::translate(model, glm::vec3(mov_tibu_x, mov_tibu_y, mov_tibu_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(cabezatibu), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		shark01.Draw(staticShader);

	    // tiburon cola 
		
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(colatibu), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		shark02.Draw(staticShader);

		//Alberca ------------------------modificado
		model = glm::translate(glm::mat4(1.0f), glm::vec3(420.0f, 0.0f, -365.0f));
		model = glm::scale(model, glm::vec3(1.8f, 0.5f, 1.3f));
		staticShader.setMat4("model", model);
		pool.Draw(staticShader);

		/*casa1 (primera empezando por la alberca)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(595.0f, 0.0f, -51.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);*/
		
		/*//casa2 (segunda empezando por la alberca)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(595.0f, 0.0f, 133.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//casa3 (tercera empezando por la alberca)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(595.0f, 0.0f, 314.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//casa4 (primera empezando por el area de juegos)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-271.0f, 0.0f, -46.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//casa5 (segunda empezando por el area de juegos)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-271.0f, 0.0f, 129.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//casa6 (tercera empezando por el area de juegos)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-271.0f, 0.0f, 314.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//MedidorLuz casa01
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3( 420.0f, 0.0f, -51.3f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);

		//MedidorLuz casa02
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, 133.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);

		//MedidorLuz casa03
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, 314.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);

		//MedidorLuz casa04
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, -54.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);

		//MedidorLuz casa05
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 129.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);

		//MedidorLuz casa06
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 0.0f, 314.0f));
		model = glm::scale(model, glm::vec3(0.35f));
		staticShader.setMat4("model", model);
		ModelMedidor.Draw(staticShader);
		*/

		//Subeybaja-1 -----------------------------modificado
	
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, 0.0f, -360.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mov_syb_x, mov_syb_y, mov_syb_z));
		staticShader.setMat4("model", model);
		SubeyBaja03.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, -3.0f, -360));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(subeybaja01), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		SubeyBaja01.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-120.0f, -3.0f, -360.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(subeybaja02), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		SubeyBaja02.Draw(staticShader);

		
		//Subeybaja-2 ---------------------------modificado
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, 0.0f, -330.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mov_syb_x, mov_syb_y, mov_syb_z));
		staticShader.setMat4("model", model);
		SubeyBaja03.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -3.0f, -330.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(subeybaja01), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		SubeyBaja01.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -3.0f, -330.0f));
		model = glm::scale(model, glm::vec3(0.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(subeybaja02), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		SubeyBaja02.Draw(staticShader);
		
		
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		colatibu += 3.0f;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		colatibu -= 3.0f;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		subeybaja01--;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		subeybaja01++;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		subeybaja02--;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		subeybaja02++;
	//Movimiento para tiburon
	if (key == GLFW_KEY_T && action == GLFW_PRESS)  //-------modificado
	{
		//Silbido->play2D("Musica/Silbando.mp3", true);
		//Fondo->stopAllSounds();
		if (play3 == false && (FrameIndex3 > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements3();

			interpolation3();

			play3 = true;
			playIndex3 = 0;
			i_curr_steps3 = 0;
		}
		else
		{
			play3 = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
	//Moviemito del sube y baja   ---------------------modificado

	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		//Silbido->play2D("Musica/Silbando.mp3", true);
		//Fondo->stopAllSounds();
		if (play4 == false && (FrameIndex4 > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements4();

			interpolation4();

			play4 = true;
			playIndex4 = 0;
			i_curr_steps4 = 0;
		}
		else
		{
			play4 = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
	
}
	

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}