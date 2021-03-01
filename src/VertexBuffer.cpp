#include "VertexBuffer.h"
#include "Renderer.h"

/* Definition of Vertex Buffer */
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* glGenBuffers() generates a buffer object name
       in [m_RendererID] for the vertex buffer*/
    GLCall(glGenBuffers(1, &m_RendererID));
    /* glBindBuffer() binds the buffer object named [m_RendererID]
       to the [GL_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    /* glBufferData() creates and initializes a new buffer object's data store */
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    /* glDeleteBuffers() deletes buffer object named [m_RendererID]
       leaving it without contents, with its name free for reuse */
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    /* glBindBuffer() binds the buffer object named [m_RendererID]
       to the [GL_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    /* glBindBuffer() unbinds the
       [GL_ARRAY_BUFFER] buffer binding point */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}