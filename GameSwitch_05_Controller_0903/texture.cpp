
#include "main.h"




unsigned int LoadTexture(const char *FileName)
{

	nn::Result result;
	nn::fs::FileHandle file;
	size_t readSize;

	result = nn::fs::OpenFile(&file, FileName, nn::fs::OpenMode_Read);
	if (nn::fs::ResultPathNotFound::Includes(result))
	{
		NN_ASSERT(false, "OpenFile:", FileName);
	}


	unsigned int	texture;
	unsigned char	header[18];
	unsigned char	*image;
	unsigned int	width, height;
	unsigned char	depth;
	unsigned char	option;
	unsigned int	bpp;
	unsigned int	format;


	// �w�b�_�ǂݍ���
	nn::fs::ReadFile(&readSize, file, 0, header, sizeof(header));

	// �摜�T�C�Y�擾
	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];
	option = header[17];

	if (depth == 32)
	{
		bpp = 4;
		format = GL_RGBA;
	}
	else if (depth == 24)
	{
		bpp = 3;
		format = GL_RGB;
	}
	else
	{
		bpp = 0;
		NN_ASSERT( false, "LoadTexture:", FileName );
	}


	// �������m��
	image = (unsigned char*)new unsigned char[width * height * bpp];

	// �摜�ǂݍ���
	nn::fs::ReadFile(&readSize, file, sizeof(header), image, width * height * bpp);


	nn::fs::CloseFile(file);



	// R<->B
	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			unsigned char c;
			c = image[(y * width + x) * bpp + 0];
			image[(y * width + x) * bpp + 0] = image[(y * width + x) * bpp + 2];
			image[(y * width + x) * bpp + 2] = c;
		}
	}


	// �㉺���]
	if(!(option & 0x20))
	{
		for (unsigned int y = 0; y < height / 2; y++)
		{
			for (unsigned int x = 0; x < width * bpp; x++)
			{
				unsigned char c;
				c = image[y * width * bpp + x];
				image[y * width * bpp + x] = image[(height - y - 1) * width * bpp + x];
				image[(height - y - 1) * width * bpp + x] = c;
			}
		}
	}



	// �e�N�X�`������
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);

	// �~�b�v�}�b�v
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, 0);

	delete image;

	return texture;
}



void UnloadTexture(unsigned int Texture)
{

	glDeleteTextures(1, &Texture);

}





void SetTexture(unsigned int Texture)
{
	if (Texture == 0)
	{
		glUniform1i(glGetUniformLocation(GetShaderProgramId(), "uTextureEnable"), 0);
	}
	else
	{
		glUniform1i(glGetUniformLocation(GetShaderProgramId(), "uTextureEnable"), 1);
		glBindTexture(GL_TEXTURE_2D, Texture);
	}
}


