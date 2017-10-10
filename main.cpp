#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
const char *fname = R"(D:\Program\downloads\ortho-gk-2mm_optimized_1cm_8bit.tif)";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#include "xtiffio.h"  /* for TIFF */
#include "geotiffio.h" /* for GeoTIFF */

//#include <QImage>

//int main()
//{

//    const int WIDTH = 200;

//    TIFF *tif=(TIFF*)0;  /* TIFF-level descriptor */
//    GTIF *gtif=(GTIF*)0; /* GeoKey-level descriptor */

//    /* Open TIFF descriptor to write GeoTIFF tags */
//    tif=XTIFFOpen(fname,"r");
//    if (!tif) return -1;

//    /* Open GTIF Key parser */
//    gtif = GTIFNew(tif);
//    if (!gtif) return -1;

////    /* Set up standar TIFF file */
////    TIFFSetField(tif,TIFFTAG_IMAGEWIDTH,    WIDTH);
////    /* set other TIFF tags and write out image ... */

////    /* Set GeoTIFF information */
////    GTIFKeySet(gtif, GTModelTypeGeoKey, TYPE_SHORT, 1, ModelGeographic);
////    /* set other GeoTIFF keys ... */

////    /* Store the keys into the TIFF Tags */
////    GTIFWriteKeys(gtif);

//    enum {VERSION=0, MAJOR, MINOR};
//    int versions[3]{10};
//    GTIFDirectoryInfo(gtif, versions, 0);

//    int w=0, h=0;
//    TIFFGetField(tif,TIFFTAG_IMAGEWIDTH, &w);
//    TIFFGetField(tif,TIFFTAG_IMAGELENGTH, &h);

//    qDebug() << versions[VERSION] << versions[MAJOR]  << versions[MINOR] << w << h;



//    /* get rid of the key parser */
//    GTIFFree(gtif);

//    /* save and close the TIFF file descriptor */
//    XTIFFClose(tif);

//    return 0;
//}
