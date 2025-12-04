#include "debugDrawer.h"

#include <iostream>
#include <GLES3/gl3.h>

#include "graphics/shader/shader.h"
#include "util/util.h"

DebugDrawer::DebugDrawer(Shader* shader) : debugMode(0), shader(shader)
{
    glGenBuffers(1, &vbo);
}

void DebugDrawer::Draw() 
{
	if (lineVertices.empty())
        return;

    shader->Bind();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(lineVertices.size() * sizeof(float)), lineVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    
    // Draw all lines in a single draw call
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lineVertices.size()) / 3);

    Shader::Unbind();

    // Clear the vector for the next frame
    lineVertices.clear();
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	lineVertices.push_back(from.getX());
    lineVertices.push_back(from.getY());
    lineVertices.push_back(from.getZ());

    lineVertices.push_back(to.getX());
    lineVertices.push_back(to.getY());
    lineVertices.push_back(to.getZ());
}

void DebugDrawer::setDebugMode(int debugMode)
{
    this->debugMode = debugMode;
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
    //glRasterPos3f(location.x(),  location.y(),  location.z());
    //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
    std::cerr << warningString << "\n";
}

void DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
    {
        //btVector3 to=pointOnB+normalOnB*distance;
        //const btVector3&from = pointOnB;
        //glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);   

        //GLDebugDrawer::drawLine(from, to, color);

        //glRasterPos3f(from.x(),  from.y(),  from.z());
        //char buf[12];
        //sprintf(buf," %d",lifeTime);
        //BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),buf);
    }
}