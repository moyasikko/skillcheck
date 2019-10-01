/*--------------------------------------------------------------------------------*
  Copyright (C)Nintendo All rights reserved.

  These coded instructions, statements, and computer programs contain proprietary
  information of Nintendo and/or its licensed developers and are protected by
  national and international copyright laws. They may not be disclosed to third
  parties or copied or duplicated in any form, in whole or in part, without the
  prior written consent of Nintendo.

  The content herein is highly confidential and should be handled accordingly.
 *--------------------------------------------------------------------------------*/

#include <nn/nn_Assert.h>
#include <nn/vi.h>
#include <nv/nv_MemoryManagement.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <cstdlib>

#include "GraphicsHelper.h"

namespace
{
    EGLDisplay s_Display;
    EGLSurface s_Surface;
    EGLContext s_Context;

    nn::vi::NativeWindowHandle  s_NativeWindowHandle;
    nn::vi::Display*            s_pDisplay;
    nn::vi::Layer*              s_pLayer;

    void* NvAllocateFunction(size_t size, size_t alignment, void* userPtr)
    {
        NN_UNUSED(userPtr);
        // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
        return aligned_alloc(alignment, nn::util::align_up(size, alignment));
    }
    void NvFreeFunction(void* addr, void* userPtr)
    {
        NN_UNUSED(userPtr);
        free(addr);
    }
    void* NvReallocateFunction(void* addr, size_t newSize, void* userPtr)
    {
        NN_UNUSED(userPtr);
        return realloc(addr, newSize);
    }

    void* NvDevtoolsAllocateFunction(size_t size, size_t alignment, void* userPtr)
    {
        NN_UNUSED(userPtr);
        // According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
        return aligned_alloc(alignment, nn::util::align_up(size, alignment));
    }
    void NvDevtoolsFreeFunction(void* addr, void* userPtr)
    {
        NN_UNUSED(userPtr);
        free(addr);
    }
    void* NvDevtoolsReallocateFunction(void* addr, size_t newSize, void* userPtr)
    {
        NN_UNUSED(userPtr);
        return realloc(addr, newSize);
    }
}

void GraphicsHelper::Initialize()
{
        /*
         * Set memory allocator for graphics subsystem.
         * This function must be called before using any graphics API's.
         */
    nv::SetGraphicsAllocator(NvAllocateFunction, NvFreeFunction, NvReallocateFunction, NULL);

        /*
         * Set memory allocator for graphics developer tools and NVN debug layer.
         * This function must be called before using any graphics developer features.
         */
    nv::SetGraphicsDevtoolsAllocator(NvDevtoolsAllocateFunction, NvDevtoolsFreeFunction, NvDevtoolsReallocateFunction, NULL);
        /*
         * Donate memory for graphics driver to work in.
         * This function must be called before using any graphics API's.
         */
    size_t graphicsSystemMemorySize = 8 * 1024 * 1024;
    void* graphicsHeap = malloc(graphicsSystemMemorySize);
    nv::InitializeGraphics(graphicsHeap, graphicsSystemMemorySize);

        /*
         * Initialize Video Interface (VI) system to display
         * to the target's screen
         */
    nn::vi::Initialize();

    nn::Result result = nn::vi::OpenDefaultDisplay(&s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::CreateLayer(&s_pLayer, s_pDisplay);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::SetLayerScalingMode(s_pLayer, nn::vi::ScalingMode_FitToLayer);
    NN_ASSERT(result.IsSuccess());

    result = nn::vi::GetNativeWindow(&s_NativeWindowHandle, s_pLayer);
    NN_ASSERT(result.IsSuccess());

        /*
         * Initialize EGL
         */
    EGLBoolean eglResult;

    s_Display = ::eglGetDisplay(EGL_DEFAULT_DISPLAY);
    NN_ASSERT(s_Display != NULL, "eglGetDisplay failed.");

    eglResult = ::eglInitialize(s_Display, 0, 0);
    NN_ASSERT(eglResult, "eglInitialize failed.");

    EGLint configAttribs[] = {
            /*
             * Require OpenGL ES2/ES3 support.
             * If you need OpenGL (not ES) supprot, you should specify EGL_OPENGL_BIT.
             */
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_SAMPLES, 4,  // This is for 4x MSAA.
        EGL_NONE
    };
    EGLint numConfigs = 0;
    EGLConfig config;
    eglResult = ::eglChooseConfig(s_Display, configAttribs, &config, 1, &numConfigs);
    NN_ASSERT(eglResult && numConfigs == 1, "eglChooseConfig failed.");

    s_Surface = ::eglCreateWindowSurface(s_Display, config, static_cast<NativeWindowType>(s_NativeWindowHandle), 0);
    NN_ASSERT(s_Surface != EGL_NO_SURFACE, "eglCreateWindowSurface failed.");

        /*
         * Set the current rendering API to OpenGL ES.
         * If you need OpenGL, you should specify EGL_OPENGL_API.
         */
    eglBindAPI(EGL_OPENGL_ES_BIT);

        /*
         * Create new context and set it as current.
         */
    EGLint contextAttribs[] = {
            /*
             * Require OpenGL ES 3.2 compatibility.
             * If you need other GL version, you should specify it.
             */
        EGL_CONTEXT_MAJOR_VERSION, 3,
        EGL_CONTEXT_MINOR_VERSION, 2,
            /* For debug callback */
        EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
        EGL_NONE
    };
    s_Context = ::eglCreateContext(s_Display, config, EGL_NO_CONTEXT, contextAttribs);
    NN_ASSERT(s_Context != EGL_NO_CONTEXT, "eglCreateContext failed. %d", eglGetError());

    eglResult = ::eglMakeCurrent(s_Display, s_Surface, s_Surface, s_Context);
    NN_ASSERT(eglResult, "eglMakeCurrent failed.");
}

void GraphicsHelper::Finalize()
{
    EGLBoolean eglResult;

    eglResult = ::eglMakeCurrent(s_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    NN_ASSERT(eglResult, "eglMakeCurrent failed.");
    eglResult = ::eglTerminate(s_Display);
    NN_ASSERT(eglResult, "eglTerminate failed.");
    eglResult = ::eglReleaseThread();
    NN_ASSERT(eglResult, "eglReleaseThread failed.");

    nn::vi::DestroyLayer(s_pLayer);
    nn::vi::CloseDisplay(s_pDisplay);
    nn::vi::Finalize();

    nv::FinalizeGraphics();
}

void GraphicsHelper::SwapBuffers()
{
    ::eglSwapBuffers(s_Display, s_Surface);
}
