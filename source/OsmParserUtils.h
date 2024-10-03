#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "LatLong.h"
#include "tinyxml2.h"

struct FFeatureGeometry;

namespace Osm {

struct OsmComponent {
	std::unordered_map<std::string, std::string> tags;
	void AddTags(tinyxml2::XMLElement* source);
	bool IsBuilding() const;
	virtual void AddGeometry(FFeatureGeometry& shape, LatLong lowerCorner, LatLong upperCorner) const = 0;
};

struct OsmNode : public OsmComponent {
	OsmNode() {}
	OsmNode(const LatLong& c) { coordinate = c; }
	LatLong coordinate;
	void AddGeometry(FFeatureGeometry& shape, LatLong lowerCorner, LatLong upperCorner) const override;
};

struct OsmWay : public OsmComponent {
	OsmWay() {}
	OsmWay(const std::vector<OsmNode*>& n) { nodes = n; }
	std::vector<OsmNode*> nodes;
	void AddGeometry(FFeatureGeometry& shape, LatLong lowerCorner, LatLong upperCorner) const override;
};

struct OsmRelation : public OsmComponent {
	std::vector<std::pair<OsmComponent*, std::string>> relations;
	void AddGeometry(FFeatureGeometry& shape, LatLong lowerCorner, LatLong upperCorner) const override;
};

typedef std::unordered_map<uint64_t, OsmNode> NodeCache;
typedef std::unordered_map<uint64_t, OsmWay> WayCache;
typedef std::unordered_map<uint64_t, OsmRelation> RelationCache;

struct OsmCache {
	NodeCache nodes;
	WayCache ways;
	RelationCache relations;
};
}