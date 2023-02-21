#pragma once

#include "src/core/base.hpp"
#include "src/event/event.hpp"
#include "src/renderer/openGLContext.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>

namespace potatocraft {

	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Potatocraft",
			        uint32_t width = 1280,
			        uint32_t height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowProps& props);
		~Window();

        void init(const WindowProps& props);
        void shutdown();
		void onUpdate();

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) { m_data.eventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const { return m_data.vSync; }

		void* getNativeWindow() const { return m_window; }

        uint32_t getWidth() const { return m_data.width; }
		uint32_t getHeight() const { return m_data.height; }

		static Scope<Window> create(const WindowProps& props = WindowProps());

    private:
        GLFWwindow* m_window;
		Scope<OpenGLContext> m_context;

        struct WindowData {
            std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
        };

        WindowData m_data;
	};

}