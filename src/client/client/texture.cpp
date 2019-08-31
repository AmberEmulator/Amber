#include <client/texture.hpp>

#include <assert.h>
#include <vector>

#include <windows.h>
#include <gl/GL.h>

using namespace Amber;
using namespace Client;

namespace
{
	void CheckError()
	{
		GLenum error = glGetError();
		assert(error == GL_NO_ERROR);
	}
}

Texture::Texture(size_t a_Width, size_t a_Height)
{
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture); CheckError();
	
	glGenTextures(1, &m_NativeHandle); CheckError();
	glBindTexture(GL_TEXTURE_2D, m_NativeHandle); CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); CheckError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); CheckError();

	std::vector<uint32_t> data(a_Width * a_Height, 0xFF0000FF);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(a_Width), static_cast<GLsizei>(a_Height), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data()); CheckError();

	glBindTexture(GL_TEXTURE_2D, last_texture); CheckError();
}

Texture::~Texture() noexcept
{
	glDeleteTextures(1, &m_NativeHandle);
}

Texture::NativeHandle Texture::GetNativeHandle() const noexcept
{
	return m_NativeHandle;
}

void Texture::Blit(size_t a_X, size_t a_Y, size_t a_Width, size_t a_Height, const void* a_Data)
{
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture); CheckError();

	glBindTexture(GL_TEXTURE_2D, m_NativeHandle); CheckError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(a_X), static_cast<GLint>(a_Y), static_cast<GLsizei>(a_Width), static_cast<GLsizei>(a_Height), GL_RGBA, GL_UNSIGNED_BYTE, a_Data); CheckError();

	glBindTexture(GL_TEXTURE_2D, last_texture); CheckError();
}