#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

/* Definition of Vertex Array */
VertexArray::VertexArray()
{
	/* glGenVertexArrays() generates a vertex array object name
	   in [m_RendererID] for the vertex array*/
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	/* glDeleteVertexArrays() deletes vertex array named [m_RendererID]
	   leaving it without contents, with its name is unused */
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();  //bind the vertex array
	vb.Bind();	//bind the vertex buffer
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		/*  glEnableVertexAttribArray() enables the 
		    generic vertex attribute array specified by index [i] */
		GLCall(glEnableVertexAttribArray(i));
		/* glVertexAttribPointer() specifies the location and data format 
		   of the array of generic vertex attributes at index [i] */
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	/* glBindVertexArray() binds the vertex array object 
	   named [m_RendererID] */
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	/* glBindVertexArray() unbinds the vertex array object 
	   named [m_RendererID] */
	GLCall(glBindVertexArray(0));
}