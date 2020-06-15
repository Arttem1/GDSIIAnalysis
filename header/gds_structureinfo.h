#ifndef GDS_STRUCTUREINFO_H
#define GDS_STRUCTUREINFO_H

#include "typedefs.h"

class GDS_StructureInfo
{
public:
    GDS_StructureInfo();

    LayersSet myActiveLayers;
    size_t     myBoxesCount = 0;
    size_t     myBondaryCount = 0;
    size_t     myPathCount = 0;
    size_t     myReferencesCountByMe = 0;

    //size_t     myReferencesCountToMe = 0;
};

#endif // GDS_STRUCTUREINFO_H
