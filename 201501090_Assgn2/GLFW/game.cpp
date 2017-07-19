#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <thread>
#include <ao/ao.h>
#include <mpg123.h>
#include <FTGL/ftgl.h>
#include <SOIL/SOIL.h>
#define XMAX 600
#define XMIN -600
#define YMAX 500
#define YMIN -500
#define out1(x)cout<<#x<<" is "<<x<<endl
#define out2(x,y)cout<<#x<<" is "<<x<<" "<<#y <<" is "<<y<<endl
#define out3(x,y,z)cout<<#x<<" is "<<x<<" "<<#y<<" is "<<y<<" "<<#z<<" is "<<z<<endl;
#define out4(a,b,c,d)cout<<#a<<" is "<<a<<" "<<#b<<"  is "<<b<<" "<<#c<<" is "<<c<<" "<<#d<<" is "<<d<<endl;
int fbwidth,fbheight;
int gameovercheck=0;
int nextlevelcheck=0;
int nextlevelshow=0;
int view=3;
using namespace std;
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;
    GLuint TextureBuffer;
    GLuint TextureID;


    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint MatrixID;
    GLuint TexMatrixID;
} Matrices;
struct FTGLFont {
    FTFont* font;
    GLuint fontMatrixID;
    GLuint fontColorID;
} GL3Font;
GLuint programID,fontProgramID,textureProgramID;


typedef struct COLOR{
    float r;
    float g;
    float b;
} COLOR;
typedef struct Point {
    float x;
    float y;
    float z;
} Point;

typedef struct Triangle {
    int p1;
    int p2;
    int p3; //Indices of the points corresponding to the triangle
} Triangle;

COLOR grey = {168.0/255.0,168.0/255.0,168.0/255.0};
COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
COLOR coingold = {255.0/255.0,223.0/255.0,0.0/255.0};
COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
COLOR lightgreen = {57/255.0,230/255.0,0/255.0};
COLOR darkgreen = {51/255.0,102/255.0,0/255.0};
COLOR black = {30/255.0,30/255.0,21/255.0};
COLOR blue = {0,0,1};
COLOR darkbrown = {46/255.0,46/255.0,31/255.0};
COLOR lightbrown = {95/255.0,63/255.0,32/255.0};
COLOR brown1 = {117/255.0,78/255.0,40/255.0};
COLOR brown2 = {134/255.0,89/255.0,40/255.0};
COLOR brown3 = {46/255.0,46/255.0,31/255.0};
COLOR cratebrown = {153/255.0,102/255.0,0/255.0};
COLOR cratebrown1 = {121/255.0,85/255.0,0/255.0};
COLOR cratebrown2 = {102/255.0,68/255.0,0/255.0};
COLOR skyblue2 = {113/255.0,185/255.0,209/255.0};
COLOR skyblue1 = {123/255.0,201/255.0,227/255.0};
COLOR skyblue = {132/255.0,217/255.0,245/255.0};
COLOR cloudwhite = {229/255.0,255/255.0,255/255.0};
COLOR cloudwhite1 = {204/255.0,255/255.0,255/255.0};
COLOR lightpink = {255/255.0,122/255.0,173/255.0};
COLOR darkpink = {255/255.0,51/255.0,119/255.0};
COLOR white = {255/255.0,255/255.0,255/255.0};
int gamestart=0;
int platform[10][10]={
        1, 1, 1, 0, 0, 0 ,0 ,0, 0, 0,
        1, 1, 1,1, 1, 1, 0, 0,0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 1, 1, 1, 1, 1 ,1 ,1 ,1,
        0, 0, 0,0, 0, 1, 1, 5 ,1 ,1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0 ,0, 0 ,0, 1, 1, 1, 1,
        0, 0, 0 ,0, 0, 0, 0 ,0, 0, 0,
        0, 0 ,0 ,0 ,0, 0, 0 ,0,0, 0,
        0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0,

};
int bridges[10][10]={
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,1,1,1,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
};
int switches[10][10]={
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,1,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
};
float view_rotation_angle=90;

typedef struct object{
    VAO *object,*borderobject;
    string id;
    string name;
    float x,y,z;
    float presentrotationx,presentrotationy,presentrotationz;
    glm::vec3 presentrotationvector;
    float presentrotationangle;
    float x_scale,y_scale,z_scale;
    float x_speed,y_speed,z_speed;
    COLOR color;
    double radius;
    float angle_x; //Current Angle (Actual rotated angle of the object)
    float angle_y;
    float angle_z;
    float rotation_x_offset;
    float rotation_y_offset;
    float rotation_z_offset;
    double height,width;
    double radius_x;
    double radius_y;
    double status;
    int typeofstage;
    int buttonpressed;
    double angle;
    double inAir;
    string lastrotate;
    int direction_x; //0 for clockwise and 1 for anticlockwise for animation
    int direction_y;
    int direction_z;
    double fixed;
    double r;
    int isRotating;
    int direction; //0 for clockwise and 1 for anticlockwise for animation
    float remAngle; //the remaining angle to finish animation
    int isMovingAnim;
    int dx;
    int dy;
    int dz;

} object;
float score = 0;
map <string, object> objects;
map <string, object> boxes;
map <string, object> cube;
map <string, object> stage;
float camera_fov=1.3;
double MAXV = 20;
float x_change = 0; //For the camera pan
float y_change = 0; //For the camera pan
int zoom_camera = 1;
int window_width = 1200;
int window_height = 600;
int boxspeed=10;
int camera_follow=0;
int camera_follow_adjust=0;
int camera_top=0;
int camera_fps=0;
float fps_head_offset=0,fps_head_offset_x=0;
int head_tilting=0;
void* play_audio(string audioFile);
void reshapeWindow (GLFWwindow* window, int width, int height);
struct object createRectangle (string name, float weight, COLOR colorA, COLOR colorB, COLOR colorC, COLOR colorD, float x, float y, float height, float width, string component);
/* Function to load Shaders - Use it as it is */
//Check collision b/w a player and an object
int check_collision_object(string name1,string name2){
    if(objects[name2].status==1 && objects[name1].y>=objects[name2].y-objects[name2].y_scale/2-objects[name1].y_scale/2-50 && objects[name1].y<=objects[name2].y+objects[name2].y_scale/2+objects[name1].y_scale/2+50 && objects[name1].x>=objects[name2].x-objects[name2].x_scale/2-objects[name1].x_scale/2-25 && objects[name1].x<=objects[name2].x+objects[name2].x_scale/2+objects[name1].x_scale/2+25 && objects[name1].z>=objects[name2].z-objects[name2].z_scale/2-objects[name1].z_scale/2-25 && objects[name1].z<=objects[name2].z+objects[name2].z_scale/2+objects[name1].z_scale/2+25 ){
        return 1;
    }
    return 0;
}
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
//    exit(EXIT_SUCCESS);
}
glm::vec3 getRGBfromHue (int hue)
{
    float intp;
    float fracp = modff(hue/60.0, &intp);
    float x = 1.0 - abs((float)((int)intp%2)+fracp-1.0);

    if (hue < 60)
        return glm::vec3(1,x,0);
    else if (hue < 120)
        return glm::vec3(x,1,0);
    else if (hue < 180)
        return glm::vec3(0,1,x);
    else if (hue < 240)
        return glm::vec3(0,x,1);
    else if (hue < 300)
        return glm::vec3(x,0,1);
    else
        return glm::vec3(1,0,x);
}

/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
            0,                  // attribute 0. Vertices
            3,                  // size (x,y,z)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
            1,                  // attribute 1. Color
            3,                  // size (r,g,b)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    return vao;
}
struct VAO* create3DTexturedObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* texture_buffer_data, GLuint textureID, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;
    vao->TextureID = textureID;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->TextureBuffer));  // VBO - textures

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
            0,                  // attribute 0. Vertices
            3,                  // size (x,y,z)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    glBindBuffer (GL_ARRAY_BUFFER, vao->TextureBuffer); // Bind the VBO textures
    glBufferData (GL_ARRAY_BUFFER, 2*numVertices*sizeof(GLfloat), texture_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
            2,                  // attribute 2. Textures
            2,                  // size (s,t)
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}
/* Create an OpenGL Texture from an image */
GLuint createTexture (const char* filename)
{
    GLuint TextureID;
    // Generate Texture Buffer
    glGenTextures(1, &TextureID);
    // All upcoming GL_TEXTURE_2D operations now have effect on our texture buffer
    glBindTexture(GL_TEXTURE_2D, TextureID);
    // Set our texture parameters
    // Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering (interpolation)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load image and create OpenGL texture
    int twidth, theight;
    unsigned char* image = SOIL_load_image(filename, &twidth, &theight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMaps to use
    SOIL_free_image_data(image); // Free the data read from file after creating opengl texture
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess it up

    return TextureID;
}
void draw3DTexturedObject (struct VAO* vao)
{
    if(vao == NULL)return;
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Bind Textures using texture units
    glBindTexture(GL_TEXTURE_2D, vao->TextureID);

    // Enable Vertex Attribute 2 - Texture
    glEnableVertexAttribArray(2);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->TextureBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle

    // Unbind Textures to be safe
    glBindTexture(GL_TEXTURE_2D, 0);
}
/**************************
 * Customizable functions *
 **************************/

float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;
int inAir=0;
//Camera eye, target and up vector components
float eye_x,eye_y,eye_z;
float target_x=-50,target_y,target_z=-50;
float angle=0;
float camera_radius;
int camera_disable_rotation=0;
int redleft,redright,greenleft,greenright;
/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
int presentlevel=1;
int noofsteps=0;
int comparecolors(struct COLOR color1, struct COLOR color2){
    if(color1.b==color2.b && color1.g==color2.g && color1.r==color2.r)
        return 1;
    else return 0;
}

//Set the audioFile parameter to the file name first before calling this function
void* play_audio(string audioFile){
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, &audioFile[0]);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * 8;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    char *p =(char *)buffer;
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
        ao_play(dev, p, done);

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
}
void rotatecube(string direction){
    cube["cube"].presentrotationangle=0;
    float x=cube["cube"].x,y=cube["cube"].y,z=cube["cube"].z,x_scale=cube["cube"].x_scale,y_scale=cube["cube"].y_scale,z_scale=cube["cube"].z_scale;
    if(direction == "left"){

        cube["cube"].lastrotate="left";
        cube["cube"].presentrotationx=-cube["cube"].x_scale;
        cube["cube"].presentrotationy=-cube["cube"].y_scale;
        cube["cube"].presentrotationz=-cube["cube"].z_scale;
        cube["cube"].presentrotationvector=glm::vec3 (0,-1,0);

    }else if(direction == "right"){
        cube["cube"].lastrotate="right";
        cube["cube"].presentrotationx=cube["cube"].x_scale;
        cube["cube"].presentrotationy=-cube["cube"].y_scale;
        cube["cube"].presentrotationz=-cube["cube"].z_scale;
        cube["cube"].presentrotationvector=glm::vec3 (0,1,0);


    }else if(direction == "up"){
        cube["cube"].lastrotate="up";
        cube["cube"].presentrotationx=-cube["cube"].x_scale;
        cube["cube"].presentrotationy=cube["cube"].y_scale;
        cube["cube"].presentrotationz=-cube["cube"].z_scale;
        cube["cube"].presentrotationvector=glm::vec3 (-1,0,0);

    }else if(direction == "down"){
        cube["cube"].lastrotate="down";
        cube["cube"].presentrotationx=-cube["cube"].x_scale;
        cube["cube"].presentrotationy=-cube["cube"].y_scale;
        cube["cube"].presentrotationz=-cube["cube"].z_scale;
        cube["cube"].presentrotationvector=glm::vec3 (1,0,0);

    }

}
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_T:
                camera_fov+=0.09;
                break;
            case GLFW_KEY_B:
                view=1;
                break;
            case GLFW_KEY_2:
                view=2;
                break;
            case GLFW_KEY_3:
                view=3;
                break;
            case GLFW_KEY_4:
                view=4;
                break;
            case GLFW_KEY_5:
                view=5;
                break;
            case GLFW_KEY_R:
                camera_fov-=0.09;

                break;
            case GLFW_KEY_Y:
                camera_disable_rotation=0;
                camera_follow=0;
                camera_fps=0;
                camera_top=0;
                camera_radius=800; //Tower view
                eye_x = -50+camera_radius*cos(angle*M_PI/180);
                eye_z = -50+camera_radius*sin(angle*M_PI/180);
                eye_y=1100;
                target_x=-50;
                target_y=0;
                target_z=-50;
                fps_head_offset=0;
                fps_head_offset_x=0;
                camera_fov=1.3;

                break;
            case GLFW_KEY_U:
                camera_disable_rotation=1;
                camera_fps=0;
                camera_top=0;
                camera_follow=1;
                fps_head_offset=0;
                fps_head_offset_x=0;
                camera_fov=1.3;
                break;
            case GLFW_KEY_I:
                camera_disable_rotation=1;
                camera_follow=0;
                camera_fps=1;
                camera_top=0;
                fps_head_offset=0;
                fps_head_offset_x=0;
                camera_fov=1.3;
                reshapeWindow(window,700,1400);
                break;
            case GLFW_KEY_LEFT:
                rotatecube("left");
                cube["cube"].buttonpressed=1;
                if(view==4)
                view_rotation_angle=-90;
                break;
            case GLFW_KEY_RIGHT:
                rotatecube("right");
                cube["cube"].buttonpressed=1;
                if(view==4)
                view_rotation_angle=180;
                break;
            case GLFW_KEY_UP:
                rotatecube("up");
                cube["cube"].buttonpressed=1;
                if(view==4)
                view_rotation_angle=90;
                break;
            case GLFW_KEY_DOWN:
                rotatecube("down");
                cube["cube"].buttonpressed=1;
                if(view==4)
                view_rotation_angle=270;
                break;
            case GLFW_KEY_C:
                gamestart=1;
            default:
                break;
        }
    }
    else if (action != GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_A:
                view_rotation_angle+=5;
                break;

            case GLFW_KEY_D:
                view_rotation_angle-=5;
                break;

            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            default:
                break;

        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
    switch (key) {
        case 'Q':
        case 'q':
            quit(window);
            break;
        default:
            break;
    }
}
int mouseleftpressed=0;
int mouseleftfirstpressed=0;
int mouserightpressed=0;
int mouserightfirstpressed=0;
double mouse_x,mouse_y;
double mouse_x_old,mouse_y_old;
int moveredbox=0;
int movegreenbox=0;
int movecannonbox=0;
void mouseright(GLFWwindow* window){
    if(mouserightpressed){

        glfwGetCursorPos(window,&mouse_x,&mouse_y);
        if(!mouserightfirstpressed){
            mouse_x_old=mouse_x;
            mouse_y_old=mouse_y;
            mouserightfirstpressed=1;
        }
        out1(mouse_x-mouse_x_old);
        view_rotation_angle+=(mouse_x-mouse_x_old)/140;

    }
}
/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            out1(action);
            if (action != GLFW_RELEASE){
                out1("mouse pressed");
                mouseleftpressed=1;

            }

            if(action == GLFW_RELEASE){
                moveredbox=0;
                movegreenbox=0;
                mouseleftfirstpressed=0;
                mouseleftpressed=0;
                movecannonbox=0;
            }

            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS){
                mouserightpressed=1;



            }
            if (action == GLFW_RELEASE) {
                mouserightpressed=0;
            }
            break;
        default:
            break;
    }
}
void mousescroll(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset==-1) {
        camera_fov*=1.1;
    }
    else if(yoffset==1){
        camera_fov/=1.1; //make it bigger than current size
    }
    if(camera_fov>=2){
        camera_fov=2;
    }
    if(camera_fov<=0.5){
        camera_fov=0.5;
    }
    reshapeWindow(window,700,1400);
}
//Ensure the panning does not go out of the map
void check_pan(){
    if(x_change-400.0f/zoom_camera<-400)
        x_change=-400+400.0f/zoom_camera;
    else if(x_change+400.0f/zoom_camera>400)
        x_change=400-400.0f/zoom_camera;
    if(y_change-300.0f/zoom_camera<-300)
        y_change=-300+300.0f/zoom_camera;
    else if(y_change+300.0f/zoom_camera>300)
        y_change=300-300.0f/zoom_camera;
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    fbwidth=width, fbheight=height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    GLfloat fov = camera_fov;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
       glLoadIdentity ();
       gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 7000.0f);
}

int checkCollisionRight(object col_object, object my_object){
    if(col_object.x>my_object.x && col_object.y+col_object.height/2>my_object.y-my_object.height/2 && col_object.y-col_object.height/2<my_object.y+my_object.height/2 && col_object.x-col_object.width/2<my_object.x+my_object.width/2 && col_object.x+col_object.width/2>my_object.x-my_object.width/2){
        return 1;
    }
    return 0;
}

int checkCollisionLeft(object col_object, object my_object){
    if(col_object.x<my_object.x && col_object.y+col_object.height/2>my_object.y-my_object.height/2 && col_object.y-col_object.height/2<my_object.y+my_object.height/2 && col_object.x+col_object.width/2>my_object.x-my_object.width/2 && col_object.x-col_object.width/2<my_object.x+my_object.width/2){
        return 1;
    }
    return 0;
}

int checkCollisionTop(object col_object, object my_object){
    if(col_object.y>my_object.y && col_object.x+col_object.width/2>my_object.x-my_object.width/2 && col_object.x-col_object.width/2<my_object.x+my_object.width/2 && col_object.y-col_object.height/2<my_object.y+my_object.height/2 && col_object.y+col_object.height/2>my_object.y-my_object.height/2){
        return 1;
    }
    return 0;
}

int checkCollisionBottom(object col_object, object my_object){
    if(col_object.y<my_object.y && col_object.x+col_object.width/2>my_object.x-my_object.width/2 && col_object.x-col_object.width/2<my_object.x+my_object.width/2 && col_object.y+col_object.height/2>my_object.y-my_object.height/2 && col_object.y-col_object.height/2<my_object.y+my_object.height/2){
        return 1;
    }
    return 0;
}
int checkCollision(string name,float dx,float dy){
    int any_collide=0;

}
void display_string(float x,float y,char *str,float fontScaleValue){
    glm::vec3 fontColor = glm::vec3(0,0,0);
    Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translateText = glm::translate(glm::vec3(x,y,0));
    glm::mat4 scaleText = glm::scale(glm::vec3(fontScaleValue,fontScaleValue,fontScaleValue));
    Matrices.model *= (translateText * scaleText);
    glm::mat4 MVP = Matrices.projection * Matrices.view * Matrices.model;
    glUniformMatrix4fv(GL3Font.fontMatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform3fv(GL3Font.fontColorID, 1, &fontColor[0]);
    GL3Font.font->Render(str);

}
// Creates the triangle object used in this sample code
struct object createTriangle (string name, float weight, COLOR color, float x[], float y[], string component, int fill)
{
    /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

    /* Define vertex array as used in glBegin (GL_TRIANGLES) */
    float xc=(x[0]+x[1]+x[2])/3;
    float yc=(y[0]+y[1]+y[2])/3;
    GLfloat vertex_buffer_data [] = {
            x[0]-xc,y[0]-yc,0, // vertex 0
            x[1]-xc,y[1]-yc,0, // vertex 1
            x[2]-xc,y[2]-yc,0 // vertex 2
    };

    GLfloat color_buffer_data [] = {
            color.r,color.g,color.b, // color 1
            color.r,color.g,color.b, // color 2
            color.r,color.g,color.b // color 3
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    VAO *triangle;
    if(fill==1)
        triangle=create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_FILL);
    else
        triangle=create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
    object vishsprite = {};
    vishsprite.color = color;
    vishsprite.name = name;
    vishsprite.object = triangle;
    vishsprite.x=(x[0]+x[1]+x[2])/3; //Position of the sprite is the position of the centroid
    vishsprite.y=(y[0]+y[1]+y[2])/3;
    vishsprite.height=-1; //Height of the sprite is undefined
    vishsprite.width=-1; //Width of the sprite is undefined
    vishsprite.status=1;
        objects[name]=vishsprite;
    return vishsprite;
}

// Creates the rectangle object used in this sample code
struct object createRectangle (string name, float weight, COLOR colorA, COLOR colorB, COLOR colorC, COLOR colorD, float x, float y, float height, float width, string component)
{
    // GL3 accepts only Triangles. Quads are not supported
    float w=width/2,h=height/2;
    GLfloat vertex_buffer_data [] = {
            -w,-h,0, // vertex 1
            -w,h,0, // vertex 2
            w,h,0, // vertex 3

            w,h,0, // vertex 3
            w,-h,0, // vertex 4
            -w,-h,0  // vertex 1
    };

    GLfloat color_buffer_data [] = {
            colorA.r,colorA.g,colorA.b, // color 1
            colorB.r,colorB.g,colorB.b, // color 2
            colorC.r,colorC.g,colorC.b, // color 3

            colorC.r,colorC.g,colorC.b, // color 4
            colorD.r,colorD.g,colorD.b, // color 5
            colorA.r,colorA.g,colorA.b // color 6
    };

    // create3DObject creates and returns a handle to a VAO that can be used later
    VAO *rectangle = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
    object vishsprite = {};
    vishsprite.color = colorA;
    vishsprite.name = name;
    vishsprite.object = rectangle;
    vishsprite.x=x;
    vishsprite.y=y;
    vishsprite.height=height;
    vishsprite.width=width;
    vishsprite.x_speed=0;
    vishsprite.y_speed=0;
    vishsprite.status=1;
    vishsprite.angle=0;
    return vishsprite;
}

struct object createCircle (string name, float weight, COLOR color, float x, float y, float r, int NoOfParts, string component, int fill)
{
    int parts = NoOfParts;
    float radius = r;
    GLfloat vertex_buffer_data[parts*9];
    GLfloat color_buffer_data[parts*9];
    int i,j;
    float angle=(2*M_PI/parts);
    float current_angle = 0;
    for(i=0;i<parts;i++){
        for(j=0;j<3;j++){
            color_buffer_data[i*9+j*3]=color.r;
            color_buffer_data[i*9+j*3+1]=color.g;
            color_buffer_data[i*9+j*3+2]=color.b;
        }
        vertex_buffer_data[i*9]=0;
        vertex_buffer_data[i*9+1]=0;
        vertex_buffer_data[i*9+2]=0;
        vertex_buffer_data[i*9+3]=radius*cos(current_angle);
        vertex_buffer_data[i*9+4]=radius*sin(current_angle);
        vertex_buffer_data[i*9+5]=0;
        vertex_buffer_data[i*9+6]=radius*cos(current_angle+angle);
        vertex_buffer_data[i*9+7]=radius*sin(current_angle+angle);
        vertex_buffer_data[i*9+8]=0;
        current_angle+=angle;
    }
    VAO* circle;
    if(fill==1)
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_FILL);
    else
        circle = create3DObject(GL_TRIANGLES, (parts*9)/3, vertex_buffer_data, color_buffer_data, GL_LINE);
    object vishsprite = {};
    vishsprite.color = color;
    vishsprite.name = name;
    vishsprite.object = circle;
    vishsprite.x=x;
    vishsprite.y=y;
    vishsprite.height=2*r; //Height of the sprite is 2*r
    vishsprite.width=2*r; //Width of the sprite is 2*r
    vishsprite.status=1;
    vishsprite.inAir=0;
    vishsprite.x_speed=0;
    vishsprite.y_speed=0;
    vishsprite.radius=r;
    vishsprite.r=r;
    return vishsprite;
}
VAO* rectangle;
struct object  generate_cube(string name,float x_pos,float y_pos,float z_pos,float l, float b, float h,COLOR L,COLOR R,COLOR T,string component,int typeofstage=0,GLenum fill_mode=GL_FILL){
    GLfloat a[]={

            /* Rectangle 1 */

            -l, -b, -h,
            -l, -b, h,
            l, -b, h,
            l, -b, h,
            l, -b, -h,
            -l, -b, -h,

            /* Rectangle 2 */

            l, b, -h,
            l, b, h,
            l, -b, h,
            l, -b, h,
            l, -b, -h,
            l, b, -h,

            /* Rectangle 3 */

            -l, b, -h,
            -l, b, h,
            l, b, h,
            l, b, h,
            l, b, -h,
            -l, b, -h,

            /* Rectangle 4 */

            -l, b, -h,
            -l, b, h,
            -l, -b, h,
            -l, -b, h,
            -l, -b, -h,
            -l, b, -h,

            /* Rectangle 5 */

            -l, -b, -h,
            -l, b, -h,
            l, b, -h,
            l, b, -h,
            l, -b, -h,
            -l, -b, -h,

            /* Rectangle 6 */
            -l, -b, h,
            -l, b, h,
            l, b, h,
            l, b, h,
            l, -b, h,
            -l, -b, h,

    };
    GLfloat C[]={
            /* Rectangle 1 (Bottom)*/
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,

            /* Rectangle 2 (left)*/
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,



            /* Rectangle 3  (Top layer)*/

            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,

            /* Rectangle 4 */
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,

            /* Rectangle 5 (right)*/

            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,

            /* Rectangle 6 */
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,


    };
    T=black;
    R=black;
    L=black;
    GLfloat C2[]={
            /* Rectangle 1 (Bottom)*/
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,

            /* Rectangle 2 (left)*/
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,



            /* Rectangle 3  (Top layer)*/

            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,
            T.r, T.g, T.b,

            /* Rectangle 4 */
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,
            L.r, L.g, L.b,

            /* Rectangle 5 (right)*/

            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,

            /* Rectangle 6 */
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,
            R.r, R.g, R.b,


    };

    VAO *myobject = create3DObject(GL_TRIANGLES,  6*6,  a,  C,  fill_mode);
    VAO *mylinesobjects=create3DObject(GL_TRIANGLES,  6*6,  a,  C2,  GL_LINE);
    object vishsprite = {};


    vishsprite.name = name;
    vishsprite.object = myobject;
    vishsprite.borderobject = mylinesobjects;
    vishsprite.x=x_pos;
    vishsprite.y=y_pos;
    vishsprite.z=z_pos;
    vishsprite.status=1;
    vishsprite.fixed=0;
    vishsprite.x_scale=l;
    vishsprite.y_scale=b;
    vishsprite.z_scale=h;
    vishsprite.x_speed=0;
    vishsprite.y_speed=0;
    vishsprite.z_speed=0;
    vishsprite.presentrotationx=0;
    vishsprite.presentrotationy=0;
    vishsprite.presentrotationz=0;
    vishsprite.angle_x=0;
    vishsprite.angle_y=0;
    vishsprite.angle_z=0;
    vishsprite.presentrotationangle=0;
    vishsprite.presentrotationvector=glm::vec3 (1,0,0);
    vishsprite.typeofstage=typeofstage;
    vishsprite.status=typeofstage;
    vishsprite.angle=0;
    if(component=="cube")
        cube[name]=vishsprite;
    if(component=="stage"){
        stage[name]=vishsprite;
    }
    return vishsprite;

}
void updategameboard(){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(platform[i][j]==1){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,grey,grey,grey,"stage",1);
            }else if(platform[i][j]==2){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,blue,blue,blue,"stage",2);
            }
            else if(platform[i][j]==3){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,gold,gold,gold,"stage",0);
            }
            else if(platform[i][j]==4){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,red,red,red,"stage",3);
            }
            else{
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,grey,grey,grey,"stage",0);
            }
        }
    }


    generate_cube("cube",0,0,2.1,1,1,2,lightgreen,lightgreen,lightgreen,"cube",1);
}
void gameover(){
    gameovercheck=1;
    if(presentlevel==3){
        score+=5;
    }
    out2(score,noofsteps);
}

void nextlevel(){
    nextlevelcheck=0;
    score=5;
    if(presentlevel==1){
        presentlevel=2;
        int platform1[10][10]={
                1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
                1, 1, 1, 4, 0, 0, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
                1, 1, 1, 1, 3 ,3, 1, 1, 5, 1,
                1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
                0, 1 ,1, 1, 0, 0, 1, 1, 1, 1,
                0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
                0, 0, 0, 1, 0, 0 ,0, 0, 0, 0,
                0, 0, 0, 0 ,0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,


        };
        int bridges1[10][10]={
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,1,1,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
        };
        int switches1[10][10]={
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,1,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
        };

        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                platform[i][j]=platform1[i][j];
            }
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                bridges[i][j]=bridges1[i][j];
            }
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                switches[i][j]=switches1[i][j];
            }
        }


    }
    else if(presentlevel==2){
        presentlevel=3;
        int platform1[10][10]={
                1 ,1, 1, 1, 0, 0, 0, 0, 0, 0,
                1, 2, 2, 2, 1 ,1 ,4 ,1, 0, 0,
                1, 2, 2, 2, 1, 1, 1, 1, 0, 0,
                1 ,1 ,1 ,1, 1, 1, 1, 3, 0, 0,
                0, 0, 0, 0, 0 ,0, 0, 3, 1, 1,
                0, 0, 0, 0, 0, 0, 0 ,1 ,1 ,1,
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 1, 5, 1, 2, 1, 1, 0,
                0, 0, 0, 1, 1, 1, 1, 1, 1, 0,
                0, 0, 0, 0, 0, 0, 1, 1, 1, 0,

        };
        int bridges1[10][10]={
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,1,0,0,
                0,0,0,0,0,0,0,1,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
        };
        int switches1[10][10]={
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,1,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0,0,0,
        };

        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                platform[i][j]=platform1[i][j];
            }
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                bridges[i][j]=bridges1[i][j];
            }
        }
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                switches[i][j]=switches1[i][j];
            }
        }


    }
    else if(presentlevel==3){
        gameover();
    }
    updategameboard();


}

void createBackground(GLuint textureID)
{

    // GL3 accepts only Triangles. Quads are not supported
    static const GLfloat vertex_buffer_data [] = {
            XMIN,YMIN,-100, // vertex 1
            XMAX,YMIN,-100, // vertex 2
            XMAX, YMAX,-100, // vertex 3

            XMAX, YMAX,-100, // vertex 3
            XMIN, YMAX,-100, // vertex 4
            XMIN,YMIN,-100  // vertex 1
    };

    static const GLfloat color_buffer_data [] = {
            1,0,0, // color 1
            0,0,1, // color 2
            0,1,0, // color 3

            0,1,0, // color 3
            0.3,0.3,0.3, // color 4
            1,0,0  // color 1
    };

    // Texture coordinates start with (0,0) at top left of the image to (1,1) at bot right
    static const GLfloat texture_buffer_data [] = {
            0,1, // TexCoord 1 - bot left
            1,1, // TexCoord 2 - bot right
            1,0, // TexCoord 3 - top right

            1,0, // TexCoord 3 - top right
            0,0, // TexCoord 4 - top left
            0,1  // TexCoord 1 - bot left
    };

    // create3DTexturedObject creates and returns a handle to a VAO that can be used later
    rectangle = create3DTexturedObject(GL_TRIANGLES, 6, vertex_buffer_data, texture_buffer_data, textureID, GL_FILL);
}
void createModel (string name, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale, string filename, string layer) //Create object from blender
{
    GLfloat vertex_buffer_data [100000] = {
    };
    GLfloat color_buffer_data [100000] = {
    };
    vector<Point> points;
    int len=0;
    string line;
    float a,b,c;
    int start=0;
    ifstream myfile;
    myfile.open(filename.c_str());
    if (myfile.is_open()){
        while (myfile >> line){
            if(line.length()==1 && line[0]=='v'){
                myfile >> a >> b >> c;
                Point cur_point = {};
                cur_point.x=a;
                cur_point.y=b;
                cur_point.z=c;
                points.push_back(cur_point);
            }
        }
        myfile.close();
    }
    int t[3],temp;
    int bcount=0,ccount=0;
    myfile.open(filename.c_str());
    if (myfile.is_open()){
        while (myfile >> line){
            if(line.length()==1 && line[0]=='f'){
                string linemod;
                getline(myfile, linemod);
                int j,ans=0,tt=0,state=0;
                for(j=0;j<linemod.length();j++){
                    if(linemod[j]==' '){
                        ans=0;
                        state=1;
                    }
                    else if(linemod[j]=='/' && ans!=0 && state==1){
                        t[tt]=ans;
                        tt++;
                        state=0;
                    }
                    else if(linemod[j]!='/'){
                        ans=ans*10+linemod[j]-'0';
                    }
                }
                t[tt]=ans;
                Triangle my_triangle = {};
                my_triangle.p1=t[0]-1;
                my_triangle.p2=t[1]-1;
                my_triangle.p3=t[2]-1;
                vertex_buffer_data[bcount]=points[my_triangle.p1].x*x_scale;
                vertex_buffer_data[bcount+1]=points[my_triangle.p1].y*y_scale;
                vertex_buffer_data[bcount+2]=points[my_triangle.p1].z*z_scale;
                vertex_buffer_data[bcount+3]=points[my_triangle.p2].x*x_scale;
                vertex_buffer_data[bcount+4]=points[my_triangle.p2].y*y_scale;
                vertex_buffer_data[bcount+5]=points[my_triangle.p2].z*z_scale;
                vertex_buffer_data[bcount+6]=points[my_triangle.p3].x*x_scale;
                vertex_buffer_data[bcount+7]=points[my_triangle.p3].y*y_scale;
                vertex_buffer_data[bcount+8]=points[my_triangle.p3].z*z_scale;
                bcount+=9;
            }
            if(line.length()==1 && line[0]=='c'){
                float r1,g1,b1,r2,g2,b2,r3,g3,b3;
                myfile >> r1 >> g1 >> b1 >> r2 >> g2 >> b2 >> r3 >> g3 >> b3;
                color_buffer_data[ccount]=r1/255.0;
                color_buffer_data[ccount+1]=g1/255.0;
                color_buffer_data[ccount+2]=b1/255.0;
                color_buffer_data[ccount+3]=r2/255.0;
                color_buffer_data[ccount+4]=g2/255.0;
                color_buffer_data[ccount+5]=b2/255.0;
                color_buffer_data[ccount+6]=r3/255.0;
                color_buffer_data[ccount+7]=g3/255.0;
                color_buffer_data[ccount+8]=b3/255.0;
                ccount+=9;
            }
        }
        myfile.close();
    }
    VAO* myobject = create3DObject(GL_TRIANGLES, bcount/3, vertex_buffer_data, color_buffer_data, GL_FILL);
    object vishsprite = {};
    vishsprite.name = name;
    vishsprite.object = myobject;
    vishsprite.x=x_pos;
    vishsprite.y=y_pos;
    vishsprite.z=z_pos;
    vishsprite.status=1;
    vishsprite.x_scale=x_scale;
    vishsprite.y_scale=y_scale;
    vishsprite.z_scale=z_scale;

        objects[name]=vishsprite;
}


inline float get_dist(float x,float y,float x1,float y1){
    return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
}
bool check_collision(object a,object b){
    float dist = get_dist(a.x,a.y,b.x,b.y);

    float exp1= a.radius+b.radius;
    //out2(dist,exp1);
    if(dist < exp1) return true;
    return false;
}
bool check_collision_rect(object a,object b){
    if((abs(a.x - b.x )<a.radius_x+b.radius_x) && (abs(a.y-b.y)<a.radius_y+b.radius_y)) return true;
    return false;
}
void activatebridge(int number){
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(bridges[i][j]==number){
                stage["stagex"+to_string(i)+"y"+to_string(j)].status=((int)stage["stagex"+to_string(i)+"y"+to_string(j)].status+1)%2;
            }
        }
    }
}
int bridgestatus(int i,int j){
     return           stage["stagex"+to_string(i)+"y"+to_string(j)].status;

}

void checkfallblock(object sampleobject){
    out3(sampleobject.z_scale,sampleobject.x,sampleobject.y);
    out1(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]);
    if(sampleobject.z_scale==2){
        if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==0 ||platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==2 || sampleobject.x<0 || sampleobject.y<0 || (platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==3 && stage["stagex"+to_string((int)sampleobject.y/2)+"y"+to_string((int)sampleobject.y/2)].status==0)){
            cube["cube"].z_speed-=5;
            stage["stagex"+to_string((int)sampleobject.x/2)+"y"+to_string((int)sampleobject.y/2)].status=0;
        }
        if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==4 ){
            activatebridge(switches[(int)sampleobject.x/2][(int)sampleobject.y/2]);
        }
        if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==5 ){
            cube["cube"].z_speed-=5;
            nextlevelcheck=1;

        }
    }else if(sampleobject.z_scale==1){
        if(sampleobject.x_scale==2){

            if((bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2)==0 && bridgestatus((int)sampleobject.x/2-1,(int)sampleobject.y/2)==0)
               ||(bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2)==0 && bridgestatus((int)sampleobject.x/2+1,(int)sampleobject.y/2)==0)
               || (bridgestatus((int)sampleobject.x/2+1,(int)sampleobject.y/2)==0 && bridgestatus((int)sampleobject.x/2+2,(int)sampleobject.y/2+1)==0)
               || sampleobject.x<0 || sampleobject.y<0){
                cube["cube"].z_speed-=5;
            }
            if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==4 ){
                activatebridge(switches[(int)sampleobject.x/2][(int)sampleobject.y/2]);
            }
            if(platform[(int)sampleobject.x/2+1][(int)sampleobject.y/2]==4 ){
                activatebridge(switches[(int)sampleobject.x/2+1][(int)sampleobject.y/2]);
            }



        }else if(sampleobject.y_scale==2){
            if(
                    (bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2)==0 && bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2-1)==0)
                    ||  (
                                                     (bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2+2)==0 && bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2+1)==0
                                           )) ||
                            (sampleobject.x<0 || sampleobject.y<0)){
                cube["cube"].z_speed-=5;
            }
                if(bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2)==0 && bridgestatus((int)sampleobject.x/2,(int)sampleobject.y/2+1)==0){
                    cube["cube"].z_speed-=5;
                }
            if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2]==4 ){
                activatebridge(switches[(int)sampleobject.x/2][(int)sampleobject.y/2]);
            }
            if(platform[(int)sampleobject.x/2][(int)sampleobject.y/2+1]==4 ){
                activatebridge(switches[(int)sampleobject.x/2][(int)sampleobject.y/2+1]);
            }

        }
    }
}
float camera_rotation_angle = 90;

float rectangle_rotation = 0;
float triangle_rotation = 0;
void drawObject(object sampleobject)
{
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model

    Matrices.model = glm::mat4(1.0f);

    glm::mat4 ObjectTransform;
    glm::mat4 translateObject = glm::translate (glm::vec3(sampleobject.x, sampleobject.y, sampleobject.z)); // glTranslatef
    glm::mat4 rotateTriangle = glm::rotate((float)((sampleobject.angle)*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
    ObjectTransform=translateObject*rotateTriangle;


    Matrices.model *= ObjectTransform;
    MVP = VP * Matrices.model; // MVP = p * V * M

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    draw3DObject(sampleobject.object);
    draw3DObject(sampleobject.borderobject);
}
void drawObjectRotate(object sampleobject)
{
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model

    Matrices.model = glm::mat4(1.0f);

    glm::mat4 ObjectTransform;
    glm::mat4 translateObject = glm::translate (glm::vec3(sampleobject.x, sampleobject.y, sampleobject.z)); // glTranslatef
    glm::mat4 translateObject1 = glm::translate (glm::vec3(-sampleobject.presentrotationx,-sampleobject.presentrotationy,-sampleobject.presentrotationz)); // glTranslatef
    //glm::mat4 translateObject1 = glm::translate (glm::vec3(1,1,2)); // glTranslatef
    //out3(sampleobject.presentrotationx,sampleobject.presentrotationy,sampleobject.presentrotationz);

    glm::mat4 rotateTrianglex = glm::rotate((float)((sampleobject.angle_x)*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
    glm::mat4 rotateTriangley = glm::rotate((float)((sampleobject.angle_y)*M_PI/180.0f), glm::vec3(0,1,0));  // rotate about vector (1,0,0)
    glm::mat4 rotateTrianglez = glm::rotate((float)((sampleobject.angle_z)*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)

    glm::mat4 rotateTriangle = glm::rotate((float)((sampleobject.presentrotationangle)*M_PI/180.0f), sampleobject.presentrotationvector);  // rotate about vector (1,0,0)
    glm::mat4 translateObject2 = glm::translate (glm::vec3(sampleobject.presentrotationx, sampleobject.presentrotationy, sampleobject.presentrotationz)); // glTranslatef
    //glm::mat4 translateObject2 = glm::translate (glm::vec3(-1,-1,-2)); // glTranslatef

    ObjectTransform=translateObject*translateObject2*rotateTriangle*translateObject1*rotateTrianglex*rotateTriangley*rotateTrianglez;
    Matrices.model *= ObjectTransform;
    MVP = VP * Matrices.model; // MVP = p * V * M

    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    draw3DObject(sampleobject.object);
    draw3DObject(sampleobject.borderobject);
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw (GLFWwindow* window, int width, int height)
{

    reshapeWindow (window, width, height);
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    // Eye - Location of camera. Don't change unless you are sure!!
    if(!gamestart){
        //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 10*sin(camera_rotation_angle*M_PI/180.0f)+10*cos(camera_rotation_angle*M_PI/180.0f), 5*sin(camera_rotation_angle*M_PI/180.0f) );
        // Target - Where is the camera looking at.  Don't change unless you are sure!!
        //glm::vec3 target (0,0,0);
        // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
        //glm::vec3 up (0, 0, 1);
        // Compute Camera maptrix (view)
        // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
        //  Don't change unless you are sure!!
        static float c = 0;
        c++;
        //Matrices.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
        //Matrices.view = glm::lookAt(glm::vec3(0,-10,5), target, up); // Fixed camera for 2D (ortho) in XY plane
        // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
        //  Don't change unless you are sure!!
        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;    // MVP = Projection * View * Model
        static int fontScale = 0;

        glUseProgram(textureProgramID);

        // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
        // glPopMatrix ();
        Matrices.model = glm::mat4(1.0f);

        glm::mat4 translateRectangle = glm::translate(glm::vec3(2, 0, 0));        // glTranslatef
        glm::mat4 rotateRectangle = glm::rotate((float) (rectangle_rotation * M_PI / 180.0f),
                                                glm::vec3(0, 0, 1)); // rotate about vector (-1,1,1)
        Matrices.model *= (translateRectangle * rotateRectangle);
        MVP = VP * Matrices.model;

        // Copy MVP to texture shaders
        glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Set the texture sampler to access Texture0 memory
        glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

        // draw3DObject draws the VAO given to it using current MVP matrix
        draw3DTexturedObject(rectangle);

        glUseProgram(programID);

        float fontScaleValue = 0.7 ;
        glm::vec3 fontColor = glm::vec3(1,0,0);
        glUseProgram(fontProgramID);
        char score_str[30];
        sprintf(score_str,"BLUXORG");
        display_string(-2,0,score_str,fontScaleValue);
        sprintf(score_str,"INSTRUCTIONS:-");
        display_string(-2,-1,score_str,fontScaleValue);
        sprintf(score_str,"TO CONTINUE PRESS C");
        display_string(-2,1,score_str,fontScaleValue);


    }
    if(gamestart && !gameovercheck  && !nextlevelshow) {
        mouseright(window);
        char score_str[45];
        float fontScaleValue = 0.4 ;
        glm::vec3 fontColor = glm::vec3(0,0,0);
        glUseProgram(fontProgramID);

        sprintf(score_str,"Your present score %0.2lf noofsteps %d level %d",score,noofsteps,presentlevel);
        display_string(-2,2,score_str,fontScaleValue);

        if (view == 1) {
            glm::vec3 eye(cube["cube"].x, cube["cube"].y, cube["cube"].z + cube["cube"].z_scale + 2);
            glm::vec3 target(cube["cube"].x + 10 * cos(view_rotation_angle * M_PI / 180.0f),
                             cube["cube"].y + 10 * sin(view_rotation_angle * M_PI / 180.0f), cube["cube"].z);
            glm::vec3 up(0, 0, 1);
            Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
        } else if (view == 2) {
            glm::vec3 eye(5, 5, 20);
            glm::vec3 target(6, 6, 0);
            glm::vec3 up(0, 0, 1);
            Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
        } else if (view == 3) {
            glm::vec3 eye(-2, -2, 15);
            glm::vec3 target(5, 5, 0);
            glm::vec3 up(0, 0, 1);
            Matrices.view = glm::lookAt(eye, target, up); // Rotating Camera for 3D
        } else if (view == 4) {
            glm::vec3 eye(cube["cube"].x + 5 * cos(view_rotation_angle * M_PI / 180.0f),
                          cube["cube"].y - 5 * sin(view_rotation_angle * M_PI / 180.0f),
                          cube["cube"].z + cube["cube"].z_scale + 2);
            glm::vec3 target(cube["cube"].x, cube["cube"].y + 10 * sin(view_rotation_angle * M_PI / 180.0f),
                             cube["cube"].z);
            glm::vec3 up(0, 0, 1);
            Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
        } else if (view == 5) {
            glm::vec3 eye(5 + 10 * cos(view_rotation_angle * M_PI / 180.0f),
                          5 + 10 * sin(view_rotation_angle * M_PI / 180.0f), 20);
            glm::vec3 target(5, 5, 0);
            glm::vec3 up(0, 0, 1);
            Matrices.view = glm::lookAt(eye, target, up); // Fixed camera for 2D (ortho) in XY plane
        }
        //glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 10*sin(camera_rotation_angle*M_PI/180.0f)+10*cos(camera_rotation_angle*M_PI/180.0f), 5*sin(camera_rotation_angle*M_PI/180.0f) );
        // Target - Where is the camera looking at.  Don't change unless you are sure!!
        //glm::vec3 target (0,0,0);
        // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
        //glm::vec3 up (0, 0, 1);
        // Compute Camera maptrix (view)
        // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
        //  Don't change unless you are sure!!
        static float c = 0;
        c++;
        //Matrices.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
        //Matrices.view = glm::lookAt(glm::vec3(0,-10,5), target, up); // Fixed camera for 2D (ortho) in XY plane
        // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
        //  Don't change unless you are sure!!
        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;    // MVP = Projection * View * Model
        static int fontScale = 0;

        glUseProgram(textureProgramID);

        // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
        // glPopMatrix ();
        Matrices.model = glm::mat4(1.0f);

        glm::mat4 translateRectangle = glm::translate(glm::vec3(2, 0, 0));        // glTranslatef
        glm::mat4 rotateRectangle = glm::rotate((float) (rectangle_rotation * M_PI / 180.0f),
                                                glm::vec3(0, 0, 1)); // rotate about vector (-1,1,1)
        Matrices.model *= (translateRectangle * rotateRectangle);
        MVP = VP * Matrices.model;

        // Copy MVP to texture shaders
        glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Set the texture sampler to access Texture0 memory
        glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

        // draw3DObject draws the VAO given to it using current MVP matrix
        draw3DTexturedObject(rectangle);

        glUseProgram(programID);

        // Load identity to model matrix
        for (map<string, object>::iterator it = stage.begin(); it != stage.end(); it++) {
            string current = it->first; //The name of the current object
            if (stage[current].status)
                drawObject(stage[current]);
        }
        for (map<string, object>::iterator it = cube.begin(); it != cube.end(); it++) {
            string current = it->first; //The name of the current object
            if (cube[current].status)
                drawObjectRotate(cube[current]);
        }

        // draw3DObject draws the VAO given to it using current MVP matrix

        // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
        // glPopMatrix ();

        camera_rotation_angle++; // Simulating camera rotation
    };
    if(gameovercheck){
        static float c = 0;
        c++;
        //Matrices.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
        //Matrices.view = glm::lookAt(glm::vec3(0,-10,5), target, up); // Fixed camera for 2D (ortho) in XY plane
        // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
        //  Don't change unless you are sure!!
        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;    // MVP = Projection * View * Model
        static int fontScale = 0;

        glUseProgram(textureProgramID);

        // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
        // glPopMatrix ();
        Matrices.model = glm::mat4(1.0f);

        glm::mat4 translateRectangle = glm::translate(glm::vec3(2, 0, 0));        // glTranslatef
        glm::mat4 rotateRectangle = glm::rotate((float) (rectangle_rotation * M_PI / 180.0f),
                                                glm::vec3(0, 0, 1)); // rotate about vector (-1,1,1)
        Matrices.model *= (translateRectangle * rotateRectangle);
        MVP = VP * Matrices.model;

        // Copy MVP to texture shaders
        glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Set the texture sampler to access Texture0 memory
        glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

        // draw3DObject draws the VAO given to it using current MVP matrix
        draw3DTexturedObject(rectangle);

        glUseProgram(programID);

        float fontScaleValue = 0.7 ;
        glm::vec3 fontColor = glm::vec3(1,0,0);
        glUseProgram(fontProgramID);
        char score_str[30];
        if(presentlevel!=3){
            sprintf(score_str,"Game Over");
        }else{
            sprintf(score_str,"You Won");
        }


        display_string(-2,0,score_str,fontScaleValue);

        sprintf(score_str,"Your final score %lf",score);
        display_string(-2,-1,score_str,fontScaleValue);

    }
    if(nextlevelshow){
        static float c = 0;
        c++;
        //Matrices.view = glm::lookAt(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(sinf(c*M_PI/180.0),3*cosf(c*M_PI/180.0),0)); // Fixed camera for 2D (ortho) in XY plane
        //Matrices.view = glm::lookAt(glm::vec3(0,-10,5), target, up); // Fixed camera for 2D (ortho) in XY plane
        // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
        //  Don't change unless you are sure!!
        glm::mat4 VP = Matrices.projection * Matrices.view;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // For each model you render, since the MVP will be different (at least the M part)
        //  Don't change unless you are sure!!
        glm::mat4 MVP;    // MVP = Projection * View * Model
        static int fontScale = 0;

        glUseProgram(textureProgramID);

        // Pop matrix to undo transformations till last push matrix instead of recomputing model matrix
        // glPopMatrix ();
        Matrices.model = glm::mat4(1.0f);

        glm::mat4 translateRectangle = glm::translate(glm::vec3(2, 0, 0));        // glTranslatef
        glm::mat4 rotateRectangle = glm::rotate((float) (rectangle_rotation * M_PI / 180.0f),
                                                glm::vec3(0, 0, 1)); // rotate about vector (-1,1,1)
        Matrices.model *= (translateRectangle * rotateRectangle);
        MVP = VP * Matrices.model;

        // Copy MVP to texture shaders
        glUniformMatrix4fv(Matrices.TexMatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Set the texture sampler to access Texture0 memory
        glUniform1i(glGetUniformLocation(textureProgramID, "texSampler"), 0);

        // draw3DObject draws the VAO given to it using current MVP matrix
        draw3DTexturedObject(rectangle);

        glUseProgram(programID);

        float fontScaleValue = 0.7 ;
        glm::vec3 fontColor = glm::vec3(1,0,0);
        glUseProgram(fontProgramID);
        char score_str[30];
        sprintf(score_str,"Congrats go to next level");
        display_string(-2,0,score_str,fontScaleValue);

        sprintf(score_str,"Your final score %lf",score);
        display_string(-2,-1,score_str,fontScaleValue);

    }

}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
//        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
//        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
    glfwSetScrollCallback(window, mousescroll);

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    // load an image file directly as a new OpenGL texture
    // GLuint texID = SOIL_load_OGL_texture ("beach.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_TEXTURE_REPEATS); // Buggy for OpenGL3
    GLuint textureID = createTexture("background.png");
    // check for an error during the load process
    if(textureID == 0 )
        cout << "SOIL loading error: '" << SOIL_last_result() << "'" << endl;

    // Create and compile our GLSL program from the texture shaders
    textureProgramID = LoadShaders( "TextureRender.vert", "TextureRender.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.TexMatrixID = glGetUniformLocation(textureProgramID, "MVP");
    createBackground(textureID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(platform[i][j]==1){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,grey,grey,grey,"stage",1);
            }else if(platform[i][j]==2){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,blue,blue,blue,"stage",2);
            }
            else if(platform[i][j]==3){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,gold,gold,gold,"stage",0);
            }
            else if(platform[i][j]==4){
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,red,red,red,"stage",3);
            }
            else{
                generate_cube("stagex"+to_string(i)+"y"+to_string(j),2*i,2*j,0,1,1,0.1,grey,grey,grey,"stage",0);
            }
        }
    }

    reshapeWindow (window, width, height);
    generate_cube("cube",0,0,2.1,1,1,2,lightgreen,lightgreen,lightgreen,"cube",1);
    //generate_cube("cube",-1,-1,0.1,0.1,0.1,0.1,black,black,black,"cube");
    // Background color of the scene
    glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    const char* fontfile = "/usr/share/fonts/truetype/lato/Lato-Italic.ttf";
    GL3Font.font = new FTExtrudeFont(fontfile); // 3D extrude style rendering

    if(GL3Font.font->Error())
    {
        cout << "Error: Could not load font `" << fontfile << "'" << endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Create and compile our GLSL program from the font shaders
    fontProgramID = LoadShaders( "fontrender.vert", "fontrender.frag" );
    GLint fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform;
    fontVertexCoordAttrib = glGetAttribLocation(fontProgramID, "vertexPosition");
    fontVertexNormalAttrib = glGetAttribLocation(fontProgramID, "vertexNormal");
    fontVertexOffsetUniform = glGetUniformLocation(fontProgramID, "pen");
    GL3Font.fontMatrixID = glGetUniformLocation(fontProgramID, "MVP");
    GL3Font.fontColorID = glGetUniformLocation(fontProgramID, "fontColor");

    GL3Font.font->ShaderLocations(fontVertexCoordAttrib, fontVertexNormalAttrib, fontVertexOffsetUniform);
    GL3Font.font->FaceSize(1);
    GL3Font.font->Depth(0);
    GL3Font.font->Outset(0, 0);
    GL3Font.font->CharMap(ft_encoding_unicode);
    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}
int randomgenerator(int min,int max){
    return rand()%(max-min + 1) + min;
}
int main (int argc, char** argv)
{
    int width = 600;
    int height = 600;

    GLFWwindow* window = initGLFW(width, height);
    thread(play_audio,"audio/background.mp3").detach();
    initGL (window, width, height);
    float current_time,last_update2_time;
    int count=0;
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime(); // Time in seconds
        // OpenGL Draw commands
        draw(window,width,height);
        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();
        if ((current_time - last_update2_time) >= 0.01) { // atleast 0.5s elapsed since last frame
            // do something every 0.5 seconds ..
            count++;
            if(count==500){
                count=0;
                nextlevelshow=0;
            }
            double x=cube["cube"].x;
            double y=cube["cube"].y;
            double z=cube["cube"].z;
            double x_scale=cube["cube"].x_scale;
            double y_scale=cube["cube"].y_scale;
            double z_scale=cube["cube"].z_scale;
            double angle_x=cube["cube"].angle_x;
            double angle_y=cube["cube"].angle_y;
            double angle_z=cube["cube"].angle_z;
            string lastrotate=cube["cube"].lastrotate;
            last_update2_time = current_time;
            if(cube["cube"].z_speed){
                cube["cube"].z+=cube["cube"].z_speed/5;
                if(cube["cube"].z <= -50){
                    if(nextlevelcheck==1){
                        nextlevelshow=1;
                        nextlevel();
                    }

                    else
                        gameover();
                }

                out1(cube["cube"].presentrotationangle);
            }
            if(cube["cube"].presentrotationangle < 90 && cube["cube"].buttonpressed)
                cube["cube"].presentrotationangle+=5;
            if(cube["cube"].presentrotationangle == 90 && cube["cube"].buttonpressed){

                if(cube["cube"].lastrotate == "left"){
                    x=cube["cube"].x-cube["cube"].x_scale-cube["cube"].z_scale;
                    z=cube["cube"].x_scale;
                    x_scale=cube["cube"].z_scale;
                    z_scale=cube["cube"].x_scale;


                }
                else if(cube["cube"].lastrotate == "right"){
                    x=cube["cube"].x+cube["cube"].x_scale+cube["cube"].z_scale;
                    z=cube["cube"].x_scale;
                    x_scale=cube["cube"].z_scale;
                    z_scale=cube["cube"].x_scale;

                }
                else if(cube["cube"].lastrotate == "up"){
                    y=cube["cube"].y+cube["cube"].z_scale+cube["cube"].y_scale;
                    z=cube["cube"].y_scale;
                    y_scale=cube["cube"].z_scale;
                    z_scale=cube["cube"].y_scale;

                }
                else if(cube["cube"].lastrotate == "down"){
                    y=cube["cube"].y-cube["cube"].z_scale-cube["cube"].y_scale;
                    z=cube["cube"].y_scale;
                    y_scale=cube["cube"].z_scale;
                    z_scale=cube["cube"].y_scale;

                }
                noofsteps+=1;
                out1(noofsteps);
                std::map<string,object>::iterator it;


                generate_cube("cube",x,y,z,x_scale,y_scale,z_scale,lightgreen,lightgreen,lightgreen,"cube",1);

                if(lastrotate == "left"){
                    //cube["cube"].angle_y=angle_y-90;

                }
                else if(lastrotate == "right"){
                    //cube["cube"].angle_y=angle_y+90;
                }
                else if(lastrotate == "up"){

                    //cube["cube"].angle_x=angle_x-90;
                }
                else if(lastrotate == "down"){
                    //cube["cube"].angle_x=angle_x+90;
                }
                checkfallblock(cube["cube"]);
                cube["cube"].presentrotationangle=0;
                cube["cube"].buttonpressed=0;
                out1(cube["cube"].z_speed);
                out3(cube["cube"].angle_x,cube["cube"].angle_y,cube["cube"].angle_z);
                out3(cube["cube"].x_scale,cube["cube"].y_scale,cube["cube"].z_scale);
                out3(cube["cube"].x,cube["cube"].y,cube["cube"].z);


            }

        }
        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
    }

    glfwTerminate();
//    exit(EXIT_SUCCESS);
}
