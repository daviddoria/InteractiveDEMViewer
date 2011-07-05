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

#ifndef Form_H
#define Form_H

// Custom
#include "ui_Form.h"
#include "Types.h"

// VTK
#include <vtkSmartPointer.h>

// Qt
#include <QMainWindow>

class vtkRenderer;
class vtkBorderWidget;
class BorderCallbackClass;
class vtkStructuredGrid;
class vtkImageActor;
class vtkActor;
class vtkDataSetMapper;

class Form : public QMainWindow, private Ui::Form

{
  Q_OBJECT
public:

  // Constructor/Destructor
  Form();
  ~Form() {};

  void BorderCallback(vtkObject* caller,
                    long unsigned int eventId,
                    void* callData );

public slots:
  void on_actionOpen_activated();
  
protected:
  
  vtkSmartPointer<vtkBorderWidget> BorderWidget;

  vtkSmartPointer<vtkRenderer> RightRenderer;
  vtkSmartPointer<vtkRenderer> LeftRenderer;

  ImageType::Pointer ITKImage;
  vtkSmartPointer<vtkImageData> Image;
  vtkSmartPointer<vtkImageActor> ImageActor;
  
  vtkSmartPointer<vtkStructuredGrid> Mesh;
  vtkSmartPointer<vtkDataSetMapper> MeshMapper;
  vtkSmartPointer<vtkActor> MeshActor;
};

#endif // Form_H
