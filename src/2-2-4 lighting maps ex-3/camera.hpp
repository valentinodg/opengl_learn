#ifndef CAMERA_H
#define CAMERA_H

#include "../lib/glad/glad.h"
#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"

#include <vector>

//define several possible options for camera movement
//used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.001f;
const float ZOOM = 45.0f;

//abstract camera class that processes input and calculates the corresponding euler angles, vectors and matrices for use in opengl
class Camera
{
    public:
        //camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
        //euler angles
        float Yaw;
        float Pitch;
        //camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        //constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        //constructor with scalar values
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();   
        }

        //returns the view matrix calculated using euler angles and the lookat matrix
        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(Position, Position + Front, Up);
        }

        //processes input received from any keyboard-like input system
        //accepts input parameter in the form of camera defined enum (to abstract it from windowing  systems)
        void ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if(direction == FORWARD)
                Position += Front * velocity;
            if(direction == BACKWARD)
                Position -= Front * velocity;
            if(direction == LEFT)
                Position -= Right * velocity;
            if(direction == RIGHT)
                Position += Right * velocity;
        }

        //processes input received from a mouse input system
        //expects the offset value in both the x and y direction
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constraintPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;

            //make sure that when pitch is out of bounds, screen doesn't get flipped
            if(constraintPitch)
            {
                if(Pitch > 89.0f)
                    Pitch = 89.0f;
                if(Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            //update front, right and up vectors using the updated euler angles
            updateCameraVectors();
        }

        //processes input received from a mouse scroll-wheel event
        //only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yoffset)
        {
            if(Zoom >= 1.0f && Zoom <= 45.0f)
                Zoom -= yoffset;
            if(Zoom <= 1.0f)
                Zoom = 1.0f;
            if(Zoom >= 45.0f)
                Zoom = 45.0f;
        }

    private:
        //calculates the front vector from the camera's (updated) euler angles
        void updateCameraVectors()
        {
            //calculate the new fron vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);
            //also re-calculate the right and up vector
            //normalize the vectors because their length gets closer to 0 the more you look up or down which results in slower movement
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }
};

#endif
