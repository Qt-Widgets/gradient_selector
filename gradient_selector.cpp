#include "gradient_selector.hpp"

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

void GradientSelector::paintHoveredTicker(
        QPainter& painter, const QPoint& top, const QPoint& bottom )
{
    const QColor tickerColor = Qt::red;
    const qint32 tickerHeight = 5;
    const qint32 tickerWidth = 12;
    const qint32 tickerColorRectHeight = 12;
    const qint32 tickerValueRectHeight = 12;

    // Draw bottom triangle.
    QPainterPath bottomTrianglePath;
    bottomTrianglePath.moveTo( bottom );
    bottomTrianglePath.lineTo( bottom + QPoint( tickerWidth/2, tickerHeight ) );
    bottomTrianglePath.lineTo( bottom + QPoint( -tickerWidth/2, tickerHeight ) );
    painter.fillPath( bottomTrianglePath, tickerColor );

    // Draw top triangle.
    QPainterPath topTrianglePath;
    topTrianglePath.moveTo( top );
    topTrianglePath.lineTo( top + QPoint( tickerWidth/2, -tickerHeight ) );
    topTrianglePath.lineTo( top + QPoint( -tickerWidth/2, -tickerHeight ) );
    painter.fillPath( topTrianglePath, tickerColor );

    // Draw ticker line.
    painter.setPen( tickerColor );
    painter.drawLine( top, bottom );

    // Draw color rect.
    painter.drawRect( QRect( top + QPoint( -tickerWidth/2, -( tickerHeight + tickerColorRectHeight ) ),
                             QSize( tickerWidth, tickerColorRectHeight ) ) );

    // Draw value rect.
    painter.drawRect( QRect( bottom + QPoint( -tickerWidth/2, tickerHeight ),
                             QSize( tickerWidth, tickerValueRectHeight ) ) );
}

void GradientSelector::paintTicker(
        QPainter& painter, const QPoint& top, const QPoint& bottom )
{
    const QColor tickerColor = Qt::black;
    const qint32 tickerHeight = 5;
    const qint32 tickerWidth = 8;

    // Draw bottom triangle.
    QPainterPath bottomTrianglePath;
    bottomTrianglePath.moveTo( bottom );
    bottomTrianglePath.lineTo( bottom + QPoint( tickerWidth/2, tickerHeight ) );
    bottomTrianglePath.lineTo( bottom + QPoint( -tickerWidth/2, tickerHeight ) );
    painter.fillPath( bottomTrianglePath, tickerColor );

    // Draw top triangle.
    QPainterPath topTrianglePath;
    topTrianglePath.moveTo( top );
    topTrianglePath.lineTo( top + QPoint( tickerWidth/2, -tickerHeight ) );
    topTrianglePath.lineTo( top + QPoint( -tickerWidth/2, -tickerHeight ) );
    painter.fillPath( topTrianglePath, tickerColor );
}

void GradientSelector::paintTickers(
        QPainter& painter, const QRect& gradientRect )
{
    for( auto it = mGradientPositions.begin(); it != mGradientPositions.end(); ++it )
    {
        qint32 x = gradientRect.left() + qCeil( it->first * gradientRect.width() );
        QPoint bottomPoint( x, gradientRect.bottom() );
        QPoint topPoint( x, gradientRect.top() );

        if( it == mHoveredTicker )
        {
            paintHoveredTicker( painter, topPoint, bottomPoint );
        }
        else
        {
            paintTicker( painter, topPoint, bottomPoint );
        }
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
    if( !gradientRect.contains( event->pos() ) )
    {
        if( mHoveredTicker != mGradientPositions.end() )
        {
            mHoveredTicker = mGradientPositions.end();
            repaint();
        }
        return;
    }

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
