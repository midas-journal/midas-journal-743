CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

INCLUDE(${CMAKE_SOURCE_DIR}/IJMacros.txt)

#Change PROJECT_NAME to the name of your project
PROJECT(ImageSamplersIJ)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
#
# The packages can be specified with a version number, for example:
#
# ITK 2.8.1
# ITK 3.2.0
#
# If no version is specified, the most recent release of the package
# will be used.
SET(Required_Packages
ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
ITKCommon
ITKIO
)

# ImageSampler code files
SET(ImageSamplerCode
 ImageSamplers/itkImageFullSampler.h
 ImageSamplers/itkImageFullSampler.txx
 ImageSamplers/itkImageGridSampler.h
 ImageSamplers/itkImageGridSampler.txx
 ImageSamplers/itkImageRandomCoordinateSampler.h
 ImageSamplers/itkImageRandomCoordinateSampler.txx
 ImageSamplers/itkImageRandomSampler.h
 ImageSamplers/itkImageRandomSampler.txx
 ImageSamplers/itkImageRandomSamplerBase.h
 ImageSamplers/itkImageRandomSamplerSparseMask.h
 ImageSamplers/itkImageRandomSamplerSparseMask.txx
 ImageSamplers/itkImageSample.h
 ImageSamplers/itkImageSamplerBase.h
 ImageSamplers/itkImageSamplerBase.txx
 ImageSamplers/itkImageToVectorContainerFilter.h
 ImageSamplers/itkImageToVectorContainerFilter.txx 
 ImageSamplers/itkVectorContainerSource.h
 ImageSamplers/itkVectorContainerSource.txx
 ImageSamplers/itkVectorDataContainer.h
 ImageSamplers/itkVectorDataContainer.txx )

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/ImageSamplers )

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project
SET(CurrentExe "ImageCompare")
ADD_EXECUTABLE(${CurrentExe} ImageCompare.cxx) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
SET(CurrentExe "TestImageSampler")
ADD_EXECUTABLE(${CurrentExe} TestImageSampler.cxx ${ImageSamplerCode} ) 
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
ADD_TEST(TestFullSampler TestImageSampler full ${CMAKE_SOURCE_DIR}/images/testimage.png )
ADD_TEST(TestFullSamplerCompare ImageCompare ${CMAKE_SOURCE_DIR}/images/testimage.png.full.png ${CMAKE_SOURCE_DIR}/images/base.full.png)

ADD_TEST(TestRandomSampler TestImageSampler random ${CMAKE_SOURCE_DIR}/images/testimage.png )
ADD_TEST(TestRandomSamplerCompare ImageCompare ${CMAKE_SOURCE_DIR}/images/testimage.png.random.png ${CMAKE_SOURCE_DIR}/images/base.random.png)

ADD_TEST(TestRandomCoordinateSampler TestImageSampler randomcoordinate ${CMAKE_SOURCE_DIR}/images/testimage.png )
ADD_TEST(TestRandomCoordinateSamplerCompare ImageCompare ${CMAKE_SOURCE_DIR}/images/testimage.png.randomcoordinate.png ${CMAKE_SOURCE_DIR}/images/base.randomcoordinate.png)

ADD_TEST(TestRandomSparseMaskSampler TestImageSampler randomsparsemask ${CMAKE_SOURCE_DIR}/images/testimage.png )
ADD_TEST(TestRandomSamplerSparseMaskCompare ImageCompare ${CMAKE_SOURCE_DIR}/images/testimage.png.randomsparsemask.png ${CMAKE_SOURCE_DIR}/images/base.randomsparsemask.png)

ADD_TEST(TestGridSampler TestImageSampler grid ${CMAKE_SOURCE_DIR}/images/testimage.png )
ADD_TEST(TestGridSamplerCompare ImageCompare ${CMAKE_SOURCE_DIR}/images/testimage.png.grid.png ${CMAKE_SOURCE_DIR}/images/base.grid.png)

#Once you're done modifying this template, you should rename it to "CMakeLists.txt"

