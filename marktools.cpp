//
// Created by Wsy on 2023/9/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MarkTools.h" resolved

#include "marktools.h"
#include "ui_MarkTools.h"


MarkTools::MarkTools(QWidget *parent) :
        QWidget(parent), ui(new Ui::MarkTools) {
    ui->setupUi(this);
}

MarkTools::MarkTools(QWidget *parent, QPixmap image, QPoint topLeftPoint) :
        QWidget(parent), ui(new Ui::MarkTools) {
    ui->setupUi(this);

    setOriginImage(image);
    initWindow();
}

MarkTools::~MarkTools() {
    delete ui;
}

void MarkTools::initWindow() {
    //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    //this->setStyleSheet("background-color:rgba(0, 0, 0, 100);");
    //this->setAttribute(Qt::WA_StyledBackground);

    // 设置背景
    //ui->toolWidget->setStyleSheet("background-color:rgb(242, 242, 242);");
    //ui->toolWidget->setAttribute(Qt::WA_StyledBackground);

    //ui->configWidget->setStyleSheet("background-color:rgb(242, 242, 242);");
    //ui->configWidget->setAttribute(Qt::WA_StyledBackground);

    QPalette bg(QColor("#f2f2f2"));

    ui->baseToolWidget->setPalette(bg);
    ui->baseToolWidget->setAutoFillBackground(true);

    ui->doToolWidget->setPalette(bg);
    ui->doToolWidget->setAutoFillBackground(true);

    ui->doneToolWidget->setPalette(bg);
    ui->doneToolWidget->setAutoFillBackground(true);

    ui->sizeConfigWidget->setPalette(bg);
    ui->sizeConfigWidget->setAutoFillBackground(true);

    ui->fillConfigWidget->setPalette(bg);
    ui->fillConfigWidget->setAutoFillBackground(true);

    ui->arrowConfigWidget->setPalette(bg);
    ui->arrowConfigWidget->setAutoFillBackground(true);

    ui->fontStyleConfigWidget->setPalette(bg);
    ui->fontStyleConfigWidget->setAutoFillBackground(true);

    ui->fontFamilyConfigWidget->setPalette(bg);
    ui->fontFamilyConfigWidget->setAutoFillBackground(true);

    ui->colorConfigWidget->setPalette(bg);
    ui->colorConfigWidget->setAutoFillBackground(true);

//    ui->arrowConfigWidget->hide();
    // 获取屏幕宽高
    QScreen *screen = QGuiApplication::primaryScreen();
    screenWidth = screen->virtualGeometry().width();
    screenHeight = screen->virtualGeometry().height();

    // 默认关闭，截图工具箱
    ui->configWidget->hide();
    isOpen = false;
    toolType = ToolWidget::Capture;
    configType = ConfigWidget::Other;

    //showToolWidget(toolType);

    // 添加一个painter
    //painterConfigArr.append(new PainterConfig());
    curDoStep = 0;
    addPainter();

    // 连接按钮组
    connect(ui->baseTBtnGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(onBaseTBtnGroupClicked(QAbstractButton *)));
    connect(ui->arrowTBtnGroup, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(onArrowTBtnGroupClicked(QAbstractButton *)));

    // 设置paintArea
    ui->paintArea->resize(originImage.width(), originImage.height());
    ui->paintArea->installEventFilter(this);
    ui->paintArea->winId();
    curMarkState = None;

    // 文本输入
    customLineEdit = new QLineEdit();
    customLineEdit->setObjectName(tr("customLineEdit"));
    // 设置透明色生效
    customLineEdit->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    customLineEdit->window()->setAttribute(Qt::WA_TranslucentBackground, true);
    connect(customLineEdit, SIGNAL(editingFinished()), this, SLOT(lineEditFinishText()));
}

void MarkTools::showEvent(QShowEvent *event)
{
    return QWidget::showEvent(event);
}

void MarkTools::mousePressEvent(QMouseEvent *event) {
    if (isOpen && isPointInImageWidget(event->pos()))
    {
        if (curMarkState != BeginMark)
        {
            curMarkState = BeginMark;
            addPoint(getImagePos(event->pos()));
        }
    }

    return QWidget::mousePressEvent(event);
}

void MarkTools::mouseMoveEvent(QMouseEvent *event)
{
    if (isOpen && isPointInImageWidget(event->pos()))
    {
        if (curMarkState == BeginMark)
        {
            addPoint(getImagePos(event->pos()));
            update();
        }
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }

    return QWidget::mouseMoveEvent(event);
}

void MarkTools::mouseReleaseEvent(QMouseEvent *event) {
    if (isOpen && curMarkState == BeginMark)
    {
        curMarkState = FinishMark;
        if (isPointInImageWidget(event->pos()))
        {
            addPoint(getImagePos(event->pos()));
            PainterConfig::LineStyle *lastLineStyle = painterConfigArr.last()->lineStyle;
            // 显示文本输入框
            if (lastLineStyle->shape == PainterConfig::Text)
            {
                // 输入框位置处理
                QPoint tempDis = lastLineStyle->points[1] - lastLineStyle->points[0];
                customLineEdit->move(event->globalPos() - tempDis);
                customLineEdit->setFixedSize(tempDis.x(), tempDis.y());
                customLineEdit->show();
            }
            else
            {
                addPainter();
            }
            update();
        }
    }

    return QWidget::mouseReleaseEvent(event);
}

void MarkTools::wheelEvent(QWheelEvent *event) {
    if (this->isOpen && isPointInImageWidget(event->pos()))
    {
        QPoint numDegrees = event->angleDelta();
        if (!numDegrees.isNull())
        {
            if (numDegrees.y() > 0)
            {
                addPenWidth(1);
            }
            else
            {
                addPenWidth(-1);
            }
        }
    }
}

void MarkTools::setOpen(bool open)
{
    isOpen = open;
    if (isOpen == false)
    {
        // 恢复工具栏按钮状态
        if (ui->baseTBtnGroup->checkedButton())
        {
            ui->baseTBtnGroup->setExclusive(false);
            ui->baseTBtnGroup->checkedButton()->setChecked(false);
            ui->baseTBtnGroup->setExclusive(true);
            configType = ConfigWidget::Other;
        }
        // 清除绘图
        clearAllPainter();
    }
}

void MarkTools::setOriginImage(QPixmap pix)
{
    this->originImage = pix;
}

void MarkTools::clearAllPainter()
{
    // 清除所有画家配置
    painterConfigArr.clear();
    curDoStep = 0;
    // 添加一个
    addPainter();
}

void MarkTools::addPenWidth(int step)
{
    if (painterConfigArr.last()->lineStyle->shape == PainterConfig::Mosaic ||
        painterConfigArr.last()->lineStyle->shape == PainterConfig::Eraser)
    {
        step += painterConfigArr.last()->lineStyle->brushWidth;
        if (step <= 0)
        {
            step = 1;
        }
        else if (step > 10)
        {
            step = 10;
        }
        // 马赛克实现
        painterConfigArr.last()->setPenWidth(step);
        painterConfigArr.last()->setBrushWidth(step);
    }
    else
    {
        step += painterConfigArr.last()->lineStyle->penWidth;
        if (step <= 0)
        {
            step = 1;
        }
        else if (step > 20)
        {
            step = 20;
        }
        painterConfigArr.last()->setPenWidth(step);
    }

    ui->sizeTBtn->setText(QString("%1").arg(step));
}

void MarkTools::setToolType(ToolWidget type)
{
    this->toolType = type;
}

// 显示工具栏
void MarkTools::showToolWidget(ToolWidget tool)
{
    ui->configWidget->hide();
    switch (tool)
    {
        case Capture:
            ui->confirmTBtn->hide();
            ui->doneToolWidget->resize(170, 40);
            break;
        case Paste:
            ui->cancelTBtn->hide();
            ui->pinTBtn->hide();
            ui->doneToolWidget->resize(130, 40);
            break;
    }
    // 调整大小
    ui->toolWidget->resize(ui->baseToolWidget->size().width() +
                           ui->doToolWidget->size().width() +
                           ui->doneToolWidget->size().width(),
                           ui->baseToolWidget->size().height());
    ui->toolWidget->show();
}

// 显示工具配置栏
void MarkTools::showConfigWidget(ConfigWidget config)
{
    switch (config)
    {
        case Rectangle:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->show();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();
            ui->configWidget->resize(260, 40);

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Ellipse:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->show();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();
            ui->configWidget->resize(260, 40);

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Polyline:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();
            ui->configWidget->resize(260, 40);

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Arrow:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->show();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Pen:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Mark:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->show();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Mosaic:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->hide();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->brushWidth));
            break;
        case Text:
            ui->sizeConfigWidget->hide();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->show();
            ui->fontFamilyConfigWidget->show();
            ui->colorConfigWidget->show();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->penWidth));
            ui->colorTBtn->setPalette(painterConfigArr.last()->lineStyle->penColor);
            break;
        case Eraser:
            ui->sizeConfigWidget->show();
            ui->fillConfigWidget->hide();
            ui->arrowConfigWidget->hide();
            ui->fontStyleConfigWidget->hide();
            ui->fontFamilyConfigWidget->hide();
            ui->colorConfigWidget->hide();

            ui->sizeTBtn->setText(QString("%1").arg(painterConfigArr.last()->lineStyle->brushWidth));
            break;
        default:
            break;
    }
    if (ui->configWidget->size().width() > ui->toolWidget->size().width())
    {
        resize(ui->configWidget->size().width(), 85 + originImage.height());
    }
    else
    {
        resize(ui->toolWidget->size().width(), 85 + originImage.height());
    }

    ui->configWidget->show();
}

// 设置显示
void MarkTools::setShow(bool flag)
{
    showToolWidget(toolType);
    if (configType != ConfigWidget::Other)
    {
        showConfigWidget(configType);
        //qDebug() << "1 before :" << size();
//        if (ui->configWidget->size().width() > ui->toolWidget->size().width())
//        {
//            resize(ui->configWidget->size().width(), 100);
//        }
//        else
//        {
//            resize(ui->toolWidget->size().width(), 100);
//        }
        //qDebug() << "1 after :" << size();
    }
    else
    {
//        qDebug() << "2 before :" << size();
//        resize(ui->toolWidget->size());
//        qDebug() << "2 after :" << size();
    }

    moveTo(lastAlignRect);
    show();
}

// 参考矩形区域进行移动
void MarkTools::moveTo(QRect rect)
{
    lastAlignRect = rect;
    int posX = rect.x() + rect.width() - frameGeometry().width();
    int posY = rect.y() + rect.height() + 5;
    QPoint topLeftPoint = QPoint(posX, posY);

    if (posX < 0)
    {
        topLeftPoint.setX(0);
    }
    if (posX + frameGeometry().width() > screenWidth)
    {
        topLeftPoint.setX(screenWidth - frameGeometry().width());
    }
    if (posY < 0)
    {
        topLeftPoint.setY(0);
    }
    if (posY + frameGeometry().height() > screenHeight)
    {
        topLeftPoint.setY(screenHeight - frameGeometry().height());
    }

    this->move(topLeftPoint);
}

// 添加一个画笔
void MarkTools::addPainter()
{
    if (painterConfigArr.isEmpty())
    {
        addPainter(new PainterConfig());
    }
    else
    {
        // 复制上一个画笔配置
        addPainter(painterConfigArr.last()->copy());
    }
}

void MarkTools::addPainter(PainterConfig *painter)
{
    painterConfigArr.append(painter);
    curDoStep++;
}

// 修改当前画笔
void MarkTools::changePainter(PainterConfig::DrawShape shape)
{
    painterConfigArr.last()->setShape(shape);
    // 设置画笔配置
    // 根据配置修改
    painterConfigArr.last()->setPenWidth(2);
    painterConfigArr.last()->setPenColor(Qt::red);

    painterConfigArr.last()->setPenStyle(Qt::SolidLine);
    painterConfigArr.last()->setPenCapStyle(Qt::SquareCap);
    painterConfigArr.last()->setPenJoinStyle(Qt::RoundJoin);
    painterConfigArr.last()->setFillRule(Qt::OddEvenFill);
    painterConfigArr.last()->setSpreadStyle(QGradient::PadSpread);
    // 根据配置修改
    painterConfigArr.last()->setBrushColor(Qt::red);
    painterConfigArr.last()->setBrushStyle(Qt::NoBrush);

    switch (shape)
    {
        case PainterConfig::Arrow_0:
            painterConfigArr.last()->setBrushStyle(Qt::SolidPattern);
            break;
        case PainterConfig::Arrow_1:
            painterConfigArr.last()->setBrushStyle(Qt::SolidPattern);
            break;
        case PainterConfig::Arrow_2:
            painterConfigArr.last()->setBrushStyle(Qt::NoBrush);
            break;
        case PainterConfig::Mark:
            painterConfigArr.last()->setPenWidth(16);
            painterConfigArr.last()->setPenColor(QColor(255, 0, 0, 100));
            break;
        case PainterConfig::Mosaic:
            // TODO 马赛克实现
//            painterConfigArr.last()->setPenWidth(1);
//            painterConfigArr.last()->setPenColor(QColor(0, 0, 0, 0));
//            painterConfigArr.last()->setBrushTexture(this->originImage.scaled(this->originImage.size()));
//            painterConfigArr.last()->setBrushStyle(Qt::TexturePattern);
//            painterConfigArr.last()->setFillRule(Qt::WindingFill);

            //painterConfigArr.last()->lineStyle->pen.setWidth(10);
            //painterConfigArr.last()->lineStyle->pen.setBrush(this->originImage.scaled(this->originImage.size()));

            painterConfigArr.last()->setPenWidth(5);
            painterConfigArr.last()->setMosaic(this->markedImage);
            break;
        case PainterConfig::Eraser:
//            painterConfigArr.last()->setPenWidth(1);
//            painterConfigArr.last()->setPenColor(QColor(0, 0, 0, 0));
//            painterConfigArr.last()->setBrushTexture(this->originImage);
//            painterConfigArr.last()->setBrushStyle(Qt::TexturePattern);
//            painterConfigArr.last()->setFillRule(Qt::WindingFill);

            painterConfigArr.last()->setPenWidth(5);
            painterConfigArr.last()->setEraser(this->originImage);
            break;
        default:
            break;
    }
}

// 为最后一个画笔添加位置点
void MarkTools::addPoint(QPoint point)
{
    // 去除撤销的过程
    if (curDoStep < painterConfigArr.count())
    {
        // 记录最后一个
        PainterConfig *lastCopy = painterConfigArr.last();
        painterConfigArr.resize(curDoStep);
        addPainter(lastCopy);
    }

    if (painterConfigArr.count() > 0)
    {
        painterConfigArr.last()->addPoint(point);
    }
}

// 为最后一个画笔添加文本
void MarkTools::addText(QString text)
{
    // 去除撤销的过程
    if (curDoStep < painterConfigArr.count())
    {
        // 记录最后一个
        PainterConfig *lastCopy = painterConfigArr.last();
        painterConfigArr.resize(curDoStep);
        addPainter(lastCopy);
    }

    if (painterConfigArr.count() > 0)
    {
        painterConfigArr.last()->addText(text);
    }
}

// 绘图
void MarkTools::draw(QPixmap *pix)
{
    for (int i = 0; i < curDoStep; i++)
    {
        painterConfigArr[i]->draw(pix);
    }
}

// 点击基本标注工具按钮组
void MarkTools::onBaseTBtnGroupClicked(QAbstractButton *button)
{
    isOpen = true;
    bool isRepeatClick = false;
    if (button->objectName() == "rectTBtn")
    {
        if (configType != ConfigWidget::Rectangle)
        {
            configType = ConfigWidget::Rectangle;
            changePainter(PainterConfig::Rectangle);
            showConfigWidget(ConfigWidget::Rectangle);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "circleTBtn")
    {
        if (configType != ConfigWidget::Ellipse)
        {
            configType = ConfigWidget::Ellipse;
            changePainter(PainterConfig::Ellipse);
            showConfigWidget(ConfigWidget::Ellipse);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "lineTBtn")
    {
        if (configType != ConfigWidget::Polyline)
        {
            configType = ConfigWidget::Polyline;
            changePainter(PainterConfig::Line);
            showConfigWidget(ConfigWidget::Polyline);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "arrowTBtn")
    {
        if (configType != ConfigWidget::Arrow)
        {
            configType = ConfigWidget::Arrow;
            changePainter(PainterConfig::Arrow_0);
            showConfigWidget(ConfigWidget::Arrow);
            ui->arrow0TBtn->setChecked(true);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "penTBtn")
    {
        if (configType != ConfigWidget::Pen)
        {
            configType = ConfigWidget::Pen;
            changePainter(PainterConfig::Polyline);
            showConfigWidget(ConfigWidget::Pen);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "markTBtn")
    {
        if (configType != ConfigWidget::Mark)
        {
            configType = ConfigWidget::Mark;
            changePainter(PainterConfig::Mark);
            showConfigWidget(ConfigWidget::Mark);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "mosaicTBtn")
    {
        if (configType != ConfigWidget::Mosaic)
        {
            configType = ConfigWidget::Mosaic;
            changePainter(PainterConfig::Mosaic);
            showConfigWidget(ConfigWidget::Mosaic);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "textTBtn")
    {
        if (configType != ConfigWidget::Text)
        {
            configType = ConfigWidget::Text;
            changePainter(PainterConfig::Text);
            showConfigWidget(ConfigWidget::Text);
        }
        else
        {
            isRepeatClick = true;
        }
    }
    else if (button->objectName() == "eraserTBtn")
    {
        if (configType != ConfigWidget::Eraser)
        {
            configType = ConfigWidget::Eraser;
            changePainter(PainterConfig::Eraser);
            showConfigWidget(ConfigWidget::Eraser);
        }
        else
        {
            isRepeatClick = true;
        }
    }

    // 关闭选择的工具
    if (isRepeatClick)
    {
        configType = ConfigWidget::Other;
        isOpen = false;
        // 恢复工具栏按钮状态
        ui->baseTBtnGroup->setExclusive(false);
        ui->baseTBtnGroup->checkedButton()->setChecked(false);
        ui->baseTBtnGroup->setExclusive(true);
        ui->configWidget->hide();
        resize(ui->toolWidget->width(), ui->toolWidget->height() + originImage.height());
    }

    // 发送工具改变的信号
    emit signalChangePainter(configType);
}

// 点击箭头配置不同样式
void MarkTools::onArrowTBtnGroupClicked(QAbstractButton *button)
{
    if (button->objectName() == "arrow0TBtn")
    {
        changePainter(PainterConfig::Arrow_0);
    }
    else if (button->objectName() == "arrow1TBtn")
    {
        changePainter(PainterConfig::Arrow_1);
    }
    else if (button->objectName() == "arrow2TBtn")
    {
        changePainter(PainterConfig::Arrow_2);
    }
    showConfigWidget(ConfigWidget::Arrow);
}

void MarkTools::initDoneTBtnGroupState()
{
    // 恢复按钮状态
    ui->doneTBtnGroup->setExclusive(false);
    ui->doneTBtnGroup->checkedButton()->setChecked(false);
    ui->doneTBtnGroup->setExclusive(true);
}

void MarkTools::initDoTBtnGroupState()
{
    // 恢复按钮状态
    ui->doTBtnGroup->setExclusive(false);
    ui->doTBtnGroup->checkedButton()->setChecked(false);
    ui->doTBtnGroup->setExclusive(true);
}

void MarkTools::undo()
{
    if (curDoStep == painterConfigArr.count())
    {
        curDoStep--;
    }
    curDoStep--;
    if (curDoStep < 0)
    {
        curDoStep = 0;
    }
    initDoTBtnGroupState();
}

void MarkTools::redo()
{
    curDoStep++;
    if (curDoStep == painterConfigArr.count() - 1)
    {
        curDoStep++;
    }
    if (curDoStep > painterConfigArr.count())
    {
        curDoStep = painterConfigArr.count();
    }
    initDoTBtnGroupState();
}

void MarkTools::on_cancelTBtn_clicked(bool checked)
{
    // 发送信号
    emit signalCancel();
    initDoneTBtnGroupState();
}

void MarkTools::on_pinTBtn_clicked(bool checked)
{
    // 发送信号
    emit signalPin();
    // 恢复按钮状态
    initDoneTBtnGroupState();
}

void MarkTools::on_saveTBtn_clicked(bool checked)
{
    // 发送信号
    emit signalSave();
    // 恢复按钮状态
    initDoneTBtnGroupState();
}

void MarkTools::on_copyTBtn_clicked(bool checked)
{
    // 发送信号
    emit signalCopy();
    // 恢复按钮状态
    initDoneTBtnGroupState();
}

void MarkTools::on_scanTBtn_clicked(bool checked)
{
    // 识别二维码
    QZXing decode;
    decode.setDecoder(QZXing::DecoderFormat_QR_CODE);
    decode.setSourceFilterType(QZXing::TryHarderBehaviour_ThoroughScanning
                                | QZXing::TryHarderBehaviour_Rotate
                                | QZXing::SourceFilter_ImageNormal);
    QString info = decode.decodeImage(this->originImage.toImage());
    qDebug() << "qr code : " << info;
}

void MarkTools::on_confirmTBtn_clicked(bool checked)
{
    // 发送信号
    emit signalConfirm();
    // 恢复按钮状态
    initDoneTBtnGroupState();
}

void MarkTools::on_undoTBtn_clicked(bool checked)
{
    undo();
    update();
    emit signalChangeDoStep();
}

void MarkTools::on_redoTBtn_clicked(bool checked)
{
    redo();
    update();
    emit signalChangeDoStep();
}

void MarkTools::on_fillTBtn_clicked(bool checked)
{
    if (checked)
    {
        painterConfigArr.last()->setBrushStyle(Qt::SolidPattern);
    }
    else
    {
        painterConfigArr.last()->setBrushStyle(Qt::NoBrush);
    }
}

void MarkTools::on_colorTBtn_clicked(bool checked)
{
    QColor color = QColorDialog::getColor(painterConfigArr.last()->lineStyle->penColor, this, QString("select color"), QColorDialog::ShowAlphaChannel);
    painterConfigArr.last()->setPenColor(color);
    ui->colorTBtn->setPalette(color);
    switch (painterConfigArr.last()->lineStyle->shape)
    {
        case PainterConfig::Arrow_0:
        case PainterConfig::Arrow_1:
        case PainterConfig::Arrow_2:
            painterConfigArr.last()->setBrushColor(color);
            break;
        default:
            break;
    }
}

bool MarkTools::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->paintArea && event->type() == QEvent::Paint)
    {
        paintWidget();
    }
    return QObject::eventFilter(watched, event);
}

void MarkTools::paintWidget() {
    painter.begin(ui->paintArea);

    markedImage = this->originImage.copy();
    draw(&markedImage);
    painter.drawPixmap(0, 0, markedImage);

    painter.end();
}

QPoint MarkTools::getImagePos(QPoint cursorPos) {
    return QPoint(cursorPos.x() - ui->paintArea->pos().x(),
                  cursorPos.y() - ui->paintArea->pos().y());
}

bool MarkTools::isPointInImageWidget(QPoint pos) {
    return ui->paintArea->rect().contains(pos);
}

void MarkTools::lineEditFinishText() {
    QString inputText = this->customLineEdit->text().trimmed();
    if (!inputText.isEmpty())
    {
        customLineEdit->clear();
        customLineEdit->close();
        addText(inputText);
        addPainter();
        qDebug() << "input text : " << inputText;
    }
}