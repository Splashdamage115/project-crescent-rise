#include "ChatBoxText.h"
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include "OnlineDispatcher.h"
#include "CommandInterpreter.h"

std::vector<ChatText> ChatBoxText::m_chatHistory;

void ChatBoxText::Start() 
{
    CommandInterpreter::append([this](std::string t) { this->ClearTextFields(t); }, "/clear");
    CommandInterpreter::append([this](std::string t) { this->ClearTextFields(t); }, "/CLEAR");

    CommandInterpreter::append([this](std::string t) { this->ChangePlayerName(t); }, "/changename");
    CommandInterpreter::append([this](std::string t) { this->ChangePlayerName(t); }, "/CHANGENAME");

    CommandInterpreter::append([this](std::string t) { this->changeDebug(t); }, "/debug");
    CommandInterpreter::append([this](std::string t) { this->changeDebug(t); }, "/DEBUG");


    m_playerName = OnlineDispatcher::GetIdentifier();
    renderPriority = RenderPriority::GUI;

    // Initialize FreeType
    if (FT_Init_FreeType(&ft)) std::cerr << "Could not init FreeType Library" << std::endl;
    if (FT_New_Face(ft, "Assets/Fonts/Architect.ttf", 0, &face)) std::cerr << "Could not open font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 22);

    // initialise the texture for the sprites
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    glGenVertexArrays(1, &m_body.vao);
    glBindVertexArray(m_body.vao);


    glGenBuffers(1, &m_body.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);

    m_shader = VertexShaders::retrieveShader(Shader::VertexShaderType::text, Shader::FragmentShaderType::text);

    // get the model coords
    uModelLoc = glGetUniformLocation(m_shader->shaderPair, "uModel");
    uViewLoc = glGetUniformLocation(m_shader->shaderPair, "uView");
    uProjLoc = glGetUniformLocation(m_shader->shaderPair, "uProj");
    uColourLoc = glGetUniformLocation(m_shader->shaderPair, "color");

    GLint coordLoc = glGetAttribLocation(m_shader->shaderPair, "coord");
    glEnableVertexAttribArray(coordLoc);
    glVertexAttribPointer(coordLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLint texLoc = glGetUniformLocation(m_shader->shaderPair, "tex");
    glUniform1i(texLoc, 0);

}

void ChatBoxText::Update()
{
    // if the enter key is pressed, the typing is done and sent
    if (KeyScan::HandleTyping(text))
    {
        if (text.size() < 1) return;
        if (text.at(0) == '/')
        {
            // the text is a command
            if (CommandInterpreter::funcCalled(text))
            {
                OnlineDispatcher::Dispatch(OnlineDispatcher::DispatchType::ChatText, text); // send online
                SentNewText("SYSTEM", text);
            }
            else
            {
                SentNewText("SYSTEM", "FUNCTION DOES NOT EXIST");
            }
        }
        else
        {
            //SentNewText(m_playerName, text);
            OnlineDispatcher::Dispatch(OnlineDispatcher::DispatchType::ChatText, text); // send online
        }
        text = "";
    }
}

void ChatBoxText::Render() {

    // initialise open gl context parts
    glBindTexture(GL_TEXTURE_2D, textureID);
    VertexShaders::LoadShader(m_shader);
    glBindVertexArray(m_body.vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_body.vbo);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glm::mat4 model = transform ? ToModelMatrix(*transform) : glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = Window::Get().GetProj();
    if (uModelLoc >= 0) glUniformMatrix4fv(uModelLoc, 1, GL_FALSE, glm::value_ptr(model));
    if (uViewLoc >= 0) glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    if (uProjLoc >= 0) glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
    if (uColourLoc >= 0) glUniform4fv(uColourLoc, 1, glm::value_ptr(color));


    // initilise the write 
    std::string readable = text;
    readable += '|';

    float x = 0.0f;
    float yStart = -0.4f;
    float textOffset = -0.06f;
    float y = yStart;

    // only display the last 5 messages
    int startPos = 0;
    if (m_chatHistory.size() > 5)
    {
        startPos = m_chatHistory.size() - 5;
    }

    // if the context for typing isnt open
    // we only show the last updates to the context
    if (!KeyScan::typingActive)
    {
        for (int i = startPos; i < m_chatHistory.size(); i++)
        {
            // find items that havent timed out
            if (m_chatHistory.at(i).TimeOut > 0.f)
            {
                readable = m_chatHistory.at(i).playerName;
                readable += " > ";
                readable += m_chatHistory.at(i).text;
        
                RenderTexts(readable, x, y);
        
                x = 0.0f;
                y -= textOffset;
        
                m_chatHistory.at(i).TimeOut -= Game::deltaTime;
            }
        }
        CleanRender();

        return;
    }

    x = 0.0f;
    y = yStart;

    for (int i = m_chatHistory.size(); i >= startPos; i--)
    {
        // display typing
        if (i == m_chatHistory.size())
        {
            y = yStart + textOffset;
            readable = text;
            readable += '|';
        }
        // display readable
        else
        {
            readable = m_chatHistory.at(i).playerName;
            readable += " > ";
            readable += m_chatHistory.at(i).text;
        }
        
        RenderTexts(readable, x, y);

        x = 0.0f;
        y -= textOffset;
    }

    CleanRender();
}

// initilise the text object
void ChatBoxText::SentNewText(std::string name, std::string text)
{
    if (text.length() != 0)
    {
        ChatText t;
        t.playerName = name;
        t.text = text;
        m_chatHistory.emplace_back(t);
    }
}

void ChatBoxText::ClearTextFields(std::string t_arguement)
{
    m_chatHistory.clear();
}

void ChatBoxText::ChangePlayerName(std::string t_arguement)
{
    m_playerName = t_arguement;
    OnlineDispatcher::playerIdentifier = m_playerName;
}

void ChatBoxText::changeDebug(std::string _)
{

}

// clean the render queue, used 2x in render, if typing is available or not
void ChatBoxText::CleanRender()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

}

// this code has been modified by me, but most of it comes from the freeType documentation for how to render the text
// still has added cleanup thanks to open gl
// these few lines of code should not be considered for my final mark
void ChatBoxText::RenderTexts(std::string t_textToRender, float x, float y)
{
    float sx = 0.002f;
    float sy = 0.002f;

    for (const char* p = t_textToRender.c_str(); *p; p++) {
        if (FT_Load_Char(face, *p, FT_LOAD_RENDER))
            continue;

        FT_GlyphSlot g = face->glyph;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            g->bitmap.width, g->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;

        GLfloat box[4][4] = {
            {x2,     -y2    , 0, 0},
            {x2 + w, -y2    , 1, 0},
            {x2,     -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        x += (g->advance.x / 64) * sx;
        y += (g->advance.y / 64) * sy;
    }
}

