#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <deque>
#include <list>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

class GDS_ArrayReference;
class GDS_Element;
class GDS_Graph;
class GDS_GraphElement;
class GDS_GraphElementCompare;
class GDS_GraphElementEqual;
class GDS_GraphElementHash;
class GDS_GraphFactory;
class GDS_Object;
class GDS_Path;
class GDS_Reference;
class GDS_Structure;
class GDS_StructureElement;

typedef std::shared_ptr<GDS_ArrayReference>    GDS_ArrayReferencePtr;
typedef std::shared_ptr<GDS_Element>           GDS_ElementPtr;
typedef std::shared_ptr<GDS_Graph>             GDS_GraphPtr;
typedef std::shared_ptr<GDS_GraphFactory>      GDS_GraphFactoryPtr;
typedef std::shared_ptr<GDS_Object>            GDS_ObjectPtr;
typedef std::shared_ptr<GDS_Path>              GDS_PathPtr;
typedef std::shared_ptr<GDS_Reference>         GDS_ReferencePtr;
typedef std::shared_ptr<GDS_Structure>         GDS_StructurePtr;
typedef std::shared_ptr<GDS_StructureElement>  GDS_StructureElementPtr;


typedef short LayerType;
typedef std::deque<GDS_GraphPtr>              GDS_GraphList;
typedef std::deque<GDS_StructureElementPtr>   AncetorsElements;
typedef std::unordered_set<LayerType>         LayersSet;
typedef std::list<LayerType>                  LayersList;
typedef std::vector<GDS_StructureElementPtr>  GDS_StructureElements;
typedef std::list<GDS_StructurePtr>           GDS_StructureList;

// GRAPH
typedef GDS_GraphElement GDS_GraphElementEdge;
typedef GDS_GraphElement GDS_GraphElementNode;

typedef std::shared_ptr<GDS_GraphElement>      GDS_GraphElementPtr;
typedef std::shared_ptr<GDS_GraphElementEdge>  GDS_GraphElementEdgePtr;
typedef std::shared_ptr<GDS_GraphElementNode>  GDS_GraphElementNodePtr;

typedef std::deque<GDS_GraphElementEdgePtr> GDS_GraphElementEdges;
typedef std::deque<GDS_GraphElementNodePtr> GDS_GraphElementNodes;

typedef std::unordered_map<GDS_GraphElementEdgePtr,
                           GDS_GraphElementNodes,
                           GDS_GraphElementHash,
                           GDS_GraphElementEqual>    ConnectionsType;
typedef std::set<GDS_GraphElementNodePtr, GDS_GraphElementCompare>  SearchContainer;

class KeyHash;
class KeyEqual;
typedef std::pair<GDS_ElementPtr, AncetorsElements>  Key;
typedef std::unordered_map<Key, GDS_GraphElementPtr, KeyHash, KeyEqual>    FactoryElementsCacheType;
typedef std::unordered_map<std::string, GDS_GraphPtr>    FactoryGraphCacheType;

const auto USE_PARALLELISM = false;


inline void hash_combine (std::size_t& seed, const int& v)
{
    std::hash<int> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

#endif // TYPEDEFS_H
