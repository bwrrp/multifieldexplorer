// TODO: remove this later, hack for include order issues
#include "GLBlaat/GL.h"

#include <QApplication>
#include "Gui/MainWindow.h"

int main(int argc, char *argv[]) 
{
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
