//
// Created by Wsy on 2023/9/25.
//

#include "PainterConfig.h"

PainterConfig::PainterConfig()
{
    init();
}

PainterConfig::PainterConfig(DrawShape shape)
{
    init();
    setShape(shape);
}

PainterConfig::~PainterConfig()
{
    delete this->lineStyle;
}

void PainterConfig::init()
{
    this->lineStyle = new LineStyle;
    this->lineStyle->shape = Line;
    this->lineStyle->penWidth = 1;
    this->lineStyle->penColor = Qt::red;
    this->lineStyle->penStyle = Qt::SolidLine;
    this->lineStyle->penCapStyle = Qt::SquareCap;
    this->lineStyle->penJoinStyle = Qt::RoundJoin;
    this->setPen();
    this->lineStyle->fillRule = Qt::OddEvenFill;
    this->lineStyle->spreadStyle = QGradient::PadSpread;
    this->lineStyle->brushColor = Qt::red;
    this->lineStyle->brushStyle = Qt::NoBrush;
    this->lineStyle->brushWidth = 5;
    this->setBrush();
    // TODO 字体初始化
    this->lineStyle->content = "";
}

void PainterConfig::setPen()
{
    this->lineStyle->pen = QPen(this->lineStyle->penColor,
                                this->lineStyle->penWidth,
                                this->lineStyle->penStyle,
                                this->lineStyle->penCapStyle,
                                this->lineStyle->penJoinStyle);
}

void PainterConfig::setPen(QPen pen)
{
    this->lineStyle->pen = pen;
}

void PainterConfig::setBrush()
{
    this->lineStyle->brush = QBrush(this->lineStyle->brushColor,
                                    this->lineStyle->brushStyle);
}

void PainterConfig::setBrush(QBrush brush)
{
    this->lineStyle->brush = brush;
}

void PainterConfig::setShape(DrawShape shape)
{
    this->lineStyle->shape = shape;
}

void PainterConfig::setPenColor(QColor color)
{
    this->lineStyle->penColor = color;
    this->lineStyle->pen.setColor(color);
}

void PainterConfig::setPenWidth(int width)
{
    this->lineStyle->penWidth = width;
    this->lineStyle->pen.setWidth(width);
}

void PainterConfig::setPenStyle(Qt::PenStyle style)
{
    this->lineStyle->penStyle = style;
    this->lineStyle->pen.setStyle(style);
}

void PainterConfig::setPenCapStyle(Qt::PenCapStyle style)
{
    this->lineStyle->penCapStyle = style;
    this->lineStyle->pen.setCapStyle(style);
}

void PainterConfig::setPenJoinStyle(Qt::PenJoinStyle style)
{
    this->lineStyle->penJoinStyle = style;
    this->lineStyle->pen.setJoinStyle(style);
}

void PainterConfig::setFillRule(Qt::FillRule rule)
{
    this->lineStyle->fillRule = rule;
}

void PainterConfig::setSpreadStyle(QGradient::Spread style)
{
    this->lineStyle->spreadStyle = style;
}

void PainterConfig::setBrushColor(QColor color)
{
    this->lineStyle->brushColor = color;
    this->lineStyle->brush.setColor(color);
}

void PainterConfig::setBrushStyle(Qt::BrushStyle style)
{
    this->lineStyle->brushStyle = style;
    this->lineStyle->brush.setStyle(style);
}

void PainterConfig::setBrushTexture(QPixmap pix)
{
    this->lineStyle->brushTexture = pix;
    this->lineStyle->brush.setTexture(pix);
}

void PainterConfig::setBrushWidth(int width)
{
    this->lineStyle->brushWidth = width;
}

void PainterConfig::setContent(QString content)
{
    this->lineStyle->content = content;
}

void PainterConfig::setFont(QFont font)
{
    this->lineStyle->font = font;
}

void PainterConfig::addPoint(QPoint point)
{
    // 添加起点
    if (lineStyle->points.count() <= 0)
    {
        // 判断绘制形状
        switch (lineStyle->shape)
        {
            case Line:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Rectangle:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case RoundRect:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Ellipse:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Circle:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Polygon:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Polyline:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Points:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Arc:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Path:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Text:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Pixmap:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Arrow_0:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Arrow_1:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Arrow_2:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Mark:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Mosaic:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
            case Eraser:
                lineStyle->points.append(point);
                lineStyle->points.append(point);
                break;
        }
    }
        // 添加非起点
    else
    {
        // 判断绘制形状
        switch (lineStyle->shape)
        {
            case Line:
                lineStyle->points[1] = point;
                break;
            case Rectangle:
                lineStyle->points[1] = point;
                break;
            case RoundRect:
                lineStyle->points[1] = point;
                break;
            case Ellipse:
                lineStyle->points[1] = point;
                break;
            case Circle:
                lineStyle->points[1] = point;
                break;
            case Polygon:
                lineStyle->points[1] = point;
                break;
            case Polyline:
                //lineStyle->points[1] = point;
                lineStyle->points.append(point);
                break;
            case Points:
                lineStyle->points[1] = point;
                break;
            case Arc:
                lineStyle->points[1] = point;
                break;
            case Path:
                lineStyle->points[1] = point;
                break;
            case Text:
                lineStyle->points[1] = point;
                break;
            case Pixmap:
                lineStyle->points[1] = point;
                break;
            case Arrow_0:
                lineStyle->points[1] = point;
                break;
            case Arrow_1:
                lineStyle->points[1] = point;
                break;
            case Arrow_2:
                lineStyle->points[1] = point;
                break;
            case Mark:
                lineStyle->points[1] = point;
                break;
            case Mosaic:
                lineStyle->points.append(point);
                break;
            case Eraser:
                lineStyle->points.append(point);
                break;
        }
    }
}

void PainterConfig::addText(QString text)
{
    this->setContent(text);
}

void PainterConfig::draw(QPixmap *pix)
{
    // 判断绘制点的数量
    if (lineStyle->points.isEmpty())
    {
        return;
    }

    painter.begin(pix);

    // 开启抗锯齿
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.setPen(lineStyle->pen);
    painter.setBrush(lineStyle->brush);

    // 文本样式
    QTextOption textOption(Qt::AlignCenter);
    textOption.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    // 判断绘制形状
    switch (lineStyle->shape)
    {
        case Line:
            painter.drawLine(lineStyle->points.at(0), lineStyle->points.at(1));
            break;
        case Rectangle:
            painter.drawRect(QRect(lineStyle->points.at(0), lineStyle->points.at(1)));
            break;
        case RoundRect:
            painter.drawRoundedRect(QRect(lineStyle->points.at(0), lineStyle->points.at(1)), 2, 2);
            break;
        case Ellipse:
            painter.drawEllipse(QRect(lineStyle->points.at(0), lineStyle->points.at(1)));
            break;
        case Circle:
            break;
        case Polygon:
            break;
        case Polyline:
            painter.drawPolyline(QPolygon(lineStyle->points));
            break;
        case Points:
            break;
        case Arc:
            break;
        case Path:
            break;
        case Text:
            painter.drawText(QRect(lineStyle->points.at(0), lineStyle->points.at(1)),
                             lineStyle->content,
                             textOption);
            break;
        case Pixmap:
            break;
        case Arrow_0:
            painter.drawPolygon(QPolygon(getArrowShape(lineStyle->points.at(0), lineStyle->points.at(1), 0)));
            break;
        case Arrow_1:
            painter.drawPolygon(QPolygon(getArrowShape(lineStyle->points.at(0), lineStyle->points.at(1), 1)));
            break;
        case Arrow_2:
            painter.drawPolygon(QPolygon(getArrowShape(lineStyle->points.at(0), lineStyle->points.at(1), 2)));
            break;
        case Mark:
            painter.drawLine(lineStyle->points.at(0), lineStyle->points.at(1));
            break;
        case Mosaic:
            //painter.drawPolygon(QPolygon(getLineShape(lineStyle->points, lineStyle->brushWidth)));
            painter.drawPolyline(QPolygon(lineStyle->points));
            //drawPathImage();
            break;
        case Eraser:
//            painter.drawPolygon(QPolygon(getLineShape(lineStyle->points, lineStyle->brushWidth)));
            painter.drawPolyline(QPolygon(lineStyle->points));
            break;
    }

    painter.end();
}

QVector<QPoint> PainterConfig::getArrowShape(QPoint beginPoint, QPoint endPoint, int type)
{
    QVector<QPoint> points;
    float x1 = beginPoint.x();
    float y1 = beginPoint.y();
    float x2 = endPoint.x();
    float y2 = endPoint.y();

    double angle = atan2(y2 - y1, x2 - x1);				//计算线段的夹角
    float arrow_length_long = 20;						//箭头边的长度--长
    float arrow_angle_long = 0.6 + type * 0.1;						//箭头与线段的夹角--长
    float arrow_length_short = 12 + type * 2;						//箭头边的长度--短
    float arrow_angle_short = 0.3;						//箭头与线段的夹角--短

    float point_long_x1 = x2 - arrow_length_long * cos(angle - arrow_angle_long);
    float point_long_y1 = y2 - arrow_length_long * sin(angle - arrow_angle_long);
    float point_long_x2 = x2 - arrow_length_long * cos(angle + arrow_angle_long);
    float point_long_y2 = y2 - arrow_length_long * sin(angle + arrow_angle_long);
    float point_short_x1 = x2 - arrow_length_short * cos(angle - arrow_angle_short);
    float point_short_y1 = y2 - arrow_length_short * sin(angle - arrow_angle_short);
    float point_short_x2 = x2 - arrow_length_short * cos(angle + arrow_angle_short);
    float point_short_y2 = y2 - arrow_length_short * sin(angle + arrow_angle_short);

    points.append(beginPoint);
    points.append(QPoint(point_short_x1, point_short_y1));
    points.append(QPoint(point_long_x1, point_long_y1));
    points.append(endPoint);
    points.append(QPoint(point_long_x2, point_long_y2));
    points.append(QPoint(point_short_x2, point_short_y2));
    points.append(beginPoint);

    return points;
}

// 从点集合扩展多边形
QVector<QPoint> PainterConfig::getLineShape(QVector<QPoint> points, double width)
{
    QVector<QPoint> pointArr;
    pointArr.append(points.at(0));

//    for (int i = 0; i < points.count(); i++)
//    {
//        pointArr.append(QPoint(points.at(i).x(), points.at(i).y() - width));
//    }

//    for (int i = points.count() - 1; i >= 0; i--)
//    {
//        pointArr.append(QPoint(points.at(i).x(), points.at(i).y() + width));
//    }

    QVector<QPoint> upPoints, downPoints;

    int len = points.count() - 1;
    QPoint a, b;
    double angle, dx, dy;
    for (int i = 0; i < len; i++)
    {
        // 判断斜率
        a = points.at(i);
        b = points.at(i + 1);

        // 水平
        if (abs(b.y() - a.y()) < 1e-6)
        {
            upPoints.append(QPoint(a.x(), a.y() - width));
            downPoints.append(QPoint(a.x(), a.y() + width));
        }
            // 垂直
        else if (abs(b.x() - a.x()) < 1e-6)
        {
            upPoints.append(QPoint(a.x() - width, a.y()));
            downPoints.append(QPoint(a.x() + width, a.y()));
        }
            // 倾斜
        else
        {
            angle = atan2(b.y() - a.y(), b.x() - a.x());
            dx = width * sin(angle);
            dy = width * cos(angle);
            upPoints.append(QPoint(a.x() - dx, a.y() - dy));
            downPoints.append(QPoint(a.x() + dx, a.y() + dy));
        }
    }

    // 添加上方的点
    for (int i = 0; i < len; i++)
    {
        pointArr.append(upPoints.at(i));
    }

    // 添加终点
    pointArr.append(points.last());

    // 添加下方的点
    for (int i = len - 1; i >= 0; i--)
    {
        pointArr.append(downPoints.at(i));
    }

    pointArr.append(points.at(0));
    return pointArr;
}

PainterConfig* PainterConfig::copy()
{
    PainterConfig *p = new PainterConfig;
    p->lineStyle->pen = this->lineStyle->pen;
    p->lineStyle->brush = this->lineStyle->brush;
    p->lineStyle->shape = this->lineStyle->shape;
    p->lineStyle->penColor = this->lineStyle->penColor;
    p->lineStyle->penWidth = this->lineStyle->penWidth;
    p->lineStyle->penStyle = this->lineStyle->penStyle;
    p->lineStyle->penCapStyle = this->lineStyle->penCapStyle;
    p->lineStyle->penJoinStyle = this->lineStyle->penJoinStyle;
    p->lineStyle->fillRule = this->lineStyle->fillRule;
    p->lineStyle->spreadStyle = this->lineStyle->spreadStyle;
    p->lineStyle->brushColor = this->lineStyle->brushColor;
    p->lineStyle->brushStyle = this->lineStyle->brushStyle;
    p->lineStyle->brushWidth = this->lineStyle->brushWidth;
    // TODO 文本以及字体
    // p->lineStyle->content = this->lineStyle->content;
    p->lineStyle->content = "";
    p->lineStyle->font = this->lineStyle->font;
    return p;
}

void PainterConfig::drawPathImage()
{
    for (int i = 0; i < lineStyle->points.size() - 1; i++)
    {
        painter.drawLine(lineStyle->points[i], lineStyle->points[i + 1]);
    }
    qDebug() << lineStyle->penWidth;
}

void PainterConfig::setMosaic(QPixmap pix)
{
    QImage img = pix.toImage();
    img = img.convertToFormat(QImage::Format_RGB888);
    lineStyle->pen.setBrush(QBrush(blurImage(img)));
}

void PainterConfig::setEraser(QPixmap pix)
{
    lineStyle->pen.setBrush(pix);
}

QImage PainterConfig::blurImage(QImage img)
{
    cv::Mat mat = this->QImage2cvMat(img);
    // 模糊
    cv::GaussianBlur(mat, mat, cv::Size(15, 15), 10);
    img = cvMat2QImage(mat);
    return img;
}

QImage PainterConfig::cvMat2QImage(const cv::Mat mat) {
    if (mat.type() == CV_8UC3)
    {
        QImage image(mat.data,
                     mat.cols,
                     mat.rows,
                     mat.step,
                     QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else
    {
        // TODO 使用提示框
        qDebug() << "error: mat not to QImage";
        return QImage();
    }
}

cv::Mat PainterConfig::QImage2cvMat(const QImage image) {
    cv::Mat mat;
    switch (image.format())
    {
        case QImage::Format_RGB888:
        {
            mat = cv::Mat(image.height(),
                          image.width(),
                          CV_8UC(3),
                          (void *)image.bits(),
                          image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_RGB2BGR);
            break;
        }
    }

    return mat;
}
