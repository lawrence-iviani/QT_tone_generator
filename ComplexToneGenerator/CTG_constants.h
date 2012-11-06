#ifndef CTG_CONSTANTS_H
#define CTG_CONSTANTS_H


//Generic time data constant
qreal static const TIMEDATA_DEFAULT_MIN_TIME=0.0;//This indicates the minimum starting point in time (sec.)
qreal static const TIMEDATA_DEFAULT_MAX_TIME=20.0;//This indicates the maximum dimension  in time (sec.)
qreal static const TIMEDATA_DEFAULT_SR=48000.0;//This indicates the constant SR
qreal static const TIMEDATA_DEFAULT_TIMESTEP=0.05;//This indcates the min time step of every time controls (ie time duration)
qreal static const TIMEDATA_DEFAULT_MIN_SR=4000.0;
qreal static const TIMEDATA_DEFAULT_MAX_SR=96000.0;
qreal static const TIMEDATA_DEFAULT_STEP_SR=4000.0;



//Some constant regarding the draw
static const int PLOTWIDGET_DEFAULT_PLOT_DIMENSION=10;//The default font size in px, every widget has its own poliicy scaling starting from this value


#endif // CTG_CONSTANTS_H
