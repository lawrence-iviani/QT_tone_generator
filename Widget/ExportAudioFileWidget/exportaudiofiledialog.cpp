#include "exportaudiofiledialog.h"

ExportAudioFileDialog::ExportAudioFileDialog(QObject *parent) : QFileDialog()
{

    this->setOption(QFileDialog::DontUseNativeDialog,true);//This to add options and file filters
    this->setAcceptMode(QFileDialog::AcceptSave);//This to set save mode
    this->setFileMode(QFileDialog::AnyFile);
    m_buttonOptions = new QPushButton();
    m_buttonOptions->setText("Options");
    m_comboBoxFormat=new QComboBox(this);

    this->populateComboBox();

    QGridLayout *layout = (QGridLayout*)this->layout();
    //layout->addWidget(new QSpacerItem(1,5),4,0,1,3 );
    layout->addWidget(m_buttonOptions,5,2,1,1);
    layout->addWidget(m_comboBoxFormat,5,1,1,1);

    connect(m_comboBoxFormat,SIGNAL(currentIndexChanged(QString)),this,SLOT(selectAudioFormat(QString)));
}

void ExportAudioFileDialog::populateComboBox() {
    m_comboBoxFormat->addItem(this->getNameFromType(ExportAudioFileDialog::WAVE ));
    m_comboBoxFormat->addItem(this->getNameFromType(ExportAudioFileDialog::FLAC));
    m_comboBoxFormat->setCurrentIndex(0);
    m_selectedType=ExportAudioFileDialog::WAVE;
    this->setFilter(ExportAudioFileDialog::getSuffixFromType(m_selectedType));
}

QString ExportAudioFileDialog::getSelectedFileName() {
    QString retval;
    QStringList strList=this->selectedFiles();
    if (strList.length()>0) {
        QFileInfo * fi=new QFileInfo(strList.first());
        if (!fi->isDir())  {
            retval.append(strList.first());
        }
    }
    return retval;
}

QString ExportAudioFileDialog::getSelectedDirectoryName() {
    QString file=this->getSelectedFileName();
    QFileInfo * fi=new QFileInfo(file);
    if (!fi->isDir())  {
        return fi->absolutePath();
    } else return "";
}

void ExportAudioFileDialog::selectAudioFormat(QString format) {
    int tFormat=ExportAudioFileDialog::getTypeFromName(format);
    if (tFormat!=-1) {
        m_selectedType=tFormat;
        this->setFilter(ExportAudioFileDialog::getSuffixFromType(m_selectedType));
    }
}

//Static functions
const QString ExportAudioFileDialog::getNameFromType(int type) {
    QString retval;
    switch(type) {
        case ExportAudioFileDialog::WAVE:
            retval.append("Wave - Waveform Audio File Format");
            break;
        case ExportAudioFileDialog::FLAC:
            retval.append("Flac - Free Lossless Audio Coding");
            break;
        default:
            retval.append("Unsupported file type");
    }
    return retval;
}

const QString ExportAudioFileDialog::getSuffixFromType(int type) {
    QString retval;
    switch(type) {
        case ExportAudioFileDialog::WAVE:
            retval.append("*.wav");
            break;
        case ExportAudioFileDialog::FLAC:
            retval.append("*.flac");
            break;
        default:
            break;
    }
    return retval;
}

const int ExportAudioFileDialog::getTypeFromName(QString name) {
    int retval =-1;
    if (name.startsWith("wave",Qt::CaseInsensitive)) {
        retval=ExportAudioFileDialog::WAVE;
    } else if (name.startsWith("flac",Qt::CaseInsensitive)) {
        retval=ExportAudioFileDialog::FLAC;
    }
    return retval;
}
