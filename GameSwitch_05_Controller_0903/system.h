#pragma once


#include <nn/nn_Assert.h>
#include <nn/nn_Log.h>
#include <nn/os.h>
#include <nn/fs.h>
#include <nn/util/util_Vector.h>
#include <nn/util/util_Color.h>
#include <nn/util/util_MathTypes.h>


#if defined( NN_BUILD_TARGET_PLATFORM_OS_NN ) && defined( NN_BUILD_APISET_NX )
#include <GLES3/gl32.h>
#else
#include <GL/glew.h>
#define GL_APIENTRY GLAPIENTRY
#endif

#include "GraphicsHelper.h"

using namespace nn::util;



GLuint GetShaderProgramId();

void InitSystem();
void UninitSystem();

void SwapBuffers();


