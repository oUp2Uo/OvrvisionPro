//
//

#include "ovrvision_pro.h"

#include <GL/gl.h> 
#include <GL/glu.h> 
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void SWAPBUFFERS();	// platform depend function

#if 1
#define GL_API_CHECK(x)	x
#else
HWND g_hWnd;

#define GL_API_CHECK(x)do{ \
    x;\
    GLenum err = glGetError(); \
    if (GL_NO_ERROR!=err) \
	{ \
    ShowWindow(g_hWnd, SW_MINIMIZE); \
    printf("GL error: %d    Happened for the following expression:\n   %s;\n    file %s, line %d\n press any key to exit...\n", err, #x, __FILE__, __LINE__);\
    return FALSE; \
	} \
}while(0)

#define GENERAL_API_CHECK(x, str)do{ \
    if(!(x))\
	{ \
    ShowWindow(g_hWnd, SW_MINIMIZE); \
    printf("Critical error: %s  Happened for the following expression:\n   %s;\n    file %s, line %d\n press any key to exit...\n", str, #x, __FILE__, __LINE__);\
    return FALSE; \
	}; \
}while(0)

#define CL_API_CHECK(x)do{ \
    cl_int ERR = (x); \
    if(ERR != CL_SUCCESS)\
	{\
    ShowWindow(g_hWnd, SW_MINIMIZE); \
    printf("OpenCL error: %s\n   Happened for the following expression:\n   %s;\n file %s, line %d\n  press any key to exit...\n", opencl_error_to_str(ERR).c_str(), #x, __FILE__, __LINE__);\
    return FALSE; \
	} \
}while(0)
#endif


GLuint textureIDs[2];


OVR::OvrvisionPro ovrvision;	// OvrvisionPro camera
OVR::ROI size;

GLvoid createObjects();

/* OpenGL code */
GLvoid initializeGL(GLsizei width, GLsizei height)
{
#if 0
	GL_API_CHECK(glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE));

	//init OpenGL color buffer and viewport
	GL_API_CHECK(glClearColor(0.0, 0.0, 0.0, 1.0));
	GL_API_CHECK(glViewport(0, 0, width, height));

	//setup camera
	GL_API_CHECK(glMatrixMode(GL_MODELVIEW));
	GL_API_CHECK(glLoadIdentity());
	GL_API_CHECK(glMatrixMode(GL_PROJECTION));
	GL_API_CHECK(glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0));


	//no lighting or depth testing, just texturing please
	GL_API_CHECK(glDisable(GL_DEPTH_TEST));
	GL_API_CHECK(glDisable(GL_LIGHTING));
	GL_API_CHECK(glEnable(GL_TEXTURE_2D));
#endif

	ovrvision.CheckGPU();

	if (ovrvision.Open(0, OVR::Camprop::OV_CAMHD_FULL, 0) == 0) // Open with OpenGL sharing mode
		puts("Can't open OvrvisionPro");

	createObjects();
}

GLvoid resize(GLsizei width, GLsizei height)
{
	GLfloat aspect;

	//glViewport(0, 0, width, height);

	aspect = (GLfloat)width / height;

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0, aspect, 3.0, 7.0);
	//glMatrixMode(GL_MODELVIEW);
}

GLvoid createObjects()
{
	// Create textures
	glEnable(GL_TEXTURE_2D);
	glGenTextures(2, textureIDs);
	size = ovrvision.SetSkinScale(2);

	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	ovrvision.CreateSkinTextures(size.width, size.height, textureIDs[0], textureIDs[1]);
}


GLvoid drawScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step6. テクスチャの画像指定
	ovrvision.Capture(OVR::Camqt::OV_CAMQT_DMSRMP);
	glFinish();
	ovrvision.UpdateSkinTextures(textureIDs[0], textureIDs[1]);
#ifdef _DEBUG
	cv::Mat left(size.height, size.width, CV_8UC4);
	cv::Mat right(size.height, size.width, CV_8UC4);
	ovrvision.InspectTextures(left.data, right.data, 3); // Get HSV images
	cv::imshow("Left", left);
	cv::imshow("Right", right);
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 1);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2i(1, 1);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2i(1, 0);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2i(0, 0);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glEnd();
	glFinish();

	SWAPBUFFERS();
}
