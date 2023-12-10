#pragma once

#include <GL/glew.h>
#include <vector>

#include "Utils.h"

struct VertexBufferLayoutElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int SizeOfType(unsigned int type)
    {
        switch (type) {
            case GL_FLOAT:         return sizeof(GLfloat);
            case GL_UNSIGNED_INT:  return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
            default: break;
        }

        ASSERT(false && "ERROR: Unreachable SizeOfType");
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout();

    void Push(unsigned int type, unsigned int count);

    inline const std::vector<VertexBufferLayoutElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
private:
    std::vector<VertexBufferLayoutElement> m_Elements;
    unsigned int m_Stride;
};
