#include "gds_graphelement.h"
#include "gds_structureelement.h"
#include "gds_element.h"
#include "printhelper.h"

#include <cassert>
#include <iostream>
#include <iterator>

size_t KeyHash::operator() (const Key& theValue) const
{
    size_t aSeed = 0;
    for (const auto& anAncestor : theValue.second) {
        hash_combine (aSeed, anAncestor->Hash());
    }

    GDS_ElementHash anElementHasher;
    auto anElementHash = anElementHasher (theValue.first);
    hash_combine (aSeed, anElementHash);
    hash_combine (aSeed, theValue.second.size());

    return aSeed;
}

bool KeyEqual::operator() (const Key& theFirst, const Key& theSecond) const
{
    return theFirst.second.size() == theSecond.second.size()
        || theFirst.first->Polygon().Coordinates().size() == theSecond.first->Polygon().Coordinates().size()
        || theFirst.first->BoundingBox() == theSecond.first->BoundingBox();
}

GDS_GraphElement::GDS_GraphElement (const GDS_GraphElement& theElement)
{
    myElement = theElement.myElement;
    myAncestors = theElement.myAncestors;
    myNumber = theElement.myNumber;
}

GDS_GraphElement::GDS_GraphElement (const GDS_ElementPtr& theElement, const AncetorsElements& theAncestors, const uint64_t& theNumber) :
    myElement (theElement),
    myAncestors (theAncestors)
{
    if (theNumber == 0) {
        ++counter;
        myNumber = counter;
    } else {
        myNumber = theNumber;
    }

    if (myNumber == 9) {
        std::cout << "";
    }

}

GDS_GraphElement::GDS_GraphElement (const uint64_t& theNumber)
{
    myNumber = theNumber;
}

GDS_GraphElementPtr GDS_GraphElement::CopyWithout2FirstAncestors() const
{
    auto aCopy = new GDS_GraphElement (myNumber);
    aCopy->myElement = myElement;
    if (myAncestors.size() > 2) {
        std::copy (myAncestors.begin() + 2, myAncestors.end(), std::front_inserter (aCopy->myAncestors));
    } else {
        assert (false);
    }
    return std::shared_ptr<GDS_GraphElement> (aCopy);
}

bool GDS_GraphElement::operator==(const GDS_GraphElement& theElement) const
{
    return myElement == theElement.myElement && myAncestors == theElement.myAncestors;
}

uint64_t GDS_GraphElement::counter = 0;

size_t GDS_GraphElementHash::operator() (const GDS_GraphElementPtr& theValue) const
{
    KeyHash aKeyHash;
    auto aHash = aKeyHash (Key (theValue->myElement, theValue->myAncestors));
    return aHash;
}

bool GDS_GraphElementEqual::operator() (const GDS_GraphElementPtr& theFirst, const GDS_GraphElementPtr& theSecond) const
{
    return theFirst->myAncestors.size() == theSecond->myAncestors.size()
        || theFirst->myElement->Polygon().Coordinates().size() == theSecond->myElement->Polygon().Coordinates().size()
        || theFirst->myElement->BoundingBox() == theSecond->myElement->BoundingBox();
}

bool GDS_GraphElementCompare::operator() (const GDS_GraphElementPtr& theFirst, const GDS_GraphElementPtr& theSecondValue) const
{
    return theFirst->myNumber < theSecondValue->myNumber;
}
