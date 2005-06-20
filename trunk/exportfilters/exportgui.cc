
#include "exportgui.h"

ExportGUI::ExportGUI()
        : QObject( qApp )
{
        
}

static ExportGUI::instance()
{
        static ExportGui* inst = new ExportGui();
        return *inst;
}

void ExportGUI::exportFigure( Figure* f )
{
        
}
