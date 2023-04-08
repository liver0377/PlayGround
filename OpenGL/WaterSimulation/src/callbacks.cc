#include "callbacks.h"

/**
 * @brief 键盘事件回调函数
 * 
 * @param window  窗口
 * @param key     触发的按键
 * @param scancode  
 * @param action  GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
 * @param mods 
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;
    (void)window;

    static float last_time = glfwGetTime();

    float curr_time = glfwGetTime();
    float delta_time = curr_time - last_time;
    last_time = curr_time;

    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // -- Camera
        if (key == GLFW_KEY_RIGHT)
            camera->ProcessKeyboard(Simulation::Global::CameraMovement::RIGHT, delta_time);
        else if (key == GLFW_KEY_LEFT)
            camera->ProcessKeyboard(Simulation::Global::CameraMovement::LEFT, delta_time);
        else if (key == GLFW_KEY_UP)
            camera->ProcessKeyboard(Simulation::Global::CameraMovement::FORWARD, delta_time);
        else if (key == GLFW_KEY_DOWN)
            camera->ProcessKeyboard(Simulation::Global::CameraMovement::BACKWARD, delta_time);
        // --- Water
        // Speed
        else if (key == GLFW_KEY_Q && Simulation::Global::opt_speed < 2.0f)
            Simulation::Global::opt_speed += 0.1f;
        else if (key == GLFW_KEY_W && Simulation::Global::opt_speed > 0.0f)
            Simulation::Global::opt_speed -= 0.1f;
        // Amount
        else if (key == GLFW_KEY_A && Simulation::Global::opt_amount < 0.1f)
            Simulation::Global::opt_amount += 0.01f;
        else if (key == GLFW_KEY_S && Simulation::Global::opt_amount > 0.0f)
            Simulation::Global::opt_amount -= 0.1f;
        // Height
        else if (key == GLFW_KEY_Z && Simulation::Global::opt_height < 0.5f)
            Simulation::Global::opt_height += 0.1f;
        else if (key == GLFW_KEY_X && Simulation::Global::opt_height > 0.0f)
            Simulation::Global::opt_height -= 0.1f;
        // -- Misc
        // Mesh grid
        else if (key == GLFW_KEY_M)
        {
            if (Simulation::Global::opt_mesh)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            Simulation::Global::opt_mesh = Simulation::Global::opt_mesh ? 0 : 1;
        }
    }
}

/**
 * @brief  鼠标回调函数
 * 
 * @param window 
 * @param xpos 鼠标当前帧x分量位置
 * @param ypos 鼠标当前帧y分量位置
 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    (void)window;

    if (!Simulation::Global::mouse_action) {
        return;
    }

    float xoffset = xpos - Simulation::Global::mouse_x;
    float yoffset = Simulation::Global::mouse_y - ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
    Simulation::Global::mouse_x = xpos;
    Simulation::Global::mouse_y = ypos;
}

/**
 * @brief 
 * 
 * @param window 
 * @param button 
 * @param action 
 * @param mods 
 */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    (void)mods;
    (void)window;

    if (button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            Simulation::Global::mouse_action = 1;
    }
    else {
        Simulation::Global::mouse_action = 0;
    }
}