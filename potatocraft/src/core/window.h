#pragma once

#include "src/core/base.h"
#include "src/event/event.h"
#include "src/renderer/openGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <sstream>

namespace potatoengine {

	struct WindowProperties
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProperties(const std::string& title = "Potatocraft",
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

        Window(const WindowProperties& properties);
		~Window();

        void init(const WindowProperties& properties);
        void shutdown();
		void onUpdate();

		// Window attributes
		void setEventCallback(const EventCallbackFn& callback) { m_data.eventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const { return m_data.vSync; }

		void* getNativeWindow() const { return m_window; }

        uint32_t getWidth() const { return m_data.width; }
		uint32_t getHeight() const { return m_data.height; }

		static Scope<Window> Create(const WindowProperties& properties);

    private:
        GLFWwindow* m_window;
		Scope<OpenGLContext> m_context;

        struct WindowData {
            std::string title;
			uint32_t width, height;
			bool vSync;

			EventCallbackFn eventCallback;
        };

        WindowData m_data;

		inline static uint8_t s_GLFWWindowCount{};
	};

}