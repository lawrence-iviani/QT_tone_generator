#ifndef CTG_CONSTANTS_H
#define CTG_CONSTANTS_H


//Generic time data constant
double static const TIMEDATA_DEFAULT_MIN_TIME=0.0;//This indicates the minimum starting point in time (sec.)
double static const TIMEDATA_DEFAULT_MAX_TIME=30.0;//This indicates the maximum dimension  in time (sec.)
double static const TIMEDATA_DEFAULT_SR=48000.0;//This indicates the constant SR
double static const TIMEDATA_DEFAULT_TIMESTEP=0.05;//This indcates the min time step of every time controls (ie time duration)


//Some constant regarding the draw
static const int PLOTWIDGET_DEFAULT_PLOT_DIMENSION=10;//The default font size in px, every widget has its own poliicy scaling starting from this value


#endif // CTG_CONSTANTS_H
