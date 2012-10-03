#include <QtGui/QApplication>
#include "audioplayer.h"

InternalStreamDevice * stream;

void setupStream(AudioPlayer * w) {
    QAudioFormat format=w->getAudioFormat();
    qint64 durationUs=10000*1000;
    int frequency=800;
    int loop=10;
    int SR=96000;
    format.setSampleRate(SR);


    Generator * gen=new Generator(durationUs,frequency,SR,loop,NULL);
    stream=new InternalStreamDevice(format);
    qreal * data=gen->getData();
    stream->setData(data,(qint64) gen->getNumberOfSample());

    w->setStream(stream);
    w->setFileName("/Volumes/MYDATA/Ascolti/Tuck&Patty - 09 Track 9-44.1k_cuted.wav");

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioPlayer w;
    setupStream(&w);
    w.show();
    

    return a.exec();
}
