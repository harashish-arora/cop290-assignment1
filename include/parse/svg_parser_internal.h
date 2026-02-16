// svg_parser_internal.h — Internal helpers for SVG parsing
#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "shapes/graphics_object.h"

namespace SvgParser {

using AttrMap = std::map<std::string, std::string>;
using ShapeVec = std::vector<std::shared_ptr<GraphicsObject>>;

AttrMap parseAttributes(const std::string& tag);

double num(const AttrMap& m, const std::string& key, double fallback = 0.0);

std::string str(const AttrMap& m, const std::string& key,
                const std::string& fallback = "");

std::string unescapeXml(std::string s);
std::string rebuildColor(const AttrMap& m, const std::string& prefix);

void parseRect(const AttrMap& a, ShapeVec& out);
void parseCircle(const AttrMap& a, ShapeVec& out);
void parseEllipse(const AttrMap& a, ShapeVec& out);
void parseLine(const AttrMap& a, ShapeVec& out);
void parseText(const AttrMap& a, const std::string& raw, ShapeVec& out);
void parsePolyline(const AttrMap& a, ShapeVec& out);
void parsePolygon(const AttrMap& a, ShapeVec& out);

}  // namespace SvgParser
