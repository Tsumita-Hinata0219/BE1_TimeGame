#pragma once

#include <string>
#include <future>
#include "../utile/utile.h"


std::future<std::string> PostScoreAsync(int score);
std::future<std::string> GetAllScoresAsync();

