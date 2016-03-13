#include "ChatColor.h"

const std::string ChatColor::ESCAPE = "\u00a7";

const std::string ChatColor::BLACK = ChatColor::ESCAPE + "0";
const std::string ChatColor::DARK_BLUE = ChatColor::ESCAPE + "1";
const std::string ChatColor::DARK_GREEN = ChatColor::ESCAPE + "2";
const std::string ChatColor::DARK_AQUA = ChatColor::ESCAPE + "3";
const std::string ChatColor::DARK_RED = ChatColor::ESCAPE + "4";
const std::string ChatColor::DARK_PURPLE = ChatColor::ESCAPE + "5";
const std::string ChatColor::GOLD = ChatColor::ESCAPE + "6";
const std::string ChatColor::GRAY = ChatColor::ESCAPE + "7";
const std::string ChatColor::DARK_GRAY = ChatColor::ESCAPE + "8";
const std::string ChatColor::BLUE = ChatColor::ESCAPE + "9";
const std::string ChatColor::GREEN = ChatColor::ESCAPE + "a";
const std::string ChatColor::AQUA = ChatColor::ESCAPE + "b";
const std::string ChatColor::RED = ChatColor::ESCAPE + "c";
const std::string ChatColor::LIGHT_PURPLE = ChatColor::ESCAPE + "d";
const std::string ChatColor::YELLOW = ChatColor::ESCAPE + "e";
const std::string ChatColor::WHITE = ChatColor::ESCAPE + "f";

const std::string ChatColor::OBFUSCATED = ChatColor::ESCAPE + "k";
const std::string ChatColor::BOLD = ChatColor::ESCAPE + "l";
const std::string ChatColor::STRIKETHROUGH = ChatColor::ESCAPE + "m";
const std::string ChatColor::UNDERLINE = ChatColor::ESCAPE + "n";
const std::string ChatColor::ITALIC = ChatColor::ESCAPE + "o";
const std::string ChatColor::RESET = ChatColor::ESCAPE + "r";