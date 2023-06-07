#include "potatoengine/renderer/rendererAPI.h"

#include <glad/glad.h>

namespace potatoengine {

void APIENTRY message_callback(GLenum source, GLenum type, uint32_t id, GLenum severity, GLsizei, GLchar const* message, void const*) {
    std::string_view _source = [source]() {
        switch (source) {
            case GL_DEBUG_SOURCE_API:
                return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION:
                return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER:
                return "OTHER";
            default:
                return "UNDEFINED SOURCE";
        }
    }();

    std::string_view _type = [type]() {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY:
                return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE:
                return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER:
                return "MARKER";
            case GL_DEBUG_TYPE_OTHER:
                return "OTHER";
            default:
                return "UNDEFINED TYPE";
        }
    }();

    std::string_view _severity = [severity]() {
        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                return "NOTIFICATION";
            case GL_DEBUG_SEVERITY_LOW:
                return "LOW";
            case GL_DEBUG_SEVERITY_MEDIUM:
                return "MEDIUM";
            case GL_DEBUG_SEVERITY_HIGH:
                return "HIGH";
            default:
                return "UNDEFINED SEVERITY";
        }
    }();

    std::string info = std::format(
        "Source[{}]| Type [{}]| Severity [{}]| ID [{}]| Message: {}\n",
        _source,
        _type,
        _severity,
        id,
        message);

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            CORE_CRITICAL(info);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            CORE_ERROR(info);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            CORE_WARN(info);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            CORE_TRACE(info);
            return;
    }
}

void RendererAPI::Init() {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(message_callback, nullptr);

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    glEnable(GL_CULL_FACE);  // BACK FACE CULLING CCW
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
}

void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    glViewport(x, y, width, height);
}

void RendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::SetWireframe(bool enabled) {
    enabled ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexed(const std::shared_ptr<VAO>& vao) {
    vao->bind();
    glDrawElements(GL_TRIANGLES, vao->getEBO()->getCount(), GL_UNSIGNED_INT, nullptr);
    vao->unbind();
}

}