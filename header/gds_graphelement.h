#ifndef GDS_GRAPHELEMENT_H
#define GDS_GRAPHELEMENT_H

#include "typedefs.h"

class KeyHash
{
public:
    size_t operator() (const Key& theValue) const;
};

class KeyEqual
{
public:
    bool operator() (const Key& theFirst, const Key& theSecond) const;
};

class GDS_GraphElement
{
public:
    bool operator==(const GDS_GraphElement& theElement) const;

    GDS_ElementPtr   myElement;
    AncetorsElements myAncestors;
    uint64_t         myNumber;

    static uint64_t  counter;

    GDS_GraphElement (const GDS_GraphElement& theElement);

    GDS_GraphElement (const GDS_ElementPtr& theElement, const AncetorsElements& theAncestors, const uint64_t& theNumber = 0);

private:
    friend class GDS_Graph;
    friend class GDS_GraphFactory;
    friend class IntersectionFounder;
    GDS_GraphElement (const uint64_t& theNumber);

    GDS_GraphElementPtr CopyWithout2FirstAncestors() const;
};

class GDS_GraphElementHash
{
public:
    size_t operator() (const GDS_GraphElementPtr& theValue) const;
};

class GDS_GraphElementEqual
{
public:
    bool operator() (const GDS_GraphElementPtr& theFirst, const GDS_GraphElementPtr& theSecondValue) const;
};

class GDS_GraphElementCompare
{
public:
    bool operator() (const GDS_GraphElementPtr& theFirst, const GDS_GraphElementPtr& theSecondValue) const;
};

#endif // GDS_GRAPHELEMENT_H
