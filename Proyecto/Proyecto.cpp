// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1000, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Proyecto - CNCE", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    
    // Load models
    Model table((char*)"Models/Table/Table.obj");
    Model sofa((char*)"Models/Sofa/Sofa.obj");
    Model fountain((char*)"Models/Fountain/Fountain.obj");
    Model tableLamp((char*)"Models/TableLamp/TableLamp.obj");
    Model bell((char*)"Models/Bell/Bell.obj");

    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] =
    {
        // Positions            // Normals              // Texture Coords
        -0.5f, -0.5f, 0.5f,		0.0f, 0.0f, -1.0f,		(160.0f / 256.0f), (250.0f / 256.0f), //Front
         0.5f, -0.5f, 0.5f,		0.0f, 0.0f, -1.0f,		(250.0f / 256.0f), (250.0f / 256.0f),
         0.5f,  0.5f, 0.5f,		0.0f, 0.0f, -1.0f,		(250.0f / 256.0f), (30.0f / 256.0f),
        -0.5f,  0.5f, 0.5f,		0.0f, 0.0f, -1.0f,		(160.0f / 256.0f), (30.0f / 256.0f),

        // Positions            // Normals              // Texture Coords
        -0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		(160.0f / 256.0f), (250.0f / 256.0f), //Back
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,		(250.0f / 256.0f), (250.0f / 256.0f),
         0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		(250.0f / 256.0f), (30.0f / 256.0f),
        -0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,		(160.0f / 256.0f), (30.0f / 256.0f),

        // Positions            // Normals              // Texture Coords
         0.5f, -0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (250.0f / 256.0f), //Right
         0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (250.0f / 256.0f),
         0.5f,  0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (30.0f / 256.0f),
         0.5f,  0.5f,  0.5f,	-1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (30.0f / 256.0f),

         // Positions            // Normals              // Texture Coords
        -0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (250.0f / 256.0f), //Left
        -0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (250.0f / 256.0f),
        -0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,		(160.0f / 256.0f), (30.0f / 256.0f),
        -0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,		(70.0f / 256.0f), (30.0f / 256.0f),

        // Positions            // Normals              // Texture Coords
        -0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		(243.0f / 965.0f), (241.0f / 723.0f), //Bottom
         0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,		(480.0f / 965.0f), (241.0f / 723.0f),
         0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		(480.0f / 965.0f), (5.0f / 723.0f),
        -0.5f, -0.5f,  0.5f,	0.0f, -1.0f, 0.0f,		(243.0f / 965.0f), (5.0f / 723.0f),

        // Positions            // Normals              // Texture Coords
        -0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		(243.0f / 965.0f), (483.0f / 723.0f), //Top
         0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,		(480.0f / 965.0f), (483.0f / 723.0f),
         0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		(480.0f / 965.0f), (719.0f / 723.0f),
        -0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,		(243.0f / 965.0f), (719.0f / 723.0f),


    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3,

        4,5,7,
        5,6,7,

        8,9,11,
        9,10,11,

        12,13,15,
        13,14,15,

        16,17,19,
        17,18,19,

        20,21,23,
        21,22,23,
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normals attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Texture Coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    GLuint lampTex;
    glGenTextures(1, &lampTex);

    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    // Diffuse map
    image = stbi_load("images/TableLamp_Mat-Base.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glBindTexture(GL_TEXTURE_2D, lampTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);
   

    // Draw in wireframe
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    // Game loop
    while( !glfwWindowShouldClose( window ) )
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Check and call events
        glfwPollEvents( );
        DoMovement( );
        
        // Clear the colorbuffer
        glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shader.Use( );
        
        // Camera
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        glUniformMatrix4fv( glGetUniformLocation( shader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );

        // Table
        glm::mat4 model(1);
		glUniformMatrix4fv( glGetUniformLocation( shader.Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
        table.Draw( shader );

        // Table Lamp
        model = glm::translate(model, glm::vec3(-0.33f, 0.531f, 0.41f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        tableLamp.Draw(shader);

        // Bell
        model = glm::translate(model, glm::vec3(-0.33f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        bell.Draw(shader);

        // Sofa
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        sofa.Draw(shader);

        // Fountain
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fountain.Draw(shader);

        // Bell
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        fountain.Draw(shader);

        // Table Lamp 2
		// Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lampTex);

        glBindVertexArray(VAO);
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        
        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

