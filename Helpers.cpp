/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// Custom
#include "Helpers.h"

// ITK
#include "itkImageRegionConstIteratorWithIndex.h"

// VTK
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

namespace Helpers
{
// Convert a scalar ITK image to a vtkStructuredGrid
void ITKImageToStructuredGrid(ImageType::Pointer image, vtkStructuredGrid* structuredGrid)
{
  
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();
  /*
  // This demonstrates the correct order to add points
  points->InsertNextPoint(0, 0, 0);
  points->InsertNextPoint(1, 0, 0);
  points->InsertNextPoint(0, 1, 0);
  points->InsertNextPoint(1, 1, 0);
  points->InsertNextPoint(0, 2, 0);
  points->InsertNextPoint(1, 2, 1);
  */
  
  structuredGrid->SetDimensions(image->GetLargestPossibleRegion().GetSize()[0],
                                image->GetLargestPossibleRegion().GetSize()[1],
                                1);

  structuredGrid->SetPoints(points);
  
  // Convert the pixel values to heights in the structuredGrid
  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image,image->GetLargestPossibleRegion());
  imageIterator.GoToBegin();

  while(!imageIterator.IsAtEnd())
    {
    float height = imageIterator.Get();
      
    points->InsertNextPoint(imageIterator.GetIndex()[0], imageIterator.GetIndex()[1], height);

    ++imageIterator;
    }
}

// Convert a specified region of an ITK image to a vtkStructuredGrid
void ITKImageToStructuredGrid(ImageType::Pointer image, itk::ImageRegion<2> region, vtkStructuredGrid* structuredGrid)
{
  
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  structuredGrid->SetDimensions(region.GetSize()[0],
                                region.GetSize()[1],
                                1);
  
  structuredGrid->SetUpdateExtent(0, region.GetSize()[0] - 1,
                                  0, region.GetSize()[1] - 1,
                                  0, 0);
  
  structuredGrid->SetPoints(points);
  
  points->Reset();
  // Convert the pixel values to heights in the structuredGrid
  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image,region);
  imageIterator.GoToBegin();

  while(!imageIterator.IsAtEnd())
    {
    float height = imageIterator.Get();
    /*
    std::cout << "Adding point " << imageIterator.GetIndex()[0] - region.GetIndex()[0]
                                 << " " << imageIterator.GetIndex()[1] - region.GetIndex()[1]
                                 << " " <<  height << std::endl;
    */
    points->InsertNextPoint(imageIterator.GetIndex()[0] - region.GetIndex()[0],
                            imageIterator.GetIndex()[1] - region.GetIndex()[1], height);

    ++imageIterator;
    }
}

// Convert a vector ITK image to a VTK image for display
void ITKImagetoVTKImage(ImageType::Pointer image, vtkImageData* outputImage)
{
  // Setup and allocate the image data
  outputImage->SetNumberOfScalarComponents(1);
  outputImage->SetScalarTypeToUnsignedChar();
  outputImage->SetDimensions(image->GetLargestPossibleRegion().GetSize()[0],
                             image->GetLargestPossibleRegion().GetSize()[1],
                             1);

  outputImage->AllocateScalars();

  // Copy all of the input image pixels to the output image
  itk::ImageRegionConstIteratorWithIndex<ImageType> imageIterator(image,image->GetLargestPossibleRegion());
  imageIterator.GoToBegin();

  while(!imageIterator.IsAtEnd())
    {
    unsigned char* pixel = static_cast<unsigned char*>(outputImage->GetScalarPointer(imageIterator.GetIndex()[0],
                                                                                     imageIterator.GetIndex()[1],0));
    for(unsigned int component = 0; component < 3; component++)
      {
      pixel[component] = static_cast<unsigned char>(imageIterator.Get());
      }

    ++imageIterator;
    }
}

} // end namespace Helpers
