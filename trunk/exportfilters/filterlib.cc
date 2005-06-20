
#include "filterlib.h"
#include "pixel/pixoutput.h"

#include <QApplication>
#include <QString>

FilterLib::FilterLib()
        : filterHash_()
{
        filterHash_["png"] = new PNGFilterFactory;
        filterHash_["bmp"] = new BMPFilterFactory;
        FilterFactory* jpgff = new JPGFilterFactory;
        filterHash_["jpg"] = jpgff;
        filterHash_["jpeg"] = jpgff;
        filterHash_["xpm"] = new XPMFilterFactory;
        filterHash_["xbm"] = new XBMFilterFactory;
        filterHash_["ppm"] = new PPMFilterFactory;
}

QStringList FilterLib::filterStringList() const
{
        QString s;
        s+= qApp->translate("FilterLib", "Exportable formats:", "English Text") + " (";
        foreach ( FilterHash_t p, filterHash_ )
                s += "*." << p.first + " ";
        s.remove( s.size()-1, 1 );
        s += ")";

        return (QStringList() << s);
}
