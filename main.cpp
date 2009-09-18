// TODO: remove this later, hack for include order issues
#include "GLBlaat/GL.h"

#include <QApplication>
#include "Gui/MainWindow.h"

#include <vtkWin32ProcessOutputWindow.h>

int main(int argc, char *argv[]) 
{
#ifdef WIN32
	// Use a more useful output window for VTK
	vtkWin32ProcessOutputWindow *ow = vtkWin32ProcessOutputWindow::New();
	vtkWin32ProcessOutputWindow::SetInstance(ow);
	ow->Delete();
#endif

	// Set up Qt application
	QApplication app(argc, argv);

	VFE::MainWindow *window = new VFE::MainWindow();

	// Go!
	window->show();
	app.exec();

	// Clean up and exit
	delete window;
	return 0;
}
