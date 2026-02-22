#include "ShaderPrograms.h"

std::vector<std::shared_ptr<Shader>> VertexShaders::m_shaders;
std::vector<ShaderFilesVertex> VertexShaders::m_vertexFiles;
std::vector<ShaderFilesFragment> VertexShaders::m_fragmentFiles;

// load fragment and vertex shader files
void VertexShaders::initialise()
{

    // - - - - - - - - - - - - - - - 
    //  pink shader, basic
    // - - - - - - - - - - - - - - - 

    const char* newVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "out vec2 TexCoords;\n"
        "void main() {\n"
        "    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);\n"
        "    TexCoords = aTexCoord;\n"
        "}\n";

    ShaderFilesVertex newVertexPair;
    newVertexPair.vertexType = Shader::VertexShaderType::standard;
    newVertexPair.file = newVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* newFragment =
        "#version 410 core\n"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4( 1.0, 0.0, 0.49, 1.0 );"
        "}";

    ShaderFilesFragment newFragmentPair;
    newFragmentPair.fragmentType = Shader::FragmentShaderType::standard;
    newFragmentPair.file = newFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::standard);


    // - - - - - - - - - - - - - - - 
    //  checkerboard 
    // - - - - - - - - - - - - - - - 

    const char* newFragment2 =
        "#version 410 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   vec2 uv = TexCoords * 8.0;\n"
        "   float checker = mod(floor(uv.x) + floor(uv.y), 2.0);\n"
        "   FragColor = checker == 0.0 ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.1, 0.1, 0.15, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::checkerboard;
    newFragmentPair.file = newFragment2;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::standard, Shader::FragmentShaderType::checkerboard);





    // - - - - - - - - - - - - - - - 
    //  colour, pass colour to frag
    // - - - - - - - - - - - - - - - 

    const char* colouredVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "out vec3 WorldPos;\n"
        "out vec3 bNormal;\n"
        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    WorldPos = worldPos.xyz;\n"
        "    bNormal = mat3(transpose(inverse(uModel))) * aNormal;\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::Colour;
    newVertexPair.file = colouredVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* newFragment3 =
        "#version 410 core\n"
        "in vec3 WorldPos;\n"
        "in vec3 bNormal;\n"
        "uniform vec3 colour;\n"
        "out vec4 frag_colour;\n"
        "void main() {\n"
        "    frag_colour = vec4(colour / 255.0, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::Colour;
    newFragmentPair.file = newFragment3;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::Colour, Shader::FragmentShaderType::Colour);


    // - - - - - - - - - - - - - - - 
    //  terrain (flat terrain)
    // - - - - - - - - - - - - - - - 

    const char* terrainVertex =
        "#version 410 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec2 aTexCoord;\n"
        "\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "\n"
        "out vec2 TexCoords;\n"
        "out float worldY;\n"
        "\n"
        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    TexCoords = aTexCoord;\n"
        "    worldY = worldPos.y;\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::terrain;
    newVertexPair.file = terrainVertex;
    m_vertexFiles.push_back(newVertexPair);


    const char* terrainFrag =
        "#version 410 core\n"
        "in vec2 TexCoords;\n"
        "in float worldY;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform float lowHeight = 12.0;\n"
        "uniform float midHeight = 14.0;\n"
        "uniform float highHeight = 20.0;\n"
        "\n"
        "vec4 getHeightColor(float y) {\n"
        "    if (y < midHeight) {\n"
        "        // Blend yellow to green\n"
        "        float t = clamp((y - lowHeight) / (midHeight - lowHeight), 0.0, 1.0);\n"
        "        return mix(vec4(1.0, 1.0, 0.0, 1.0), vec4(0.0, 1.0, 0.0, 1.0), t);\n"
        "    }\n"
        "    else {\n"
        "        // Blend green to grey\n"
        "        float t = clamp((y - midHeight) / (highHeight - midHeight), 0.0, 1.0);\n"
        "        return mix(vec4(0.0, 1.0, 0.0, 1.0), vec4(0.5, 0.5, 0.5, 1.0), t);\n"
        "    }\n"
        "}\n"
        "\n"
        "void main() {\n"
        "    FragColor = getHeightColor(worldY);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::terrain;
    newFragmentPair.file = terrainFrag;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::terrain, Shader::FragmentShaderType::terrain);




    // - - - - - - - - - - - - - - - 
    //  lit ~ this is for the planet
    // - - - - - - - - - - - - - - - 

    
    setUpPlanetShader();



    // - - - - - - - - - - - - - - - 
    //  text
    // - - - - - - - - - - - - - - - 

    const char* textVertex =
        "#version 410\n"
        "layout(location = 0) in vec4 coord;"
        "out vec2 texcoord;"
        "uniform mat4 uModel;"
        "uniform mat4 uView;"
        "uniform mat4 uProj;"
        "void main(void) {"
        "    gl_Position = uProj * uView * uModel * vec4(coord.xy, 0.0, 1.0);"
        "    texcoord = coord.zw;"
        "}";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::text;
    newVertexPair.file = textVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* textFragment =

    "#version 410\n"
    "in vec2 texcoord;"
    "uniform sampler2D tex;"
    "uniform vec4 color;"
    "out vec4 fragColor;"
    "void main(void) {"
    "    float alpha = texture(tex, texcoord).r;"
    "    fragColor = vec4(color.rgb, alpha);"
    "}";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::text;
    newFragmentPair.file = textFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::text, Shader::FragmentShaderType::text);




    // - - - - - - - - - - - - - - - 
    //  Line 
    // - - - - - - - - - - - - - - - 


    const char* lineVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "void main() {\n"
        "    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);\n"
        "}\n";


    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::Line;
    newVertexPair.file = lineVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* lineFragment =
        "#version 410 core\n"
        "out vec4 frag_colour;\n"
        "uniform vec3 baseColour;\n"
        "void main() {\n"
        "    frag_colour = vec4(baseColour, 1.0);\n"
        "}\n";


    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::Line;
    newFragmentPair.file = lineFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::Line, Shader::FragmentShaderType::Line);
    


    // - - - - - - - - - - - - - - - 
    //  Texture
    // - - - - - - - - - - - - - - - 



    const char* TextureVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTex;\n"
        "\n"
        "out vec2 vTex;\n"
        "\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "\n"
        "void main() {\n"
        "    vTex = aTex;\n"
        "    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);\n"
        "}\n";



    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::texture;
    newVertexPair.file = TextureVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* TextureFragment =
        "#version 410 core\n"
        "in vec2 vTex;\n"
        "out vec4 frag_colour;\n"
        "\n"
        "uniform sampler2D uTexture;\n"
        "uniform vec3 colour;\n"
        "\n"
        "void main() {\n"
        "    vec4 texColor = texture(uTexture, vTex);\n"
        "    frag_colour = texColor * vec4(colour, 1.0);\n"
        "}\n";



    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::texture;
    newFragmentPair.file = TextureFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::texture, Shader::FragmentShaderType::texture);



    // - - - - - - - - - - - - - - - 
    //  Lit Texture
    // - - - - - - - - - - - - - - - 


    const char* LitTextureVertex =
    "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTex;\n"
        "layout (location = 2) in vec3 aNormal;\n"
        "layout (location = 3) in float aMaterialID;\n"

        "uniform vec3 CenterPoint;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"

        "out vec2 vTex;\n"
        "out vec3 WorldPos;\n"
        "out vec3 bNormal;\n"
        "out float vMaterialID;\n"

        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    WorldPos = worldPos.xyz;\n"
        "    bNormal = mat3(transpose(inverse(uModel))) * aNormal;\n"
        "    vTex = aTex;\n"
        "    vMaterialID = aMaterialID;\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::litTexture;
    newVertexPair.file = LitTextureVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* LitTextureFragment =
    "#version 410 core\n"
        "in vec2 vTex;\n"
        "in vec3 WorldPos;\n"
        "in vec3 bNormal;\n"
        "in float vMaterialID;\n"
        "out vec4 FragColor;\n"

        "uniform sampler2D uTexture;\n"
        "uniform sampler2D uTexture2;\n"
        "uniform vec3 colour;\n"
        "uniform sampler2D uHeightMap;\n"
        "uniform bool uHighlight;\n"
        "const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));\n"
        "const vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "const vec3 viewPos = vec3(0.0, 0.0, 10.0);\n"

        "void main() {\n"
        "    vec3 norm = normalize(bNormal);\n"
        "    vec3 viewDir = normalize(viewPos - WorldPos);\n"
        "    float height = texture(uHeightMap, vTex).r;\n"
        "    vec2 parallaxUV = vTex - viewDir.xy * (height * 0.08);\n"
        "    vec4 texColor = vMaterialID < 0.5 ? texture(uTexture,  parallaxUV) : texture(uTexture2, parallaxUV);\n"
        "    float ambientStrength = 0.2;\n"
        "    vec3 ambient = ambientStrength * lightColor;\n"
        "    float diff = max(dot(norm, -lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "    float specularStrength = 0.5;\n"
        "    vec3 reflectDir = reflect(lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "    vec3 specular = specularStrength * spec * lightColor;\n"
        "    vec3 lighting = (ambient + diffuse) + specular;\n"
        "    FragColor = vec4(texColor.rgb * lighting * colour, texColor.a);\n"
        "    if (uHighlight) {\n"
        "        float rim = 1.0 - max(dot(viewDir, norm), 0.0);\n"
        "        float outline = smoothstep(0.55, 1.0, rim);\n"
        "        FragColor = vec4(mix(FragColor.rgb, vec3(1.0, 1.0, 1.0), outline), FragColor.a);\n"
        "    }\n"
        "}\n";


    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::litTexture;
    newFragmentPair.file = LitTextureFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::litTexture, Shader::FragmentShaderType::litTexture);

    // - - - - - - - - - - - - - - - 
    //  SKY BOX 
    // - - - - - - - - - - - - - - - 

    const char* skyBoxVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "out vec3 vDir;\n"
        "\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "\n"
        "void main() {\n"
        "    mat4 rotView = mat4(mat3(uView));\n"
        "    vec4 pos = uProj * rotView * vec4(aPos, 1.0);\n"
        "    gl_Position = pos;\n"
        "    gl_Position.z = gl_Position.w;\n"
        "    vDir = aPos;\n"
        "}\n";


    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::skyBox;
    newVertexPair.file = skyBoxVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* skyBoxFragment =
        "#version 410 core\n"
        "in vec3 vDir;\n"
        "out vec4 frag_colour;\n"
        "\n"
        "uniform samplerCube uSky;\n"
        "\n"
        "void main() {\n"
        "    frag_colour = texture(uSky, normalize(vDir));\n"
        "}\n";




    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::skyBox;
    newFragmentPair.file = skyBoxFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::skyBox, Shader::FragmentShaderType::skyBox);
}











// *** NEVER USE THIS ***
void VertexShaders::LoadShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (unsigned int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i)->vertexType == t_vertex &&
            m_shaders.at(i)->fragmentType == t_fragment)
        {
            glUseProgram(m_shaders.at(i)->shaderPair);
            return;
        }
    }


    // if the combo requested doesnt exist yet, try to mount it, and use it
    try {
        mountShader(t_vertex, t_fragment);
        glUseProgram(m_shaders.at(m_shaders.size() - 1)->shaderPair);
    }
    catch(std::errc)
    {
        glUseProgram(m_shaders.at(0)->shaderPair); // otherwise revert to basic shader
    }

}

// try to keep the used shader in memory somewhere, otherwise call the other type
// we use this to speed up execution
void VertexShaders::LoadShader(std::shared_ptr<Shader> t_shader)
{
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(t_shader->shaderPair);
}

// retrieve the pointer to the wanted shader
std::shared_ptr<Shader> VertexShaders::retrieveShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    // find the shader and try to use it
    for (unsigned int i = 0; i < m_shaders.size(); i++)
    {
        if (m_shaders.at(i)->vertexType == t_vertex &&
            m_shaders.at(i)->fragmentType == t_fragment)
        {
            return m_shaders.at(i);
        }
    }


    // if the combo requested doesnt exist yet, try to mount it, and use it
    try {
        mountShader(t_vertex, t_fragment);
        return m_shaders.at(m_shaders.size() - 1);
    }
    catch (std::errc)
    {
        return m_shaders.at(0);
    }

}

// mount any non existent shaders, and make a new shader for it
std::errc VertexShaders::mountShader(Shader::VertexShaderType t_vertex, Shader::FragmentShaderType t_fragment)
{
    std::shared_ptr< Shader > newShader = std::make_shared<Shader>();

    // find the file location (if either doesnt exist, exit)
    int vertexNum = -1;
    int fragNum = -1;

    for (unsigned int i = 0; i < m_vertexFiles.size(); i++)
    {
        if (m_vertexFiles.at(i).vertexType == t_vertex)
        {
            vertexNum = i;
            break;
        }
    }
    for (unsigned int i = 0; i < m_fragmentFiles.size(); i++)
    {
        if (m_fragmentFiles.at(i).fragmentType == t_fragment)
        {
            fragNum = i;
            break;
        }
    }

    if (vertexNum == -1 || fragNum == -1) return std::errc::timed_out;


    // initialise the new shader
    newShader->vertexType = m_vertexFiles.at(vertexNum).vertexType;
    newShader->fragmentType = m_fragmentFiles.at(fragNum).fragmentType;

    newShader->shaderPair = glCreateProgram();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &m_vertexFiles.at(vertexNum).file, NULL);
    glCompileShader(vs);
    GLint success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
        return std::errc::timed_out;
    }


    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &m_fragmentFiles.at(fragNum).file, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
        return std::errc::timed_out;
    }

    glAttachShader(newShader->shaderPair, vs);
    glAttachShader(newShader->shaderPair, fs);

    glLinkProgram(newShader->shaderPair);

    m_shaders.push_back(newShader);
}

static std::string composeFragmentShader(
    const char* baseTemplate,
    const char* waterModule) 
{
    char buffer[16384];  // Large enough for all modules combined
    snprintf(buffer, sizeof(buffer), baseTemplate, waterModule);
    return std::string(buffer);
}

void VertexShaders::setUpPlanetShader()
{
    const char* vertexDeformationModule =
        "vec3 gerstnerWave(vec3 pos, vec2 dir, float amplitude, float wavelength, float speed, float time) {\n"
        "    float k = 2.0 * 3.14159 / wavelength;\n"
        "    float phase = speed * time;\n"
        "    float theta = dot(dir, pos.xz) * k + phase;\n"
        "\n"
        "    float s = sin(theta);\n"
        "    float c = cos(theta);\n"
        "\n"
        "    vec2 horizontal = dir * (c * amplitude * 0.5);\n"
        "    float vertical = s * amplitude;\n"
        "\n"
        "    return pos + vec3(horizontal.x, vertical, horizontal.y);\n"
        "}\n"
        "\n"
        "vec3 applyGerstnerDisplacement(vec3 pos, float time) {\n"
        "    const int WAVE_COUNT = 5;\n"
        "    float amplitudes[WAVE_COUNT]  = float[](1.5, 0.35, 0.25, 0.15, 0.10);\n"
        "    float wavelengths[WAVE_COUNT] = float[](200.0, 30.0, 15.0, 8.0, 4.0);\n"
        "    float speeds[WAVE_COUNT]      = float[](0.4, 1.0, 1.4, 1.8, 2.3);\n"
        "    vec2 directions[WAVE_COUNT]   = vec2[](\n"
        "        vec2( 1.0,  0.0),\n"
        "        vec2( 0.3,  1.0),\n"
        "        vec2(-0.8,  0.4),\n"
        "        vec2(-0.2, -1.0),\n"
        "        vec2( 0.9, -0.3)\n"
        "    );\n"
        "\n"
        "    for (int i = 0; i < WAVE_COUNT; i++) {\n"
        "        directions[i] = normalize(directions[i]);\n"
        "        pos = gerstnerWave(pos, directions[i], amplitudes[i], wavelengths[i], speeds[i], time);\n"
        "    }\n"
        "\n"
        "    return pos;\n"
        "}\n";


    const char* waterVertexShader =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "layout (location = 2) in vec3 aNormal;\n"
        "\n"
        "uniform vec3 CenterPoint;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "uniform float uTime;\n"
        "\n"
        "out vec2 TexCoords;\n"
        "out vec3 WorldPos;\n"
        "out vec3 bNormal;\n"
        "out float height;\n"
        "\n"
        "%s\n"
        "void main()\n"
        "{\n"
        "    // Apply Gerstner displacement to ALL vertices\n"
        "    vec3 displacedPos = applyGerstnerDisplacement(aPos, uTime);\n"
        "\n"
        "    // Transform to world space\n"
        "    vec4 worldPos = uModel * vec4(displacedPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    WorldPos = worldPos.xyz;\n"
        "\n"
        "    // Normal passthrough\n"
        "    bNormal = mat3(transpose(inverse(uModel))) * aNormal;\n"
        "\n"
        "    // UV passthrough\n"
        "    TexCoords = aTexCoord;\n"
        "\n"
        "    // Height for depth tinting\n"
        "    height = length(worldPos.xyz - CenterPoint);\n"
        "}\n";

    std::string composedVertex = composeFragmentShader(
        waterVertexShader,
        vertexDeformationModule
    );


    ShaderFilesVertex newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::Water;
    newVertexPair.file = composedVertex.c_str();
    m_vertexFiles.push_back(newVertexPair);



    const char* waterFragmentShader =
        "#version 410 core\n"
        "in vec3 WorldPos;\n"
        "in float height;\n"
        "in vec3 bNormal;\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform vec2 minMax;\n"
        "uniform vec3 viewPos;\n"
        "uniform float uTime;\n"
        "uniform sampler2D uWaterNormalMap;\n"
        "uniform sampler2D uWaterColorMap;\n"
        "uniform float uTextureScale;\n"
        "uniform float uTextureSpeed;\n"
        "\n"
        "const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));\n"
        "const vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec3 normal = normalize(bNormal);\n"
        "\n"
        "    float heightPercent = (height - minMax.x) / (minMax.y - minMax.x);\n"
        "    vec3 viewDir = normalize(viewPos - WorldPos);\n"
        "\n"
        "    float dist = length(viewPos - WorldPos);\n"
        "    float detailFade = clamp(1.0 - dist * 0.0005, 0.0, 1.0);\n"
        "\n"
        "    const int WAVE_COUNT = 5;\n"
        "    float amplitudes[WAVE_COUNT]  = float[](1.5, 0.35, 0.25, 0.15, 0.10);\n"
        "    float wavelengths[WAVE_COUNT] = float[](200.0, 30.0, 15.0, 8.0, 4.0);\n"
        "    float speeds[WAVE_COUNT]      = float[](0.4, 1.0, 1.4, 1.8, 2.3);\n"
        "    vec2 directions[WAVE_COUNT]   = vec2[](\n"
        "        vec2( 1.0,  0.0),\n"
        "        vec2( 0.3,  1.0),\n"
        "        vec2(-0.8,  0.4),\n"
        "        vec2(-0.2, -1.0),\n"
        "        vec2( 0.9, -0.3)\n"
        "    );\n"
        "\n"
        "    for (int i = 0; i < WAVE_COUNT; i++)\n"
        "        directions[i] = normalize(directions[i]);\n"
        "\n"
        "    vec3 gerstnerNormal = vec3(0.0);\n"
        "\n"
        "    for (int i = 0; i < WAVE_COUNT; i++)\n"
        "    {\n"
        "        float k = 6.28318 / wavelengths[i];\n"
        "        float theta = dot(directions[i], WorldPos.xz) * k + speeds[i] * uTime;\n"
        "\n"
        "        float A = amplitudes[i];\n"
        "        float s = sin(theta);\n"
        "        float c = cos(theta);\n"
        "\n"
        "        if (i >= 3)\n"
        "            A *= detailFade;\n"
        "\n"
        "        gerstnerNormal.x += directions[i].x * A * c;\n"
        "        gerstnerNormal.z += directions[i].y * A * c;\n"
        "        gerstnerNormal.y += A * s;\n"
        "    }\n"
        "\n"
        "    float macro = sin(WorldPos.x * 0.02 + uTime * 0.1) *\n"
        "                  cos(WorldPos.z * 0.015 + uTime * 0.07) * 0.5;\n"
        "    gerstnerNormal.y += macro * 0.2;\n"
        "\n"
        "    vec2 uv1 = TexCoords * uTextureScale + vec2(uTime * uTextureSpeed, 0.0);\n"
        "    vec2 uv2 = TexCoords * uTextureScale + vec2(0.0, uTime * uTextureSpeed * 0.7);\n"
        "    vec3 texN1 = texture(uWaterNormalMap, uv1).xyz * 2.0 - 1.0;\n"
        "    vec3 texN2 = texture(uWaterNormalMap, uv2).xyz * 2.0 - 1.0;\n"
        "    vec3 texNormal = normalize(texN1 + texN2);\n"
        "\n"
        "    normal = normalize(normal + gerstnerNormal + texNormal * 0.3);\n"
        "\n"
        "    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), 5.0);\n"
        "\n"
        "    float depth = pow(clamp(heightPercent, 0.0, 1.0), 0.6);\n"
        "    vec3 waterColor = vec3(0.00, 0.12, 0.6);\n"
        "\n"
        "    float foamNoise = fract(sin(dot(WorldPos.xz, vec2(12.9898, 78.233))) * 43758.5453);\n"
        "    float foamFactorDepth = smoothstep(0.0, 0.18, depth);\n"
        "    float slope = 1.0 - normal.y;\n"
        "    float foamMaskSlope = smoothstep(0.2, 0.6, slope);\n"
        "    float foam = foamFactorDepth * foamMaskSlope * smoothstep(0.3, 1.0, foamNoise);\n"
        "\n"
        //"    vec3 foamColor = vec3(0.9, 0.95, 1.0);\n"
        //"    waterColor = mix(waterColor, foamColor, foam * 0.6);\n"
        "\n"
        "    float fresnelStrength = 0.3;\n"
        "    float baseStrength = 0.7;\n"
        //"    vec3 colorTex = texture(uWaterColorMap, uv1).rgb;\n"
        //"    waterColor = mix(waterColor, colorTex, 0.2);\n"
        "    vec3 finalColor = waterColor * (baseStrength + fresnelStrength * fresnel);\n"
        "\n"
        "    FragColor = vec4(finalColor, 0.9);\n"
        "}\n";



    ShaderFilesFragment newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::Water;
    newFragmentPair.file = waterFragmentShader;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::Water, Shader::FragmentShaderType::Water);












        const char* litVertex =
            "#version 410 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec2 aTexCoord;\n"
            "layout (location = 2) in vec3 aNormal;\n"

            "uniform vec3 CenterPoint;\n"
            "uniform mat4 uModel;\n"
            "uniform mat4 uView;\n"
            "uniform mat4 uProj;\n"

            "out vec2 TexCoords;\n"
            "out vec3 WorldPos;\n"
            "out vec3 bNormal;\n"
            "out float height;\n"

            "void main() {\n"
            "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
            "    gl_Position = uProj * uView * worldPos;\n"
            "    WorldPos = worldPos.xyz;\n"
            "    bNormal = mat3(transpose(inverse(uModel))) * aNormal;\n"
            "    TexCoords = aTexCoord;\n"
            "    height = sqrt((CenterPoint.x - WorldPos.x) * (CenterPoint.x - WorldPos.x) + (CenterPoint.y - WorldPos.y) * (CenterPoint.y - WorldPos.y) + (CenterPoint.z - WorldPos.z) * (CenterPoint.z - WorldPos.z));\n"
            "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::Planet;
    newVertexPair.file = litVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* litFragment =
        "#version 410 core\n"
        "in vec3 WorldPos;\n"
        "in float height;\n"
        "in vec3 bNormal;\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform vec2 minMax;\n"
        "const int MAX_VEC_SIZE = 32;\n"
        "\n"
        "uniform vec3 heightColours[MAX_VEC_SIZE];\n"
        "uniform float startHeight[MAX_VEC_SIZE];\n"
        "uniform int shaderType[MAX_VEC_SIZE];\n"
        "uniform sampler2D layerTextures[MAX_VEC_SIZE];\n"
        "uniform sampler2D uLayerNormal[MAX_VEC_SIZE];\n"
        "uniform float textureScale[MAX_VEC_SIZE];\n"
        "uniform float uLayerNormalStrength[MAX_VEC_SIZE];\n"
        "uniform float ambientLightStrength;\n"
        "\n"
        "uniform vec3 viewPos;\n"
        "uniform float uTime;\n"
        "\n"
        "const float FEATHER = 0.1;\n"
        "\n"
        "const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));\n"
        "const vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "void main() {\n"
        "    vec3 norm = normalize(bNormal);\n"
        "    float mini = minMax.x;\n"
        "    float maxi = minMax.y;\n"
        "    float heightPercent = (height - mini) / (maxi - mini);\n"
        "    vec3 colourA = vec3(0.0);\n"
        "    vec3 colourB = vec3(0.0);\n"
        "    vec3 normalA = vec3(0.0);\n"
        "    vec3 normalB = vec3(0.0);\n"
        "    float strengthA = 0.0;\n"
        "    float strengthB = 0.0;\n"
        "    int lower = 0;\n"
        "    int upper = 0;\n"
        "    for (int i = 0; i < MAX_VEC_SIZE - 1; i++) {\n"
        "        if (startHeight[i] <= heightPercent && heightPercent < startHeight[i+1]) {\n"
        "            lower = i;\n"
        "            upper = i + 1;\n"
        "            break;\n"
        "        }\n"
        "    }\n"
        "    float h0 = startHeight[lower];\n"
        "    float h1 = startHeight[upper];\n"
        "    float blend = smoothstep(h0 - FEATHER, h0 + FEATHER, heightPercent);\n"
        "    blend = clamp(blend, 0.0, 1.0);\n"
        "    if (shaderType[lower] == 0) {\n"
        "        colourA = heightColours[lower] / 255.0;\n"
        "    } else {\n"
        "        vec2 uvA = TexCoords * textureScale[lower];\n"
        "        colourA = texture(layerTextures[lower], uvA).rgb;\n"
        "        normalA = texture(uLayerNormal[lower], uvA).xyz * 2.0 - 1.0;\n"
        "        strengthA = uLayerNormalStrength[lower];\n"
        "    }\n"
        "    if (shaderType[upper] == 0) {\n"
        "        colourB = heightColours[upper] / 255.0;\n"
        "    } else {\n"
        "        vec2 uvB = TexCoords * textureScale[upper];\n"
        "        colourB = texture(layerTextures[upper], uvB).rgb;\n"
        "        normalB = texture(uLayerNormal[upper], uvB).xyz * 2.0 - 1.0;\n"
        "        strengthB = uLayerNormalStrength[upper];\n"
        "    }\n"
        "    vec3 colourOverride = mix(colourA, colourB, blend);\n"
        "    vec3 blendedNormal = normalize(\n"
        "        normalA * strengthA * (1.0 - blend) +\n"
        "        normalB * strengthB * blend\n"
        "    );\n"
        "    norm = normalize(norm + blendedNormal);\n"
        "    vec3 ambient = ambientLightStrength * lightColor;\n"
        "    float diff = max(dot(norm, -lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "    vec3 viewDir = normalize(viewPos - WorldPos);\n"
        "    vec3 reflectDir = reflect(lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "    vec3 specular = 0.5 * spec * lightColor;\n"
        "    vec3 result = (ambient + diffuse) * colourOverride + specular;\n"
        "    FragColor = vec4(result, 1.0);\n"
        "}\n";


    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::Planet;
    newFragmentPair.file = litFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::Planet, Shader::FragmentShaderType::Planet);




    const char* billboardVert =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTex;\n"
        "layout (location = 2) in vec3 aPlanetPos;\n"

        "uniform vec3 CenterPoint;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"

        "out vec2 vTex;\n"
        "out vec3 WorldPos;\n"
        "out vec3 bNormal;\n"

        "void main() {\n"
        "    vec4 worldPos = uModel * vec4(aPos, 1.0);\n"
        "    gl_Position = uProj * uView * worldPos;\n"
        "    WorldPos = worldPos.xyz;\n"
        "    bNormal = vec3(transpose(inverse(uModel)));\n"
        "    vTex = aTex;\n"
        "}\n";


    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::billboard;
    newVertexPair.file = billboardVert;
    m_vertexFiles.push_back(newVertexPair);

    const char* billboardFrag =
        "#version 410 core\n"
        "in vec2 vTex;\n"
        "in vec3 WorldPos;\n"
        "in vec3 bNormal;\n"
        "out vec4 FragColor;\n"

        "uniform sampler2D uTexture;\n"
        "uniform vec3 colour;\n"
        "uniform sampler2D uHeightMap;\n"
        "const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));\n"
        "const vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
        "const vec3 viewPos = vec3(0.0, 0.0, 10.0);\n"

        "void main() {\n"
        "    // Normalize the interpolated normal\n"
        "    vec3 norm = normalize(bNormal);\n"
        ""
        "    float height = texture(uHeightMap, vTex).r;\n"
        "    float scale = 0.08;\n"
        "    vec3 viewDir = normalize(viewPos - WorldPos);\n"
        "    vec2 parallaxUV = vTex - viewDir.xy * (height * scale);\n"
        "    vec4 texColor = texture(uTexture, parallaxUV);\n"
        ""
        "    float ambientStrength = 0.2;\n"
        "    vec3 ambient = ambientStrength * lightColor;\n"
        "    float diff = max(dot(norm, -lightDir), 0.0);\n"
        "    vec3 diffuse = diff * lightColor;\n"
        "    float specularStrength = 0.5;\n"
        "    vec3 reflectDir = reflect(lightDir, norm);\n"
        "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
        "    vec3 specular = specularStrength * spec * lightColor;\n"
        "    vec3 lighting = (ambient + diffuse) + specular;\n"
        "    FragColor = vec4(texColor.rgb * lighting * colour, texColor.a);"
        "}\n";



    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::billboard;
    newFragmentPair.file = billboardFrag;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::billboard, Shader::FragmentShaderType::billboard);
    const char* grassShader =
        "// Random function implementation"
        "float rand(vec2 co) {\n"
        "    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);\n"
        "}\n";


    // - - - - - - - - - - - - - - - 
    //  Outline (backface expand, second pass)
    // - - - - - - - - - - - - - - - 

    const char* outlineVertex =
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "uniform mat4 uModel;\n"
        "uniform mat4 uView;\n"
        "uniform mat4 uProj;\n"
        "uniform float uOutlineWidth;\n"
        "void main() {\n"
        "    vec3 expanded = aPos + normalize(aNormal) * uOutlineWidth;\n"
        "    gl_Position = uProj * uView * uModel * vec4(expanded, 1.0);\n"
        "}\n";

    newVertexPair = ShaderFilesVertex();
    newVertexPair.vertexType = Shader::VertexShaderType::outline;
    newVertexPair.file = outlineVertex;
    m_vertexFiles.push_back(newVertexPair);

    const char* outlineFragment =
        "#version 410 core\n"
        "out vec4 frag_colour;\n"
        "uniform vec3 outlineColor;\n"
        "void main() {\n"
        "    frag_colour = vec4(outlineColor, 1.0);\n"
        "}\n";

    newFragmentPair = ShaderFilesFragment();
    newFragmentPair.fragmentType = Shader::FragmentShaderType::outline;
    newFragmentPair.file = outlineFragment;
    m_fragmentFiles.push_back(newFragmentPair);

    mountShader(Shader::VertexShaderType::outline, Shader::FragmentShaderType::outline);
}
