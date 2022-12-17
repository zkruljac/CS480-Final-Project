#include "shader.h"

Shader::Shader()
{
  m_shaderProg = 0;
}

Shader::~Shader()
{
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin() ; it != m_shaderObjList.end() ; it++)
  {
    glDeleteShader(*it);
  }

  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType)
{
  std::string s;

  if(ShaderType == GL_VERTEX_SHADER)
  {
    s = "#version 460\
        \
        struct PositionalLight {    \
            vec4 ambient; \
            vec4 diffuse;   \
            vec4 spec;  \
            vec3 position;  \
        };  \
        uniform vec4 GlobalAmbient; \
        uniform PositionalLight light; \
        \
        struct Material { \
            vec4 ambient; \
            vec4 diffuse; \
            vec4 spec;   \ \
            float shininess; \
        }; \
        uniform Material material; \
        \
        layout(location = 0) in vec3 v_position; \
        layout(location = 1) in vec3 v_normal; \
        layout(location = 2) in vec2 v_tc; \
        \
        out vec3 varNorm; \
        out vec3 varLdir; \
        out vec3 varPos; \
        out vec2 tc; \
        \
        layout(binding = 0) uniform sampler2D samp; \
        layout(binding = 1) uniform sampler2D samp1; \
        \
        uniform mat4 projectionMatrix; \
        uniform mat4 viewMatrix; \
        uniform mat4 modelMatrix; \
        uniform mat3 normMatrix; \
        \
        void main(void) \
        { \
            vec4 v = vec4(v_position, 1.0); \
            gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; \
            tc = v_tc; \
            varPos = (viewMatrix * modelMatrix * vec4(v_position, 1.0f)).xyz; \
            varLdir = light.position - varPos; \
            varNorm = normMatrix * v_normal; \
         } \
          ";
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    s = "#version 460\n \
         \ \
        in vec3 varyingNormal; \
        in vec3 varyingLightDir; \
        in vec3 varyingVertPos; \
        in vec3 varyingHalfVector; \
        \
        out vec4 fragColor; \
        \
        struct PositionalLight  \
        { \
            vec4 ambient; \
            vec4 diffuse; \
            vec4 specular; \
            vec3 position; \
        }; \
        \
        struct Material \
        { \
            vec4 ambient; \
            vec4 diffuse; \
            vec4 specular; \
            float shininess; \
        }; \
        \
        uniform vec4 globalAmbient; \
        uniform PositionalLight light; \
        uniform Material material; \
        uniform mat4 mv_matrix; \
        uniform mat4 proj_matrix; \
        uniform mat4 norm_matrix; \
        \
        void main(void) \
        {	// normalize the light, normal, and view vectors: \
             vec3 L = normalize(varyingLightDir); \
             vec3 N = normalize(varyingNormal); \
             vec3 V = normalize(-varyingVertPos); \
            \
            // get the angle between the light and surface normal: \
            float cosTheta = dot(L, N); \
            \
            // halfway vector varyingHalfVector was computed in the vertex shader, \
            // and interpolated prior to reaching the fragment shader. \
            // It is copied into variable H here for convenience later. \
            vec3 H = normalize(varyingHalfVector);  \
            \
            // get angle between the normal and the halfway vector \
            float cosPhi = dot(H, N); \
            \
            // compute ADS contributions (per pixel): \
            vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz; \
            vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0); \
            vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi, 0.0), material.shininess * 3.0); \
            fragColor = vec4((ambient + diffuse + specular), 1.0); \
         } \
          ";
  }

  GLuint ShaderObj = glCreateShader(ShaderType);

  if (ShaderObj == 0) 
  {
    std::cerr << "Error creating shader type " << ShaderType << std::endl;
    return false;
  }

  // Save the shader object - will be deleted in the destructor
  m_shaderObjList.push_back(ShaderObj);

  const GLchar* p[1];
  p[0] = s.c_str();
  GLint Lengths[1] = { (GLint)s.size() };

  glShaderSource(ShaderObj, 1, p, Lengths);

  glCompileShader(ShaderObj);

  GLint success;
  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

  if (!success) 
  {
    GLchar InfoLog[1024];
    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
    std::cerr << "Error compiling: " << InfoLog << std::endl;
    return false;
  }

  glAttachShader(m_shaderProg, ShaderObj);

  return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

  // Delete the intermediate shader objects that have been added to the program
  for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
  {
    glDeleteShader(*it);
  }

  m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_UNIFORM_LOCATION) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

GLint Shader::GetAttribLocation(const char* pAttribName)
{
    GLuint Location = glGetAttribLocation(m_shaderProg, pAttribName);

    if (Location == -1) {
        fprintf(stderr, "Warning! Unable to get the location of attribute '%s'\n", pAttribName);
    }

    return Location;
}


