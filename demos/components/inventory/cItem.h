#pragma once

#include "engineAPI.h"

namespace demos {

struct CItem {
    // TODO define
    std::string name{};
    std::string description{};
    std::string icon{};
    std::string model{};
    int value{};

    CItem() = default;
    explicit CItem(std::string&& n, std::string&& d, std::string&& i,
                   std::string&& m, int v)
      : name(std::move(n)), description(std::move(d)), icon(std::move(i)),
        model(std::move(m)), value(v) {}

    void print() const {
      APP_BACKTRACE("\t\tname: {0}\n\t\t\t\tdescription: {1}\n\t\t\t\ticon: "
                    "{2}\n\t\t\t\tmodel: {3}\n\t\t\t\tvalue: {4}",
                    name, description, icon, model, value);
    }

    std::map<std::string, std::string, engine::NumericComparator> getInfo() const {
      std::map<std::string, std::string, engine::NumericComparator> info;
      info["name"] = name;
      info["description"] = description;
      info["icon"] = icon;
      info["model"] = model;
      info["value"] = std::to_string(value);

      return info;
    }
};
}