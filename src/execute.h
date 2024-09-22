#pragma once

#include <string_view>
#include <unordered_map>
#include "parser.h"

bool execute(const FiniteStateMachine& fsm, std::string_view input, std::ostream& stream);
