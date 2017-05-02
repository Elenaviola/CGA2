#include "GameWindow.h"

GameWindow* GameWindow::windowHandlerInstance;

GameWindow::GameWindow(const GLint sizex, const GLint sizey, bool fullscreen, bool vsync, const GLint cvmaj, const GLint cvmin, const std::string& title, const GLint msaasamples, const GLboolean uselatestglver, const GLdouble updatefrequency)
{
	this->windowWidth = sizex;
	this->windowHeight = sizey;
	this->m_uselatestglcontext = uselatestglver;
	this->m_cvmaj = cvmaj;
	this->m_cvmin = cvmin;
	this->title = title;
	this->m_samples = msaasamples;
	this->fullscreen = fullscreen;
	this->vsync = vsync;
	this->m_updatefrequency = static_cast<GLfloat>(updatefrequency);
	if (!this->initialize())
		throw "Initiatization error";
}


GameWindow::~GameWindow()
{
	glfwTerminate();
}

GLboolean GameWindow::initialize()
{
	//glfwSetErrorCallback({})

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, m_samples);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, this->m_cvmaj);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, this->m_cvmin);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	this->m_window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), (fullscreen ? glfwGetPrimaryMonitor() : NULL), NULL);
	if (this->m_window == NULL){
		fprintf(stderr, "Failed to open GLFW window. Please check if your system supports the OpenGL version you passed.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(this->m_window);

	glewExperimental = GL_TRUE;
	// Initialize GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		getchar();
		glfwTerminate();
		return false;
	}

	err = glGetError();
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	//Setup input instance
	input.reset(new Input(m_window));
	input->setHandlerInstance(input.get());
	input->addInputHandler(this);

	//Setup window/framebuffer resize callback
	GameWindow::windowHandlerInstance = this;
	glfwSetFramebufferSizeCallback(m_window, &GameWindow::frz_callback);
	glfwSetWindowSizeCallback(m_window, &GameWindow::wrz_callback);

	glfwSwapInterval((vsync ? 1 : 0));
	if (m_samples > 0)
	{
		glEnable(GL_MULTISAMPLE);
		if (checkglerror())
		{
			return false;
		}
	}

	return true;
}

GLvoid GameWindow::run()
{
	init();
	GLdouble timeDelta = 1.0f / m_updatefrequency;
	GLdouble timeAccumulator = 0;
	GLdouble timeAccumulator2 = 0;
	GLdouble startTime;
	GLdouble frametime;
	while (!glfwWindowShouldClose(this->m_window))
	{
		startTime = glfwGetTime();
		glfwPollEvents();
		while (timeAccumulator >= timeDelta)
		{
			update(timeAccumulator);
			
			timeAccumulator -= timeDelta;
		}
		auto ta = glfwGetTime();
		render(glfwGetTime() - startTime);
		if (m_window != nullptr)
		{
			glfwSwapBuffers(m_window);
		}
		if (timeAccumulator2 >= 1.f / 3.f)
		{
			frametime = (glfwGetTime() - ta);
			//std::cout << "Frame time: " << frametime<< "s FPS: "<<1.f/frametime<<"FPS\n";
			timeAccumulator2 = 0;
		}
		
		timeAccumulator += glfwGetTime() - startTime;
		timeAccumulator2 += glfwGetTime() - startTime;
	}
	shutdown();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

GLvoid GameWindow::quit()
{
	glfwSetWindowShouldClose(m_window, GL_TRUE);
}
