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

//TAG for XML data
//params
const static QString ENEVELOPEPARAMETERS_TAG="EnvelopeParams";
const static QString TIMEDATACURVEPARAMETERS_TAG="TimeDataCurveParams";
const static QString PROJECTTIMEPARAMETERS_TAG="ProjectParams";


//root tag
const static QString ENVELOPE_TAG="EnvelopeData";
const static QString PROJECT_TAG="ProjectRoot";
const static QString TIMEDATACURVE_TAG="TimeDataCurve";

//other
//const static QString WINDOW_TAG="MainWindow";//Not really used, for future development

//DOCTYPE
const static QString PROJECTTIMEPARAMS_DOCTYPE="CTG_projectprams";
const static QString PROJECT_DOCTYPE="CTG_project";
const static QString TIMEDATACURVE_DOCTYPE="CTG_curve";
const static QString ENVELOPE_DOCTYPE="CTG_envelope";
//doc version
const static uint TIMEDATACURVE_PARAMSVERSION=1;
const static uint ENVELOPE_PARAMSVERSION=1;
const static uint PROJECT_DOCVERSION=1;
const static uint TIMEDATACURVE_DOCVERSION=1;
const static uint PROJECTTIMEPARAMS_DOCVERSION=1;

//SUFFIX
const static QString PROJECT_SUFFIX="cpf";
const static QString TIMEDATACURVE_SUFFIX="ccf";
const static QString ENVELOPE_SUFFIX="cef";

//Some constant regarding the draw
static const int PLOTWIDGET_DEFAULT_PLOT_DIMENSION=10;//The default font size in px, every widget has its own poliicy scaling starting from this value
static const qreal PLOTWIDGET_DEFAULT_PLOT_CURVE_WIDTH=0.5;
static const int PLOTWIDGET_DEFAULT_PLOT_CURVE_TRANSPARENCY=125;

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
