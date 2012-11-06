#ifndef SCROLLRUBBERBAND_H
#define SCROLLRUBBERBAND_H

#include <QEvent>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>

class ScrollRubberBand: public QwtPlotPicker
{
public:
     ScrollRubberBand( QwtPlotCanvas *canvas ):
         QwtPlotPicker( canvas )
     {
         //setRubberBand( VLineRubberBand );
         setRubberBandDirection(Qt::Vertical);
         setRubberBandPen( QPen( Qt::darkGray,2.0 ) );
         setStateMachine(NULL);
         begin();
         append( QPoint( 0, 0 ) );
     }

     ScrollRubberBand( QwtPlotCanvas *canvas, Qt::Orientation align):
         QwtPlotPicker( canvas )
     {
         //setRubberBand( VLineRubberBand );
         setRubberBandDirection(align);
         setRubberBandPen( QPen( Qt::darkGray,2.0 ) );
         setStateMachine(NULL);
         begin();
         append( QPoint( 0, 0 ) );
     }

     void setRubberBandDirection(Qt::Orientation align) {
         if (align==Qt::Vertical) {
             setRubberBand( VLineRubberBand );
         }
         if (align==Qt::Horizontal) {
             setRubberBand( HLineRubberBand );
         }
         else setRubberBand( VLineRubberBand );
     }

     void setValue( qreal x )
     {
         const QwtScaleMap xMap = plot()->canvasMap( xAxis() );
         int tx = qRound( xMap.transform( x ) );
         QwtPlotPicker::move( QPoint( tx, 0 ) );
     }

     virtual bool eventFilter( QObject *object, QEvent *event )
     {
         bool ok = QwtPlotPicker::eventFilter( object, event );
         if ( event->type() == QEvent::Resize && object == this->parent() )
         {
             // work around a bug in QwtPicker
             updateDisplay();
         }
         return ok;
     }
};

#endif // SCROLLRUBBERBAND_H
