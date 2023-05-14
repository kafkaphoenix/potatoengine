#pragma once
#include "src/core/application.h"

extern potatoengine::Application* potatoengine::CreateApp(const std::string &name, CommandLineArgs args);

int main(int argc, char **argv) // TODO create entrypoint in core and create class app inhereting from application
{
  potatoengine::Log::Init();

  auto app = potatoengine::CreateApp("potatocraft", {argc, argv});

  app->run();

  delete app;
}