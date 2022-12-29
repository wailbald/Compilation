#pragma once

#include <iostream>
#include <regex>
#include <string>

#include "token.hpp"

const std::string line_terminator(";{");

std::string read_line(FILE* f);