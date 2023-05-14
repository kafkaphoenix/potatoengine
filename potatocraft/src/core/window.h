#pragma once

#include "src/pch.h"
#include "src/events/event.h"
#include "src/renderer/openGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace potatoengine {

	struct WindowProperties
	{
		std::string title;
		int width;
		int height;

		WindowProperties(const std::string& title, int width = 1280, int height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	enum class CursorMode
	{
		Normal = GLFW_CURSOR_NORMAL,
		Hidden = GLFW_CURSOR_HIDDEN,
		Disabled = GLFW_CURSOR_DISABLED
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

        int getWidth() const { return m_data.width; }
		int getHeight() const { return m_data.height; }

		void setCursorMode(CursorMode mode);
		void setUpdateCameraPosition(bool update) { m_data.updateCameraPosition = update; }
		void setLastMousePosition(float x, float y) { m_data.lastX = x; m_data.lastY = y; }

		static std::unique_ptr<Window> Create(const WindowProperties& properties);

    private:
        GLFWwindow* m_window;
		std::shared_ptr<OpenGLContext> m_context;

        struct WindowData {
            std::string title;
			int width, height;
			float lastX;
			float lastY;
			bool firstMouse = true;
			bool vSync;
			CursorMode cursorMode = CursorMode::Disabled;
			bool updateCameraPosition = true;

			EventCallbackFn eventCallback;
        };

        WindowData m_data;

		inline static uint8_t s_GLFWWindowCount{};
	};

}