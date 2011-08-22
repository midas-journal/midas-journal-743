
#include "itkWin32Header.h"
#include <iostream>
#include <fstream>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkContinuousIndex.h"

#include "itkImageSample.h"
#include "itkVectorDataContainer.h"
#include "itkImageRandomSampler.h"
#include "itkImageFullSampler.h"
#include "itkImageGridSampler.h"
#include "itkImageRandomCoordinateSampler.h"
#include "itkImageRandomSamplerSparseMask.h"

int main( int argc, char** argv )
{
  std::string inputFileName = "";  
  std::string samplerName = "";  
  if ( argc > 1 )
  {
    samplerName = argv[ 1 ];
    inputFileName = argv[ 2 ];
  }
  else
  {
    std::cerr << "ERROR: Not enough input arguments!" << std::endl;
    return 1;
  }
  
  ::itk::OStringStream outputFileName;
  outputFileName << inputFileName << "." << samplerName << ".png";
  
  /** Typedefs. */
  typedef unsigned char  InputPixelType;
  typedef unsigned char  OutputPixelType;
  const unsigned int     Dimension = 2;

  typedef itk::Image< InputPixelType, Dimension >   InputImageType;  
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;  
  typedef itk::ImageFileReader< InputImageType >    ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >   WriterType;

  typedef itk::ImageSamplerBase< InputImageType >        SamplerBaseType;
  
  typedef itk::ImageFullSampler< InputImageType >                FullSamplerType;  
  typedef itk::ImageGridSampler< InputImageType >                GridSamplerType;  
  typedef itk::ImageRandomSampler< InputImageType >              RandomSamplerType;
  typedef itk::ImageRandomCoordinateSampler< InputImageType >    RandomCoordinateSamplerType;
  typedef itk::ImageRandomSamplerSparseMask< InputImageType >    RandomSparseMaskSamplerType;

  typedef SamplerBaseType::ImageSampleContainerType         SampleContainerType;
  typedef SamplerBaseType::ImageSampleType                  SampleType;
  typedef GridSamplerType::SampleGridSpacingType            GridSpacingType;

  typedef OutputImageType::IndexType IndexType;
  typedef IndexType::IndexValueType  IndexValueType;
  typedef itk::ContinuousIndex<double, Dimension> CIndexType;
    
  /** Read input image. */
  std::cout << "Reading input image..." << std::endl;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFileName.c_str() );
  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cerr << err << std::endl;
    std::cerr << "ERROR: input image " 
      << inputFileName 
      << " could not be read!" << std::endl;
    return 1;
  }
  InputImageType::Pointer inputImage = reader->GetOutput();
 
  /** Instantiate and configure sampler */
  std::cout << "Instantiating and configuring sampler..." << std::endl;
  SamplerBaseType::Pointer sampler = 0;
  if ( samplerName == "full" )
  {
    sampler = FullSamplerType::New();    
  }
  else if ( samplerName == "random" )
  {
    RandomSamplerType::Pointer tempSampler = RandomSamplerType::New();    
    tempSampler->SetNumberOfSamples( 2000 );
    sampler = tempSampler;
  }
  else if ( samplerName == "randomcoordinate" )
  {
    RandomCoordinateSamplerType::Pointer tempSampler = RandomCoordinateSamplerType::New();    
    tempSampler->SetNumberOfSamples( 2000 );    
    sampler = tempSampler;
  }
  else if ( samplerName == "randomsparsemask" )
  {
    RandomSparseMaskSamplerType::Pointer tempSampler = RandomSparseMaskSamplerType::New();    
    tempSampler->SetNumberOfSamples( 2000 );
    sampler = tempSampler;
  }
  else if ( samplerName == "grid" )
  {
    GridSamplerType::Pointer tempSampler = GridSamplerType::New();    
    tempSampler->SetNumberOfSamples( 2000 );
    sampler = tempSampler;
  }

  sampler->SetInput( inputImage );
  sampler->SetInputImageRegion( inputImage->GetBufferedRegion() );
  
  //sampler->SetMask(mask);
  
  std::cout << "Updating sampler..." << std::endl;
  try
  {
    sampler->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cerr << err << std::endl;
    std::cerr << "number of samples still obtained: " 
      << sampler->GetOutput()->Size() << std::endl;
    for ( unsigned int i = 0; i < sampler->GetOutput()->Size(); i++ )
    {
      SampleType sample = sampler->GetOutput()->ElementAt( i );
      std::cout << i << "\t" << sample.m_ImageValue 
        << "\t" << sample.m_ImageCoordinates << std::endl;
    }
    return 2;
  }

  /** Get sample container */
  std::cout << "Getting output of sampler..." << std::endl;
  SampleContainerType::Pointer samples = sampler->GetOutput();
  SampleType sample;

  /** Plot first 10 samples */
  std::cout << "First 10 samples:" << std::endl;
  std::cout << "Sample\tValue\tCoordinates" << std::endl;
  for ( unsigned int i = 0; i < 10; i++ )
  {
    sample = samples->ElementAt( i );
    std::cout << i << "\t" << sample.m_ImageValue 
      << "\t" << sample.m_ImageCoordinates << std::endl;
  }

  /** Create image showing selected samples */
  std::cout << "Creating output image..." << std::endl;
  OutputImageType::Pointer outputImage = OutputImageType::New();
  outputImage->SetRegions( inputImage->GetLargestPossibleRegion().GetSize() );
  outputImage->CopyInformation( inputImage );
  outputImage->Allocate();
  outputImage->FillBuffer( 0 );  
  for (unsigned int i = 0; i < samples->Size(); ++i)
  {
    IndexType index;
    CIndexType cindex;
    sample = samples->ElementAt( i );
    outputImage->TransformPhysicalPointToContinuousIndex( sample.m_ImageCoordinates, cindex);
    for ( unsigned int d = 0; d < Dimension; ++d )
    {
      index[d] = static_cast<IndexValueType>( vnl_math_rnd( cindex[d] ) );
    }
    outputImage->SetPixel( index, sample.m_ImageValue );
  }

  /** Write output image */
  std::cout << "Writing output image..." << std::endl;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputFileName.str().c_str() );
  writer->SetInput( outputImage );
  try
  {
    writer->Update();    
  }
  catch (itk::ExceptionObject & err)
  {
     std::cerr << err << std::endl;
     std::cerr << "ERROR: output image " 
       << outputFileName.str() 
       << " could not be written!" << std::endl;
     return 3;
  }
 
  /** Return a value. */
  std::cout << "Successfully finished!" << std::endl;
	return 0;

} // end main


