#include "LogWidgetAppender.h"
#include "loggingevent.h"
#include "ttcclayout.h"


//void tezsts(){
//    LogWidget w;
//    w.show();


//    Log4Qt::BasicConfigurator::configure();
//    Log4Qt::LogManager::setHandleQtMessages(true);
//    Log4Qt::Logger *logger = Log4Qt::Logger::rootLogger();
////    logger->removeAllAppenders();

//#if 0
//    Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout();
//    layout->setDateFormat("yyyy-MM-dd hh:mm:ss");
//#else
//    Log4Qt::TTCCLayout *layout = new Log4Qt::TTCCLayout(Log4Qt::TTCCLayout::ISO8601);
//#endif
//    // 激活选项
//    layout->activateOptions();
//    //    layout->setThreadPrinting(true);

//    {
//        Log4Qt::LogWidgetAppender *appender = new Log4Qt::LogWidgetAppender();
//        appender->setName("LogWidgetAppender");
//        appender->setLayout(layout);
//        appender->activateOptions();
//        logger->addAppender(appender);
//        //设置日志信息输出的窗口组件
//        appender->setLogWidget(w);
//    }

////    {
////        Log4Qt::DatabaseAppender *appender = new Log4Qt::LogWidgetAppender();
////        appender->setName("LogWidgetAppender");
////        appender->setLayout(layout);
////        appender->activateOptions();
////        logger->addAppender(appender);
////    }

//    {
//        // 创建Appender
//        Log4Qt::DailyRollingFileAppender *appender = new Log4Qt::DailyRollingFileAppender;
//        // 设置输出目的地为应用程序所在目录下的logFile.log
//        appender->setFile("logFile.log");
//        // 设置日志文件每天回滚
//        //appender->setDatePattern(Log4Qt::DailyRollingFileAppender::MINUTELY_ROLLOVER);
//        appender->setDatePattern("'.'yyyy-MM-dd");
//        // 设置日志为追加方式写入输出文件
//        appender->setAppendFile(true);
//        appender->setLayout(layout);
//        // 设置编码
//        appender->setEncoding(QTextCodec::codecForName("UTF-8"));

//        appender->setImmediateFlush(true);
//        // 设置阈值级别为INFO
//        appender->setThreshold(Log4Qt::Level::INFO_INT);
//        // 激活选项
//        appender->activateOptions();
//        logger->addAppender(appender);
//        // 设置级别为 DEBUG
//        logger->setLevel(Log4Qt::Level::DEBUG_INT);
//    }


//    logger->warn("hello Log4Qt");
//    logger->info("hello Log4Qt");
//    logger->debug("hello Log4Qt");
//    logger->info("你好 Log4Qt");

//}

namespace Log4Qt
{

LogWidgetAppender::LogWidgetAppender(QObject *parent): AppenderSkeleton(parent)
{
    m_logWidget = NULL;
}

LogWidgetAppender::~LogWidgetAppender()
{

}

void LogWidgetAppender::setLogWidget(const QWidget &widget)
{
    m_logWidget = const_cast<QWidget*>(&widget);
    //连接槽函数到输出窗口的onAppendLog(const QString&)槽函数
    connect(this, SIGNAL(logAppend(const QString&)), m_logWidget, SLOT(onAppendLog(const QString&)));
}

bool LogWidgetAppender::requiresLayout() const
{
    return true;
}

void LogWidgetAppender::append(const LoggingEvent &rEvent)
{
    // 格式化日志信息
    QString message = (dynamic_cast<TTCCLayout*>(layout().data()))->format(rEvent);
    emit logAppend(message);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug LogWidgetAppender::debug(QDebug &rDebug) const
{
    return rDebug.space();
}
#endif //QT_NO_DEBUG_STREAM

}
