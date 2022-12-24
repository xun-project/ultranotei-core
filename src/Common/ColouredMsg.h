// Copyright (c) 2018, The TurtleCoin Developers
// Copyright (c) 2019-2020, The Lithe Project Development Team
// Copyright (c) 2017-2020 UltraNote developers
// 
// Please see the included LICENSE file for more information.

#pragma once

#include <Common/ConsoleTools.h>
#include <iomanip>
#include <ostream>
#include <string>

class ColouredMsg {
public:
  ColouredMsg(std::string msg, common::console::Color colour)
    : msg(msg), colour(colour) {}

  ColouredMsg(std::string msg, int padding,
    common::console::Color colour)
    : msg(msg), colour(colour), padding(padding), pad(true) {}


  /* Set the text colour, write the message, then reset. We use a class
   * as it seems the only way to have a valid << operator. We need this
   * so we can nicely do something like:
   *
   * std::cout << "Hello " << GreenMsg("user") << std::endl;
   * Without having to write:
   * std::cout << "Hello ";
   * GreenMsg("user");
   * std::cout << std::endl; 
   */

  friend std::ostream& operator<<(std::ostream& os, const ColouredMsg &m) {
    common::console::setTextColor(m.colour);

    if (m.pad) {
      os << std::left << std::setw(m.padding) << m.msg;
    } else {
      os << m.msg;
    }

    common::console::setTextColor(common::console::Color::Default);
    return os;
  }

protected:
  std::string msg;
  const common::console::Color colour;
  const int padding = 0;
  const bool pad = false;
};

///// SignalColouredMsg's

class SuccessMsg : public ColouredMsg {
public:
  explicit SuccessMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::Green) {}

  explicit SuccessMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::Green) {}
};

class InformationMsg : public ColouredMsg {
public:
  explicit InformationMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightYellow) {}

  explicit InformationMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding,
      common::console::Color::BrightYellow) {}
};

class SuggestionMsg : public ColouredMsg {
public:
  explicit SuggestionMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightBlue) {}

  explicit SuggestionMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding,
      common::console::Color::BrightBlue) {}
};

class WarningMsg : public ColouredMsg {
public:
  explicit WarningMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightRed) {}

  explicit WarningMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding,
      common::console::Color::BrightRed) {}
};

///// ColouredMsg's

class RedMsg : public ColouredMsg {
public:
  explicit RedMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::Red) {}

  explicit RedMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::Red) {}
};

class BrightRedMsg : public ColouredMsg {
public:
  explicit BrightRedMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightRed) {}

  explicit BrightRedMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::BrightRed) {}
};

class MagentaMsg : public ColouredMsg {
public:
  explicit MagentaMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::Magenta) {}

  explicit MagentaMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::Magenta) {}
};

class BrightMagentaMsg : public ColouredMsg {
public:
  explicit BrightMagentaMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightMagenta) {}

  explicit BrightMagentaMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::BrightMagenta) {}
};

class GreenMsg : public ColouredMsg {
public:
  explicit GreenMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::Green) {}

  explicit GreenMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::Green) {}
};

class BrightGreenMsg : public ColouredMsg {
public:
  explicit BrightGreenMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightGreen) {}

  explicit BrightGreenMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::BrightGreen) {}
};

class YellowMsg : public ColouredMsg {
public:
  explicit YellowMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::Yellow) {}

  explicit YellowMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::Yellow) {}
};

class BrightYellowMsg : public ColouredMsg {
public:
  explicit BrightYellowMsg(std::string msg)
    : ColouredMsg(msg, common::console::Color::BrightYellow) {}

  explicit BrightYellowMsg(std::string msg, int padding)
    : ColouredMsg(msg, padding, common::console::Color::BrightYellow) {}
};