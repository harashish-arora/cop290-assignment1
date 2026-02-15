// svg_parser.h — Reads an SVG file and returns a vector of GraphicsObjects
#pragma once
#include <memory>
#include <string>
#include <vector>

#include "shapes/graphics_object.h"

namespace SvgParser {

// Parse an SVG file and return the shapes it contains.
std::vector<std::shared_ptr<GraphicsObject>> load(const std::string& filePath);

}  // namespace SvgParser
