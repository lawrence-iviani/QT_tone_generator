#ifndef CTG_CONSTANTS_H
#define CTG_CONSTANTS_H

//Generic time data constant
qreal static const TIMEDATA_DEFAULT_MIN_TIME=0.0;//This indicates the minimum starting point in time (sec.)
qreal static const TIMEDATA_DEFAULT_PROJECT_TIME=10.0;
qreal static const TIMEDATA_DEFAULT_MAX_TIME=60.0;//This indicates the maximum dimension  in time (sec.)
qreal static const TIMEDATA_DEFAULT_SR=48000.0;//This indicates the constant SR
qreal static const TIMEDATA_DEFAULT_TIMESTEP=0.001;//This indcates the min time step of every time controls (ie time duration)
qreal static const TIMEDATA_DEFAULT_MIN_SR=4000.0;
qreal static const TIMEDATA_DEFAULT_MAX_SR=96000.0;
qreal static const TIMEDATA_DEFAULT_STEP_SR=4000.0;

//The version of the generated XML data structure
//uint static const DOMHELPER_VERSION=1;

//TAG for XML data
const static QString ENEVELOPEPARAMETERS_TAG="EnvelopeParams";
const static QString GENERICTIMEDATA_TAG="TimeData";
const static QString GENERICTIMEDATAPARAMETERS_TAG="TimeDataParams";
const static QString PROJECTPARAMETERS_TAG="ProjectParams";
const static QString PROJECTROOT_TAG="ProjectRoot";
const static QString WINDOW_TAG="MainWindow";//Not really used, for future development

//DOCTYPE
const static QString PROJECT_DOCTYPE="CTG_project";
const static QString PROJECT_CURVETYPE="CTG_curve";
//Some constant regarding the draw
static const int PLOTWIDGET_DEFAULT_PLOT_DIMENSION=10;//The default font size in px, every widget has its own poliicy scaling starting from this value

//About my company, yes I'm an industrial! :)))
const static QString COMPANYNAME="PuppaSoft";
const static QString PROGRAMNAME="Complex Tone Generator";

//Version
const uint MAJORVERSION=0;
const uint MINORVERSION=0;
const uint RELEASE=1;

//MATH
#ifndef M_RADIANS
#define M_RADIANS(deg) deg*M_PI/180.0;
#endif

#endif // CTG_CONSTANTS_H
