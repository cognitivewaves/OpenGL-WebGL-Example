// =============================================================================
// gl api calls to draw in the viewport
// =============================================================================

#include <windows.h>

#include <gl\gl.h>          // Header File For The OpenGL32 Library
#include <gl\glu.h>         // Header File For The GLu32 Library
#include "glextload.h"      // My utility to load extensions

// ================ Global Variables ======================

HDC          hDC=NULL;      // Private GDI Device Context
HGLRC        hRC=NULL;      // Permanent Rendering Context
extern HWND  hWnd;          // Holds Our Window Handle

GLuint shaderProgram = 0;   // Id of the shader program

// =================== Shader Code =========================

const char* vertex_shader_source =
    "attribute vec3 aVertexPosition;"
    "uniform mat4 uModelViewMatrix;"       // ModelView Matrix
    "uniform mat4 uProjectionMatrix;"      // Projection Matrix
    "void main () {"
    "gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(aVertexPosition, 1.0);"
    "}";

const char* fragment_shader_source =
    "void main () {"
    "  gl_FragColor = vec4 (0.3, 0.5, 0.1, 1.0);"
    "}";

// ========================================================


void CreateShaderProgram()
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader (vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader (fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader (shaderProgram, fragmentShader);
    glAttachShader (shaderProgram, vertexShader);
    glLinkProgram (shaderProgram);

    glUseProgram (shaderProgram);
}

void DrawGL()
{
    RECT rect;
    GetClientRect(hWnd, &rect);
    LONG width = rect.right;
    LONG height = rect.bottom;

    //glViewport(0,0,width,height);                            // Reset The Current Viewport
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                      // Black Background
    glEnable(GL_DEPTH_TEST);                                   // Enables Depth Testing
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        // Clear Screen And Depth Buffer

    // Set the perspective matrix
    glMatrixMode(GL_PROJECTION);                               // Select The Projection Matrix
    glLoadIdentity();                                          // Reset The Projection Matrix
    gluPerspective(45.0f,width/height,0.1f,100.0f);            // Calculate The Aspect Ratio Of The Window

    GLfloat pMat[16]; 
    glGetFloatv(GL_PROJECTION_MATRIX, pMat); 
    GLint uProjectionMatrixLoc = glGetUniformLocation(shaderProgram, "uProjectionMatrix");
    glUniformMatrix4fv(uProjectionMatrixLoc, 1, false, pMat);

    // Initialize the ModelView matrix
    glMatrixMode(GL_MODELVIEW);                                // Select The Modelview Matrix
    glLoadIdentity();                                          // Reset The Modelview Matrix

    glEnableClientState(GL_VERTEX_ARRAY);

    float triangleVertices[] = { 0.0f, 1.0f, 0.0f,
                                -1.0f,-1.0f, 0.0f,
                                 1.0f,-1.0f, 0.0f };

    GLuint triangleVertexPositionBuffer = 0;
    glGenBuffers (1, &triangleVertexPositionBuffer);
    glBindBuffer (GL_ARRAY_BUFFER, triangleVertexPositionBuffer);
    glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (float), triangleVertices, GL_STATIC_DRAW);

    glTranslatef(-1.5f, 0.0f, -7.0f);                          // Move Left 1.5 Units And Into The Screen 6.0
    GLfloat mvMat[16]; 
    glGetFloatv(GL_MODELVIEW_MATRIX, mvMat); 
    GLint uModelViewMatrixLoc = glGetUniformLocation(shaderProgram, "uModelViewMatrix");
    glUniformMatrix4fv(uModelViewMatrixLoc, 1, false, mvMat);

    GLint aVertexPositionLoc = glGetAttribLocation(shaderProgram, "aVertexPosition");
    glEnableVertexAttribArray(aVertexPositionLoc);             // Indicate that a vertex array will be used
    glVertexAttribPointer (aVertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays( GL_TRIANGLES, 0, 3 );
    
    float rectangleVertices[] = { 1.0f, 1.0f, 0.0f,
                                 -1.0f, 1.0f, 0.0f,
                                  1.0f,-1.0f, 0.0f,
                                 -1.0f,-1.0f, 0.0f };

    GLuint rectangleVertexPositionBuffer = 0;
    glGenBuffers (1, &rectangleVertexPositionBuffer);
    glBindBuffer (GL_ARRAY_BUFFER, rectangleVertexPositionBuffer);
    glBufferData (GL_ARRAY_BUFFER, 12 * sizeof (float), rectangleVertices, GL_STATIC_DRAW);

    glTranslatef(3.0f, 0.0f, 0.0f);                            // Move Right 3.0 Units from the previous location
    glGetFloatv(GL_MODELVIEW_MATRIX, mvMat); 
    glUniformMatrix4fv(uModelViewMatrixLoc, 1, false, mvMat);

    glEnableVertexAttribArray(aVertexPositionLoc);            // Indicate that a vertex array will be used
    glVertexAttribPointer (aVertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    glDisableVertexAttribArray(aVertexPositionLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);                         // Unbind the buffer

    SwapBuffers(hDC);                                         // Double buffering
}

int CreateRenderingContext()
{
    static PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
        1,                                              // Version Number
        PFD_DRAW_TO_WINDOW |                            // Format Must Support Window
        PFD_SUPPORT_OPENGL |                            // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,                               // Must Support Double Buffering
        PFD_TYPE_RGBA,                                  // Request An RGBA Format
        16,                                             // Select Our Color Depth
        0, 0, 0, 0, 0, 0,                               // Color Bits Ignored
        0,                                              // No Alpha Buffer
        0,                                              // Shift Bit Ignored
        0,                                              // No Accumulation Buffer
        0, 0, 0, 0,                                     // Accumulation Bits Ignored
        16,                                             // 16Bit Z-Buffer (Depth Buffer)  
        0,                                              // No Stencil Buffer
        0,                                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                                 // Main Drawing Layer
        0,                                              // Reserved
        0, 0, 0                                         // Layer Masks Ignored
    };

    hDC=GetDC(hWnd);                                    // Did We Get A Device Context?

    GLuint PixelFormat = ChoosePixelFormat(hDC,&pfd);   // Did Windows Find A Matching Pixel Format?
    SetPixelFormat(hDC,PixelFormat,&pfd);               // Are We Able To Set The Pixel Format?

    hRC=wglCreateContext(hDC);                          // Are We Able To Get A Rendering Context?

    wglMakeCurrent(hDC,hRC);                            // Try To Activate The Rendering Context

    return 0;
}

void StartGL()
{
    CreateRenderingContext();
    LoadGLExtensions();
    CreateShaderProgram();
}

void StopGL()
{
    if (hRC)                                   // Do We Have A Rendering Context?
    {
        wglMakeCurrent(NULL,NULL);             // Are We Able To Release The DC And RC Contexts?
        wglDeleteContext(hRC);                 // Are We Able To Delete The RC?
        hRC=NULL;                              // Set RC To NULL
    }

    if (hDC)
        ReleaseDC(hWnd,hDC);                   // Are we able to release the DC?
}
