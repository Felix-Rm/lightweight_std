#pragma once

#ifdef ARDUINO
#    include "Arduino.h"
#else
#    include <string>
#endif

namespace lw_std {

#ifdef ARDUINO

// lw_std string delegates to Arduino String
using string = String;
template <typename T>
inline string to_string(T t) {
    return String(t);
};

#else

// lw_std string delegates to std::string
using string = std::string;
using std::to_string;

#endif

}  // namespace lw_std
