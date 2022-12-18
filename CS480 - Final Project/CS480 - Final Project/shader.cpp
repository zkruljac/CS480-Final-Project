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
    s = "#version 460\n \
          \
          layout (location = 0) in vec3 v_position; \
          layout (location = 1) in vec3 v_color; \
          layout (location = 2) in vec2 v_tc;  \
             \
            out vec3 color; \
            out vec3 pos; \
            out vec2 tc;\
            \
            layout (binding=0) uniform sampler2D sp; \
            layout (binding=1) uniform sampler2D samp1; \
            \
            uniform mat4 projectionMatrix; \
            uniform mat4 viewMatrix; \
            uniform mat4 modelMatrix; \
            uniform mat3 normalMatrix; \
            uniform bool hasTC; \
            uniform bool isMesh; \
            \
            uniform float timef; \
            \
            mat4 buildRotateX(float rad); \
            mat4 buildRotateY(float rad); \
            mat4 buildRotateZ(float rad); \
            mat4 buildTranslate(float x, float y, float z, float i); \
            \
            vec2 offsets[1500]; \
            \
            void main(void) \
            { \
                if(isMesh) { \
                    float i = gl_InstanceID + timef; \
                    float a = sin(.35 * i) * 8.0; \
                    float b = cos(.52 * 1) * 8.0; \
                    float c = sin(.70 * i) * 8.0; \
                    \
                    mat4 localRotX = buildRotateX(i); \
                    mat4 localRotY = buildRotateY(i); \
                    mat4 localRotZ = buildRotateZ(i); \
                    mat4 localTrans = buildTranslate(1000 * cos(i/10), 100000* sin(i/10), 1000* cos(i/10), i); \
                    \
                    mat4 newModelMat = localRotY *  modelMatrix * localRotX * localTrans * localRotX * localRotZ; \
                    mat4 newMVMat = viewMatrix * newModelMat; \
                    gl_Position = projectionMatrix * newMVMat * vec4(v_position, 1.0); \
                    pos = (newMVMat * vec4(v_position, 1.0f)).xyz; \
                    color = normalize(mat3(localRotY) * normalMatrix * mat3(localRotX) * mat3(localTrans) * mat3(localRotX) * mat3(localRotZ) * v_color); \
                } \
                else { \
                    \
                    vec4 v = vec4(v_position, 1.0); \
                    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v; \
                    pos = (viewMatrix * modelMatrix * vec4(v_position, 1.0f)).xyz; \
                    color = normalize(normalMatrix * v_color); \
                } \
                tc = v_tc; \
            } \
            \
            mat4 buildTranslate(float x, float y, float z, float i) { \
                mat4 trans = mat4(1.0, 0.0, 0.0,0.0, \
                    0.0, 1.0, 0.0, 0.0, \
                    0.0, 0.0, 1.0, 0.0, \
                    100.0, 100.0,100.0, 1.0); \
                return trans; \
            } \
            mat4 buildRotateX(float rad) { \
                mat4 xrot = mat4(1.0,0.0,0.0,0.0, \
                    0.0, cos(rad), -sin(rad), 0.0, \
                    0.0, sin(rad), cos(rad), 0.0, \
                    0.0, 0.0, 0.0, 1.0); \
                return xrot; \
            } \
            mat4 buildRotateY(float rad) { \
                mat4 yrot = mat4(cos(rad),0.0,sin(rad),0.0, \
                    0.0, 1.0, 0.0, 0.0, \
                    -sin(rad), 0.0, cos(rad), 0.0, \
                    0.0, 0.0, 0.0, 1.0); \
                return yrot; \
            } \
            mat4 buildRotateZ(float rad) { \
                mat4 zrot = mat4(cos(rad),sin(rad),0.0,0.0, \
                    -sin(rad), cos(rad), 0.0, 0.0, \
                    0.0, 0.0, 1.0, 0.0, \
                    0.0, 0.0, 0.0, 1.0); \
                return zrot; \
            } \
            ";
  }
  else if(ShaderType == GL_FRAGMENT_SHADER)
  {
    s = "#version 460\n \
            \
            uniform sampler2D sp; \
            uniform sampler2D samp1; \
            \
            in vec3 color; \
            in vec3 pos; \
            in vec2 tc;\
            struct Material{ \
                vec4 ambient; \
                vec4 diffuse; \
                vec4 spec; \
                float shininess;\
            };\
            \
            struct PositionalLight{\
                vec4 ambient;\
                vec4 diffuse;\
                vec4 spec;\
                vec3 position;\
            };\
            \
            uniform PositionalLight light; \
            uniform Material material; \
            uniform vec4 GlobalAmbient; \
            \
            uniform bool hasTexture;\
            uniform bool hasNormalMap; \
            \
            out vec4 frag_color; \
            \
            void main(void) \
            { \
                if(hasTexture) {\
                    vec3 L = normalize(light.position - pos); \
                    vec3 N; \
                    if (hasNormalMap) N = normalize(color + texture(samp1, tc).xyz * 2 - 1); \
                    else N = normalize(color); \
                    vec3 V = normalize(-pos); \
                    vec3 R = normalize(reflect(-L,N)); \
                    \
                    float cosTheta = dot(L,N); \
                    float cosPhi = dot(R,V); \
                    vec3 amb = ((GlobalAmbient) + (texture(sp, tc) * light.ambient * material.ambient)/1).xyz;\
                    vec3 dif = light.diffuse.xyz * material.diffuse.xyz * texture(sp, tc).xyz * max(0.0f,cosTheta); \
                    vec3 spc = light.spec.xyz * material.spec.xyz * pow(max(0.0f,cosPhi),material.shininess); \
                    frag_color = vec4(amb + dif + spc, 1); \
                }\
                else \
			        frag_color = vec4(vec3(.3,.3,.3),1);\
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


