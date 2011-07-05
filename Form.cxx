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
#include "ui_Form.h"
#include "Form.h"
#include "Types.h"
#include "Helpers.h"

// Qt
#include <QFileDialog>

// ITK
#include "itkImageFileReader.h"
#include "itkRescaleIntensityImageFilter.h"

// VTK
#include <vtkActor.h>
#include <vtkBorderWidget.h>
#include <vtkBorderRepresentation.h>
#include <vtkCommand.h>
#include <vtkDataSetMapper.h>
#include <vtkInteractorStyleImage.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
//#include <vtkProperty.h>
#include <vtkProperty2D.h> // For BorderWidget color
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkStructuredGrid.h>

void Form::BorderCallback(vtkObject *caller, unsigned long vtkNotUsed(eventId),
                                void* vtkNotUsed(callData))
{
  std::cout << "BorderCallback()" << std::endl;
  
  vtkBorderWidget *borderWidget =
      reinterpret_cast<vtkBorderWidget*>(caller);

  // Get the world coordinates of the two corners of the box
  vtkCoordinate* lowerLeftCoordinate = static_cast<vtkBorderRepresentation*>(
      borderWidget->GetRepresentation())->GetPositionCoordinate();
  double* lowerLeft = lowerLeftCoordinate->GetComputedWorldValue(this->LeftRenderer);
  //std::cout << "Lower left coordinate: " << lowerLeft[0] << ", " << lowerLeft[1] << ", " << lowerLeft[2] << std::endl;

  vtkCoordinate* upperRightCoordinate = static_cast<vtkBorderRepresentation*>(
      borderWidget->GetRepresentation())->GetPosition2Coordinate();
  double* upperRight = upperRightCoordinate ->GetComputedWorldValue(this->LeftRenderer);
  //std::cout << "Upper right coordinate: " << upperRight[0] << ", " << upperRight[1] << ", " << upperRight[2] << std::endl;

  double* bounds = this->ImageActor->GetBounds();
  double imageXMin = bounds[0];
  double imageXMax = bounds[1];
  double imageYMin = bounds[2];
  double imageYMax = bounds[3];

  // First, assume the border widget is entirely inside the image
  double selectionXMin = lowerLeft[0];
  double selectionXMax = upperRight[0];
  double selectionYMin = lowerLeft[1];
  double selectionYMax = upperRight[1];

  // If the entire border widget is outside the image, return.
  if( (selectionXMax < imageXMin) || (selectionXMin > imageXMax) ||
      (selectionYMax < imageYMin) || (selectionYMin > imageYMax ) )
    {
    return;
    }

  // Don't allow the selection to be outside the valid range
  selectionXMin = std::max(selectionXMin,imageXMin);
  selectionXMax = std::min(selectionXMax,imageXMax);
  selectionYMin = std::max(selectionYMin,imageYMin);
  selectionYMax = std::min(selectionYMax,imageYMax);
  
  // Create a region from the data
  itk::Index<2> index;
  index[0] = static_cast<unsigned int>(selectionXMin);
  index[1] = static_cast<unsigned int>(selectionYMin);
  
  itk::Size<2> size;
  size[0] = static_cast<unsigned int>(selectionXMax - selectionXMin);
  size[1] = static_cast<unsigned int>(selectionYMax - selectionYMin);
  
  itk::ImageRegion<2> region(index,size);
  
  // Mesh the selected portion of the data
  Helpers::ITKImageToStructuredGrid(this->ITKImage, region, this->Mesh);
  
  std::cout << "There are " << this->Mesh->GetNumberOfPoints() << " points." << std::endl;
    
  this->Mesh->Modified();
  
  this->RightRenderer->ResetCamera();

  this->RightRenderer->Render();
  
  this->RightRenderer->ResetCamera();

  this->qvtkWidgetRight->GetRenderWindow()->Render();
}

// Constructor
Form::Form()
{
  this->setupUi(this);

  // Setup Renderers
  this->LeftRenderer = vtkSmartPointer<vtkRenderer>::New();
  this->RightRenderer = vtkSmartPointer<vtkRenderer>::New();

  this->qvtkWidgetLeft->GetRenderWindow()->AddRenderer(this->LeftRenderer);
  this->qvtkWidgetRight->GetRenderWindow()->AddRenderer(this->RightRenderer);
  
  // Setup interactors
  vtkSmartPointer<vtkInteractorStyleImage> leftInteractorStyle =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor()->SetInteractorStyle(leftInteractorStyle);

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> rightInteractorStyle =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
  this->qvtkWidgetRight->GetRenderWindow()->GetInteractor()->SetInteractorStyle(rightInteractorStyle);
  
  // Setup border widget for selection
  this->BorderWidget = vtkSmartPointer<vtkBorderWidget>::New();
  this->BorderWidget->SetInteractor(this->qvtkWidgetLeft->GetRenderWindow()->GetInteractor());
  static_cast<vtkBorderRepresentation*>(this->BorderWidget->GetRepresentation())->GetBorderProperty()->SetColor(1,0,0); //red

  // Setup left display (image)
  this->ITKImage = ImageType::New();
  this->Image = vtkSmartPointer<vtkImageData>::New();
  this->ImageActor = vtkSmartPointer<vtkImageActor>::New();
  this->ImageActor->SetInput(this->Image);
  this->LeftRenderer->AddActor(this->ImageActor);
  
  // Setup right display (mesh)
  this->Mesh = vtkSmartPointer<vtkStructuredGrid>::New();
  this->MeshMapper = vtkSmartPointer<vtkDataSetMapper>::New();
  this->MeshMapper->SetInputConnection(this->Mesh->GetProducerPort());
  this->MeshActor = vtkSmartPointer<vtkActor>::New();
  this->MeshActor->SetMapper(this->MeshMapper);
  this->RightRenderer->AddActor(this->MeshActor);

  // Setup toolbar
  QIcon openIcon = QIcon::fromTheme("document-open");
  actionOpen->setIcon(openIcon);
  this->toolBar->addAction(actionOpen);
  /*
  QIcon saveIcon = QIcon::fromTheme("document-save");
  actionSave->setIcon(saveIcon);
  this->toolBar->addAction(actionSave);
  */
};

void Form::on_actionOpen_activated()
{
  // Get a filename to open
  QString fileName = QFileDialog::getOpenFileName(this,
     "Open File", ".", "Image Files (*.mhd *.png)");

  if(fileName.toStdString().empty())
    {
    std::cout << "Filename was empty - not opening any file." << std::endl;
    return;
    }

  std::cout << "Got filename: " << fileName.toStdString() << std::endl;
  
  // Open the file
  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(fileName.toStdString());
  reader->Update();

  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(reader->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();
  
  this->ITKImage->Graft(rescaleFilter->GetOutput());

  Helpers::ITKImagetoVTKImage(rescaleFilter->GetOutput(), this->Image);
  
  // Add Actor to renderer
  this->LeftRenderer->ResetCamera();

  //this->BorderWidget->AddObserver(vtkCommand::InteractionEvent,this->BorderCallback);
  //this->BorderWidget->AddObserver(vtkCommand::EndInteractionEvent,this->BorderCallback);
  this->BorderWidget->AddObserver(vtkCommand::EndInteractionEvent, this, &Form::BorderCallback);
  this->BorderWidget->On();

}
