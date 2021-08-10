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

//Camara
Camera camera(glm::vec3(150.0f, 200.0f, 1100.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Musica
ISoundEngine *Fondo = createIrrKlangDevice();
ISoundEngine *Aguila = createIrrKlangDevice();
ISoundEngine *Silbido = createIrrKlangDevice();
ISoundEngine *Tiburon = createIrrKlangDevice();

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//Variables para la animacin de la mujer caminando
float mov_Mujer_Z = 0.0f,
	  mov_Mujer_X = 0.0f,
	  orienta = 0.0f,
	  orienta2 = 0.0f,
	  orienta3 = 0.0f;
bool Mujer_1 = true;
bool Mujer_2 = true;
bool Mujer_3 = true;
bool Mujer_4 = true;
bool recorrido1 = true;

//Variable para el dibujo y animacion del coche

bool animacion_coche = false;
bool carro_1 = true;
bool carro_2 = true;
bool carro_3 = true;
bool carro_4 = true;
bool carro_5 = true;
bool carro_6 = true;

float movAuto_x= 0.0f,
movAuto_y = 0.0f,
movAuto_z = 0.0f,
rotcoche = 0.0f,
giroLlantas = 0.0f;

//Variables para el dibujo y animacion del Aguila
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
giro_aguila = 0.0f,
alaIzq = 0.0f,
alaDer = 0.0f;

float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
giro_aguilaInc = 0.0f,
alaIzqInc = 0.0f,
alaDerInc = 0.0f;

//Variables para el dibujo y animacion de la persona tirando la basura
float	posX1 = 0.0f,
posY1 = 0.0f,
posZ1 = 0.0f,
rotpiernaIzq = 0.0f,
rotpiernaDer = 0.0f,
rotTorso = 0.0f,
rotBolsa = 0.0f,
rotCabeza = 0.0f,
rotbrazoIzq = 0.0f,
rotbrazoDer = 0.0f;

float	incposX1 = 0.0f,
incposY1 = 0.0f,
incposZ1 = 0.0f,
incrotpiernaIzq = 0.0f,
incrotpiernaDer = 0.0f,
incrotTorso = 0.0f,
incrotBolsa = 0.0f,
incrotCabeza = 0.0f,
incrotbrazoIzq = 0.0f,
incrotbrazoDer = 0.0f;

//Variables para el dibujo y animacion de la pelota
float	posX_ball = 0.0f,
posY_ball = 0.0f,
posZ_ball = 0.0f,
rotball = 0.0f;

float	posX_ball_INC = 0.0f,
posY_ball_INC = 0.0f,
posZ_ball_INC = 0.0f,
rotball_INC = 0.0f;


//Variables para el dibujo y animacion del tiburon
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

//Variables para el dibujo y animacion del sube y baja
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

//Arreglos para la animacion del aguila
#define MAX_FRAMES 21
int i_max_steps = 60;
int i_curr_steps = 0;

typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float giro_aguila;
	float alaIzq;
	float alaDer;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 17;			//introducir datos
bool play = false;
int playIndex = 0;

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	giro_aguila = KeyFrame[0].giro_aguila;
	alaIzq = KeyFrame[0].alaIzq;
	alaDer = KeyFrame[0].alaDer;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;
	giro_aguilaInc = (KeyFrame[playIndex + 1].giro_aguila - KeyFrame[playIndex].giro_aguila) / i_max_steps;
	alaIzqInc = (KeyFrame[playIndex + 1].alaIzq - KeyFrame[playIndex].alaIzq) / i_max_steps;
	alaDerInc = (KeyFrame[playIndex + 1].alaDer - KeyFrame[playIndex].alaDer) / i_max_steps;
}

//Arreglos para la aniamcion de la persona tirando la basura

#define MAX_FRAMES2 30
int i_max_steps2 = 60;
int i_curr_steps2 = 0;

typedef struct _frame2
{
	//Variables para GUARDAR Key Frames
	float posX1;		//Variable para PosicionX
	float posY1;		//Variable para PosicionY
	float posZ1;		//Variable para PosicionZ
	float rotpiernaIzq;
	float rotpiernaDer;
	float rotTorso;
	float rotBolsa;
	float rotCabeza;
	float rotbrazoIzq;
	float rotbrazoDer;

}FRAME2;

FRAME2 KeyFrame2[MAX_FRAMES2];
int FrameIndex2 = 22;			//introducir datos
bool play2 = false;
int playIndex2 = 0;

void resetElements2(void)
{
	posX1 = KeyFrame2[0].posX1;
	posY1 = KeyFrame2[0].posY1;
	posZ1 = KeyFrame2[0].posZ1;
	rotpiernaIzq = KeyFrame2[0].rotpiernaIzq;
	rotpiernaDer = KeyFrame2[0].rotpiernaDer;
	rotTorso = KeyFrame2[0].rotTorso;
	rotBolsa = KeyFrame2[0].rotBolsa;
	rotCabeza = KeyFrame2[0].rotCabeza;
	rotbrazoIzq = KeyFrame2[0].rotbrazoIzq;
	rotbrazoDer = KeyFrame2[0].rotbrazoDer;
}

void interpolation2(void)
{
	incposX1 = (KeyFrame2[playIndex2 + 1].posX1 - KeyFrame2[playIndex2].posX1) / i_max_steps2;
	incposY1 = (KeyFrame2[playIndex2 + 1].posY1 - KeyFrame2[playIndex2].posY1) / i_max_steps2;
	incposZ1 = (KeyFrame2[playIndex2 + 1].posZ1 - KeyFrame2[playIndex2].posZ1) / i_max_steps2;

	incrotpiernaIzq = (KeyFrame2[playIndex2 + 1].rotpiernaDer - KeyFrame2[playIndex2].rotpiernaDer) / i_max_steps2;
	incrotpiernaDer = (KeyFrame2[playIndex2 + 1].rotpiernaDer - KeyFrame2[playIndex2].rotpiernaDer) / i_max_steps2;
	
	incrotTorso= (KeyFrame2[playIndex2 + 1].rotTorso - KeyFrame2[playIndex2].rotTorso) / i_max_steps2;
	incrotBolsa = (KeyFrame2[playIndex + 1].rotBolsa - KeyFrame2[playIndex2].rotBolsa) / i_max_steps2;

	incrotCabeza = (KeyFrame2[playIndex2 + 1].rotCabeza - KeyFrame2[playIndex2].rotCabeza) / i_max_steps2;
	incrotbrazoIzq = (KeyFrame2[playIndex2 + 1].rotbrazoIzq - KeyFrame2[playIndex2].rotbrazoIzq) / i_max_steps2;
	incrotbrazoDer = (KeyFrame2[playIndex2 + 1].rotbrazoDer - KeyFrame2[playIndex2].rotbrazoDer) / i_max_steps2;
}

//Arreglos para la aniamcion del Tiburon

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
int FrameIndex3 = 20;			//introducir datos
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

// Arreglos para la animacion del Subeybaja

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

// Arreglos para la animacion de la pelota

#define MAX_FRAMES5 20
int i_max_steps5 = 60;
int i_curr_steps5 = 0;
typedef struct _frame5
{
	//Variables para GUARDAR Key Frames
	float posX_ball = 0.0f;
	float posY_ball = 0.0f;
	float posZ_ball = 0.0f;
	float rotball = 0.0f;

}FRAME5;

FRAME5 KeyFrame5[MAX_FRAMES5];
int FrameIndex5 = 20;			//introducir datos
bool play5 = false;
int playIndex5 = 0;

void resetElements5(void)
{
	posX_ball = KeyFrame5[0].posX_ball;
	posY_ball = KeyFrame5[0].posY_ball;
	posZ_ball = KeyFrame5[0].posZ_ball;
	rotball = KeyFrame5[0].rotball;
}

void interpolation5(void)
{
	posX_ball_INC = (KeyFrame5[playIndex5 + 1].posX_ball - KeyFrame5[playIndex5].posX_ball) / i_max_steps5;
	posY_ball_INC = (KeyFrame5[playIndex5 + 1].posY_ball - KeyFrame5[playIndex5].posY_ball) / i_max_steps5;
	posZ_ball_INC = (KeyFrame5[playIndex5 + 1].posZ_ball - KeyFrame5[playIndex5].posZ_ball) / i_max_steps5;
	rotball_INC = (KeyFrame5[playIndex5 + 1].rotball- KeyFrame5[playIndex5].rotball) / i_max_steps5;
	
}


void animate(void)
{
	//Animacion del aguila
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;
			giro_aguila += giro_aguilaInc;
			alaIzq += alaIzqInc;
			alaDer += alaDerInc;

			i_curr_steps++;
		}
	}
	else
	{
		Aguila->stopAllSounds();
	}

	//Animacion del coche

	if (animacion_coche)
	{
		if (carro_1)
		{
			movAuto_z -= 2.0f;
			giroLlantas += 2.0f;
			if (movAuto_z < -550.0f)
				carro_1 = false;
		}
		else if (carro_2)
		{
			rotcoche = 90.0f;
			movAuto_x -= 2.0f;
			giroLlantas += 2.0f;
			if (movAuto_x < -205.0f)
				carro_2 = false;
		}
		else if (carro_3)
		{
			movAuto_z += 2.0f;
			giroLlantas += 2.0f;
			rotcoche = 180.0f;
			if (movAuto_z > -120.0f)
				carro_3 = false;
		}
		else if (carro_4)
		{
			movAuto_z -= 0.5f;
			movAuto_x -= 1.1f;
			giroLlantas -= 0.5f;
			rotcoche = 220.0f;
				if (movAuto_z < -140.0f)
					carro_4 = false;
		}
		else if (carro_5)
		{
			movAuto_z -= 0.3f;
			giroLlantas -= 0.5f;
			rotcoche = 200.0f;
				if (movAuto_z < -155.0f)
					carro_5 = false;
		}
		else if (carro_6)
		{
			movAuto_z += 0.3f;
			giroLlantas += 0.5f;
			rotcoche = 180.0f;
				if (movAuto_z > -137.0f)
					carro_6 = false;
		}
	}

	//Animacion de la mujer caminando (extra)

	if (recorrido1)
	{		
		if (Mujer_1)
		{
			mov_Mujer_Z -= 1.0f;
			if (mov_Mujer_Z < -550.0f)
				Mujer_1 = false;
		}
		else if (Mujer_2)
		{
			mov_Mujer_X -= 1.0f;
			orienta = 90.0f;
			if (mov_Mujer_X < -400.0f)
				Mujer_2 = false;
		}
		else if (Mujer_3)
		{
			mov_Mujer_Z+= 1.0f;
			orienta2 = 90.0f;
			if (mov_Mujer_Z > -100.0f)
				Mujer_3 = false;
		}
		else if (Mujer_4)
		{
			mov_Mujer_X -= 1.0f;
			orienta3 = -90.0f;
			if (mov_Mujer_X < -560.0f)
				Mujer_4 = false;
		}
	}

	//Animacion del hombre tirando la basura
	if (play2)
	{
		if (i_curr_steps2 >= i_max_steps2) //end of animation between frames?
		{
			playIndex2++;
			if (playIndex2 > FrameIndex2 - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex2 = 0;
				play2 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps2 = 0;

				interpolation2();
			}
		}
		else
		{
			//Draw animation
			posX1 += incposX1;
			posY1 += incposY1;
			posZ1 += incposZ1;

			rotpiernaIzq += incrotbrazoIzq;
			rotpiernaDer += incrotpiernaDer;
			rotTorso += incrotTorso;
			rotBolsa += incrotBolsa;
			rotCabeza += incrotCabeza;

			rotbrazoIzq += incrotbrazoIzq;
			rotbrazoDer += incrotbrazoDer;

			i_curr_steps2++;
		}
	}
	else
	{
		Silbido->stopAllSounds();
	}

	//Animacion del Tiburon
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
	else
	{
		Tiburon->stopAllSounds();
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

	if (play5)
	{
		if (i_curr_steps5 >= i_max_steps5) //end of animation between frames?
		{
			playIndex5++;
			if (playIndex5 > FrameIndex5 - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex5 = 0;
				play5 = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps5 = 0; //Reset counter
								  //Interpolation
				interpolation5();
			}
		}
		else
		{
			//Draw animation
			posX_ball += posX_ball_INC;
			posY_ball += posY_ball_INC;
			posZ_ball += posZ_ball_INC;
			rotball += rotball_INC;
			i_curr_steps5++;
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main()
{
	//Fondo->play2D("Musica/Fondo.mp3", true);
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final: Zona Residencial Ceiba", NULL, NULL);
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

	//Carga del Skybox
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

	// Carga de Modelos utilizados

	Model piso("resources/objects/piso_new/piso_new.obj");
	Model Coche1("resources/objects/Coche1/Coche1.obj");
	Model Coche2("resources/objects/Coche2/Coche2.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model Banca("resources/objects/Banca/Banca.obj");
	Model Pierna_Izquierda("resources/objects/Humano/Pierna_Izquierda.obj");
	Model Pierna_Derecha("resources/objects/Humano/Pierna_Derecha.obj");
	Model Torso("resources/objects/Humano/Torso.obj");
	Model Brazo_Izquierdo("resources/objects/Humano/Brazo_Izquierdo.obj");
	Model Brazo_Derecho("resources/objects/Humano/Brazo_Derecho.obj");
	Model Cabeza("resources/objects/Humano/Cabeza.obj");
	Model Bolsa("resources/objects/Humano/Bolsa_Basura.obj");
	Model modelcasa("resources/objects/casa01/Modelo_Casa.obj");
	Model Lampara("resources/objects/Lampara/Lampara.obj");
	Model intento_arbol("resources/objects/Arbol/arbol.obj");
	Model Basurero("resources/objects/Basurero/Basurero.obj");
	Model cuerpo("resources/objects/Aguila/cuerpo3.obj");
	Model ala_derecha("resources/objects/Aguila/ala_derecha2l.obj");
	Model ala_izquierda("resources/objects/Aguila/ala_izquierda2.obj");
	Model ModelMedidor("resources/objects/Medidor/medidorluz-1.obj");
	Model SubeyBaja01("resources/objects/subeybaja/subeybaja_01.obj");
	Model SubeyBaja02("resources/objects/subeybaja/subeybaja_02.obj");
	Model SubeyBaja03("resources/objects/subeybaja/subeybaja_centro.obj");
	Model shark01("resources/objects/tiburon/cabeza.obj");
	Model shark02("resources/objects/tiburon/cola.obj");
	Model pool("resources/objects/pool/pool01.obj");
	Model perro("resources/objects/perro/Dog.obj");
	Model ball("resources/objects/pelota/pelota.obj");

	ModelAnim personajeCaminando("resources/objects/Caminando/Caminando.dae");
	personajeCaminando.initShaders(animShader.ID);

	ModelAnim Mujer_Caminando("resources/objects/Mujer_Caminando/Mujer_Caminando.dae");
	Mujer_Caminando.initShaders(animShader.ID);

	ModelAnim Inge("resources/objects/Inge/Inge.dae");
	Inge.initShaders(animShader.ID);

	/*
		//Keyframes para la animacion del aguila

		KeyFrame[0].posX = 0;
		KeyFrame[0].posY = 0;
		KeyFrame[0].posZ = 0;
		KeyFrame[0].giro_aguila = 0;
		KeyFrame[0].alaIzq = 0;
		KeyFrame[0].alaDer = 0;

		KeyFrame[1].posX = 0.0f;
		KeyFrame[1].posY = 40.0f;
		KeyFrame[1].posZ = 60.0f;
		KeyFrame[1].giro_aguila = 0.0f;
		KeyFrame[1].alaIzq = -30.0f;
		KeyFrame[1].alaDer = 30.0f;

		KeyFrame[2].posX = 0;
		KeyFrame[2].posY = 80.0f;
		KeyFrame[2].posZ = 120.0f;
		KeyFrame[2].giro_aguila = 0.0f;
		KeyFrame[2].alaIzq = 30.0f;
		KeyFrame[2].alaDer = -30.0f;

		KeyFrame[3].posX = 0;
		KeyFrame[3].posY = 120.0f;
		KeyFrame[3].posZ = 180.0f;
		KeyFrame[3].giro_aguila = 0.0f;
		KeyFrame[3].alaIzq = -30.0f;
		KeyFrame[3].alaDer = 30.0f;

		KeyFrame[4].posX = 0;
		KeyFrame[4].posY = 120.0f;
		KeyFrame[4].posZ = 240.0f;
		KeyFrame[4].giro_aguila = 0.0f;
		KeyFrame[4].alaIzq = 30.0f;
		KeyFrame[4].alaDer = -30.0f;

		KeyFrame[5].posX = 0;
		KeyFrame[5].posY = 120.0f;
		KeyFrame[5].posZ = 300.0f;
		KeyFrame[5].giro_aguila = 0.0f;
		KeyFrame[5].alaIzq = -30.0f;
		KeyFrame[5].alaDer = 30.0f;

		KeyFrame[6].posX = 0;
		KeyFrame[6].posY = 120.0f;
		KeyFrame[6].posZ = 360.0f;
		KeyFrame[6].giro_aguila = 0.0f;
		KeyFrame[6].alaIzq = 30.0f;
		KeyFrame[6].alaDer = -30.0f;

		KeyFrame[7].posX = 0;
		KeyFrame[7].posY = 120.0f;
		KeyFrame[7].posZ = 420.0f;
		KeyFrame[7].giro_aguila = 0.0f;
		KeyFrame[7].alaIzq = -30.0f;
		KeyFrame[7].alaDer = 30.0f;

		KeyFrame[8].posX = 55.0f;
		KeyFrame[8].posY = 120.0f;
		KeyFrame[8].posZ = 450.0f;
		KeyFrame[8].giro_aguila = 90.0f;
		KeyFrame[8].alaIzq = 30.0f;
		KeyFrame[8].alaDer = -30.0f;

		KeyFrame[9].posX = 110.0f;
		KeyFrame[9].posY = 120.0f;
		KeyFrame[9].posZ = 420.0f;
		KeyFrame[9].giro_aguila = 180.0f;
		KeyFrame[9].alaIzq = -30.0f;
		KeyFrame[9].alaDer = 30.0f;

		KeyFrame[10].posX = 110.0f;
		KeyFrame[10].posY = 120.0f;
		KeyFrame[10].posZ = 360.0f;
		KeyFrame[10].giro_aguila = 180.0f;
		KeyFrame[10].alaIzq = 30.0f;
		KeyFrame[10].alaDer = -30.0f;

		KeyFrame[11].posX = 110.0f;
		KeyFrame[11].posY = 120.0f;
		KeyFrame[11].posZ = 300.0f;
		KeyFrame[11].giro_aguila = 180.0f;
		KeyFrame[11].alaIzq = -30.0f;
		KeyFrame[11].alaDer = 30.0f;

		KeyFrame[12].posX = 110.0f;
		KeyFrame[12].posY = 120.0f;
		KeyFrame[12].posZ = 240.0f;
		KeyFrame[12].giro_aguila = 180.0f;
		KeyFrame[12].alaIzq = 30.0f;
		KeyFrame[12].alaDer = -30.0f;

		KeyFrame[13].posX = 110.0f;
		KeyFrame[13].posY = 110.0f;
		KeyFrame[13].posZ = 180.0f;
		KeyFrame[13].giro_aguila = 180.0f;
		KeyFrame[13].alaIzq = -30.0f;
		KeyFrame[13].alaDer = 30.0f;

		KeyFrame[14].posX = 110.0f;
		KeyFrame[14].posY = 100.0f;
		KeyFrame[14].posZ = 120.0f;
		KeyFrame[14].giro_aguila = 180.0f;
		KeyFrame[14].alaIzq = 30.0f;
		KeyFrame[14].alaDer = -30.0f;

		KeyFrame[15].posX = 110.0f;
		KeyFrame[15].posY = 90.0f;
		KeyFrame[15].posZ = 60.0f;
		KeyFrame[15].giro_aguila = 180.0f;
		KeyFrame[15].alaIzq = -30.0f;
		KeyFrame[15].alaDer = 30.0f;

		KeyFrame[16].posX = 110.0f;
		KeyFrame[16].posY = 70.0f;
		KeyFrame[16].posZ = 5.0f;
		KeyFrame[16].giro_aguila = 180.0f;
		KeyFrame[16].alaIzq = 0.0f;
		KeyFrame[16].alaDer = 0.0f;

	//Keyframes para la animacion de la persona caminando

		KeyFrame2[0].posX1 = 0;
		KeyFrame2[0].posY1 = 0;
		KeyFrame2[0].posZ1 = 0;
		KeyFrame2[0].rotpiernaIzq = 0;
		KeyFrame2[0].rotpiernaDer = 0;
		KeyFrame2[0].rotTorso = 0;
		KeyFrame2[0].rotBolsa = 0;
		KeyFrame2[0].rotCabeza = 0;
		KeyFrame2[0].rotbrazoIzq = 0;
		KeyFrame2[0].rotbrazoDer = 0;

		KeyFrame2[1].posX1 = 50.0f;
		KeyFrame2[1].posY1 = 0;
		KeyFrame2[1].posZ1 = 0;
		KeyFrame2[1].rotpiernaIzq = 30.0f;
		KeyFrame2[1].rotpiernaDer = -30.0f;
		KeyFrame2[1].rotTorso = 0;
		KeyFrame2[1].rotBolsa = 10.0f;
		KeyFrame2[1].rotCabeza = 0.0f;
		KeyFrame2[1].rotbrazoIzq = -30.0f;
		KeyFrame2[1].rotbrazoDer = 30.0f;

		KeyFrame2[2].posX1 = 100.0f;
		KeyFrame2[2].posY1 = 0;
		KeyFrame2[2].posZ1 = 0;
		KeyFrame2[2].rotpiernaIzq = -30.0f;
		KeyFrame2[2].rotpiernaDer = 30.0f;
		KeyFrame2[2].rotTorso = 0;
		KeyFrame2[2].rotBolsa = 10.0f;
		KeyFrame2[2].rotCabeza = 0;
		KeyFrame2[2].rotbrazoIzq = 30.0f;
		KeyFrame2[2].rotbrazoDer = -30.0f;

		KeyFrame2[3].posX1 = 150.0f;
		KeyFrame2[3].posY1 = 0;
		KeyFrame2[3].posZ1 = 0;
		KeyFrame2[3].rotpiernaIzq = 30.0f;
		KeyFrame2[3].rotpiernaDer = -30.0f;
		KeyFrame2[3].rotTorso = 0;
		KeyFrame2[3].rotBolsa = 10.0f;
		KeyFrame2[3].rotCabeza = 0;
		KeyFrame2[3].rotbrazoIzq = -30.0f;
		KeyFrame2[3].rotbrazoDer = 30.0f;

		KeyFrame2[4].posX1 = 200.0f;
		KeyFrame2[4].posY1 = 0;
		KeyFrame2[4].posZ1 = 0;
		KeyFrame2[4].rotpiernaIzq = -30.0f;
		KeyFrame2[4].rotpiernaDer = 30.0f;
		KeyFrame2[4].rotTorso = 0;
		KeyFrame2[4].rotBolsa = 10.0f;
		KeyFrame2[4].rotCabeza = 0;
		KeyFrame2[4].rotbrazoIzq = 30.0f;
		KeyFrame2[4].rotbrazoDer = -30.0f;

		KeyFrame2[5].posX1 = 255.0f;
		KeyFrame2[5].posY1 = 0;
		KeyFrame2[5].posZ1 = 0.0f;
		KeyFrame2[5].rotpiernaIzq = 0.0f;
		KeyFrame2[5].rotpiernaDer = 0.0f;
		KeyFrame2[5].rotTorso = -90.0f;
		KeyFrame2[5].rotBolsa = 0.0f;
		KeyFrame2[5].rotCabeza = 0;
		KeyFrame2[5].rotbrazoIzq = 0.0f;
		KeyFrame2[5].rotbrazoDer = 0.0f;

		KeyFrame2[6].posX1 = 255.0f;
		KeyFrame2[6].posY1 = 0;
		KeyFrame2[6].posZ1 = 50.0f;
		KeyFrame2[6].rotpiernaIzq = 30.0f;
		KeyFrame2[6].rotpiernaDer = -30.0f;
		KeyFrame2[6].rotTorso = -90.0f;
		KeyFrame2[6].rotBolsa = 10.0f;
		KeyFrame2[6].rotCabeza = 0;
		KeyFrame2[6].rotbrazoIzq = -30.0f;
		KeyFrame2[6].rotbrazoDer = 30.0f;

		KeyFrame2[7].posX1 = 255.0f;
		KeyFrame2[7].posY1 = 0;
		KeyFrame2[7].posZ1 = 100;
		KeyFrame2[7].rotpiernaIzq = -30.0f;
		KeyFrame2[7].rotpiernaDer = 30.0f;
		KeyFrame2[7].rotTorso = -90.0f;
		KeyFrame2[7].rotBolsa = 10.0f;
		KeyFrame2[7].rotCabeza = 0;
		KeyFrame2[7].rotbrazoIzq = 30.0f;
		KeyFrame2[7].rotbrazoDer = -30.0f;

		KeyFrame2[8].posX1 = 255.0f;
		KeyFrame2[8].posY1 = 0;
		KeyFrame2[8].posZ1 =150.0f;
		KeyFrame2[8].rotpiernaIzq = 30.0f;
		KeyFrame2[8].rotpiernaDer = -30.0f;
		KeyFrame2[8].rotTorso = -90.0f;
		KeyFrame2[8].rotBolsa = 10.0f;
		KeyFrame2[8].rotCabeza = 0;
		KeyFrame2[8].rotbrazoIzq = -30.0f;
		KeyFrame2[8].rotbrazoDer = 30.0f;

		KeyFrame2[9].posX1 = 255.0f;
		KeyFrame2[9].posY1 = 0;
		KeyFrame2[9].posZ1 = 200;
		KeyFrame2[9].rotpiernaIzq = -30.0f;
		KeyFrame2[9].rotpiernaDer = 30.0f;
		KeyFrame2[9].rotTorso = -90.0f;
		KeyFrame2[9].rotBolsa = 10.0f;
		KeyFrame2[9].rotCabeza = 0;
		KeyFrame2[9].rotbrazoIzq = 30.0f;
		KeyFrame2[9].rotbrazoDer = -30.0f;

		KeyFrame2[10].posX1 = 255.0f;
		KeyFrame2[10].posY1 = 0;
		KeyFrame2[10].posZ1 = 250.0f;
		KeyFrame2[10].rotpiernaIzq = 30.0f;
		KeyFrame2[10].rotpiernaDer = -30.0f;
		KeyFrame2[10].rotTorso = -90.0f;
		KeyFrame2[10].rotBolsa = 10.0f;
		KeyFrame2[10].rotCabeza = -90.0f;
		KeyFrame2[10].rotbrazoIzq = -30.0f;
		KeyFrame2[10].rotbrazoDer = 30.0f;

		KeyFrame2[11].posX1 = 255.0f;
		KeyFrame2[11].posY1 = 0;
		KeyFrame2[11].posZ1 = 300;
		KeyFrame2[11].rotpiernaIzq = -30.0f;
		KeyFrame2[11].rotpiernaDer = 30.0f;
		KeyFrame2[11].rotTorso = -90.0f;
		KeyFrame2[11].rotBolsa = 10.0f;
		KeyFrame2[11].rotCabeza = 0;
		KeyFrame2[11].rotbrazoIzq = 30.0f;
		KeyFrame2[11].rotbrazoDer = -30.0f;

		KeyFrame2[12].posX1 = 255.0f;
		KeyFrame2[12].posY1 = 0;
		KeyFrame2[12].posZ1 = 350.0f;
		KeyFrame2[12].rotpiernaIzq = 30.0f;
		KeyFrame2[12].rotpiernaDer = -30.0f;
		KeyFrame2[12].rotTorso = -90.0f;
		KeyFrame2[12].rotBolsa = 10.0f;
		KeyFrame2[12].rotCabeza = 0;
		KeyFrame2[12].rotbrazoIzq = -30.0f;
		KeyFrame2[12].rotbrazoDer = 30.0f;

		KeyFrame2[13].posX1 = 255.0f;
		KeyFrame2[13].posY1 = 0;
		KeyFrame2[13].posZ1 = 400;
		KeyFrame2[13].rotpiernaIzq = -30.0f;
		KeyFrame2[13].rotpiernaDer = 30.0f;
		KeyFrame2[13].rotTorso = -90.0f;
		KeyFrame2[13].rotBolsa = 10.0f;
		KeyFrame2[13].rotCabeza = 0;
		KeyFrame2[13].rotbrazoIzq = 30.0f;
		KeyFrame2[13].rotbrazoDer = -30.0f;

		KeyFrame2[14].posX1 = 255.0f;
		KeyFrame2[14].posY1 = 0;
		KeyFrame2[14].posZ1 = 450.0f;
		KeyFrame2[14].rotpiernaIzq = 30.0f;
		KeyFrame2[14].rotpiernaDer = -30.0f;
		KeyFrame2[14].rotTorso = -90.0f;
		KeyFrame2[14].rotBolsa = 10.0f;
		KeyFrame2[14].rotCabeza = 0;
		KeyFrame2[14].rotbrazoIzq = -30.0f;
		KeyFrame2[14].rotbrazoDer = 30.0f;

		KeyFrame2[15].posX1 = 255.0f;
		KeyFrame2[15].posY1 = 0;
		KeyFrame2[15].posZ1 = 500;
		KeyFrame2[15].rotpiernaIzq = -30.0f;
		KeyFrame2[15].rotpiernaDer = 30.0f;
		KeyFrame2[15].rotTorso = -90.0f;
		KeyFrame2[15].rotBolsa = 10.0f;
		KeyFrame2[15].rotCabeza = 0;
		KeyFrame2[15].rotbrazoIzq = 30.0f;
		KeyFrame2[15].rotbrazoDer = -30.0f;

		KeyFrame2[16].posX1 = 255.0f;
		KeyFrame2[16].posY1 = 0;
		KeyFrame2[16].posZ1 = 550;
		KeyFrame2[16].rotpiernaIzq = 0.0f;
		KeyFrame2[16].rotpiernaDer = 0.0f;
		KeyFrame2[16].rotTorso = -180.0f;
		KeyFrame2[16].rotBolsa = 10.0f;
		KeyFrame2[16].rotCabeza = 0;
		KeyFrame2[16].rotbrazoIzq = 0.0f;
		KeyFrame2[16].rotbrazoDer = 0.0f;

		KeyFrame2[17].posX1 = 200.0f;
		KeyFrame2[17].posY1 = 0;
		KeyFrame2[17].posZ1 = 550.0f;
		KeyFrame2[17].rotpiernaIzq = 30.0f;
		KeyFrame2[17].rotpiernaDer = -30.0f;
		KeyFrame2[17].rotTorso = -180.0f;
		KeyFrame2[17].rotBolsa = 10.0f;
		KeyFrame2[17].rotCabeza = 0.0f;
		KeyFrame2[17].rotbrazoIzq = -30.0f;
		KeyFrame2[17].rotbrazoDer = 30.0f;

		KeyFrame2[18].posX1 = 150.0f;
		KeyFrame2[18].posY1 = 0;
		KeyFrame2[18].posZ1 = 550.0f;
		KeyFrame2[18].rotpiernaIzq = -30.0f;
		KeyFrame2[18].rotpiernaDer = 30.0f;
		KeyFrame2[18].rotTorso = -180.0f;
		KeyFrame2[18].rotBolsa = 10.0f;
		KeyFrame2[18].rotCabeza = 0;
		KeyFrame2[18].rotbrazoIzq = 30.0f;
		KeyFrame2[18].rotbrazoDer = -30.0f;

		KeyFrame2[19].posX1 = 100.0f;
		KeyFrame2[19].posY1 = 0;
		KeyFrame2[19].posZ1 = 550.0f;
		KeyFrame2[19].rotpiernaIzq = 30.0f;
		KeyFrame2[19].rotpiernaDer = -30.0f;
		KeyFrame2[19].rotTorso = -180.0f;
		KeyFrame2[19].rotBolsa = 10.0f;
		KeyFrame2[19].rotCabeza = 0.0f;
		KeyFrame2[19].rotbrazoIzq = -30.0f;
		KeyFrame2[19].rotbrazoDer = 30.0f;

		KeyFrame2[20].posX1 = 50.0f;
		KeyFrame2[20].posY1 = 0;
		KeyFrame2[20].posZ1 = 550;
		KeyFrame2[20].rotpiernaIzq = -30.0f;
		KeyFrame2[20].rotpiernaDer = 30.0f;
		KeyFrame2[20].rotTorso = -180.0f;
		KeyFrame2[20].rotBolsa = 10.0f;
		KeyFrame2[20].rotCabeza = 0;
		KeyFrame2[20].rotbrazoIzq = 30.0f;
		KeyFrame2[20].rotbrazoDer = -30.0f;

		KeyFrame2[21].posX1 = 0.0f;
		KeyFrame2[21].posY1 = 0;
		KeyFrame2[21].posZ1 = 550.0f;
		KeyFrame2[21].rotpiernaIzq = 30.0f;
		KeyFrame2[21].rotpiernaDer = -30.0f;
		KeyFrame2[21].rotTorso = -180.0f;
		KeyFrame2[21].rotBolsa = 10.0f;
		KeyFrame2[21].rotCabeza = 0.0f;
		KeyFrame2[21].rotbrazoIzq = -30.0f;
		KeyFrame2[21].rotbrazoDer = 30.0f;

	*/
	//Keyframes para la animacion del tiburon

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

	KeyFrame3[2].mov_tibu_x = 50;
	KeyFrame3[2].mov_tibu_y = 0;
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

	KeyFrame3[9].mov_tibu_x = 450;
	KeyFrame3[9].mov_tibu_y = -20;
	KeyFrame3[9].mov_tibu_z = 0;
	KeyFrame3[9].cabezatibu = 10;
	KeyFrame3[9].colatibu = -15;

	KeyFrame3[10].mov_tibu_x = 450;
	KeyFrame3[10].mov_tibu_y = -20;
	KeyFrame3[10].mov_tibu_z = 100;
	KeyFrame3[10].cabezatibu = -90;
	KeyFrame3[10].colatibu = 15;

	KeyFrame3[11].mov_tibu_x = 400;
	KeyFrame3[11].mov_tibu_y = 0;
	KeyFrame3[11].mov_tibu_z = 100;
	KeyFrame3[11].cabezatibu = -180;
	KeyFrame3[11].colatibu = -15;

	KeyFrame3[12].mov_tibu_x = 350;
	KeyFrame3[12].mov_tibu_y = 0;
	KeyFrame3[12].mov_tibu_z = 100;
	KeyFrame3[12].cabezatibu = -180;
	KeyFrame3[12].colatibu = 15;

	KeyFrame3[13].mov_tibu_x = 400;
	KeyFrame3[13].mov_tibu_y = 0;
	KeyFrame3[13].mov_tibu_z = 100;
	KeyFrame3[13].cabezatibu = -180;
	KeyFrame3[13].colatibu = -15;

	KeyFrame3[14].mov_tibu_x = 250;
	KeyFrame3[14].mov_tibu_y = 0;
	KeyFrame3[14].mov_tibu_z = 100;
	KeyFrame3[14].cabezatibu = -180;
	KeyFrame3[14].colatibu = 15;

	KeyFrame3[15].mov_tibu_x = 200;
	KeyFrame3[15].mov_tibu_y = 0;
	KeyFrame3[15].mov_tibu_z = 100;
	KeyFrame3[15].cabezatibu = -180;
	KeyFrame3[15].colatibu = -15;

	KeyFrame3[16].mov_tibu_x = 150;
	KeyFrame3[16].mov_tibu_y = 0;
	KeyFrame3[16].mov_tibu_z = 100;
	KeyFrame3[16].cabezatibu = -180;
	KeyFrame3[16].colatibu = 15;

	KeyFrame3[17].mov_tibu_x = 100;
	KeyFrame3[17].mov_tibu_y = 0;
	KeyFrame3[17].mov_tibu_z = 100;
	KeyFrame3[17].cabezatibu = -180;
	KeyFrame3[17].colatibu = -15;

	KeyFrame3[18].mov_tibu_x = 50;
	KeyFrame3[18].mov_tibu_y = 0;
	KeyFrame3[18].mov_tibu_z = 100;
	KeyFrame3[18].cabezatibu = -180;
	KeyFrame3[18].colatibu = 15;

	KeyFrame3[19].mov_tibu_x = 5;
	KeyFrame3[19].mov_tibu_y = 0;
	KeyFrame3[19].mov_tibu_z = 100;
	KeyFrame3[19].cabezatibu = -180;
	KeyFrame3[19].colatibu = -15;

//KeyFrames para la animacion del Sube y baja

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

//Keyframes para la animacion de la pelota

	KeyFrame5[0].posX_ball =0;
	KeyFrame5[0].posY_ball =0;
	KeyFrame5[0].posZ_ball =0;
	KeyFrame5[0].rotball =0;

	KeyFrame5[1].posX_ball =160;
	KeyFrame5[1].posY_ball =180;
	KeyFrame5[1].posZ_ball = 0;
	KeyFrame5[1].rotball = 360;

	KeyFrame5[2].posX_ball =360;
	KeyFrame5[2].posY_ball =270;
	KeyFrame5[2].posZ_ball = 0;
	KeyFrame5[2].rotball = 720;

	KeyFrame5[3].posX_ball = 560;
	KeyFrame5[3].posY_ball =360;
	KeyFrame5[3].posZ_ball = 0;
	KeyFrame5[3].rotball =1080;

	KeyFrame5[4].posX_ball = 760;
	KeyFrame5[4].posY_ball = 400;
	KeyFrame5[4].posZ_ball = 0;
	KeyFrame5[4].rotball = 1440;

	KeyFrame5[5].posX_ball = 960;
	KeyFrame5[5].posY_ball = 360;
	KeyFrame5[5].posZ_ball = 0;
	KeyFrame5[5].rotball = 1800;

	KeyFrame5[6].posX_ball = 1160;
	KeyFrame5[6].posY_ball = 270;
	KeyFrame5[6].posZ_ball = 0;
	KeyFrame5[6].rotball = 2160;

	KeyFrame5[7].posX_ball = 1360;
	KeyFrame5[7].posY_ball = 180;
	KeyFrame5[7].posZ_ball = 0;
	KeyFrame5[7].rotball = 2520;

	KeyFrame5[8].posX_ball = 1560;
	KeyFrame5[8].posY_ball = 0;
	KeyFrame5[8].posZ_ball = 0;
	KeyFrame5[8].rotball = 2880;

	KeyFrame5[9].posX_ball = 1760;
	KeyFrame5[9].posY_ball = -125;
	KeyFrame5[9].posZ_ball = 0;
	KeyFrame5[9].rotball = 3240;

	KeyFrame5[10].posX_ball = 1960;
	KeyFrame5[10].posY_ball = 30;
	KeyFrame5[10].posZ_ball = 0;
	KeyFrame5[10].rotball = 3600;

	KeyFrame5[11].posX_ball = 2160;
	KeyFrame5[11].posY_ball = -125;
	KeyFrame5[11].posZ_ball = 0;
	KeyFrame5[11].rotball = 3960;

	KeyFrame5[12].posX_ball = 2360;
	KeyFrame5[12].posY_ball = 10;
	KeyFrame5[12].posZ_ball = 0;
	KeyFrame5[12].rotball = 4320;

	KeyFrame5[13].posX_ball = 2560;
	KeyFrame5[13].posY_ball = -125;
	KeyFrame5[13].posZ_ball = 0;
	KeyFrame5[13].rotball = 4680;

	KeyFrame5[14].posX_ball = 2640;
	KeyFrame5[14].posY_ball = 5;
	KeyFrame5[14].posZ_ball = 0;
	KeyFrame5[14].rotball = 4320;

	KeyFrame5[15].posX_ball = 2500;
	KeyFrame5[15].posY_ball = -125;
	KeyFrame5[15].posZ_ball = 0;
	KeyFrame5[15].rotball = 3960;

	KeyFrame5[16].posX_ball = 2400;
	KeyFrame5[16].posY_ball = -80;
	KeyFrame5[16].posZ_ball = 0;
	KeyFrame5[16].rotball = 3600;

	KeyFrame5[17].posX_ball = 2300;
	KeyFrame5[17].posY_ball = -125;
	KeyFrame5[17].posZ_ball = 0;
	KeyFrame5[17].rotball = 3240;

	KeyFrame5[18].posX_ball = 2200;
	KeyFrame5[18].posY_ball = -125;
	KeyFrame5[18].posZ_ball = 0;
	KeyFrame5[18].rotball = 2880;

	KeyFrame5[19].posX_ball = 2000;
	KeyFrame5[19].posY_ball = -125;
	KeyFrame5[19].posZ_ball = 0;
	KeyFrame5[19].rotball = 2520;
	
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
		glm::vec3 diffuseColor = lightColor * glm::vec3(1.0f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(1.0f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Configuracion para los personajes con animacion
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
		// Mujer Caminando
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(350.0f + mov_Mujer_X, 0.0f, 400.0f + mov_Mujer_Z));
		model = glm::scale(model, glm::vec3(0.2f));	
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta2), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orienta3), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		Mujer_Caminando.Draw(animShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Ingeniero Saludando
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(540.0f, 125.0f, 13.0f)); 
		model = glm::scale(model, glm::vec3(0.2f));	
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		Inge.Draw(animShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Tiburon
		// -------------------------------------------------------------------------------------------------------------------------

		//Cabeza
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 17.0f, -380.0f));
		model = glm::scale(model, glm::vec3(0.5f)); 
		model = glm::translate(model, glm::vec3(mov_tibu_x, mov_tibu_y, mov_tibu_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(cabezatibu), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		shark01.Draw(staticShader);

		//Cola
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(colatibu), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		shark02.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Alberca
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(420.0f, 0.0f, -365.0f));
		model = glm::scale(model, glm::vec3(1.8f, 0.5f, 1.3f));
		staticShader.setMat4("model", model);
		pool.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Arbol
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(185.0f, 35.0f, -310.0f));
		staticShader.setMat4("model", model);
		intento_arbol.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
	    // Casas
		// -------------------------------------------------------------------------------------------------------------------------

		//casa1 (primera empezando por la alberca)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(595.0f, 0.0f, -51.3f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.93f, 2.5f, 1.3f));
		staticShader.setMat4("model", model);
		modelcasa.Draw(staticShader);

		//casa2 (segunda empezando por la alberca)
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
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-271.0f, 0.0f, -55.0f));
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

		// -------------------------------------------------------------------------------------------------------------------------
		//Medidores
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		//MedidorLuz casa01
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(420.0f, 0.0f, -51.3f));
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
		// -------------------------------------------------------------------------------------------------------------------------
		//Sube y Baja 1
		// -------------------------------------------------------------------------------------------------------------------------

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

		// -------------------------------------------------------------------------------------------------------------------------
		//Sube y Baja 2
		// -------------------------------------------------------------------------------------------------------------------------

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
		//Banca del area de juegos
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-270.0f, 0.0f, -350.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.3f));
		staticShader.setMat4("model", model);
		Banca.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		//Coche1
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 185.0f));
		staticShader.setMat4("model", model);
		Coche1.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		//Coche2
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.0f, 0.0f, 425.0f));
		staticShader.setMat4("model", model);
		Coche2.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Persona tirando la basura
		// -------------------------------------------------------------------------------------------------------------------------

		//Torso
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-210.0f, -1.0f, 128.0f));
		model = glm::scale(model, glm::vec3(0.6f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotTorso), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Torso.Draw(staticShader);

		//Pierna Izquierda
		model = glm::translate(tmp, glm::vec3(-13.5f, 27.62f, 14.6f));
		model = glm::rotate(model, glm::radians(-rotpiernaIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pierna_Izquierda.Draw(staticShader);

		//Pierna Derecha
		model = glm::translate(tmp, glm::vec3(-20.0f, 27.62f, 15.0f));
		model = glm::rotate(model, glm::radians(rotpiernaDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Pierna_Derecha.Draw(staticShader);

		//Brazo Izquierdo
		model = glm::translate(tmp, glm::vec3(-10.6f, 51.3f, 12.6f));
		model = glm::rotate(model, glm::radians(rotbrazoIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Brazo_Izquierdo.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(-17.1f, 58.9f, 14.9f));
		model = glm::rotate(model, glm::radians(rotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Cabeza.Draw(staticShader);

		//Brazo Derecho
		model = glm::translate(tmp, glm::vec3(-22.5f, 52.8f, 13.55f));
		tmp = model = glm::rotate(model, glm::radians(rotbrazoDer), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Brazo_Derecho.Draw(staticShader);

		//Bolsa de Basura
		model = glm::translate(tmp, glm::vec3(-10.0f, -23.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.4));
		model = glm::rotate(model, glm::radians(180.0f),glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotBolsa), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		Bolsa.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------		
		// Lamparas
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		//Primera iniciando desde la piscina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, 3.0f, -311.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);
		
		//Segunda iniciando desde la piscina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, 3.0f, -61.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);

		//Tercera iniciando desde la piscina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(37.0f, 3.0f, 155.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);

		//Primera iniciando desde el area de juegos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-295.0f, 3.0f, -305.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);

		//Segunda iniciando desde el area de juegos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-294.0f, 3.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);

		//Tercera iniciando desde el area de juegos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-293.0f, 3.0f, 161.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		staticShader.setMat4("model", model);
		Lampara.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Basureros
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		//Basurero lado Derecho
		model = glm::translate(glm::mat4(1.0f), glm::vec3(586.5f, 1.0f, 454.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.35f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Basurero.Draw(staticShader);

		//Basurero lado Izquierdo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-261.5f, 1.0f, 475.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.35f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Basurero.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Carro que tendra animacion
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		model = glm::translate(glm::mat4(1.0f), glm::vec3(290.0f + movAuto_x, -1.0f + movAuto_y, 450.0f+ movAuto_z));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotcoche), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(80.0f, 35.0f, 130.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-80.0f, 35.0f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-80.0f, 35.0f, -150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(85.0f, 35.0f, -150.0f));
		model = glm::rotate(model, glm::radians(giroLlantas), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Aguila
		// -------------------------------------------------------------------------------------------------------------------------
		/*
		//cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, 12.0f, -330.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giro_aguila), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		cuerpo.Draw(staticShader);

		//ala derecha
		model = glm::translate(tmp, glm::vec3(-2.9f, 1.1f, -2.1f));
		model = glm::rotate(model, glm::radians(alaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		ala_derecha.Draw(staticShader);

		//ala izquierda
		model = glm::translate(tmp, glm::vec3(2.7f, 1.0f, -2.4f));
		model = glm::rotate(model, glm::radians(alaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		ala_izquierda.Draw(staticShader);
		*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Perro
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, -1.0f, -170.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		perro.Draw(staticShader);

		
		//-------------------------------------------------------------------------------------------------------------------------
		//Pelota 

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-143.0f, 23.0f, -340.0f));
		model = glm::scale(model, glm::vec3(0.15f));
		model = glm::translate(model, glm::vec3(posX_ball, posY_ball, posZ_ball));
		tmp = model = glm::rotate(model, glm::radians(rotball), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		ball.Draw(staticShader);

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

// Acciones con el Teclado
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	//Acercar la camara
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	//Alejar la camara
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	//Mover a la izquierda la camara
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	//Mover a la derecha la camara
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);


	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posX_ball++;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		posX_ball--;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posY_ball--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posY_ball++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		posZ_ball--;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		posZ_ball++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotball--;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		rotball++;


	//Reinicio de la Musica de Fondo
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{ 
		//Fondo->stopAllSounds();
		//Fondo->play2D("Musica/Fondo.mp3", true);
		Silbido->stopAllSounds();
		Aguila->stopAllSounds();
		Tiburon->stopAllSounds();
	}

	//Inicio de la Animacion del Coche
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		animacion_coche ^= true;
	}

	//Inicio de la animacion por KeyFrames del Tiburon
	if (key == GLFW_KEY_T && action == GLFW_PRESS) 
	{

		Tiburon->stopAllSounds();
		//Tiburon->play2D("Musica/Tiburon.mp3", true);
		Fondo->stopAllSounds();
		Silbido->stopAllSounds();
		Aguila->stopAllSounds();

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

	//Inicio de la animacion por KeyFrames del pelota
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{

		//Tiburon->stopAllSounds();
		//Tiburon->play2D("Musica/Tiburon.mp3", true);
		Fondo->stopAllSounds();
		Silbido->stopAllSounds();
		Aguila->stopAllSounds();

		if (play5 == false && (FrameIndex5 > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements5();

			interpolation5();

			play5 = true;
			playIndex5 = 0;
			i_curr_steps5 = 0;
		}
		else
		{
			play5 = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//Inicio de la animacion por KeyFrames del Sube y Baja
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
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

	//Inicio de la animacion por KeyFrames del Aguila
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		Aguila->stopAllSounds();
		Aguila->play2D("Musica/Aguila.mp3", true);
		Fondo->stopAllSounds();
		Silbido->stopAllSounds();
		Tiburon->stopAllSounds();

		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//Inicio de la Animacion del Hombre tirando la basura
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		Silbido->stopAllSounds();
		Silbido->play2D("Musica/Silbando.mp3", true);
		Fondo->stopAllSounds();
		Aguila->stopAllSounds();
		Tiburon->stopAllSounds();

		if (play2 == false && (FrameIndex2 > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements2();

			interpolation2();

			play2 = true;
			playIndex2 = 0;
			i_curr_steps2 = 0;
		}
		else
		{
			play2 = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
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
