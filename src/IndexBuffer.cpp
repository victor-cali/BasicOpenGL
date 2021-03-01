#include "IndexBuffer.h"
#include "Renderer.h"

/* Definition of Index Buffer */
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));
    /* glGenBuffers() generates a buffer object name 
       in [m_RendererID] for the index buffer*/
    GLCall(glGenBuffers(1, &m_RendererID));
    /* glBindBuffer() binds the buffer object named [m_RendererID]
       to the [GL_ELEMENT_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    /* glBufferData() creates and initializes a new buffer object's data store */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    /* glDeleteBuffers() deletes buffer object named [m_RendererID]
       leaving it without contents, with its name free for reuse */
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    /* glBindBuffer() binds the buffer object named [m_RendererID]
       to the [GL_ELEMENT_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    /* glBindBuffer() unbinds the 
       [GL_ELEMENT_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}