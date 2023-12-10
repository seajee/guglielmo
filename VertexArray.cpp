#include "VertexArray.h"

#include <GL/glew.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererId);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); ++i) {
        const auto& element = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(intptr_t)offset);

        offset += element.count * VertexBufferLayoutElement::SizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererId);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
