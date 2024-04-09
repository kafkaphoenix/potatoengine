class DemoApp : public engine::Application {
  public:
    DemoApp(std::unique_ptr<engine::SettingsManager>&& s, engine::CLArgs&& args)
      : engine::Application(std::move(s), std::move(args)) {}

    m_states_manager->push_state(states::MenuState::Create());

    ~Demos() override { APP_WARN("Deleting DemoApp"); }
};

engine::Application* engine::CreateApp(engine::CLArgs&& args) {
  auto settings_manager = serializers::load_settings("Demos");
  APP_INFO("Initializating Demos application");
  return new demos::Demos(std::move(settings_manager), std::move(args));
}