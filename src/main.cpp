#include "src/core/base.hpp"
#include "src/core/application.hpp"

int main(int argc, char **argv)
{
  potatocraft::Log::init();

  potatocraft::Application app("potatocraft", {argc, argv});

  app.run();
}