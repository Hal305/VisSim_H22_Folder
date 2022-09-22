#version 410 core

layout(location = 0) in vec4 positionIn;    // 1st attribute buffer = vertex positions
layout(location = 1) in vec4 colorIn;       // 2nd attribute buffer = colors

out vec4 color;                             //color sent to rest of pipeline

uniform mat4 mMatrix;                       //the matrix for the model
uniform mat4 vMatrix;                       //the matrix for the view
uniform mat4 pMatrix;                       //the matrix for the projection

void main() {
   color = colorIn;                         //passing on the vertex color
   gl_Position = pMatrix * vMatrix * mMatrix * positionIn;  //calculate the position of the model
}
