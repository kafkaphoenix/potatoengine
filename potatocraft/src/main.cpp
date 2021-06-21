#include <src/core/base.hpp>
#include <src/core/application.hpp>

int main(int argc, char** argv)
{
    potatocraft::Application app("potatocraft", {argc, argv});

    app.run();
}