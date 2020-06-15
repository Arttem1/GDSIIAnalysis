#include "gds_box.h"

GDS_Box::GDS_Box()
{

}

GDS_Box::BoxType GDS_Box::Type()
{
    return myType;
}

void GDS_Box::SetType (BoxType theType)
{
    myType = theType;
}
