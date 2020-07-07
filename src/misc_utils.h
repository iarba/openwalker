#ifndef MISC_UTILS_H
#define MISC_UTILS_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

bool eq(double a, double b);

bool eq(double a, double b, double eps);

bool eq(glm::dvec2 a, glm::dvec2 b);

bool eq(glm::dvec2 a, glm::dvec2 b, double eps);

std::vector<std::string> tokenize(std::string origin);

#endif // MISC_UTILS_H
