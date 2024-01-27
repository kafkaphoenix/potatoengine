#pragma once

#include "pch.h"

namespace potatoengine {

struct NumericComparator {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
      if (lhs == rhs) {
        return false;
      }

      if (lhs.empty()) {
        return true;
      }

      if (rhs.empty()) {
        return false;
      }

      if (std::isdigit(lhs[0]) and not std::isdigit(rhs[0])) {
        return true;
      }

      if (!std::isdigit(lhs[0]) and std::isdigit(rhs[0])) {
        return false;
      }

      if (!std::isdigit(lhs[0]) and not std::isdigit(rhs[0])) {
        if (lhs[0] == rhs[0]) {
          return NumericComparator{}(lhs.substr(1), rhs.substr(1));
        }
        return lhs[0] < rhs[0];
      }

      // Both lhs and rhs start with digit -> parse both numbers
      std::istringstream lhsStream{lhs};
      std::istringstream rhsStream{rhs};

      int lhsNumber{};
      lhsStream >> lhsNumber;

      int rhsNumber{};
      rhsStream >> rhsNumber;

      if (lhsNumber == rhsNumber) {
        return NumericComparator{}(lhs.substr(lhsStream.tellg()),
                                   rhs.substr(rhsStream.tellg()));
      }

      return lhsNumber < rhsNumber;
    }
};
}