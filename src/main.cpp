#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Client/Rendering/Buffers.h"
#include "Client/Rendering/Camera.h"
#include "Client/Rendering/Shader.h"
#include "Client/Rendering/util.h"
#include "Client/Rendering/TextureArray.h"
#include "Client/Voxel/ChunkManager.h"
#include "Client/Voxel/Cube.h"
#include <glm/gtx/string_cast.hpp>
#include <FastNoise/FastNoise.h>
#define TRACY_ENABLE
#include "Tracy/Tracy.hpp"
#define STB_IMAGE_IMPLEMENTATION

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;




int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenCraft", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(message_callback, nullptr);
    const std::vector<GLfloat> m_clear_color{ 0.2f, 0.2f, 0.2f, 1.0f };

    uint32_t gBuffer, gPosition, gNormal, gAlbedoSpec, gDepth;

    glCreateTextures(GL_TEXTURE_2D, 1, &gPosition);
    glTextureParameteri(gPosition, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gPosition, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(gPosition, 1, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT);

    glCreateTextures(GL_TEXTURE_2D, 1, &gNormal);
    glTextureParameteri(gNormal, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gNormal, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(gNormal, 1, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT);

    glCreateTextures(GL_TEXTURE_2D, 1, &gAlbedoSpec);
    glTextureParameteri(gAlbedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(gAlbedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureStorage2D(gAlbedoSpec, 1, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT);

    glCreateTextures(GL_TEXTURE_2D, 1, &gDepth);
    glTextureParameteri(gDepth, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(gDepth, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureStorage2D(gDepth, 1, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);

    glCreateFramebuffers(1, &gBuffer);

    glNamedFramebufferTexture(gBuffer, GL_COLOR_ATTACHMENT0, gPosition, 0);
    glNamedFramebufferTexture(gBuffer, GL_COLOR_ATTACHMENT1, gNormal, 0);
    glNamedFramebufferTexture(gBuffer, GL_COLOR_ATTACHMENT2, gAlbedoSpec, 0);
    glNamedFramebufferTexture(gBuffer, GL_DEPTH_ATTACHMENT, gDepth, 0);

    if(glCheckNamedFramebufferStatus(gBuffer, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "framebuffer error\n";

    static const GLenum draw_buffers[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glNamedFramebufferDrawBuffers(gBuffer, 1, draw_buffers);

    //ShaderClass shader(vertexSource, fragmentSource);

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


    Cube cube;

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

    for(int x = -4; x < 5; x++) {
        for(int y = -1; y < 1; y++) {
            for(int z = -4; z < 5; z++) {
                manager.AddChunk({x,y,z});
            }
        }
    }

    TextureArray array;
    array.AddTexture("res/Textures/1.jpg");
    array.AddTexture("res/Textures/2.jpg");
    array.AddTexture("res/Textures/3.jpg");
    array.AddTexture("res/Textures/4.jpg");

    Camera camera(SCR_WIDTH, SCR_HEIGHT, {18,18, 18});

    Shader sky("res/Shader/Atmo.vert", "res/Shader/Atmo.frag");
    sky.Unbind();
    Shader shader("res/Shader/Chunk.vert", "res/Shader/Chunk.frag");
    Shader cubeShader("res/Shader/Cube.vert", "res/Shader/Cube.frag");

    glm::vec3 lightPos = {0.0f, 5.0f, 0.0f};

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        /*glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearNamedFramebufferfv(fbo, GL_COLOR, 0, m_clear_color.data());*/
        glClearBufferfv(GL_COLOR, 0, m_clear_color.data());
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0, 0);
        camera.Inputs(window);

        sky.Bind();
        sky.SetMat4("projection", camera.GetProjection());
        sky.SetMat4("view", camera.GetView());
        glm::mat4 model = glm::translate(glm::mat4(1.0f), camera.GetPosition());
        model = glm::scale(glm::mat4(1.0f), {500.0f, 500.0f, 500.0f});
        sky.SetMat4("model", model);
        cube.Draw();
        sky.Unbind();
        shader.Bind();
        shader.SetMat4("projection", camera.GetProjection());
        glActiveTexture(GL_TEXTURE0);
        array.Bind();
        //shader.SetInt("material.textures", array.GetID());
        shader.SetInt("texture", array.GetID());
        shader.SetFloat3("light.position", lightPos);
        shader.SetFloat3("viewPos", camera.GetPosition());

        // light properties
        shader.SetFloat3("light.ambient", {0.25f, 0.25f, 0.25f});
        shader.SetFloat3("light.diffuse", {0.25f, 0.25f, 0.25f});
        shader.SetFloat3("light.specular", {1.0f, 1.0f, 1.0f});
        shader.SetFloat("light.constant", 1.0f);
        shader.SetFloat("light.linear", 0.09);
        shader.SetFloat("light.quadratic", 0.032);

        // material properties
        //shader.SetInt("material.array", array.GetID());
        shader.SetFloat("material.shininess", 64.0f);

        model = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        shader.SetMat4("view", camera.GetView());
        manager.Render(shader);
        shader.Unbind();
        cubeShader.Bind();
        cubeShader.SetMat4("projection", camera.GetProjection());
        cubeShader.SetMat4("view", camera.GetView());
        glm::mat4 bruh = glm::translate(glm::mat4(1.0f), lightPos);
        //bruh = glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
        cubeShader.SetMat4("model", bruh);
        cube.Draw();

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