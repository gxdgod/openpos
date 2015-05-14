#include "parallaxhome.h"
#include "com2test.h"

#include "keyboardtest.h"
#include "magnetictest.h"
#include "mfccardtest.h"
#include "modemtest.h"
#include "wifitest.h"

#include "ethernettest.h"
#include "barcodeprinttest.h"
#include "samtest.h"
#include "cameratest.h"
#include "signintest.h"
#include "beeptest.h"
#include "batterytest.h"
#include "gpstest.h"
#include "keytest.h"
//#include "mainentrythread.h"


extern "C"{
#include "sand_key.h"
#include "sand_main.h"
#include "sysglobal.h"
#include "fsync_drvs.h"
#include "osevent.h"
#include "camera.h"
#include "sand_debug.h"
#include "devSignal.h"
#include "gsm.h"
#include "ostime.h"
#include "osdriver.h"
}
extern void _print(char *fmt,...);

//#define thread 1
#ifdef thread
MainEntryThread *mainEntryThread;
#endif


ParallaxHome::ParallaxHome(int argc, char *argv[], QWidget *parent) :
    QGraphicsView(parent),
    m_pageOffset(-1),
    pageIndex(0)
{
    _print("IN ParallaxHome 0000000*******\n");
    setFrameShape(QFrame::NoFrame);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _print("IN ParallaxHome 11111*******\n");

    dragFlag = Flag_Slide;
    gsmFlag = false;
    enterFLag = false;
    pItem = NULL;
    m_naviBar = NULL;
    sM_gsm = 0;
    pressFlag = false;

//    testNames << tr("1.SCREEN") << tr("2.PRINTER") << tr("3.KEYBOARD") << tr("4.MAGNETIC")
//              << tr("5.ICC") << tr("6.SAM") << tr("7.UART") << tr("8.MFC CARD")
//              << tr("9.MODEM") << tr("1.GSM") << tr("2.ETHERNET")<< tr("3.CONNECT")
//              << tr("4.MASS \nMEMORY") << tr("5.BARCODE\n PRINT")<< tr("6.CAMERA")<<tr("7.SIGN IN")
//              << tr("8.BUZZER")<<tr("9.BATTERY")<<tr("1.GPS")<<("2.KEY TEST")<<("3.EXIT")<<tr("4.SLEEP");//<<tr("4.ADJUST\nSCREEN");


//    testIcons << tr("/daemon/image/1.png") << tr("/daemon/image/2.png")
//              << tr("/daemon/image/3.png") << tr("/daemon/image/4.png")
//              << tr("/daemon/image/5.png") << tr("/daemon/image/6.png")
//              << tr("/daemon/image/7.png") << tr("/daemon/image/8.png")
//              << tr("/daemon/image/9.png") << tr("/daemon/image/10.png")
//              << tr("/daemon/image/11.png")
//              <<  tr("/daemon/image/12.png")
//              <<tr("/daemon/image/13.png")<< tr("/daemon/image/14.png")
//             << tr("/daemon/image/15.png")<<tr("/daemon/image/16.png")
//             << tr("/daemon/image/17.png")<<tr("/daemon/image/18.png")
//             <<tr("/daemon/image/19.png")<<tr("/daemon/image/22.png")
//            <<tr("/daemon/image/20.png")
//            <<tr("/daemon/image/21.png");


    testNames << tr("1.SCREEN") << tr("2.PRINTER") << tr("3.KEYBOARD") << tr("4.MAGNETIC")
              << tr("5.ICC") << tr("6.SAM")<< tr("7.MFC CARD")<< tr("8.BARCODE\n PRINT") <<tr("9.SIGN IN")

               << tr("1.CAMERA")<<tr("2.GPS")<<tr("3.KEY TEST")
               << tr("4.GSM")<< tr("5.CONNECT")<<tr("6.BATTERY")
               << tr("7.BUZZER") << tr("8.MASS \nMEMORY")<< tr("9.SLEEP")

                 << tr("1.ETHERNET")<<tr("2.UART")//<< tr("1.MODEM")

                << tr("3.EXIT");


    testIcons << tr("/daemon/image/1.png") << tr("/daemon/image/2.png")
              << tr("/daemon/image/3.png") << tr("/daemon/image/4.png")
              << tr("/daemon/image/5.png") << tr("/daemon/image/6.png")
               << tr("/daemon/image/8.png")<< tr("/daemon/image/14.png")<<tr("/daemon/image/16.png")

               << tr("/daemon/image/15.png")<<tr("/daemon/image/19.png")<<tr("/daemon/image/22.png")
                  << tr("/daemon/image/10.png")<<  tr("/daemon/image/12.png")<<tr("/daemon/image/18.png")
                << tr("/daemon/image/17.png")  <<tr("/daemon/image/13.png") << tr("/daemon/image/21.png")


               << tr("/daemon/image/11.png")  << tr("/daemon/image/7.png")

            <<tr("/daemon/image/20.png");



    _print("IN ParallaxHome 3333333*******\n");

    fristFLag = true;


    pageCount = ceil(static_cast<double>(testNames.count())/9); //pageCountÏòÉÏÉáÈëfd

    setDragMode(RubberBandDrag);
    setAcceptDrops(true);
    setupScene(); //load item
    setScene(&m_scene);
    _print("IN ParallaxHome 4444444*******\n");

    connect(&m_pageAnimator, SIGNAL(frameChanged(int)), SLOT(shiftPage(int)));
    m_pageAnimator.setDuration(500);
    m_pageAnimator.setFrameRange(0, 100);
    m_pageAnimator.setCurveShape(QTimeLine::EaseInCurve);
    _print("IN ParallaxHome 55555*******\n");

    connect(&gsmTimer,SIGNAL(timeout()),this,SLOT(gsmSlot()));


    choosePage(0);
#ifdef thread

    //qDebug()<<"~~~~~~~~~~~~~~~~~1";
    qt_main_entry_init(argc, argv);
    //qDebug()<<"~~~~~~~~~~~~~~~~~2";

    mainEntryThread = new MainEntryThread(argc,argv,this);
    //qDebug()<<"~~~~~~~~~~~~~~~~~3";

    connect(mainEntryThread,SIGNAL(sysEventSignal(int)),this,SLOT(sysEventSlot(int)));
    mainEntryThread->start();

#endif

//    this->parentWidget()->hide();
}

void ParallaxHome::sysEventSlot(int sysEvent)
{
    #ifdef thread
    ((struct seven *)sysEvent)->pt_eve_out->treatment_status = NOK;
    _print("~~~~~~~~~~~~~~~~~sysEvent =%02x\n",(((struct seven *)sysEvent)->event_nr + ((struct seven *)sysEvent)->event_type));
    switch ((((struct seven *)sysEvent)->event_nr + ((struct seven *)sysEvent)->event_type))
    {
    case OSEVENTTYPE_SEL+0xff:
    case OSEVENTTYPE_SEL+0xfe:
        ////qDebug("qtest show");
        _print("~~~~~~~~~~~~~~~~~~~~event 1~~~~~~~~\n");

        ((struct seven *)sysEvent)->pt_eve_out->treatment_status = NOK;
        usleep(200000);
        this->show();
        break;
    default:
        _print("~~~~~~~~~~~~~~~~~~~~event 2~~~~~~~~\n");
        mainEntryThread->wakeUp();

        break;
    }

    _print("~~~~~~~~~~~~~~~~~~~~end slot~~~~~~~~\n");
#endif
}

void ParallaxHome::slideBy(qreal dx)
{
    m_pageOffset = static_cast<qreal>(pageIndex);
    int start = -(m_pageOffset + dx) * width();
    int end = m_pageOffset * width();

    if(fristFLag){
         fristFLag = false;
    }
    else
    {
        m_pageAnimator.setFrameRange(start, end);
        m_pageAnimator.start();
    }

    m_naviBar->setIndexPage(-pageIndex);
}

void ParallaxHome::choosePage(int page)
{
    if (m_pageAnimator.state() != QTimeLine::NotRunning)
        return;
    slideBy(-page - m_pageOffset);

}

void ParallaxHome::shiftPage(int frame) //¶¨Î»¸÷¸öitemµÄ×ø±ê
{
    int ww = width();
    int hh = height() - m_naviBar->rect().height();
    m_pageOffset = static_cast<qreal>(frame) / qreal(width()) ;
    int ofs = m_pageOffset * ww;
    for (int i = 0; i < m_items.count(); ++i)
    {
        QPointF pos = m_positions[i];
        QPointF xy(pos.x() * ww, pos.y() * hh);
        m_items[i]->setPos(xy + QPointF(ofs, 0));
    }
}

void ParallaxHome::setupScene()
{
    for (int i = 0; i < pageCount; i++) //Ò³Êý
    {
        int yp = 55;

        for (int j = 0; j < 3; j++) //ÐÐÊý
        {
            int xp = 40;
            for (int k = 0; k < 3; k++) //ÁÐÊý
            {
                if (!m_items.isEmpty() && m_items.count() >= testNames.count())
                    break;

                Node *node = new Node;

                int index = i*9 + j*3 + k;
                node->setText(testNames.at(index));//ÉèÖÃitemµÄÃû³Æ
                QPixmap pix(testIcons.at(index));
                //pix.scaled(QSize(59, 50), Qt::IgnoreAspectRatio);
                node->setItemPixmap(pix); //ÉèÖÃitemµÄicon

                m_scene.addItem(node);
                m_items << node;

                qreal qx = static_cast<qreal>(xp)/qreal(qApp->desktop()->screen(0)->width()) + static_cast<qreal>(i);
                qreal qy = static_cast<qreal>(yp)/qreal(qApp->desktop()->screen(0)->height()-70);//xl: 50);

                node->setItemPoint(QPointF(qx, qy));

                m_positions << QPointF(qx, qy);
                node->setZValue(1);
                xp = xp + 80; //ÁÐ¼ä¾à
            }
            yp = yp + 80-6; //ÐÐ¼ä¾à
        }
    }

    m_naviBar = new NaviBar(pageCount);
    m_scene.addItem(m_naviBar);
    m_naviBar->setIndexPage(-pageCount);
    m_naviBar->setZValue(2);

    QPixmap pixmap("/daemon/image/surfacing.png");
    //TODO
    pixmap = pixmap.scaled(qApp->desktop()->screen(0)->width(),qApp->desktop()->screen(0)->height());

   // pixmap = pixmap.scaled(width()*pageCount, height() + m_naviBar->rect().height() + 50);
    m_scene.addPixmap(pixmap); //ÉèÖÃ³¡¾°±³¾°

    m_scene.setItemIndexMethod(QGraphicsScene::NoIndex);
}

void ParallaxHome::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    layoutScene();
}


void ParallaxHome::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    pressFlag = true;//Êó±êµãÔÚ×ÀÃæÄÚ
    oldPoint = event->pos();//È¡µÃ°´ÏÂÊ±µÄ×ø±ê

    QList<QGraphicsItem *> pitems = QGraphicsView::items(event->pos());
    isHasItem = pitems.count();

    if (isHasItem == 2) //background and item, select one item.
    {
        pItem = dynamic_cast<Node*>(QGraphicsView::itemAt(event->pos()));
        if (pItem!= NULL)
        {
           // ////qDebug()<<"is ining ***********";
            pItem->setBackgroundColor(QColor(159, 159, 159));
            dragFlag = Flag_Start;//????
        }
    }
    else //only background, no item is selected.
    {
        dragFlag = Flag_Slide; //????
       // ////qDebug()<<"not  ining ***********";

    }

    event->accept();
}

void ParallaxHome::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    curPoint = event->pos();
    int dValue = curPoint.x() - oldPoint.x();
    if (qAbs(dValue) >= 8)
        dragFlag = Flag_Slide;
    else
        dragFlag = Flag_Start;
   // ////qDebug()<<"release dragFlag=========="<<dragFlag;


    if (pressFlag)
    {
        if (dragFlag == Flag_Start)  //????
        {
            if (pItem == NULL)
                return;
           // ////qDebug()<<"aaaaaaa ***********";

            pressFlag = false;
            pItem->setBackgroundColor(Qt::transparent);
            QString name = pItem->text();
            testByItemName(name);
            pItem = NULL;
        }
        else if (dragFlag == Flag_Slide) //????
        {
            if (pItem != NULL)
            {
                pItem->setBackgroundColor(Qt::transparent);
                pItem = NULL;
            }
           // ////qDebug()<<"bbbbbbbbbb ***********";


            //»¬¶¯¾àÀë³¬¹ý´°¿Ú¿í¶ÈµÄÒ»°ë²Å»á»»Ò³
            if((dValue >= width()/6) && (pageIndex < 0))
                pageIndex++;
            else if((dValue <= -width()/6) && (pageIndex > -pageCount+1))
                pageIndex--;

            choosePage(pageIndex);
        }
        event->accept();
    }
}

void ParallaxHome::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
          event->ignore();

    if (pressFlag)
    {
       // ////qDebug()<<"mouse move *******";
        int value = event->pos().x() - oldPoint.x();
       // ////qDebug()<<"curpos   oldpox  value  width()/6"<<event->pos().x()<<oldPoint.x()<<value<<width()/6;
        if (qAbs(value) >= 8)
            dragFlag = Flag_Slide;
//        else
//            dragFlag = Flag_Start;

       // ////qDebug()<<"move dragFlag=========="<<dragFlag;
        shiftPage(value + pageIndex * width());
        event->accept();
    }
}

void ParallaxHome::layoutScene()
{
    m_scene.setSceneRect(0, 0, pageCount * width() - 1, height() - 1);//ÉèÖÃ³¡¾°µÄ´óÐ¡
    centerOn(/*width() / 2, height() / 2*/0, 0); //¶¨Î»³¡¾°µÄ×ø±êÔ­µã

    int nw = m_naviBar->rect().width();
    int nh = m_naviBar->rect().height();
    m_naviBar->setPos((width() - nw) / 2, height() - nh);

    shiftPage(m_pageOffset * width());
}

void ParallaxHome::setEnterFlag()
{
    enterFLag = false;

    if(gsmFlag){
       system("/daemon/dial.dm.4.1.4 dbg=0 &");
       gsmFlag = false;
    }

}


void ParallaxHome::gsmSlot()
{
    int ret=access("/var/lock/LCK..ttymxc1",F_OK);
    if(ret==0)
    {
        if (!GSM__dial_check(1))
        {
            system("/etc/ppp/ppp-off");
        }
    }
    else
    {
        system("pkill dial");
       // Os__xdelay(100);

        sM_gsm->setEnabled(true);
        connect(sM_gsm,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
        gsmTimer.stop();
        return;
    }
    Os__xdelay(50);
}



void ParallaxHome::testByItemName(const QString &itemName) //´ò¿ª²âÊÔ½çÃæ
{
    QStringList items;
    int ret = 0;

    if((!enterFLag) &&(itemName == testNames.at(0)))
    {

        screenFunc();
    }
    else if((!enterFLag) &&(itemName == testNames.at(1)))
    {
       printerFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(2)))
    {
        keyboradFunc();
    }
    else if((!enterFLag) &&(itemName == testNames.at(3)))
    {
       magneticFunc();
    }
    else if((!enterFLag) &&(itemName == testNames.at(4)))
    {
      iccFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(5)))
    {
      samFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(6)))//uart OR SERIAL
    {
       mfcFunc();


    }
    else if((!enterFLag) &&(itemName == testNames.at(7)))//MFC CARD
    {
       barcodeFunc();


    }
    else if((!enterFLag) &&(itemName == testNames.at(8)))
    {
       signinFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(9)))//gsm
    {
      cameraFunc();


    }
    else if((!enterFLag) &&(itemName == testNames.at(10)))
    {
       gpsFunc();
    }
    else if((!enterFLag) &&(itemName == testNames.at(11)))
    {
        keytestFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(12)))
    {
      gsmFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(13)))
    {
      connectFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(14)))
    {
       batteryFunc();
    }

    else if((!enterFLag) &&(itemName == testNames.at(15)))
    {
      beepFunc();

    }
    else if((!enterFLag) &&(itemName == testNames.at(16)))
    {
       massmemoryFunc();


    }
    else if((!enterFLag) &&(itemName == testNames.at(17)))
    {
       sleepFunc();

    }
//    else if((!enterFLag) &&(itemName == testNames.at(18)))
//    {
//        modemFunc();

//    }
    else if((!enterFLag) &&(itemName == testNames.at(18)))//key test
    {
        ethernetFunc();
    }

    else if((!enterFLag) &&(itemName == testNames.at(19)))
    {
        uartFunc();

    }

    else if((!enterFLag) &&(itemName == testNames.at(20)))
    {
        exitFunc();

    }


    items.clear();
}

ParallaxHome::~ParallaxHome()
{
    _print("IN ~ParallaxHome()\n");
    if(pItem!=NULL)
    {
        delete pItem;
        pItem = NULL;
    }

    if( m_naviBar!=NULL)
    {
        delete m_naviBar;
        m_naviBar = NULL;
    }

    testNames.clear();
    testIcons.clear();

    m_positions.clear();

    if(m_items.count())
        qDeleteAll(m_items.begin(),m_items.end());

    m_items.clear();

    if(sM_gsm!=NULL)
    {
        delete sM_gsm;
        sM_gsm = NULL;
    }


    _print("end IN ~ParallaxHome()\n");

//    for(QList<Node*>::iterator
//        i = m_items.begin();i<m_items.end();i++)
//    {
//        if(*i!=NULL)
//        {
//            delete *i;
//            *i = NULL;
//        }
//    }
   // ////qDebug()<<"22222IN ~ParallaxHome()";

//    while(!m_items.isEmpty())
//        delete m_items.takeFirst();

   // m_items.clear();
   // ////qDebug()<<"333333333IN ~ParallaxHome()";


}


void ParallaxHome:: keyPressEvent(QKeyEvent *event)
{
    int ret = 0;
    if(!isActiveWindow())
        return;

    QStringList items;
    switch(event->key())
    {
      case Qt::Key_1:
        if((pageIndex==0) && (!enterFLag) )
        {
           screenFunc();
        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
           cameraFunc();
        }
        else if((pageIndex==-2)&&(!enterFLag) )
        {
            ethernetFunc();

        }
        break;
    case Qt::Key_2:
        if((pageIndex==0) && (!enterFLag) )
        {
           printerFunc();

        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
           gpsFunc();
        }
        else if((pageIndex==-2) && (!enterFLag) )
        {
            uartFunc();

        }
       break;
    case Qt::Key_3:
       if((pageIndex==0) && (!enterFLag) )
        {
          keyboradFunc();

        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
          keytestFunc();

        }
        else if((pageIndex==-2) && (!enterFLag) )
        {
            exitFunc();
        }
       break;

    case Qt::Key_4:
        if((pageIndex==0) && (!enterFLag) )
        {
           magneticFunc();
        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
          gsmFunc();

        }
//        else if((pageIndex==-2) && (!enterFLag) )
//        {
//            exitFunc();
//        }
        break;

    case Qt::Key_5:
        if((pageIndex==0) && (!enterFLag) )
        {
           iccFunc();

        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
            connectFunc();

        }

       break;
    case Qt::Key_6:
        if((pageIndex==0) && (!enterFLag) )
        {
           samFunc();

        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
            batteryFunc();
        }

       break;
    case Qt::Key_7:
        if((pageIndex==0) && (!enterFLag) )
        {
           mfcFunc();


        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
          beepFunc();

        }

       break;
    case Qt::Key_8:
        if((pageIndex==0) && (!enterFLag) )
        {
           barcodeFunc();

        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
          massmemoryFunc();

        }

       break;
    case Qt::Key_9:
        if((pageIndex==0) && (!enterFLag) )
        {
           signinFunc();
        }
        else if((pageIndex==-1) && (!enterFLag) )
        {
            sleepFunc();
        }

        break;

//    case Qt::Key_F1:{
//        if(pageIndex==0)
//            pageIndex--;
//        else if(pageIndex==-1)
//            pageIndex = -pageCount+1;
//        choosePage(pageIndex);
//    }
//        break;

//    case Qt::Key_F2:{
//        if(pageIndex==-1)
//            pageIndex = 0;
//        else if(pageIndex==-2)
//            pageIndex = -1;
//        choosePage(pageIndex);
//    }
//        break;
    case Qt::Key_Escape:
    case Qt::Key_Backspace:
        {
        exitFunc();

        }
        break;
    }

    items.clear();


}

void ParallaxHome::screenFunc()
{
    enterFLag = true;
    QStringList itemList;
    itemList<<tr("1.AUTOMATIC TEST")<<tr("2.MANUAL TEST")<<tr("3.LED TEST")<<tr("4.LIGHT TEST");
    SecondMenu *sM = new SecondMenu(tr("MMI TEST MENU"),1,itemList);/* second menu*/
    sM->show();
    connect(sM,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::printerFunc()
{
    enterFLag = true;
    QStringList itemList;
    itemList<<tr("1.CHECK PAPER")<<tr("2.PRINTER TEST")<<tr("3.PRINTER LINE FEED")<<tr("4.PRINTER HOT TEST");

    SecondMenu *sM = new SecondMenu(tr("PRINTER TEST MENU"),2,itemList);
    sM->show();
    connect(sM,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::keyboradFunc()
{
    enterFLag = true;
    KeyBoardTest *keyboardTest = new KeyBoardTest();
    keyboardTest->show();
    connect(keyboardTest,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}

void ParallaxHome::magneticFunc()
{
    enterFLag = true;
   MagneticTest *mt = new MagneticTest();
   mt->show();
   connect(mt,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::iccFunc()
{
    enterFLag = true;
    QStringList itemList;
    itemList<<tr("1.ASYNC CARD TEST")<<tr("2.4442 CARD TEST");

    SecondMenu *sM = new SecondMenu(tr("ICC TEST MENU"),3,itemList);

    sM->show();
    connect(sM,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}

void ParallaxHome::samFunc()
{
    enterFLag = true;
    SamTest *st = new SamTest();
    st->show();
    connect(st,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::uartFunc()
{
    enterFLag = true;
    COM2Test *ct = new COM2Test();
    ct->show();
    connect(ct,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::mfcFunc()
{
    enterFLag = true;
    MFCCardTest *mfc = new MFCCardTest();
    mfc->show();
    connect(mfc,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}

void ParallaxHome::modemFunc()
{
    enterFLag = true;
   ModemTest *modem = new ModemTest();
   modem->show();
   connect(modem,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::gsmFunc()
{
    enterFLag = true;
    gsmFlag = true;

    QStringList itemList;
    itemList<<tr("1.GSM INCOMING CALL")<<tr("2.GSM CSQ")<<tr("3.GSM DIAL");

    sM_gsm = new SecondMenu(tr("GSM TEST MENU"),4,itemList);
    sM_gsm->show();
    sM_gsm->setEnabled(false);


    gsmTimer.start();
}

void ParallaxHome::ethernetFunc()
{
   enterFLag = true;
  EthernetTest *et = new EthernetTest();
  et->show();
  connect(et,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::connectFunc()
{
    enterFLag = true;
    QStringList itemList;
    itemList<<tr("1.WIFI CONNECTION")<<tr("2.GSM CONNECTION")<<tr("3.GSM STATUS TEST");

    SecondMenu *sM = new SecondMenu(tr("CONNECT MENU"),5,itemList);
    sM->show();
    connect(sM,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::massmemoryFunc()
{
    enterFLag = true;
    QStringList itemList;
    itemList<<tr("1.USB TEST")<<tr("2.TF TEST");

    SecondMenu *sM = new SecondMenu(tr("MASS MEMORY MENU"),6,itemList);

    sM->show();
    connect(sM,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::barcodeFunc()
{
    enterFLag = true;
    BarcodePrintTest *bpt = new BarcodePrintTest();
    bpt->showMaximized();
    connect(bpt,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::cameraFunc()
{
    enterFLag = true;
    CameraTest *ct  = new CameraTest();
    ct->show();
    connect(ct,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}

void ParallaxHome::signinFunc()
{
    enterFLag = true;
    SignInTest *st = new SignInTest();
    st->show();
    connect(st,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::beepFunc()
{
    enterFLag = true;
    BeepTest *bt = new BeepTest(3);
    bt->show();
    connect(bt,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::batteryFunc()
{
    enterFLag = true;
    BatteryTest *bt = new BatteryTest();
    bt->showFullScreen();
    connect(bt,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}
void ParallaxHome::gpsFunc()
{
    enterFLag = true;
    int rec = Os__get_device_signal(DEV_GPS);

    if(rec==0)
    {
        QMessageBox box(QMessageBox::Information,tr("    title   "),tr("GPS DEVICE DID NOT OPEN!"));
        box.setWindowFlags(Qt::CustomizeWindowHint);

        QPushButton *okBtn = new QPushButton("Ok");
        okBtn->setFixedWidth(80);
        okBtn->setFixedHeight(25);
        box.addButton(okBtn,QMessageBox::YesRole);

        box.exec();
        enterFLag = false;

        return;
    }

    GPSTest *gt = new GPSTest();
    gt->show();
    connect(gt,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));
}

void ParallaxHome::keytestFunc()
{
    enterFLag = true;
    KeyTest *keyTest  = new KeyTest();
    keyTest->show();
    connect(keyTest,SIGNAL(destroyed()),this,SLOT(setEnterFlag()));

}

void ParallaxHome::sleepFunc()
{
    enterFLag = true;
    SYSGLOBAL_sys_suspend();
    SYSGLOBAL_sys_awake();
    enterFLag = false;
}

void ParallaxHome::exitFunc()
{
    enterFLag = true;

#ifdef thread
    mainEntryThread->wakeUp();
    hide();
#else
       close();
       Os__set_key_ensure(KEY_SYS_EXIT);

#endif
       enterFLag = false;
}