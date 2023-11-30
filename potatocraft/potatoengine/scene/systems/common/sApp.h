#pragma once

#include <imgui.h>

#include "potatoengine/core/application.h"
#include "potatoengine/events/appEvent.h"
#include "potatoengine/events/windowEvent.h"
#include "potatoengine/pch.h"

namespace potatoengine {

bool onWindowClosed(WindowCloseEvent& e) {
    Application::Get().close();

    return true;
}

bool onWindowRestored(WindowRestoredEvent& e) {
    Application::Get().pause(false);

    return true;
}

bool onWindowMinimized(WindowMinimizedEvent& e) {
    Application::Get().pause(true);

    return true;
}

bool onWindowMaximized(WindowMaximizedEvent& e) {
    return true;
}

bool onWindowFocus(WindowFocusEvent& e) {
    return true;
}

bool onWindowMoved(WindowMovedEvent& e) {
    return true;
}

bool onWindowResized(WindowResizeEvent& e) {
    RendererAPI::SetViewport(0, 0, e.getWidth(), e.getHeight());
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    return true;
}

bool onAppTick(AppTickEvent& e) {
    return true;
}

bool onAppUpdate(AppUpdateEvent& e) {
    return true;
}

bool onAppRender(AppRenderEvent& e) {
    return true;
}

void appSystem(Event& e) {
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(BIND_STATIC_EVENT(onWindowClosed));
    dispatcher.dispatch<WindowRestoredEvent>(BIND_STATIC_EVENT(onWindowRestored));
    dispatcher.dispatch<WindowMinimizedEvent>(BIND_STATIC_EVENT(onWindowMinimized));
    dispatcher.dispatch<WindowMaximizedEvent>(BIND_STATIC_EVENT(onWindowMaximized));
    dispatcher.dispatch<WindowResizeEvent>(BIND_STATIC_EVENT(onWindowResized));
    dispatcher.dispatch<WindowFocusEvent>(BIND_STATIC_EVENT(onWindowFocus));
    dispatcher.dispatch<WindowMovedEvent>(BIND_STATIC_EVENT(onWindowMoved));

    dispatcher.dispatch<AppTickEvent>(BIND_STATIC_EVENT(onAppTick));
    dispatcher.dispatch<AppUpdateEvent>(BIND_STATIC_EVENT(onAppUpdate));
    dispatcher.dispatch<AppRenderEvent>(BIND_STATIC_EVENT(onAppRender));
}

}