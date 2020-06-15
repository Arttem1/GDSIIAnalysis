#ifndef GDS_BOX_H
#define GDS_BOX_H

#include "gds_element.h"

class GDS_Box : public GDS_Element
{
public:
    typedef short BoxType;

    GDS_Box();

    BoxType Type();
    void SetType (BoxType theType);

private:
    BoxType myType;
};



#endif // GDS_BOX_H
