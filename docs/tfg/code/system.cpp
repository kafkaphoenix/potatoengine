class System {
  public:
    System(int32_t priority = 0) : m_priority(priority) {}
    virtual ~System() = default;

    int32_t getPriority() const { return m_priority; }

    virtual void init(entt::registry& registry){};
    virtual void update(entt::registry& registry, const Time& ts){};

  protected:
    int32_t m_priority = 0;
};