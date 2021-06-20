#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <iostream>
#include "ViewMat.h"
#include "ViewRenderer.h"
#include "PointRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const int windown_width = 1024;
const int windown_height = 1024;
int view_width;
int view_height;
static ViewMat *viewMat;

int main()
{
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
 
    ViewRenderer viewRenderer;
    if (viewRenderer.init() != 0) {
        glfwTerminate();
        return -1;
    }
    PointRenderer pointRenderer;
    if (pointRenderer.init() != 0) {
        glfwTerminate();
        return -1;
    }
    viewMat = new ViewMat(view_width, view_height);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        
        viewRenderer.draw(viewMat->getProjectionMat(), viewMat->getViewMat());
        pointRenderer.draw(viewMat->getProjectionMat(), viewMat->getViewMat());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        // usleep(100*1000);
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete viewMat;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        viewMat->left();
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        viewMat->right();
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        viewMat->up();
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        viewMat->down();
    if(glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        viewMat->zoom_in();
    if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        viewMat->zoom_out();
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