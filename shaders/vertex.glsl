#version 330

//вершинный шейдер
//получает точку
layout(location = 0) in vec2 vertex;

//то, что передаем во фрагментный шейдер
out vec2 fragmentTexCoord;

void main(void)
{
  fragmentTexCoord = vertex * 0.5 + 0.5; 
  gl_Position      = vec4(vertex,0.0,1.0);
}
