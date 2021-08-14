#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
// #include "ViewMat.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const int windown_width = 512;
const int windown_height = 512;
int view_width;
int view_height;
// static ViewMat *viewMat;
static ModelPose pose;

int main()
{
    Image image("../res/persp_504x378.jpg");
    if (image.isError())
        return -1;

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(windown_width, windown_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwGetFramebufferSize(window, &view_width, &view_height); //获取渲染缓冲区大小

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    PhotoRenderer render;
    if (render.init(&image) != 0) {
        glfwTerminate();
        return -1;
    }
    // viewMat = new ViewMat(view_width, view_height);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glViewport(0, 0, view_width, view_height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        render.draw(pose);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    // delete viewMat;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        pose.left();
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        pose.right();
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pose.up();
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pose.down();
    if(glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        pose.zoom_in();
    if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        pose.zoom_out();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    std::cout << "window size changed: " << width << "x" << height << std::endl; 
    view_width = width;
    view_height = height;
    glViewport(0, 0, view_width, view_height);
}