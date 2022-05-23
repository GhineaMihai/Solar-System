#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>

GLuint texture;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
std::vector<float> calotaSferica(int nMeridiane, int nParalele, float R, float a);
std::vector <float> TexCoords = {};

// configurari
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

const unsigned int noParalele = 40;
const unsigned int noMeridiane = 40;
const float razaSferei = 2;
const float razaCalotei = 0.9;

const unsigned int noParalele1 = 40;
const unsigned int noMeridiane1 = 40;
const float razaSferei1 = 0.4;
const float razaCalotei1 = 0.9;

const unsigned int noParalele2 = 40;
const unsigned int noMeridiane2 = 40;
const float razaSferei2 = 0.1;
const float razaCalotei2 = 0.9;

const unsigned int noParalele3 = 40;
const unsigned int noMeridiane3 = 40;
const float razaSferei3 = 0.25;
const float razaCalotei3 = 0.9;

const unsigned int noParalele4 = 40;
const unsigned int noMeridiane4 = 40;
const float razaSferei4 = 0.4;
const float razaCalotei4 = 0.9;

const unsigned int noParalele5 = 40;
const unsigned int noMeridiane5 = 40;
const float razaSferei5 = 0.3;
const float razaCalotei5 = 0.9;

const unsigned int noParalele6 = 40;
const unsigned int noMeridiane6 = 40;
const float razaSferei6 = 0.9;
const float razaCalotei6 = 0.9;

const unsigned int noParalele7 = 40;
const unsigned int noMeridiane7 = 40;
const float razaSferei7 = 0.7;
const float razaCalotei7 = 0.9;

const unsigned int noParalele8 = 40;
const unsigned int noMeridiane8 = 40;
const float razaSferei8 = 0.4;
const float razaCalotei8 = 0.9;

const unsigned int noParalele9 = 40;
const unsigned int noMeridiane9 = 40;
const float razaSferei9 = 0.35;
const float razaCalotei9 = 0.9;

GLuint textura(char fisier[]){


    GLuint textura;
    int width, height, nrChannels;

    // cream un obiect de tip textura
    glGenTextures(1, &textura);

    // il legam de contextul curent
    glBindTexture(GL_TEXTURE_2D, textura);

    // definim parametrii ei
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // definim parametrii de filtrare
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps

    stbi_set_flip_vertically_on_load(true); // facem un flip pe axa Oy

    // incarcam datele din fisierul imagine
    unsigned char *data = stbi_load(fisier, &width, &height, &nrChannels, 0);
    if (data)
    {
        //transferam datele incarcate in obiectul textura
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        // generam mipmap -uri pentru aceasta textura
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    //eliberam datele incarcate din fisierul imagine
    stbi_image_free(data);

    //returnam aliasul texturii create
    return textura;

}


std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath;
        std::cerr << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}




int main()
{
    // glfw: initializare si configurare

    glfwInit();
    // precizam versiunea 3.3 de openGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw cream fereastra

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Calota Sferica", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // facem ca aceasta fereastra sa fie contextul curent

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: incarcam referintele la functiile OpenGL

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // incarcam si compilam shaderele:

    // vertex shader

    GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
    if( 0 == vertexShader )
    {
        std::cout << "Error creating vertex shader." << std::endl;
        exit(1);
    }

    std::string shaderCode = readFile("calota.vert");
    const char *codeArray = shaderCode.c_str();
    glShaderSource( vertexShader, 1, &codeArray, NULL );

    glCompileShader(vertexShader);

    // verficam daca s-a reusit compilarea codului

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader (repetam aceleasi operatii)

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    shaderCode = readFile("basic.frag");
    codeArray = shaderCode.c_str();
    glShaderSource( fragmentShader, 1, &codeArray, NULL );


    glCompileShader(fragmentShader);

    // se verifica compilarea codului

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // se verifica procesul de link

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // initializam punctele de pe calota sferica
    std::vector<float> vertices;
    std::vector<float> vertices1;
    std::vector<float> vertices2;
    std::vector<float> vertices3;
    std::vector<float> vertices4;
    std::vector<float> vertices5;
    std::vector<float> vertices6;
    std::vector<float> vertices7;
    std::vector<float> vertices8;
    std::vector<float> vertices9;

    vertices = calotaSferica(noMeridiane, noParalele, razaSferei, razaCalotei);
    vertices1 = calotaSferica(noMeridiane1, noParalele1, razaSferei1, razaCalotei1);
    vertices2 = calotaSferica(noMeridiane2, noParalele2, razaSferei2, razaCalotei2);
    vertices3 = calotaSferica(noMeridiane3, noParalele3, razaSferei3, razaCalotei3);
    vertices4 = calotaSferica(noMeridiane4, noParalele4, razaSferei4, razaCalotei4);
    vertices5 = calotaSferica(noMeridiane5, noParalele5, razaSferei5, razaCalotei5);
    vertices6 = calotaSferica(noMeridiane6, noParalele6, razaSferei6, razaCalotei6);
    vertices7 = calotaSferica(noMeridiane7, noParalele7, razaSferei7, razaCalotei7);
    vertices8 = calotaSferica(noMeridiane8, noParalele8, razaSferei8, razaCalotei8);
    vertices9 = calotaSferica(noMeridiane9, noParalele9, razaSferei9, razaCalotei9);

    unsigned int VBO, VAO;
    unsigned int VBO1, VAO1;
    unsigned int VBO2, VAO2;
    unsigned int VBO3, VAO3;
    unsigned int VBO4, VAO4;
    unsigned int VBO5, VAO5;
    unsigned int VBO6, VAO6;
    unsigned int VBO7, VAO7;
    unsigned int VBO8, VAO8;
    unsigned int VBO9, VAO9;

    // se face bind a obiectului Vertex Array, apoi se face bind si se stabilesc
    // vertex buffer(ele), si apoi se configureaza vertex attributes.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),  &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(float),  &vertices1.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(float),  &vertices2.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);
    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(float),  &vertices3.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO4);
    glBindVertexArray(VAO4);
    glGenBuffers(1, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, vertices4.size() * sizeof(float),  &vertices4.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO5);
    glBindVertexArray(VAO5);
    glGenBuffers(1, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, vertices5.size() * sizeof(float),  &vertices5.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO6);
    glBindVertexArray(VAO6);
    glGenBuffers(1, &VBO6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO6);
    glBufferData(GL_ARRAY_BUFFER, vertices6.size() * sizeof(float),  &vertices6.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO7);
    glBindVertexArray(VAO7);
    glGenBuffers(1, &VBO7);
    glBindBuffer(GL_ARRAY_BUFFER, VBO7);
    glBufferData(GL_ARRAY_BUFFER, vertices7.size() * sizeof(float),  &vertices7.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO8);
    glBindVertexArray(VAO8);
    glGenBuffers(1, &VBO8);
    glBindBuffer(GL_ARRAY_BUFFER, VBO8);
    glBufferData(GL_ARRAY_BUFFER, vertices8.size() * sizeof(float),  &vertices8.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO9);
    glBindVertexArray(VAO9);
    glGenBuffers(1, &VBO9);
    glBindBuffer(GL_ARRAY_BUFFER, VBO9);
    glBufferData(GL_ARRAY_BUFFER, vertices9.size() * sizeof(float),  &vertices9.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // se face unbind pentru VAO
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);
    glBindVertexArray(0);

    // ciclu de desenare -- render loop

    //definim cu cat rotim obiectul
    GLuint texture1 = textura("resurse/Solarsystemscope_texture_2k_sun.jpg");
    GLuint texture2 = textura("resurse/earth.bmp");
    GLuint texture3 = textura("resurse/Solarsystemscope_texture_4k_haumea_fictional.jpg");
    GLuint texture4 = textura("resurse/dcklc0u-feeae0dc-f164-436a-8e75-5c9a7f3e3c71.jpg");
    GLuint texture5 = textura("resurse/Dh_venus_surface_texture.jpg");
    GLuint texture6 = textura("resurse/Mars_Map.jpg");
    GLuint texture7 = textura("resurse/20181107_hlsp_opal_hst_wfc3-uvis_jupiter-2017a_color_globalmap2.jpg");
    GLuint texture8 = textura("resurse/Solarsystemscope_texture_8k_saturn.jpg");
    GLuint texture9 = textura("resurse/Solarsystemscope_texture_2k_uranus.jpg");
    GLuint texture10 = textura("resurse/Solarsystemscope_texture_2k_neptune.jpg");

    float step=1, angle=0;
    float step1=0.01, angle1=0;
    float step2=0.013, angle2=0;
    float step3=0.02, angle3=0;
    float step4=0.015, angle4=0;
    float step5=0.009, angle5=0;
    float step6=0.008, angle6=0;
    float step7=0.007, angle7=0;
    float step8=0.006, angle8=0;
    float step9=0.005, angle9=0;
    double alpha=0, alpha1=0, alpha2=0, alpha3=0, alpha4=0, alpha5=0, alpha6=0, alpha7=0, alpha8=0, alpha9=0;
    double x = 0, y =0;
    double x1 = 0, y1 = 0;
    double x2 = 0, y2 = 0;
    double x3 = 0, y3 = 0;
    double x4 = 0, y4 = 0;
    double x5 = 0, y5 = 0;
    double x6 = 0, y6 = 0;
    double x7 = 0, y7 = 0;
    double x8 = 0, y8 = 0;
    double x9 = 0, y9 = 0;
    float r1 = 7, r2 = 1, r3 = 3, r4 = 5, r5 = 9, r6 = 11, r7 = 14, r8 = 16, r9 = 18;


    while (!glfwWindowShouldClose(window))
    {
        // input

        processInput(window);

        // render

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        // specificam programul ce trebuie folosit
        glUseProgram(shaderProgram);

        // cream transformarile care ne definesc modul in care privim obiectul
        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        glm::mat4 model1        = glm::mat4(1.0f);
        glm::mat4 projection1   = glm::mat4(1.0f);
        glm::mat4 model2        = glm::mat4(1.0f);
        glm::mat4 projection2   = glm::mat4(1.0f);
        glm::mat4 model3        = glm::mat4(1.0f);
        glm::mat4 projection3   = glm::mat4(1.0f);
        glm::mat4 model4        = glm::mat4(1.0f);
        glm::mat4 projection4   = glm::mat4(1.0f);
        glm::mat4 model5        = glm::mat4(1.0f);
        glm::mat4 projection5   = glm::mat4(1.0f);
        glm::mat4 model6        = glm::mat4(1.0f);
        glm::mat4 projection6   = glm::mat4(1.0f);
        glm::mat4 model7        = glm::mat4(1.0f);
        glm::mat4 projection7   = glm::mat4(1.0f);
        glm::mat4 model8        = glm::mat4(1.0f);
        glm::mat4 projection8   = glm::mat4(1.0f);
        glm::mat4 model9        = glm::mat4(1.0f);
        glm::mat4 projection9   = glm::mat4(1.0f);


        angle = angle + step;
        alpha = angle;
        if (angle > 360)
            angle = angle - 360;

        angle1 = angle1 + step1;
        alpha1 = angle1;
        if (angle1 > 360)
            angle1 = angle1 - 360;
        x1 = r1*cos(alpha1);
        y1 = r1*sin(alpha1);

        angle2 = angle2 + step2;
        alpha2 = angle2;
        if (angle2 > 360)
            angle2 = angle2 - 360;
        x2 = r2*cos(alpha2)+x1;
        y2 = r2*sin(alpha2)+y1;

        angle3 = angle3 + step3;
        alpha3 = angle3;
        if (angle3 > 360)
            angle3 = angle3 - 360;
        x3 = r3*cos(alpha3);
        y3 = r3*sin(alpha3);

        angle4 = angle4 + step4;
        alpha4 = angle4;
        if (angle4 > 360)
            angle4 = angle4 - 360;
        x4 = r4*cos(alpha4);
        y4 = r4*sin(alpha4);

        angle5 = angle5 + step5;
        alpha5 = angle5;
        if (angle5 > 360)
            angle5 = angle5 - 360;
        x5 = r5*cos(alpha5);
        y5 = r5*sin(alpha5);

        angle6 = angle6 + step6;
        alpha6 = angle6;
        if (angle6 > 360)
            angle6 = angle6 - 360;
        x6 = r6*cos(alpha6);
        y6 = r6*sin(alpha6);

        angle7 = angle7 + step7;
        alpha7 = angle7;
        if (angle7 > 360)
            angle7 = angle7 - 360;
        x7 = r7*cos(alpha7);
        y7 = r7*sin(alpha7);

        angle8 = angle8 + step8;
        alpha8 = angle8;
        if (angle8 > 360)
            angle8 = angle8 - 360;
        x8 = r8*cos(alpha8);
        y8 = r8*sin(alpha8);

        angle9 = angle9 + step9;
        alpha9 = angle9;
        if (angle9 > 360)
            angle9 = angle9 - 360;
        x9 = r9*cos(alpha9);
        y9 = r9*sin(alpha9);

        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -45.0f));

{
        model = glm::translate(model, glm::vec3(x,y,0));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // obtinem locatiile variabilelor uniforms in program
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        // transmitem valorile lor catre shadere (2 metode)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        // specificam modul in care vrem sa desenam -- aici din spate in fata, si doar contur
        // implicit pune fete, dar cum nu avem lumini si umbre deocamdata cubul nu va arata bine
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices.size()/5);// vertices.size()/3-12, 12);//
}

{
        model1 = glm::translate(model1, glm::vec3(x1,y1,0));
        model1 = glm::rotate(model1, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection1 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO1);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices1.size()/5);
}

{
        model2 = glm::translate(model2, glm::vec3(x2,y2,0));
        model2 = glm::rotate(model2, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model2 = glm::rotate(model2, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection2 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO2);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture3);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices2.size()/5);
}

{
        model3 = glm::translate(model3, glm::vec3(x3,y3,0));
        model3 = glm::rotate(model3, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model3 = glm::rotate(model3, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection3 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO3);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture4);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices3.size()/5);
}

{
        model4 = glm::translate(model4, glm::vec3(x4,y4,0));
        model4 = glm::rotate(model4, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model4 = glm::rotate(model4, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection4 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO4);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture5);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices4.size()/5);
}

{
        model5 = glm::translate(model5, glm::vec3(x5,y5,0));
        model5 = glm::rotate(model5, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model5 = glm::rotate(model5, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection5 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO5);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture6);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices5.size()/5);
}

{
        model6 = glm::translate(model6, glm::vec3(x6,y6,0));
        model6 = glm::rotate(model6, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model6 = glm::rotate(model6, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection6 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO6);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture7);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices6.size()/5);
}

{
        model7 = glm::translate(model7, glm::vec3(x7,y7,0));
        model7 = glm::rotate(model7, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model7 = glm::rotate(model7, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection7 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO7);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture8);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices7.size()/5);
}

{
        model8 = glm::translate(model8, glm::vec3(x8,y8,0));
        model8 = glm::rotate(model8, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model8 = glm::rotate(model8, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection8 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO8);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture9);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices8.size()/5);
}

{
        model9 = glm::translate(model9, glm::vec3(x9,y9,0));
        model9 = glm::rotate(model9, glm::radians(-45.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        model9 = glm::rotate(model9, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        projection9 = glm::perspective(glm::radians(-45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc  = glGetUniformLocation(shaderProgram, "view");
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindVertexArray(VAO9);

        glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
        glBindTexture(GL_TEXTURE_2D, texture10);

        glDrawArrays(GL_TRIANGLE_STRIP, 0 , vertices9.size()/5);
}
        // glfw: se inverseaza zonele tamponm si se trateaza evenimentele IO

        glfwSwapBuffers(window);
        Sleep(10);
        glfwPollEvents();
    }

    // optional: se elibereaza resursele alocate

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(shaderProgram);

    // glfw: se termina procesul eliberand toate resursele alocate de GLFW

    glfwTerminate();
    return 0;
}

// se proceseaza inputurile de la utilizator

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ce se intampla la o redimensionalizare a ferestrei

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ne asiguram ca viewportul este in concordanta cu noile dimensiuni

    glViewport(0, 0, width, height);
}

std::vector <float> calotaSferica(int nMeridiane, int nParalele, float R, float a){

    std::vector <float> vertex = {};
    std::cout<< 'fgds'<<vertex.size()<<std::endl;
    double theta0, theta1, phi, x, y;
    int i,j;
    for(i = 1; i <= nParalele ; i++){

        theta0 = M_PI * ( (double) (i - 1) / nParalele);
        theta1 = M_PI * ( (double) i / nParalele);

        for(j = 0; j <= nMeridiane; j++) {
            phi = 2 * M_PI * (double) (j - 1) / nMeridiane;

            vertex.push_back(R * cos(phi) * sin(theta0));
            vertex.push_back(R * sin(phi) * sin(theta0));
            vertex.push_back(R * cos(theta0));

            vertex.push_back(((i-1)*1.0)/(nParalele));
            vertex.push_back(((j)*1.0)/(nMeridiane));

            vertex.push_back(R * cos(phi) * sin(theta1));
            vertex.push_back(R * sin(phi) * sin(theta1));
            vertex.push_back(R * cos(theta1));

            vertex.push_back(((i)*1.0)/(nParalele));
            vertex.push_back(((j)*1.0)/(nMeridiane));
            }
    }

    return vertex;

}

