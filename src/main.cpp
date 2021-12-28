#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Client/Rendering/Buffers.h"
#include "Client/Rendering/Camera.h"
#include "Client/Rendering/Shader.h"
#include "Client/Rendering/VertexArray.h"
#include "Client/Voxel/ChunkManager.h"
#include <glm/gtx/string_cast.hpp>
#include <FastNoise/FastNoise.h>
#define TRACY_ENABLE
#include "Tracy/Tracy.hpp"
#include "glm/gtx/hash.hpp"

void message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
{
    auto const src_str = [source]() {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
        }
    }();

    auto const type_str = [type]() {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
        }
    }();

    auto const severity_str = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW: return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
        }
    }();
    std::cout << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;


bool block[16] = {};

bool testVoxel(const int& pos) {
    if(-1 < pos && pos < 16) {

        std::cout << "yes" << std::endl;
    } else {
        std::cout << "no" << std::endl;
    }
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenCraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, nullptr);

    const std::vector<GLfloat> m_clear_color{ 0.2f, 0.2f, 0.2f, 1.0f };

    std::string vertexSource = R"(
        #version 450 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 color;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;

        void main()
        {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        color = aColor;
        }
    )";

    std::string fragmentSource = R"(
        #version 450 core
        out vec4 FragColor;

        in vec3 color;

        void main()
        {
        	FragColor = vec4(color, 1.0f);
        }
    )";

    //ShaderClass shader(vertexSource, fragmentSource);
    Shader shader(vertexSource, fragmentSource);

    /*uint32_t fbo, tex, depthTex;

    glCreateTextures(GL_TEXTURE_2D, 1, &tex);

    glTextureParameteri(tex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(tex, 1, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT);


    glCreateTextures(GL_TEXTURE_2D, 1, &depthTex);

    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTex, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(depthTex, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(depthTex, 1, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    //glTextureSubImage2D(tex, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glCreateFramebuffers(1, &fbo);

    glNamedFramebufferTexture(fbo, GL_COLOR_ATTACHMENT0, tex, 0);
    //glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, depthTex, 0);

    if(glCheckNamedFramebufferStatus(fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "framebuffer error\n";*/

    std::vector<float> ver = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
    };

    std::vector<uint32_t> ind = {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
    };

    /*auto fnSimplex = FastNoise::New<FastNoise::Simplex>();
    auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();
    fnFractal->SetSource( fnSimplex );
    fnFractal->SetOctaveCount( 5 );*/

    auto OpenSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
    auto FractalFBm = FastNoise::New<FastNoise::FractalFBm>();
    FractalFBm->SetSource(OpenSimplex);
    FractalFBm->SetGain(0.280f);
    FractalFBm->SetOctaveCount(4);
    FractalFBm->SetLacunarity(2.0f);

    auto DomainScale = FastNoise::New<FastNoise::DomainScale>();
    DomainScale->SetSource(FractalFBm);
    DomainScale->SetScale(0.76f);

    auto PosationOutput = FastNoise::New<FastNoise::PositionOutput>();
    PosationOutput->Set<FastNoise::Dim::Y>(3.72f);

    auto add = FastNoise::New<FastNoise::Add>();
    add->SetLHS(DomainScale);
    add->SetRHS(PosationOutput);

    ChunkManager manager(add);

    for(int x = -1; x < 0; x++) {
        for(int y = -1; y < 0; y++) {
            for(int z = -1; z < 0; z++) {
                manager.AddChunk({x,y,z});
            }
        }
    }
    //manager.AddChunk({1,1,1});
    /*std::vector<float> noiseOutput(16 * 16 * 16);
    add->GenUniformGrid3D(noiseOutput.data(), 16 * 1, 16 * 1, 16 * 1, 16, 16, 16, 0.05f, 1337);


    Chunk chunk({1,1,1}, noiseOutput);*/



    Camera camera(SCR_WIDTH, SCR_HEIGHT, {18,18, 18});

    shader.SetMat4("projection", camera.GetProjection());

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /*glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearNamedFramebufferfv(fbo, GL_COLOR, 0, m_clear_color.data());*/
        glClearBufferfv(GL_COLOR, 0, m_clear_color.data());
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0, 0);
        camera.Inputs(window);
        shader.Bind();
        //shader.SetFloat3("color", {0.9f, 0.9f, 0.9f});
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, {0.0f, 0.0f, 0.0f});
        shader.SetMat4("view", camera.GetView());
        manager.Render(shader);
        //chunk.Render(shader);
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBlitNamedFramebuffer(fbo, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    ZoneScoped;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
