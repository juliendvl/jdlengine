#include "core/Application.hpp"

#include "utils/Logger.hpp"


namespace jdl
{
namespace core
{

static void APIENTRY s_DebugCallback(GLenum source,
                                     GLenum type,
                                     unsigned int id,
                                     GLenum severity,
                                     GLsizei length,
                                     const char* message,
                                     const void* userData)
{
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        return;
    }

    std::string messageFormat = "OpenGL {}: {}";
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            JDL_ERROR(messageFormat, id, message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            JDL_WARN(messageFormat, id, message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            JDL_DEBUG(messageFormat, id, message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            JDL_TRACE(messageFormat, id, message);
            break;
        default:
            break;
    }
}


Application* Application::IApplication = nullptr;

Application::Application(const char* name, int width, int height)
    : m_name(name)
{
    if (IApplication != nullptr)
    {
        JDL_FATAL("An application already exists!");
    }
    IApplication = this;

    JDL_INFO("Application - Create window");
    m_window = std::make_unique<Window>(name, width, height);

#ifndef NDEBUG
    setupDebugContext();
#endif // !NDEBUG

    JDL_INFO("Application - Create renderer");
    m_renderer = std::make_unique<Renderer>();
}

Application::~Application()
{
    JDL_INFO("Application - Destroy renderer");
    m_renderer.reset();

    JDL_INFO("Application - Destroy window");
    m_window.reset();
}

void Application::run()
{
    while (m_window->isRunning())
    {
        m_window->swapBuffers();
        m_window->pollEvents();
        m_renderer->renderFrame();
    }
}

void Application::setupDebugContext()
{
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        JDL_INFO("Application - Setup the OpenGL debug context");

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(s_DebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
    else
    {
        JDL_WARN("Application - The OpenGL debug context is not supported!");
    }
}

} // namespace core
} // namespace jdl
