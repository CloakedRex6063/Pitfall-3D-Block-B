#pragma once
#include <vector>

#include "engine/util/common.h"
WARNING_DISABLE
#include "LinearMath/btIDebugDraw.h"
WARNING_ENABLE

class Shader;

class DebugDrawer : public btIDebugDraw
{
    int debugMode;
    std::vector<float> lineVertices;
    Shader* shader;

    unsigned int vbo;
public:

    DebugDrawer(Shader* shader);
    void Draw();

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;

    void reportErrorWarning(const char* warningString) override;

    void draw3dText(const btVector3& location, const char* textString) override;

    void setDebugMode(int debugMode) override;

    int getDebugMode() const override { return debugMode; }

};