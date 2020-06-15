#include "gds_reader.h"
#include "gds_element.h"
#include "gds_path.h"
#include "gds_bondary.h"
#include "gds_structure.h"
#include "gds_reference.h"
#include "gds_arrayReference.h"
#include "gds_file.h"
#include "gds_box.h"
#include "transformation2d.h"
#include "typedefs.h"
#include "cassert"
#include "gds_structureelement.h"
#include "gds_helper.h"
#include "gds_object.h"
#include <algorithm>
#include <iostream>

#include <string>

namespace {
Point2D<double> PointF (const Point2D<int>& thePoint)
{
    return Point2D<double> (thePoint.X(), thePoint.Y());
}
}

GDS_Reader::GDS_Reader()
{

}

GDS_File GDS_Reader::File() const
{
    return myFile;
}

bool GDS_Reader::ReadFile (const std::string& theFileName)
{
    myFile = GDS_File (theFileName);

    if (!myStream.Open (theFileName)) {
        return false;
    }

    while (myStream.ReadInfo()) {

        auto aRecordType = myStream.CurrentRecordType();

        if (aRecordType == BGNSTR) {
            myStream.SkipData();
            auto aStructure = ReadElement<GDS_Structure, ENDSTR>();

            auto aCachedStructure = GetStructure (aStructure->Name(), false);

            if (aCachedStructure) {
                *aCachedStructure = *aStructure;
            } else {
                myCachedStructures.push_back (aStructure);
            }
        } else {
            myStream.SkipData();
        }
    }

	
    for (const auto& aPtr : myCachedStructures) {
        if (aPtr.unique()) {
            assert (!myFile.MainStructure());
            myFile.SetMainStructure (aPtr);
        }
        myFile.AddStucture (aPtr);
    }
	
    myCachedStructures.clear();
    myStream.Close();
    return true;
}

template <typename T, GDS_RecordType EndType>
std::shared_ptr<T> GDS_Reader::ReadElement()
{
    auto anElement = std::make_shared<T>(); // AttributesReader<T> aReader;

    while (myStream.ReadInfo()) { // 4 bytes // or while (true)

        if (myStream.CurrentRecordType() == EndType) {
			break;
        }

        auto aRes = ReadAttributes (anElement); // data // aReader.Read();

        if (!aRes) {
            myStream.SkipData();
        }
    }

    return anElement; // aReader.Element();
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_Structure>& theStructure)
{
    auto aRecordType = myStream.CurrentRecordType();

    if (aRecordType == BOX) {
        auto aBox = ReadElement<GDS_Box>();
        theStructure->AddElement (std::make_shared<GDS_Object> (aBox, GDS_Object::Box));
        return true;
    }

    if (aRecordType == BONDARY) {
        auto aBondary = ReadElement<GDS_Bondary>();
        theStructure->AddElement (std::make_shared<GDS_Object> (aBondary, GDS_Object::Bondary));
        return true;
    }

    if (aRecordType == PATH) {
        auto aPath = ReadElement<GDS_Path>();
        aPath->ToPolygon();
        theStructure->AddElement (std::make_shared<GDS_Object> (aPath, GDS_Object::Path));
        return true;
    }

    if (aRecordType == SREF) {
        auto aReference = ReadElement<GDS_Reference>();
        theStructure->AddElement (std::static_pointer_cast<GDS_StructureElement> (aReference));
        return true;
    }

	if (aRecordType == AREF) {
		auto aReference = ReadElement<GDS_ArrayReference>();
		theStructure->AddElement(std::static_pointer_cast<GDS_StructureElement> (aReference));
		return true;
	}

    if (aRecordType == STRNAME) {
        std::string aName;
        myStream.ReadData (aName);
        theStructure->SetName (aName);
        return true;
    }

    return false;
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_Path>& thePath)
{
    auto aReaded = ReadAttributes (std::static_pointer_cast<GDS_Element> (thePath));

    if (aReaded) {
        return true;
    }

    if (myStream.CurrentRecordType() == WIDTH) {
        GDS_Path::WidthType aWidth;
        myStream.ReadData (aWidth);
        thePath->SetWidth (aWidth);
        return true;
    }
    if (myStream.CurrentRecordType() == PATHTYPE) {
        short aShortPath;
        myStream.ReadData (aShortPath);
        thePath->SetType (static_cast<GDS_Path::PathType> (aShortPath));
        return true;
    }

    return false;
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_Bondary>& theBondary)
{
    return ReadAttributes (std::static_pointer_cast<GDS_Element> (theBondary));
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_Box>& theBox)
{
    auto aReaded = ReadAttributes (std::static_pointer_cast<GDS_Element> (theBox));

    if (aReaded) {
        return true;
    }

    if (myStream.CurrentRecordType() == BOXTYPE) {
        GDS_Box::BoxType aType;
        myStream.ReadData (aType);
        theBox->SetType (aType);
        return true;
    }

    return false;
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_Element>& theElement)
{
    if (myStream.CurrentRecordType() == LAYER) {
        LayerType aLayer;
        myStream.ReadData (aLayer);
        theElement->SetLayer (aLayer);
        return true;
    }

    if (myStream.CurrentRecordType() == XY) {
        GDS_Element::CoordinatesType aCoordinates;
        myStream.ReadData (aCoordinates);
        theElement->SetCoordinates (aCoordinates);
        return true;
    }

    return false;
}

std::shared_ptr<GDS_Structure> GDS_Reader::GetStructure (const std::string& theName, bool theCreate)
{
    auto anIt = std::find_if (myCachedStructures.begin(), myCachedStructures.end(), [&theName] (const StructureType& theStructure) {
        return theStructure->Name() == theName;
    });

    auto aFounded = anIt != myCachedStructures.end();

    std::shared_ptr<GDS_Structure> aStructure;

    if (aFounded) {
        aStructure = *anIt;
    } else if (theCreate) {
        aStructure = std::make_shared<GDS_Structure>();
        aStructure->SetName (theName);
        myCachedStructures.push_back (aStructure);
    }

    return aStructure;
}

template<typename T>
bool GDS_Reader::ReadReference (const std::shared_ptr<T>& theReference)
{
    if (myStream.CurrentRecordType() == SNAME) {
        std::string aName;
        myStream.ReadData(aName);

        auto aStructure = GetStructure(aName, true);

        theReference->SetReference(aStructure);

        return true;
    }

    if (myStream.CurrentRecordType() == STRANS) {
        Transformation2D::TransformationType aType;
        myStream.ReadData((short&)aType);
        theReference->Transformation().SetType (aType);
        return true;
    }

    if (myStream.CurrentRecordType() == ANGLE) {
        Transformation2D::AngleType anAngle;
        myStream.ReadData(anAngle);
        theReference->Transformation().SetAngle (anAngle);
        return true;
    }

    if (myStream.CurrentRecordType() == MAG) {
        Transformation2D::MagType aMag;
        myStream.ReadData(aMag);
        theReference->Transformation().SetMag (aMag);
        return true;
    }

    return false;
}

bool GDS_Reader::ReadAttributes (const std::shared_ptr<GDS_ArrayReference>& theArrayReference)
{
    if (ReadReference (theArrayReference)) {
        return true;
    }

    if (myStream.CurrentRecordType() == XY) {
        GDS_Element::CoordinatesType aCoordinates;
        myStream.ReadData (aCoordinates);
        assert (aCoordinates.size() == 3);
        theArrayReference->Transformation().SetPosition (aCoordinates[0]);
        theArrayReference->SetStepX (aCoordinates[1]);
        theArrayReference->SetStepY (aCoordinates[2]);
        return true;
    }


    if (myStream.CurrentRecordType() == COLROW) {
        GDS_ArrayReference::COL aColumns;
        myStream.ReadData (aColumns);
        theArrayReference->SetColumns (aColumns);

        GDS_ArrayReference::ROW aRows;
        myStream.ReadData (aRows);
        theArrayReference->SetRows (aRows);
        return true;
    }

    return false;
}

bool GDS_Reader::ReadAttributes(const std::shared_ptr<GDS_Reference>& theReference)
{
    if (ReadReference (theReference)) {
        return true;
    }
	
	if (myStream.CurrentRecordType() == XY) {
		Point2D<int> aPosition;
		myStream.ReadData(aPosition);
        theReference->Transformation().SetPosition (PointF (aPosition));
		return true;
	}

	return false;
}
