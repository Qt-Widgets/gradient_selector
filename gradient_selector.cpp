#include "gradient_selector.hpp"
#include "gradient_selector_item.hpp"

#include <QPainter>
#include <QLinearGradient>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QColorDialog>

#include <QtMath>

GradientSelector::GradientSelector(
        const QColor& left, const QColor& right, QWidget* parent )
    : QWidget( parent ),
      mHeight( 25 ),
      mMargin( 10 )
{
    this->addGradientPoint( 0.0, left );
    this->addGradientPoint( 1.0, right );

    this->setMouseTracking( true );
}

void GradientSelector::addGradientPoint( const qreal pos, const QColor& color )
{
    mGradientPositions.append( qMakePair( pos, color ) );
    mHoveredTicker = mGradientPositions.end();
}

QRect GradientSelector::getGradientRect() const
{
    return getGradientRect( this->rect() );
}

QRect GradientSelector::getGradientRect( const QRect& rect ) const
{
    return QRect( rect.left() + this->margin(),
                  rect.top() + ( rect.height() - this->height() ) / 2,
                  rect.width() - 2 * this->margin(), this->height() );
}

void GradientSelector::paintTickers(
        QPainter& painter, const QRect& gradientRect )
{
    for( auto it = mGradientPositions.begin(); it != mGradientPositions.end(); ++it )
    {
        QPoint startPoint(
                    gradientRect.left() + qCeil( it->first * gradientRect.width() ),
                    gradientRect.bottom() );

        QPainterPath path;
        path.moveTo( startPoint );
        path.lineTo( startPoint + QPoint( 5, 5 ) );
        path.lineTo( startPoint + QPoint( -5, 5 ) );

        QColor tickerColor = Qt::black;
        if( it == mHoveredTicker ) tickerColor = Qt::red;

        painter.fillPath( path, tickerColor );
    }
}

void GradientSelector::paintEvent( QPaintEvent* event )
{
    QRect gradientRect = this->getGradientRect( event->rect() );
    QLinearGradient gradient( gradientRect.topLeft(), gradientRect.topRight() );
    for( const auto& item : mGradientPositions )
    {
        gradient.setColorAt( item.first, item.second );
    }

    QPainter painter( this );
    painter.fillRect( gradientRect, gradient );
    paintTickers( painter, gradientRect );
}

void GradientSelector::mousePressEvent( QMouseEvent* event )
{
    QRect gradientRect = this->getGradientRect();
    if( !gradientRect.contains( event->pos() ) ) return;

    auto newColor = QColorDialog::getColor( Qt::white, this );
    if( !newColor.isValid() ) return;

    this->addGradientPoint(
                qreal( event->x() - gradientRect.x() ) / gradientRect.width(), newColor );
    this->repaint();
}

void GradientSelector::mouseMoveEvent( QMouseEvent* event )
{
    QRect gradientRect = this->getGradientRect();
    if( !gradientRect.contains( event->pos() ) ) return;

    auto newHoveredTicker = std::find_if(
                mGradientPositions.begin(), mGradientPositions.end(),
                [ &event, gradientRect ]( const QPair< qreal, QColor >& pos ){
                    auto tickerPos = gradientRect.left() + qCeil( pos.first * gradientRect.width() );
                    return qAbs( event->x() - tickerPos ) < 4;
                } );

    if( mHoveredTicker != newHoveredTicker )
    {
        mHoveredTicker = newHoveredTicker;
        repaint();
    }
}

QSize GradientSelector::minimumSizeHint() const
{
    return QSize( this->margin() * 4, this->margin() * 2 + this->height() );
}

qint32 GradientSelector::margin() const
{
    return mMargin;
}

void GradientSelector::setMargin( const qint32& margin )
{
    mMargin = margin;
}

qint32 GradientSelector::height() const
{
    return mHeight;
}

void GradientSelector::setHeight( const qint32& height )
{
    mHeight = height;
}
