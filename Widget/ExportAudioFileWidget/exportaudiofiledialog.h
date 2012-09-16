#ifndef EXPORTAUDIOFILEDIALOG_H
#define EXPORTAUDIOFILEDIALOG_H

#include <QFileDialog>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QFileInfo>
#include <QDebug>

class ExportAudioFileDialog : public QFileDialog
{
    Q_OBJECT
    Q_ENUMS(SupportedFormat)
public:
    explicit ExportAudioFileDialog(QObject *parent = 0);
    enum SupportedFormat {WAVE,FLAC};
    int getSelecedType() {return m_selectedType;}
    QString getSelectedFileName();//return dir+path of the selected file, empty string if no files was selected
    QString getSelectedDirectoryName();//return path of the selected file, empty string otherwise

    static const QString getNameFromType(int type);
    static const QString getSuffixFromType(int type);
    static const int getTypeFromName(QString name);

signals:
    
public slots:
    void selectAudioFormat(QString format);
private:
    QPushButton * m_buttonOptions;
    QComboBox * m_comboBoxFormat;
    int m_selectedType;
    void populateComboBox();


};




#endif // EXPORTAUDIOFILEDIALOG_H
