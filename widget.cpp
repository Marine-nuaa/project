#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include "string.h"
#include <QDateTime>
#include <cstring>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_bOpenDevice(false)
    , mvCamera(nullptr)
    , grabThread(nullptr)
    , ui(new Ui::Widget)
    , isSaveImgBMP(false)
    , isSaveImgJPG(false)
    , isSaveImgTIFF(false)
    , isSaveImgPNG(false)
    , isUpdateData(false)

    , exposure(0)
    , gain(0)
    , frameRate(0)
    , gamma(0)

{
    ui->setupUi(this);

    // 点击“开始抓图”， 开始抓取图像
    connect(ui->btnStartGrab,&QPushButton::clicked,this,&Widget::slotBtnStartGrab);
    // 点击“停止抓图”， 停止抓取图像
    connect(ui->btnStopGrab,&QPushButton::clicked,this,&Widget::slotBtnStopGrab);

    // 点击“保存图像”，开始保存图像
    connect(ui->isSaveBMP,&QPushButton::clicked,this,&Widget::slotBtnSaveBMP);
    connect(ui->isSaveJPG,&QPushButton::clicked,this,&Widget::slotBtnSaveJPG);
    connect(ui->isSaveTIFF,&QPushButton::clicked,this,&Widget::slotBtnSaveTIFF);
    connect(ui->isSavePNG,&QPushButton::clicked,this,&Widget::slotBtnSavePNG);

    connect(ui->btnCloseWin,&QPushButton::clicked,this,&Widget::close);

    // 点击“参数获取”，获取相机参数
    connect(ui->B_getParam,&QPushButton::clicked,this,&Widget::slotBtnGetParam);
    // 点击“参数设置”，设置相机参数
    connect(ui->B_setParam,&QPushButton::clicked,this,&Widget::slotBtnSetParam);

    // 点击“参数获取”，获取gamma值
   // connect(ui->B_getGamma,&QPushButton::clicked,this,&Widget::slotBtnGetGamma);
    // 点击“伽马校正”，进行伽马校正
   // connect(ui->B_setGamma,&QPushButton::clicked,this,&Widget::slotBtnSetGamma);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::slotBtnOpenDevice()
{

    if(true == m_bOpenDevice || mvCamera != nullptr)
    {
        qDebug()<<"is open Device";
        return;
    }


    // get current link camera
    MV_CC_DEVICE_INFO_LIST  m_stDevList;
    int nRet = MvCamera::EnumDevices(MV_GIGE_DEVICE , &m_stDevList);
    if (MV_OK != nRet)
    {
        qDebug()<<"get Camera Error";
        return;
    }

    mvCamera = new MvCamera;

    if(nullptr == mvCamera)
    {
        return;
    }


    nRet = mvCamera->Open(m_stDevList.pDeviceInfo[0]);
    if (MV_OK != nRet)
    {
        delete mvCamera;
        mvCamera = nullptr;
        qDebug()<<"Open Fail";
        return;
    }

    qDebug()<<"Open success";
    m_bOpenDevice = true;
}

void Widget::slotBtnCloseDevice()
{
    if(mvCamera != nullptr)
    {
        mvCamera->Close();

        delete mvCamera;
        mvCamera = nullptr;
        qDebug()<<"close success";
        m_bOpenDevice = false;
    }
    else
    {
       qDebug()<<"device is not create";
    }
}

void Widget::slotBtnStartGrab()
{
    slotBtnOpenDevice();

    QThread::msleep(500);

    if(!m_bOpenDevice )
    {
        QMessageBox::information(this,"error","device is not open");
        return;
    }

    grabThread = new GrabThread(mvCamera);

    connect(grabThread,&GrabThread::grabImg,this,&Widget::slotDisImg);

    int nRet = mvCamera->StartGrabbing();

    if (MV_OK != nRet)
    {
        qDebug()<<"Start grabbing fail : "<< nRet;
        return;
    }
    qDebug()<<"Start grabbing sussess";

    grabThread->setThreadState(true);
    grabThread->start();

    updateState(GrabState_OpenGrab);

}

void Widget::slotBtnStopGrab()
{
    if(true != m_bOpenDevice || mvCamera == nullptr|| grabThread == nullptr)
    {
        return;
    }

    grabThread->setThreadState(false);
    grabThread->wait();
    grabThread->quit();
    disconnect(grabThread,&GrabThread::grabImg,this,&Widget::slotDisImg);

    int nRet = mvCamera->StopGrabbing();

    delete grabThread;
//    m_pGrabThread->deleteLater();
    grabThread = nullptr;

    if (MV_OK != nRet)
    {
        qDebug()<<"Stop grabbing fail";
        return;
    }
    qDebug()<<"Stop grabbing sussess";

    QThread::msleep(500);

    slotBtnCloseDevice();

    updateState(GrabState_CloseGrab);

}

void Widget::slotDisImg(QImage &img)
{
    //qDebug()<<"slot frame";
    ui->label->clear();
//    img = img.scaled(ui->label->width(), ui->label->height());
    ui->label->setScaledContents(true);
    img = img.scaled(ui->label->size(),Qt::IgnoreAspectRatio);

    Algorithm alg;
    cv::Mat matImg = alg.QImage2cvMat(img);

//    cv::threshold(matImg,matImg,128,255,1);

    QImage retImg = alg.Mat2QImage(matImg);

//    QImage retImg =  img;
    ui->label->setPixmap(QPixmap::fromImage(retImg));

    if(isSaveImgBMP == true)
    {
        isSaveImgBMP = false;
        // add code to save img
        QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        QString filename = "C:/Users/Administrator/Desktop/Test/";
        cv::imwrite(filename.toStdString() + dateTime.toStdString() + "_test.bmp" , matImg);
    }
    if(isSaveImgJPG == true)
    {
        isSaveImgJPG = false;
        // add code to save img
        QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        QString filename = "C:/Users/Administrator/Desktop/Test/";
        cv::imwrite(filename.toStdString() + dateTime.toStdString() + "_test.jpg" , matImg);
    }
    if(isSaveImgTIFF == true)
    {
        isSaveImgTIFF = false;
        // add code to save img
        QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        QString filename = "C:/Users/Administrator/Desktop/Test/";
        cv::imwrite(filename.toStdString() + dateTime.toStdString() + "_test.tiff" , matImg);
    }
    if(isSaveImgPNG == true)
    {
        isSaveImgPNG = false;
        // add code to save img
        QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss");
        QString filename = "C:/Users/Administrator/Desktop/Test/";
        cv::imwrite(filename.toStdString() + dateTime.toStdString() + "_test.png" , matImg);
    }

}

void Widget::slotBtnSaveBMP()
{
    isSaveImgBMP = true;
}

void Widget::slotBtnSaveJPG()
{
    isSaveImgJPG = true;
}
void Widget::slotBtnSaveTIFF()
{
    isSaveImgTIFF = true;
}
void Widget::slotBtnSavePNG()
{
    isSaveImgPNG= true;
}

void Widget::slotBtnGetParam()
{
    if(true != m_bOpenDevice)
    {
        qDebug() << "Device is not open";
    }
    else
    {
        int nRet = GetExposureTime();
        if (nRet != MV_OK)
        {
            QMessageBox::information(this,"error","Get Exposure Time Fail");

        }

        nRet = GetGain();
        if (nRet != MV_OK)
        {
            QMessageBox::information(this,"error","Get Gain Fail");
        }

        nRet = GetFrameRate();
        if (nRet != MV_OK)
        {
            QMessageBox::information(this,"error","Get Frame Rate Fail");
        }

        isUpdateData = false;
    }
}

void Widget::slotBtnSetParam()
{
    if(true != m_bOpenDevice)
    {
        qDebug() << "Device is not open";
    }
    else
    {

        isUpdateData = true;

        bool bIsSetSucceed = true;
        int nRet = SetExposureTime();
        if (nRet != MV_OK)
        {
            bIsSetSucceed = false;
            QMessageBox::information(this,"error","Set Exposure Time Fail");

        }
        nRet = SetGain();
        if (nRet != MV_OK)
        {
            bIsSetSucceed = false;
            QMessageBox::information(this,"error","设置增益失败，增益范围：0~15.0062");
        }
        nRet = SetFrameRate();
        if (nRet != MV_OK)
        {
            bIsSetSucceed = false;
            QMessageBox::information(this,"error","Set Frame Rate Fail");
        }

        if (true == bIsSetSucceed)
        {
            QMessageBox::information(this,"Succeed","Set Parameter Succeed");
        }
    }

}

//int Widget::GetPixelFormat()
//{
//    MVCC_ENUMVALUE stEnumValue = {0};
//    MVCC_ENUMENTRY stPixelFormatInfo = {0};

//    QString m_chPixelFormat;

//    int nRet = mvCamera->GetEnumValue("PixelFormat", &stEnumValue);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    stPixelFormatInfo.nValue = stEnumValue.nCurValue;
//    nRet = mvCamera->GetEnumEntrySymbolic("PixelFormat", &stPixelFormatInfo);
//    if (MV_OK != nRet)
//    {
//        return nRet;
//    }

//    //strcpy_s(m_chPixelFormat, MV_MAX_SYMBOLIC_LEN, stPixelFormatInfo.chSymbolic);

//    return MV_OK;
//}

void Widget::updateState(GrabState ret)
{
    if(ret == GrabState_OpenGrab)
    {
        ui->btnCloseWin->setEnabled(false);
        ui->B_getParam->setEnabled(true);
        ui->B_setParam->setEnabled(true);
       // ui->B_getGamma->setEnabled(true);
        //ui->B_setGamma->setEnabled(true);
        ui->isSaveBMP->setEnabled(true);
        ui->isSaveJPG->setEnabled(true);
        ui->isSaveTIFF->setEnabled(true);
        ui->isSavePNG->setEnabled(true);
    }
    else
    {
        ui->btnCloseWin->setEnabled(true);
        ui->B_getParam->setEnabled(false);
        ui->B_setParam->setEnabled(false);
        //ui->B_getGamma->setEnabled(false);
        //ui->B_setGamma->setEnabled(false);
        ui->isSaveBMP->setEnabled(false);
        ui->isSaveJPG->setEnabled(false);
        ui->isSaveTIFF->setEnabled(false);
        ui->isSavePNG->setEnabled(false);
    }
}



// 设置相机参数
int Widget::GetExposureTime()
{
    MVCC_FLOATVALUE stFloatValue = {0};

        int nRet = mvCamera->GetFloatValue("ExposureTime", &stFloatValue);
        if (MV_OK != nRet)
        {
            return nRet;
        }

        exposure = stFloatValue.fCurValue;
        QString exposure_str = QString::number(exposure, 'f', 2);
        ui->B__expose->setPlainText(exposure_str);
        return MV_OK;
}

int Widget::SetExposureTime()
{
    // ch:调节这两个曝光模式，才能让曝光时间生效
//        int nRet = mvCamera->SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }

        mvCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
        exposure = ui->B__expose->toPlainText().toDouble();
        return mvCamera->SetFloatValue("ExposureTime", (float)exposure);
}

int Widget::GetGain()
{
    MVCC_FLOATVALUE stFloatValue = {0};
        int nRet = mvCamera->GetFloatValue("Gain", &stFloatValue);
        if (MV_OK != nRet)
        {
            return nRet;
        }
        gain = stFloatValue.fCurValue;
        QString gain_str = QString::number(gain, 'f', 2);
        ui->B__gamma_2->setPlainText(gain_str);
        return MV_OK;
}

int Widget::SetGain()
{
    // ch:设置增益前先把自动增益关闭，失败无需返回
        mvCamera->SetEnumValue("GainAuto", 100);
        gain = ui->B__gamma_2->toPlainText().toDouble();
        return mvCamera->SetFloatValue("Gain", (float)gain);
}

int Widget::GetFrameRate()
{
    MVCC_FLOATVALUE stFloatValue = {0};
        int nRet = mvCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
        if (MV_OK != nRet)
        {
            return nRet;
        }
        frameRate = stFloatValue.fCurValue;
        QString frameRate_str = QString::number(frameRate, 'f', 2);
        ui->B__fps->setPlainText(frameRate_str);
        return MV_OK;
}

int Widget::SetFrameRate()
{
    int nRet = mvCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
        if (MV_OK != nRet)
        {
            return nRet;
        }
        frameRate = ui->B__fps->toPlainText().toDouble();
        return mvCamera->SetFloatValue("AcquisitionFrameRate", (float)frameRate);
}

//int Widget::GetGamma()
//{
//        MVCC_FLOATVALUE stFloatValue = {0};
//        int nRet = mvCamera->GetFloatValue("Gamma", &stFloatValue);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }
//        gamma = stFloatValue.fCurValue;
//        QString gamma_str = QString::number(gamma, 'f', 2);
//        ui->B__Gamma->setPlainText(gamma_str);
//        return MV_OK;
//}

//int Widget::SetGamma()
//{
//    int nRet = mvCamera->SetBoolValue("GammaEnable", true);
//        if (MV_OK != nRet)
//        {
//            return nRet;
//        }
//        gamma = ui->B__Gamma->toPlainText().toDouble();
//        return mvCamera->SetFloatValue("Gamma", (float)gamma);
//}

//void Widget::slotBtnGetGamma()
//{
//    int nRet = GetGamma();
//        if (nRet != MV_OK)
//        {
//            QMessageBox::information(this,"error","Get Gamma Fail");
//        }
//        isUpdateData = false;
//}

//void Widget::slotBtnSetGamma()
//{
//    isUpdateData = true;

//        bool bIsSetSucceed = true;
//        int nRet = SetGamma();
//        if (nRet != MV_OK)
//        {
//            bIsSetSucceed = false;
//            QMessageBox::information(this,"error","设置伽马校正失败, gamma取值范围：0~4");

//        }

//        if (true == bIsSetSucceed)
//        {
//            QMessageBox::information(this,"Succeed","Gamma Correction Succeed");
//        }
//}
