#pragma once

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
        default:
        {
            //OnKeyDown(key);
            break;
        }
        }
    }
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        //Poll mouse cursor position
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        //OnMouseButtonDown(button, xPos, yPos);
    }
}