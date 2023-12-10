#include "VertexBufferLayout.h"

#include <GL/glew.h>

VertexBufferLayout::VertexBufferLayout()
    : m_Stride(0)
{
}

void VertexBufferLayout::Push(unsigned int type, unsigned int count)
{
    m_Elements.push_back({ type, count, GL_FALSE });
    m_Stride += count * VertexBufferLayoutElement::SizeOfType(type);
}
