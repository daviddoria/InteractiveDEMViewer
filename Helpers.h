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

#ifndef HELPERS_H
#define HELPERS_H

// Custom
#include "Types.h"

// ITK
#include "itkImage.h"

// VTK
#include <vtkImageData.h>
#include <vtkStructuredGrid.h>

namespace Helpers
{
// Convert an entire image to a structured grid
void ITKImageToStructuredGrid(ImageType::Pointer image, vtkStructuredGrid* structuredGrid);

// Convert a specified region of an image to a structured grid
void ITKImageToStructuredGrid(ImageType::Pointer image, itk::ImageRegion<2> region, vtkStructuredGrid* structuredGrid);

// Convert a scalar ITK image to a vtkImageData
void ITKImagetoVTKImage(ImageType::Pointer image, vtkImageData* outputImage);

} // end namespace Helpers
#endif
