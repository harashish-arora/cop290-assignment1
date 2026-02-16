// svg_parser.h
// reads an SVG file and returns a vector of GraphicsObjects
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "shapes/graphics_object.h"

namespace SvgParser {

// parse an SVG file and return the shapes it contains.
std::vector<std::shared_ptr<GraphicsObject>> load(const std::string& filePath);

}  // namespace SvgParser
