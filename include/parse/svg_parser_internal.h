// svg_parser_internal.h
// helper functions for SVG parsing
#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "shapes/graphics_object.h"

// internal namespace for SVG parsing helpers and implementation details
namespace SvgParser {

// internal types for attribute maps and shape vectors
using AttrMap = std::map<std::string, std::string>;
using ShapeVec = std::vector<std::shared_ptr<GraphicsObject>>;

// parsing helpers for attributes and colors
AttrMap parseAttributes(const std::string& tag);

double num(const AttrMap& m, const std::string& key, double fallback = 0.0);

// get string attribute or fallback if not found
std::string str(const AttrMap& m, const std::string& key,
                const std::string& fallback = "");

// unescape XML entities in a string (e.g. &lt; → <)
std::string unescapeXml(std::string s);
std::string rebuildColor(const AttrMap& m, const std::string& prefix);

// parsing helpers for specific SVG elements
void parseRect(const AttrMap& a, ShapeVec& out);
void parseCircle(const AttrMap& a, ShapeVec& out);
void parseEllipse(const AttrMap& a, ShapeVec& out);
void parseLine(const AttrMap& a, ShapeVec& out);
void parseText(const AttrMap& a, const std::string& raw, ShapeVec& out);
void parsePolyline(const AttrMap& a, ShapeVec& out);
void parsePolygon(const AttrMap& a, ShapeVec& out);

}  // namespace SvgParser
