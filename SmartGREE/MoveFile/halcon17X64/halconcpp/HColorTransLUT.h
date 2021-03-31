/***********************************************************
 * File generated by the HALCON-Compiler hcomp version 17.12
 * Usage: Interface to C++
 *
 * Software by: MVTec Software GmbH, www.mvtec.com
 ***********************************************************/


#ifndef HCPP_HCOLORTRANSLUT
#define HCPP_HCOLORTRANSLUT

namespace HalconCpp
{

// Represents an instance of a color space transformation lookup table
class LIntExport HColorTransLUT : public HToolBase
{

public:

  // Create an uninitialized instance
  HColorTransLUT():HToolBase() {}

  // Copy constructor
  HColorTransLUT(const HColorTransLUT& source) : HToolBase(source) {}

  // Create HColorTransLUT from handle, taking ownership
  explicit HColorTransLUT(Hlong handle);

  // Set new handle, taking ownership
  void SetHandle(Hlong handle);



/*****************************************************************************
 * Operator-based class constructors
 *****************************************************************************/

  // create_color_trans_lut: Creates the look-up-table for transformation of an image from the RGB color space to an arbitrary color space.
  explicit HColorTransLUT(const HString& ColorSpace, const HString& TransDirection, Hlong NumBits);

  // create_color_trans_lut: Creates the look-up-table for transformation of an image from the RGB color space to an arbitrary color space.
  explicit HColorTransLUT(const char* ColorSpace, const char* TransDirection, Hlong NumBits);




  /***************************************************************************
   * Operators                                                               *
   ***************************************************************************/

  // Color space transformation using pre-generated look-up-table.
  HImage ApplyColorTransLut(const HImage& Image1, const HImage& Image2, const HImage& Image3, HImage* ImageResult2, HImage* ImageResult3) const;

  // Creates the look-up-table for transformation of an image from the RGB color space to an arbitrary color space.
  void CreateColorTransLut(const HString& ColorSpace, const HString& TransDirection, Hlong NumBits);

  // Creates the look-up-table for transformation of an image from the RGB color space to an arbitrary color space.
  void CreateColorTransLut(const char* ColorSpace, const char* TransDirection, Hlong NumBits);

};

// forward declarations and types for internal array implementation

template<class T> class HSmartPtr;
template<class T> class HToolArrayRef;

typedef HToolArrayRef<HColorTransLUT> HColorTransLUTArrayRef;
typedef HSmartPtr< HColorTransLUTArrayRef > HColorTransLUTArrayPtr;


// Represents multiple tool instances
class LIntExport HColorTransLUTArray : public HToolArray
{

public:

  // Create empty array
  HColorTransLUTArray();

  // Create array from native array of tool instances
  HColorTransLUTArray(HColorTransLUT* classes, Hlong length);

  // Copy constructor
  HColorTransLUTArray(const HColorTransLUTArray &tool_array);

  // Destructor
  virtual ~HColorTransLUTArray();

  // Assignment operator
  HColorTransLUTArray &operator=(const HColorTransLUTArray &tool_array);

  // Clears array and all tool instances
  virtual void Clear();

  // Get array of native tool instances
  const HColorTransLUT* Tools() const;

  // Get number of tools
  virtual Hlong Length() const;

  // Create tool array from tuple of handles
  virtual void SetFromTuple(const HTuple& handles);

  // Get tuple of handles for tool array
  virtual HTuple ConvertToTuple() const;

protected:

// Smart pointer to internal data container
   HColorTransLUTArrayPtr *mArrayPtr;
};

}

#endif
