#ifndef GDS_READER_H
#define GDS_READER_H

#include <fstream>
#include <memory>
#include <vector>
#include "gds_recordtype.h"
#include "gds_element.h"
#include "gds_stream.h"
#include "gds_file.h"

class GDS_Box;
class GDS_ArrayReference;
class GDS_Reference;
class GDS_Structure;
class GDS_Bondary;
class GDS_Path;
class GDS_File;

class GDS_Reader
{
public:
    GDS_Reader();

    bool ReadFile (const std::string& theFileName);

    GDS_File File() const;

private:
    void ReadStructure();
    void ReadBox();
    void ReadPath();
    void ReadBoundary();
    void ReadReference();

    void ReadPoints();
    void ReadLayer();

    template <typename T, GDS_RecordType EndType = ENDEL>
    std::shared_ptr<T> ReadElement();

    template<typename T>
    bool ReadReference (const std::shared_ptr<T>& theReference);

    bool ReadAttributes (const std::shared_ptr<GDS_Path>& thePath);
    bool ReadAttributes (const std::shared_ptr<GDS_Element>& theElement);
    bool ReadAttributes (const std::shared_ptr<GDS_Box>& theBox);
    bool ReadAttributes (const std::shared_ptr<GDS_Bondary>& theBondary);
    bool ReadAttributes (const std::shared_ptr<GDS_Structure>& theElement);
    bool ReadAttributes (const std::shared_ptr<GDS_Reference>& theReference);
	bool ReadAttributes (const std::shared_ptr<GDS_ArrayReference>& theArrayReference);

    std::shared_ptr<GDS_Structure> GetStructure (const std::string& theName, bool theCreate);

private:
    typedef std::shared_ptr<GDS_Structure>    StructureType;
    typedef std::vector<StructureType>        StructuresType;

    StructuresType myCachedStructures;
    GDS_Stream     myStream;
    GDS_File       myFile;
};

#endif // GDS_READER_H
